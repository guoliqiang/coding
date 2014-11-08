// This software has been changed to serve as a simple httpd
// for use with pprof and google-perftools.  It is not the original thttpd.c.
// See README for details.

/* thttpd.c - tiny/turbo/throttling HTTP server
**
** Copyright � 1995,1998,1999,2000,2001 by Jef Poskanzer <jef@mail.acme.com>.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/


#include "config.h"
#include "version.h"

#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <pthread.h>

#include <errno.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <pwd.h>
#ifdef HAVE_GRP_H
#include <grp.h>
#endif
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TIME_WITH_SYS_TIME
#include <time.h>
#endif
#include <unistd.h>

#include "fdwatch.h"
#include "libhttpd.h"
#include "timers.h"

#ifndef SHUT_WR
#define SHUT_WR 1
#endif

#ifndef HAVE_INT64T
typedef long long int64_t;
#endif


static char* argv0;
static int debug;
static unsigned short port;
static char* dir;
static char* data_dir;
static int do_chroot, no_log, no_symlink_check, do_vhost, do_global_passwd;
static char* cgi_pattern;
static int cgi_limit;
static char* url_pattern;
static int no_empty_referers;
static char* local_pattern;
static char* logfile;
static char* throttlefile;
static char* hostname;
static char* pidfile;
static char* user;
static char* charset = "utf-8";
static char* p3p = "";
static int max_age;

typedef struct {
    int conn_state;
    int next_free_connect;
    httpd_conn* hc;
    long max_limit, min_limit;
    time_t started_at, active_at;
    Timer* wakeup_timer;
    Timer* linger_timer;
    long wouldblock_delay;
    off_t bytes;
    off_t end_byte_index;
    off_t next_byte_index;
    } connecttab;
static connecttab* connects;
static int num_connects, max_connects, first_free_connect;
static int httpd_conn_count;

/* The connection states. */
#define CNST_FREE 0
#define CNST_READING 1
#define CNST_SENDING 2
#define CNST_PAUSING 3
#define CNST_LINGERING 4
#define CNST_SLEEPING 5


static httpd_server* hs = (httpd_server*) 0;
int terminate = 0;
time_t start_time, stats_time;
long stats_connections;
off_t stats_bytes;
int stats_simultaneous;

/* Forwards. */
static void lookup_hostname( httpd_sockaddr* sa4P, size_t sa4_len, int* gotv4P, httpd_sockaddr* sa6P, size_t sa6_len, int* gotv6P );
static void shut_down( void );
static int handle_newconnect( struct timeval* tvP, int listen_fd );
static void handle_read( connecttab* c, struct timeval* tvP );
static void handle_send( connecttab* c, struct timeval* tvP );
static void handle_linger( connecttab* c, struct timeval* tvP );
static void finish_connection( connecttab* c, struct timeval* tvP );
static void clear_connection( connecttab* c, struct timeval* tvP );
static void really_clear_connection( connecttab* c, struct timeval* tvP );
static void idle( ClientData client_data, struct timeval* nowP );
static void wakeup_connection( ClientData client_data, struct timeval* nowP );
static void linger_clear_connection( ClientData client_data, struct timeval* nowP );
static void occasional( ClientData client_data, struct timeval* nowP );

int
thttpd_run(void)
    {
    char* cp;
    struct passwd* pwd;
    uid_t uid = 32767;
    gid_t gid = 32767;
    int num_ready;
    int cnum;
    connecttab* c;
    httpd_conn* hc;
    httpd_sockaddr sa4;
    httpd_sockaddr sa6;
    int gotv4, gotv6;
    struct timeval tv;
    
    cp = getenv( "GHTTPPORT" );
    if ( cp )
	port = atoi( cp );
    if( port == 0 )
	port = 9999;

    /* Read zone info now, in case we chroot(). */
    tzset();

    /* Look up hostname now, in case we chroot(). */
    lookup_hostname( &sa4, sizeof(sa4), &gotv4, &sa6, sizeof(sa6), &gotv6 );
    if ( ! ( gotv4 || gotv6 ) )
	{
	memset(&sa4, 0, sizeof sa4);
	gotv4 = 1;
	}

    /* Initialize the fdwatch package.  Have to do this before chroot,
    ** if /dev/poll is used.
    */
    max_connects = fdwatch_get_nfiles();
    if ( max_connects < 0 )
	{
	return;
	}
    max_connects -= SPARE_FDS;

    /* Set up to catch signals. */
#ifdef HAVE_SIGSET
    (void) sigset( SIGPIPE, SIG_IGN );          /* get EPIPE instead */
#else /* HAVE_SIGSET */
    (void) signal( SIGPIPE, SIG_IGN );          /* get EPIPE instead */
#endif /* HAVE_SIGSET */

    /* Initialize the timer package. */
    tmr_init();

    /* Initialize the HTTP layer.  Got to do this before giving up root,
    ** so that we can bind to a privileged port.
    */
    hs = httpd_initialize(
	hostname,
	gotv4 ? &sa4 : (httpd_sockaddr*) 0, gotv6 ? &sa6 : (httpd_sockaddr*) 0,
	port, cgi_pattern, cgi_limit, charset, p3p, max_age, "/", no_log, 
	no_symlink_check, do_vhost, do_global_passwd, url_pattern,
	local_pattern, no_empty_referers );
    if ( hs == (httpd_server*) 0 )
	exit( 1 );

    /* Set up the occasional timer. */
    if ( tmr_create( (struct timeval*) 0, occasional, JunkClientData, OCCASIONAL_TIME * 1000L, 1 ) == (Timer*) 0 )
	{
	return;
	}
    /* Set up the idle timer. */
    if ( tmr_create( (struct timeval*) 0, idle, JunkClientData, 5 * 1000L, 1 ) == (Timer*) 0 )
	{
	return;
	}
    start_time = stats_time = time( (time_t*) 0 );
    stats_connections = 0;
    stats_bytes = 0;
    stats_simultaneous = 0;

    /* Initialize our connections table. */
    connects = NEW( connecttab, max_connects );
    if ( connects == (connecttab*) 0 )
	{
	return;
	}
    for ( cnum = 0; cnum < max_connects; ++cnum )
	{
	connects[cnum].conn_state = CNST_FREE;
	connects[cnum].next_free_connect = cnum + 1;
	connects[cnum].hc = (httpd_conn*) 0;
	}
    connects[max_connects - 1].next_free_connect = -1;	/* end of link list */
    first_free_connect = 0;
    num_connects = 0;
    httpd_conn_count = 0;

    if ( hs != (httpd_server*) 0 )
	{
	if ( hs->listen4_fd != -1 )
	    fdwatch_add_fd( hs->listen4_fd, (void*) 0, FDW_READ );
	if ( hs->listen6_fd != -1 )
	    fdwatch_add_fd( hs->listen6_fd, (void*) 0, FDW_READ );
	}

    /* Main loop. */
    (void) gettimeofday( &tv, (struct timezone*) 0 );
    while ( ( ! terminate ) || num_connects > 0 )
	{

	/* Do the fd watch. */
	num_ready = fdwatch( tmr_mstimeout( &tv ) );
	if ( num_ready < 0 )
	    {
	    if ( errno == EINTR || errno == EAGAIN )
		continue;       /* try again */
	    return;
	    }
	(void) gettimeofday( &tv, (struct timezone*) 0 );

	if ( num_ready == 0 )
	    {
	    /* No fd's are ready - run the timers. */
	    tmr_run( &tv );
	    continue;
	    }

	/* Is it a new connection? */
	if ( hs != (httpd_server*) 0 && hs->listen6_fd != -1 &&
	     fdwatch_check_fd( hs->listen6_fd ) )
	    {
	    if ( handle_newconnect( &tv, hs->listen6_fd ) )
		/* Go around the loop and do another fdwatch, rather than
		** dropping through and processing existing connections.
		** New connections always get priority.
		*/
		continue;
	    }
	if ( hs != (httpd_server*) 0 && hs->listen4_fd != -1 &&
	     fdwatch_check_fd( hs->listen4_fd ) )
	    {
	    if ( handle_newconnect( &tv, hs->listen4_fd ) )
		/* Go around the loop and do another fdwatch, rather than
		** dropping through and processing existing connections.
		** New connections always get priority.
		*/
		continue;
	    }

	/* Find the connections that need servicing. */
	while ( ( c = (connecttab*) fdwatch_get_next_client_data() ) != (connecttab*) -1 )
	    {
	    if ( c == (connecttab*) 0 )
		continue;
	    hc = c->hc;
	    if ( ! fdwatch_check_fd( hc->conn_fd ) )
		/* Something went wrong. */
		clear_connection( c, &tv );
	    else
		switch ( c->conn_state )
		    {
		    case CNST_READING: handle_read( c, &tv ); break;
		    case CNST_SENDING: handle_send( c, &tv ); break;
		    case CNST_LINGERING: handle_linger( c, &tv ); break;
		    }
	    }
	tmr_run( &tv );
	}

    /* The main loop terminated. */
    shut_down();
    return 0;
    }

void
ghttpd( void )
    {
    pthread_t id;
    pthread_create(&id, 0, (void*(*)(void*))thttpd_run, 0);
    }

static void
lookup_hostname( httpd_sockaddr* sa4P, size_t sa4_len, int* gotv4P, httpd_sockaddr* sa6P, size_t sa6_len, int* gotv6P )
    {
#ifdef USE_IPV6

    struct addrinfo hints;
    char portstr[10];
    int gaierr;
    struct addrinfo* ai;
    struct addrinfo* ai2;
    struct addrinfo* aiv6;
    struct addrinfo* aiv4;

    *gotv4P = 0;
    *gotv6P = 0;

    (void) memset( &hints, 0, sizeof(hints) );
    hints.ai_family = PF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    (void) snprintf( portstr, sizeof(portstr), "%d", (int) port );
    if ( (gaierr = getaddrinfo( hostname, portstr, &hints, &ai )) != 0 )
	{
	return;
	}

    /* Find the first IPv6 and IPv4 entries. */
    aiv6 = (struct addrinfo*) 0;
    aiv4 = (struct addrinfo*) 0;
    for ( ai2 = ai; ai2 != (struct addrinfo*) 0; ai2 = ai2->ai_next )
	{
	switch ( ai2->ai_family )
	    {
	    case AF_INET6:
	    if ( aiv6 == (struct addrinfo*) 0 )
		aiv6 = ai2;
	    break;
	    case AF_INET:
	    if ( aiv4 == (struct addrinfo*) 0 )
		aiv4 = ai2;
	    break;
	    }
	}

    if ( aiv6 == (struct addrinfo*) 0 )
	*gotv6P = 0;
    else
	{
	if ( sa6_len < aiv6->ai_addrlen )
	    {
	    return;
	    }
	(void) memset( sa6P, 0, sa6_len );
	(void) memmove( sa6P, aiv6->ai_addr, aiv6->ai_addrlen );
	*gotv6P = 1;
	}

    if ( aiv4 == (struct addrinfo*) 0 )
	*gotv4P = 0;
    else
	{
	if ( sa4_len < aiv4->ai_addrlen )
	    {
	    return;
	    }
	(void) memset( sa4P, 0, sa4_len );
	(void) memmove( sa4P, aiv4->ai_addr, aiv4->ai_addrlen );
	*gotv4P = 1;
	}

    freeaddrinfo( ai );

#else /* USE_IPV6 */

    struct hostent* he;

    *gotv6P = 0;

    (void) memset( sa4P, 0, sa4_len );
    sa4P->sa.sa_family = AF_INET;
    if ( hostname == (char*) 0 )
	sa4P->sa_in.sin_addr.s_addr = htonl( INADDR_ANY );
    else
	{
	sa4P->sa_in.sin_addr.s_addr = inet_addr( hostname );
	if ( (int) sa4P->sa_in.sin_addr.s_addr == -1 )
	    {
	    he = gethostbyname( hostname );
	    if ( he == (struct hostent*) 0 )
		{
		return;
		}
	    if ( he->h_addrtype != AF_INET )
		{
		return;
		}
	    (void) memmove(
		&sa4P->sa_in.sin_addr.s_addr, he->h_addr, he->h_length );
	    }
	}
    sa4P->sa_in.sin_port = htons( port );
    *gotv4P = 1;

#endif /* USE_IPV6 */
    }

static void
shut_down( void )
    {
    int cnum;
    struct timeval tv;

    (void) gettimeofday( &tv, (struct timezone*) 0 );
    for ( cnum = 0; cnum < max_connects; ++cnum )
	{
	if ( connects[cnum].conn_state != CNST_FREE )
	    httpd_close_conn( connects[cnum].hc, &tv );
	if ( connects[cnum].hc != (httpd_conn*) 0 )
	    {
	    httpd_destroy_conn( connects[cnum].hc );
	    free( (void*) connects[cnum].hc );
	    --httpd_conn_count;
	    connects[cnum].hc = (httpd_conn*) 0;
	    }
	}
    if ( hs != (httpd_server*) 0 )
	{
	httpd_server* ths = hs;
	hs = (httpd_server*) 0;
	if ( ths->listen4_fd != -1 )
	    fdwatch_del_fd( ths->listen4_fd );
	if ( ths->listen6_fd != -1 )
	    fdwatch_del_fd( ths->listen6_fd );
	httpd_terminate( ths );
	}
    tmr_destroy();
    free( (void*) connects );
    }


static int
handle_newconnect( struct timeval* tvP, int listen_fd )
    {
    connecttab* c;
    ClientData client_data;

    /* This loops until the accept() fails, trying to start new
    ** connections as fast as possible so we don't overrun the
    ** listen queue.
    */
    for (;;)
	{
	/* Is there room in the connection table? */
	if ( num_connects >= max_connects )
	    {
	    /* Out of connection slots.  Run the timers, then the
	    ** existing connections, and maybe we'll free up a slot
	    ** by the time we get back here.
	    */
	    tmr_run( tvP );
	    return 0;
	    }
	/* Get the first free connection entry off the free list. */
	if ( first_free_connect == -1 || connects[first_free_connect].conn_state != CNST_FREE )
	    {
	    return;
	    }
	c = &connects[first_free_connect];
	/* Make the httpd_conn if necessary. */
	if ( c->hc == (httpd_conn*) 0 )
	    {
	    c->hc = NEW( httpd_conn, 1 );
	    if ( c->hc == (httpd_conn*) 0 )
		{
		return;
		}
	    c->hc->initialized = 0;
	    c->hc->conn = c;
	    ++httpd_conn_count;
	    }

	/* Get the connection. */
	switch ( httpd_get_conn( hs, listen_fd, c->hc ) )
	    {
	    /* Some error happened.  Run the timers, then the
	    ** existing connections.  Maybe the error will clear.
	    */
	    case GC_FAIL:
	    tmr_run( tvP );
	    return 0;

	    /* No more connections to accept for now. */
	    case GC_NO_MORE:
	    return 1;
	    }
	c->conn_state = CNST_READING;
	/* Pop it off the free list. */
	first_free_connect = c->next_free_connect;
	c->next_free_connect = -1;
	++num_connects;
	client_data.p = c;
	c->active_at = tvP->tv_sec;
	c->wakeup_timer = (Timer*) 0;
	c->linger_timer = (Timer*) 0;
	c->next_byte_index = 0;

	/* Set the connection file descriptor to no-delay mode. */
	httpd_set_ndelay( c->hc->conn_fd );

	fdwatch_add_fd( c->hc->conn_fd, c, FDW_READ );

	++stats_connections;
	if ( num_connects > stats_simultaneous )
	    stats_simultaneous = num_connects;
	}
    }


static void handle_read_2( connecttab*, struct timeval* );

static void
handle_read( connecttab* c, struct timeval* tvP )
    {
    int sz;
    httpd_conn* hc = c->hc;

    /* Is there room in our buffer to read more bytes? */
    if ( hc->read_idx >= hc->read_size )
	{
	if ( hc->read_size > 5000 )
	    {
	    httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	    finish_connection( c, tvP );
	    return;
	    }
	httpd_realloc_str(
	    &hc->read_buf, &hc->read_size, hc->read_size + 1000 );
	}

    /* Read some more bytes. */
    sz = read(
	hc->conn_fd, &(hc->read_buf[hc->read_idx]),
	hc->read_size - hc->read_idx );
    if ( sz == 0 )
	{
	httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	finish_connection( c, tvP );
	return;
	}
    if ( sz < 0 )
	{
	/* Ignore EINTR and EAGAIN.  Also ignore EWOULDBLOCK.  At first glance
	** you would think that connections returned by fdwatch as readable
	** should never give an EWOULDBLOCK; however, this apparently can
	** happen if a packet gets garbled.
	*/
	if ( errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK )
	    return;
	httpd_send_err(
	    hc, 400, httpd_err400title, "", httpd_err400form, "" );
	finish_connection( c, tvP );
	return;
	}
    hc->read_idx += sz;
    c->active_at = tvP->tv_sec;

    /* Do we have a complete request yet? */
    switch ( httpd_got_request( hc ) )
	{
	case GR_NO_REQUEST:
	return;
	case GR_BAD_REQUEST:
	httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	finish_connection( c, tvP );
	return;
	}

    /* Yes.  Try parsing and resolving it. */
    if ( httpd_parse_request( hc ) < 0 )
	{
	finish_connection( c, tvP );
	return;
	}

    /* Start the connection going. */
    if ( httpd_start_request( hc, tvP ) < 0 )
	{
	/* Something went wrong.  Close down the connection. */
	finish_connection( c, tvP );
	return;
	}

    if ( c->conn_state == CNST_SLEEPING )
        {
        return;
        }
    handle_read_2( c, tvP );
    }

static void
handle_read_2( connecttab *c, struct timeval *tvP )
    {
    httpd_conn* hc = c->hc;
    ClientData client_data;

    /* Fill in end_byte_index. */
    if ( hc->got_range )
	{
	c->next_byte_index = hc->first_byte_index;
	c->end_byte_index = hc->last_byte_index + 1;
	}
    else if ( hc->bytes_to_send < 0 )
	c->end_byte_index = 0;
    else
	c->end_byte_index = hc->bytes_to_send;

    /* Check if it's already handled. */
    if ( hc->body_data == (char*) 0 )
	{
	/* No body data means someone else is handling it. */
	c->next_byte_index = hc->bytes_sent;
	finish_connection( c, tvP );
	return;
	}
    if ( c->next_byte_index >= c->end_byte_index )
	{
	/* There's nothing to send. */
	finish_connection( c, tvP );
	return;
	}

    /* Cool, we have a valid connection and a file to send to it. */
    c->conn_state = CNST_SENDING;
    c->started_at = tvP->tv_sec;
    c->wouldblock_delay = 0;
    client_data.p = c;

    fdwatch_del_fd( hc->conn_fd );
    fdwatch_add_fd( hc->conn_fd, c, FDW_WRITE );
    }


static void
handle_send( connecttab* c, struct timeval* tvP )
    {
    size_t max_bytes;
    int sz, coast;
    ClientData client_data;
    time_t elapsed;
    httpd_conn* hc = c->hc;
    int tind;

    max_bytes = 1000000000L;

    /* Do we need to write the headers first? */
    if ( hc->responselen == 0 )
	{
	/* No, just write the file. */
	sz = write(
	    hc->conn_fd, &(hc->body_data[c->next_byte_index]),
	    MIN( c->end_byte_index - c->next_byte_index, max_bytes ) );
	}
    else
	{
	/* Yes.  We'll combine headers and file into a single writev(),
	** hoping that this generates a single packet.
	*/
	struct iovec iv[2];

	iv[0].iov_base = hc->response;
	iv[0].iov_len = hc->responselen;
	iv[1].iov_base = &(hc->body_data[c->next_byte_index]);
	iv[1].iov_len = MIN( c->end_byte_index - c->next_byte_index, max_bytes );
	sz = writev( hc->conn_fd, iv, 2 );
	}

    if ( sz < 0 && errno == EINTR )
	return;

    if ( sz == 0 ||
	 ( sz < 0 && ( errno == EWOULDBLOCK || errno == EAGAIN ) ) )
	{
	/* This shouldn't happen, but some kernels, e.g.
	** SunOS 4.1.x, are broken and select() says that
	** O_NDELAY sockets are always writable even when
	** they're actually not.
	**
	** Current workaround is to block sending on this
	** socket for a brief adaptively-tuned period.
	** Fortunately we already have all the necessary
	** blocking code, for use with throttling.
	*/
	c->wouldblock_delay += MIN_WOULDBLOCK_DELAY;
	c->conn_state = CNST_PAUSING;
	fdwatch_del_fd( hc->conn_fd );
	client_data.p = c;
	c->wakeup_timer = tmr_create(
	    tvP, wakeup_connection, client_data, c->wouldblock_delay, 0 );
	if ( c->wakeup_timer == (Timer*) 0 )
	    {
	    return;
	    }
	return;
	}

    if ( sz < 0 )
	{
	/* Something went wrong, close this connection.
	*/
	clear_connection( c, tvP );
	return;
	}

    /* Ok, we wrote something. */
    c->active_at = tvP->tv_sec;
    /* Was this a headers + file writev()? */
    if ( hc->responselen > 0 )
	{
	/* Yes; did we write only part of the headers? */
	if ( sz < hc->responselen )
	    {
	    /* Yes; move the unwritten part to the front of the buffer. */
	    int newlen = hc->responselen - sz;
	    (void) memmove( hc->response, &(hc->response[sz]), newlen );
	    hc->responselen = newlen;
	    sz = 0;
	    }
	else
	    {
	    /* Nope, we wrote the full headers, so adjust accordingly. */
	    sz -= hc->responselen;
	    hc->responselen = 0;
	    }
	}
    /* And update how much of the file we wrote. */
    c->next_byte_index += sz;
    c->hc->bytes_sent += sz;

    /* Are we done? */
    if ( c->next_byte_index >= c->end_byte_index )
	{
	/* This connection is finished! */
	finish_connection( c, tvP );
	return;
	}

    /* Tune the (blockheaded) wouldblock delay. */
    if ( c->wouldblock_delay > MIN_WOULDBLOCK_DELAY )
	c->wouldblock_delay -= MIN_WOULDBLOCK_DELAY;

    /* (No check on min_limit here, that only controls connection startups.) */
    }


static void
handle_linger( connecttab* c, struct timeval* tvP )
    {
    char buf[4096];
    int r;

    /* In lingering-close mode we just read and ignore bytes.  An error
    ** or EOF ends things, otherwise we go until a timeout.
    */
    r = read( c->hc->conn_fd, buf, sizeof(buf) );
    if ( r < 0 && ( errno == EINTR || errno == EAGAIN ) )
	return;
    if ( r <= 0 )
	really_clear_connection( c, tvP );
    }

static void
finish_connection( connecttab* c, struct timeval* tvP )
    {
    /* If we haven't actually sent the buffered response yet, do so now. */
    httpd_write_response( c->hc );

    /* And clear. */
    clear_connection( c, tvP );
    }


static void
clear_connection( connecttab* c, struct timeval* tvP )
    {
    ClientData client_data;

    if ( c->wakeup_timer != (Timer*) 0 )
	{
	tmr_cancel( c->wakeup_timer );
	c->wakeup_timer = 0;
	}

    /* This is our version of Apache's lingering_close() routine, which is
    ** their version of the often-broken SO_LINGER socket option.  For why
    ** this is necessary, see http://www.apache.org/docs/misc/fin_wait_2.html
    ** What we do is delay the actual closing for a few seconds, while reading
    ** any bytes that come over the connection.  However, we don't want to do
    ** this unless it's necessary, because it ties up a connection slot and
    ** file descriptor which means our maximum connection-handling rate
    ** is lower.  So, elsewhere we set a flag when we detect the few
    ** circumstances that make a lingering close necessary.  If the flag
    ** isn't set we do the real close now.
    */
    if ( c->conn_state == CNST_LINGERING )
	{
	/* If we were already lingering, shut down for real. */
	tmr_cancel( c->linger_timer );
	c->linger_timer = (Timer*) 0;
	c->hc->should_linger = 0;
	}
    if ( c->hc->should_linger )
	{
	if ( c->conn_state != CNST_PAUSING && c->conn_state != CNST_SLEEPING )
	    fdwatch_del_fd( c->hc->conn_fd );
	c->conn_state = CNST_LINGERING;
	shutdown( c->hc->conn_fd, SHUT_WR );
	fdwatch_add_fd( c->hc->conn_fd, c, FDW_READ );
	client_data.p = c;
	c->linger_timer = tmr_create(
	    tvP, linger_clear_connection, client_data, LINGER_TIME, 0 );
	}
    else
	really_clear_connection( c, tvP );
    }


static void
really_clear_connection( connecttab* c, struct timeval* tvP )
    {
    stats_bytes += c->hc->bytes_sent;
    if ( c->conn_state != CNST_PAUSING && c->conn_state != CNST_SLEEPING )
	fdwatch_del_fd( c->hc->conn_fd );
    httpd_close_conn( c->hc, tvP );
    if ( c->linger_timer != (Timer*) 0 )
	{
	tmr_cancel( c->linger_timer );
	c->linger_timer = 0;
	}
    c->conn_state = CNST_FREE;
    c->next_free_connect = first_free_connect;
    first_free_connect = c - connects;	/* division by sizeof is implied */
    --num_connects;
    }


static void
idle( ClientData client_data, struct timeval* nowP )
    {
    int cnum;
    connecttab* c;

    for ( cnum = 0; cnum < max_connects; ++cnum )
	{
	c = &connects[cnum];
	switch ( c->conn_state )
	    {
	    case CNST_READING:
	    if ( nowP->tv_sec - c->active_at >= IDLE_READ_TIMELIMIT )
		{
		httpd_send_err(
		    c->hc, 408, httpd_err408title, "", httpd_err408form, "" );
		finish_connection( c, nowP );
		}
	    break;
	    case CNST_SENDING:
	    case CNST_PAUSING:
	    if ( nowP->tv_sec - c->active_at >= IDLE_SEND_TIMELIMIT )
		{
		clear_connection( c, nowP );
		}
	    break;
	    case CNST_SLEEPING:;
	    }
	}
    }


static void
wakeup_connection( ClientData client_data, struct timeval* nowP )
    {
    connecttab* c;

    c = (connecttab*) client_data.p;
    c->wakeup_timer = (Timer*) 0;
    if ( c->conn_state == CNST_PAUSING || c->conn_state == CNST_SLEEPING )
	{
	c->conn_state = CNST_SENDING;
	fdwatch_add_fd( c->hc->conn_fd, c, FDW_WRITE );
	}
    }

static void
linger_clear_connection( ClientData client_data, struct timeval* nowP )
    {
    connecttab* c;

    c = (connecttab*) client_data.p;
    c->linger_timer = (Timer*) 0;
    really_clear_connection( c, nowP );
    }


static void
occasional( ClientData client_data, struct timeval* nowP )
    {
    tmr_cleanup();
    }

static void
httpd_conn_wakeup( ClientData client_data, struct timeval* nowP )
    {
    connecttab* c = client_data.p;

    c->conn_state = CNST_READING;
    c->wakeup_timer = (Timer*) 0;
    if( httpd_continue_request( c->hc, nowP ) < 0 )
        {
        finish_connection( c, nowP );
        return;
        }
    if( c->conn_state == CNST_READING )
      handle_read_2( c, nowP );
    }

void
httpd_conn_sleep( httpd_conn *hc, int seconds )
    {
    connecttab* c = hc->conn;
    ClientData client_data;

    if ( c->wakeup_timer != (Timer*) 0 )
	{
	tmr_cancel( c->wakeup_timer );
	c->wakeup_timer = 0;
	}
    fdwatch_del_fd( hc->conn_fd );
    c->conn_state = CNST_SLEEPING;
    client_data.p = c;
    c->wakeup_timer = tmr_create(
        0, httpd_conn_wakeup, client_data, seconds * 1000, 0 );
    }

