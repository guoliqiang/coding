/* libhttpd.c - HTTP protocol library
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

#ifdef SHOW_SERVER_VERSION
#define EXPOSED_SERVER_SOFTWARE SERVER_SOFTWARE
#else /* SHOW_SERVER_VERSION */
#define EXPOSED_SERVER_SOFTWARE "thttpd"
#endif /* SHOW_SERVER_VERSION */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif /* HAVE_MEMORY_H */
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#ifdef HAVE_OSRELDATE_H
#include <osreldate.h>
#endif /* HAVE_OSRELDATE_H */

#ifdef HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# ifdef HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# ifdef HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# ifdef HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

extern char* crypt( const char* key, const char* setting );

#include "libhttpd.h"
#include "timers.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

#ifndef SHUT_WR
#define SHUT_WR 1
#endif

#ifndef HAVE_INT64T
typedef long long int64_t;
#endif

#ifndef HAVE_SOCKLENT
typedef int socklen_t;
#endif

#ifdef __CYGWIN__
#define timezone  _timezone
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif


/* Forwards. */
static void check_options( void );
static void free_httpd_server( httpd_server* hs );
static int initialize_listen_socket( httpd_sockaddr* saP );
static void add_response( httpd_conn* hc, char* str );
static void send_mime( httpd_conn* hc, int status, char* title, char* encodings, char* extraheads, char* type, off_t length, time_t mod );
static void send_response( httpd_conn* hc, int status, char* title, char* extraheads, char* form, char* arg );
static void send_response_tail( httpd_conn* hc );
static void defang( char* str, char* dfstr, int dfsize );
static int hexit( char c );
static void strdecode( char* to, char* from );
static char* bufgets( httpd_conn* hc );
static void de_dotdot( char* file );
static void figure_mime( httpd_conn* hc );
static int sockaddr_check( httpd_sockaddr* saP );
static size_t sockaddr_len( httpd_sockaddr* saP );
static int my_snprintf( char* str, size_t size, const char* format, ... );
#ifndef HAVE_ATOLL
static long long atoll( const char* str );
#endif /* HAVE_ATOLL */


/* This global keeps track of whether we are in the main process or a
** sub-process.  The reason is that httpd_write_response() can get called
** in either context; when it is called from the main process it must use
** non-blocking I/O to avoid stalling the server, but when it is called
** from a sub-process it wants to use blocking I/O so that the whole
** response definitely gets written.  So, it checks this variable.  A bit
** of a hack but it seems to do the right thing.
*/
static int sub_process = 0;

static void
free_httpd_server( httpd_server* hs )
    {
    if ( hs->binding_hostname != (char*) 0 )
	free( (void*) hs->binding_hostname );
    if ( hs->cwd != (char*) 0 )
	free( (void*) hs->cwd );
    if ( hs->cgi_pattern != (char*) 0 )
	free( (void*) hs->cgi_pattern );
    if ( hs->charset != (char*) 0 )
	free( (void*) hs->charset );
    if ( hs->p3p != (char*) 0 )
	free( (void*) hs->p3p );
    if ( hs->url_pattern != (char*) 0 )
	free( (void*) hs->url_pattern );
    if ( hs->local_pattern != (char*) 0 )
	free( (void*) hs->local_pattern );
    free( (void*) hs );
    }


httpd_server*
httpd_initialize(
    char* hostname, httpd_sockaddr* sa4P, httpd_sockaddr* sa6P,
    unsigned short port, char* cgi_pattern, int cgi_limit, char* charset,
    char* p3p, int max_age, char* cwd, int no_log, 
    int no_symlink_check, int vhost, int global_passwd, char* url_pattern,
    char* local_pattern, int no_empty_referers )
    {
    httpd_server* hs;
    static char ghnbuf[256];
    char* cp;

    hs = NEW( httpd_server, 1 );
    if ( hs == (httpd_server*) 0 )
	{
	return (httpd_server*) 0;
	}

    if ( hostname != (char*) 0 )
	{
	hs->binding_hostname = strdup( hostname );
	if ( hs->binding_hostname == (char*) 0 )
	    {
	    return (httpd_server*) 0;
	    }
	hs->server_hostname = hs->binding_hostname;
	}
    else
	{
	hs->binding_hostname = (char*) 0;
	hs->server_hostname = (char*) 0;
	if ( gethostname( ghnbuf, sizeof(ghnbuf) ) < 0 )
	    ghnbuf[0] = '\0';
	if ( hs->server_hostname == (char*) 0 )
	    {
	    if ( ghnbuf[0] != '\0' )
		hs->server_hostname = ghnbuf;
	    }
	}

    hs->port = port;
    hs->cgi_pattern = (char*) 0;
    hs->cgi_limit = cgi_limit;
    hs->cgi_count = 0;
    hs->charset = strdup( charset );
    hs->p3p = strdup( p3p );
    hs->max_age = max_age;
    hs->cwd = "/";
    hs->url_pattern = (char*) 0;
    hs->local_pattern = (char*) 0;
    hs->no_log = no_log;
    hs->no_symlink_check = no_symlink_check;
    hs->vhost = vhost;
    hs->global_passwd = global_passwd;
    hs->no_empty_referers = no_empty_referers;

    /* Initialize listen sockets.  Try v6 first because of a Linux peculiarity;
    ** like some other systems, it has magical v6 sockets that also listen for
    ** v4, but in Linux if you bind a v4 socket first then the v6 bind fails.
    */
    if ( sa6P == (httpd_sockaddr*) 0 )
	hs->listen6_fd = -1;
    else
	hs->listen6_fd = initialize_listen_socket( sa6P );
    if ( sa4P == (httpd_sockaddr*) 0 )
	hs->listen4_fd = -1;
    else
	hs->listen4_fd = initialize_listen_socket( sa4P );
    /* If we didn't get any valid sockets, fail. */
    if ( hs->listen4_fd == -1 && hs->listen6_fd == -1 )
	{
	free_httpd_server( hs );
	return (httpd_server*) 0;
	}

    return hs;
    }


static int
initialize_listen_socket( httpd_sockaddr* saP )
    {
    int listen_fd;
    int on, flags;

    /* Check sockaddr. */
    if ( ! sockaddr_check( saP ) )
	{
	return -1;
	}

    /* Create socket. */
    listen_fd = socket( saP->sa.sa_family, SOCK_STREAM, 0 );
    if ( listen_fd < 0 )
	{
	return -1;
	}
    (void) fcntl( listen_fd, F_SETFD, 1 );

    /* Allow reuse of local addresses. */
    on = 1;
    setsockopt(
	     listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &on,
	     sizeof(on) );

    /* Bind to it. */
    if ( bind( listen_fd, &saP->sa, sockaddr_len( saP ) ) < 0 )
	{
	(void) close( listen_fd );
	return -1;
	}

    /* Set the listen file descriptor to no-delay / non-blocking mode. */
    flags = fcntl( listen_fd, F_GETFL, 0 );
    if ( flags == -1 )
	{
	(void) close( listen_fd );
	return -1;
	}
    if ( fcntl( listen_fd, F_SETFL, flags | O_NDELAY ) < 0 )
	{
	(void) close( listen_fd );
	return -1;
	}

    /* Start a listen going. */
    if ( listen( listen_fd, LISTEN_BACKLOG ) < 0 )
	{
	(void) close( listen_fd );
	return -1;
	}

    /* Use accept filtering, if available. */
#ifdef SO_ACCEPTFILTER
    {
#if ( __FreeBSD_version >= 411000 )
#define ACCEPT_FILTER_NAME "httpready"
#else
#define ACCEPT_FILTER_NAME "dataready"
#endif
    struct accept_filter_arg af;
    (void) bzero( &af, sizeof(af) );
    (void) strcpy( af.af_name, ACCEPT_FILTER_NAME );
    (void) setsockopt(
	listen_fd, SOL_SOCKET, SO_ACCEPTFILTER, (char*) &af, sizeof(af) );
    }
#endif /* SO_ACCEPTFILTER */

    return listen_fd;
    }


void
httpd_terminate( httpd_server* hs )
    {
    httpd_unlisten( hs );
    free_httpd_server( hs );
    }


void
httpd_unlisten( httpd_server* hs )
    {
    if ( hs->listen4_fd != -1 )
	{
	(void) close( hs->listen4_fd );
	hs->listen4_fd = -1;
	}
    if ( hs->listen6_fd != -1 )
	{
	(void) close( hs->listen6_fd );
	hs->listen6_fd = -1;
	}
    }


/* Conditional macro to allow two alternate forms for use in the built-in
** error pages.  If EXPLICIT_ERROR_PAGES is defined, the second and more
** explicit error form is used; otherwise, the first and more generic
** form is used.
*/
#ifdef EXPLICIT_ERROR_PAGES
#define ERROR_FORM(a,b) b
#else /* EXPLICIT_ERROR_PAGES */
#define ERROR_FORM(a,b) a
#endif /* EXPLICIT_ERROR_PAGES */


static char* ok200title = "OK";
static char* ok206title = "Partial Content";

static char* err302title = "Found";
static char* err302form = "The actual URL is '%.80s'.\n";

static char* err304title = "Not Modified";

char* httpd_err400title = "Bad Request";
char* httpd_err400form =
    "Your request has bad syntax or is inherently impossible to satisfy.\n";

static char* err403title = "Forbidden";
#ifndef EXPLICIT_ERROR_PAGES
static char* err403form =
    "You do not have permission to get URL '%.80s' from this server.\n";
#endif /* !EXPLICIT_ERROR_PAGES */

char* httpd_err404title = "Not Found";
char* httpd_err404form =
    "The requested URL '%.80s' was not found on this server.\n";

char* httpd_err408title = "Request Timeout";
char* httpd_err408form =
    "No request appeared within a reasonable time period.\n";

static char* err500title = "Internal Error";
static char* err500form =
    "There was an unusual problem serving the requested URL '%.80s'.\n";

static char* err501title = "Not Implemented";
static char* err501form =
    "The requested method '%.80s' is not implemented by this server.\n";

char* httpd_err503title = "Service Temporarily Overloaded";
char* httpd_err503form =
    "The requested URL '%.80s' is temporarily overloaded.  Please try again later.\n";


/* Append a string to the buffer waiting to be sent as response. */
static void
add_response( httpd_conn* hc, char* str )
    {
    size_t len;

    len = strlen( str );
    httpd_realloc_str( &hc->response, &hc->maxresponse, hc->responselen + len );
    (void) memmove( &(hc->response[hc->responselen]), str, len );
    hc->responselen += len;
    }

/* Send the buffered response. */
void
httpd_write_response( httpd_conn* hc )
    {
    /* If we are in a sub-process, turn off no-delay mode. */
    if ( sub_process )
	httpd_clear_ndelay( hc->conn_fd );
    /* Send the response, if necessary. */
    if ( hc->responselen > 0 )
	{
	(void) httpd_write_fully( hc->conn_fd, hc->response, hc->responselen );
	hc->responselen = 0;
	}
    }


/* Set no-delay / non-blocking mode on a socket. */
void
httpd_set_ndelay( int fd )
    {
    int flags, newflags;

    flags = fcntl( fd, F_GETFL, 0 );
    if ( flags != -1 )
	{
	newflags = flags | (int) O_NDELAY;
	if ( newflags != flags )
	    (void) fcntl( fd, F_SETFL, newflags );
	}
    }


/* Clear no-delay / non-blocking mode on a socket. */
void
httpd_clear_ndelay( int fd )
    {
    int flags, newflags;

    flags = fcntl( fd, F_GETFL, 0 );
    if ( flags != -1 )
	{
	newflags = flags & ~ (int) O_NDELAY;
	if ( newflags != flags )
	    (void) fcntl( fd, F_SETFL, newflags );
	}
    }


static void
send_mime( httpd_conn* hc, int status, char* title, char* encodings, char* extraheads, char* type, off_t length, time_t mod )
    {
    time_t now, expires;
    const char* rfc1123fmt = "%a, %d %b %Y %H:%M:%S GMT";
    char nowbuf[100];
    char modbuf[100];
    char expbuf[100];
    char fixed_type[500];
    char buf[1000];
    int partial_content;
    int s100;

    hc->status = status;
    hc->bytes_to_send = length;
    if ( hc->mime_flag )
	{
	if ( status == 200 && hc->got_range &&
	     ( hc->last_byte_index >= hc->first_byte_index ) &&
	     ( ( hc->last_byte_index != length - 1 ) ||
	       ( hc->first_byte_index != 0 ) ) &&
	     ( hc->range_if == (time_t) -1 ||
	       hc->range_if == hc->sb.st_mtime ) )
	    {
	    partial_content = 1;
	    hc->status = status = 206;
	    title = ok206title;
	    }
	else
	    {
	    partial_content = 0;
	    hc->got_range = 0;
	    }

	now = time( (time_t*) 0 );
	if ( mod == (time_t) 0 )
	    mod = now;
	(void) strftime( nowbuf, sizeof(nowbuf), rfc1123fmt, gmtime( &now ) );
	(void) strftime( modbuf, sizeof(modbuf), rfc1123fmt, gmtime( &mod ) );
	(void) my_snprintf(
	    fixed_type, sizeof(fixed_type), type, hc->hs->charset );
	(void) my_snprintf( buf, sizeof(buf),
	    "%.20s %d %s\015\012Server: %s\015\012Content-Type: %s\015\012Date: %s\015\012Last-Modified: %s\015\012Accept-Ranges: bytes\015\012Connection: close\015\012",
	    hc->protocol, status, title, EXPOSED_SERVER_SOFTWARE, fixed_type,
	    nowbuf, modbuf );
	add_response( hc, buf );
	s100 = status / 100;
	if ( s100 != 2 && s100 != 3 )
	    {
	    (void) my_snprintf( buf, sizeof(buf),
		"Cache-Control: no-cache,no-store\015\012" );
	    add_response( hc, buf );
	    }
	if ( encodings[0] != '\0' )
	    {
	    (void) my_snprintf( buf, sizeof(buf),
		"Content-Encoding: %s\015\012", encodings );
	    add_response( hc, buf );
	    }
	if ( partial_content )
	    {
	    (void) my_snprintf( buf, sizeof(buf),
		"Content-Range: bytes %lld-%lld/%lld\015\012Content-Length: %lld\015\012",
		(int64_t) hc->first_byte_index, (int64_t) hc->last_byte_index,
		(int64_t) length,
		(int64_t) ( hc->last_byte_index - hc->first_byte_index + 1 ) );
	    add_response( hc, buf );
	    }
	else if ( length >= 0 )
	    {
	    (void) my_snprintf( buf, sizeof(buf),
		"Content-Length: %lld\015\012", (int64_t) length );
	    add_response( hc, buf );
	    }
	if ( hc->hs->p3p[0] != '\0' )
	    {
	    (void) my_snprintf( buf, sizeof(buf), "P3P: %s\015\012", hc->hs->p3p );
	    add_response( hc, buf );
	    }
	if ( hc->hs->max_age >= 0 )
	    {
	    expires = now + hc->hs->max_age;
	    (void) strftime(
		expbuf, sizeof(expbuf), rfc1123fmt, gmtime( &expires ) );
	    (void) my_snprintf( buf, sizeof(buf),
		"Cache-Control: max-age=%d\015\012Expires: %s\015\012",
		hc->hs->max_age, expbuf );
	    add_response( hc, buf );
	    }
	if ( extraheads[0] != '\0' )
	    add_response( hc, extraheads );
	add_response( hc, "\015\012" );
	}
    }


static int str_alloc_count = 0;
static size_t str_alloc_size = 0;

void
httpd_realloc_str( char** strP, size_t* maxsizeP, size_t size )
    {
    if ( *maxsizeP == 0 )
	{
	*maxsizeP = MAX( 200, size + 100 );
	*strP = NEW( char, *maxsizeP + 1 );
	++str_alloc_count;
	str_alloc_size += *maxsizeP;
	}
    else if ( size > *maxsizeP )
	{
	str_alloc_size -= *maxsizeP;
	*maxsizeP = MAX( *maxsizeP * 2, size * 5 / 4 );
	*strP = RENEW( *strP, char, *maxsizeP + 1 );
	str_alloc_size += *maxsizeP;
	}
    else
	return;
    if ( *strP == (char*) 0 )
	{
	abort();  // TODO: What to do?
	return;
	}
    }

static char* template =
"<html><head><title>%d %s</title></head>\n"
"<body bgcolor=\"#ffffff\">\n"
"<H2>%d %s</H2>\n"
;

static void
send_response( httpd_conn* hc, int status, char* title, char* extraheads, char* form, char* arg )
    {
    char defanged_arg[1000], buf[2000];

    send_mime(
	hc, status, title, "", extraheads, "text/html; charset=%s", (off_t) -1,
	(time_t) 0 );
    (void) my_snprintf( buf, sizeof(buf), template,
	status, title, status, title );
    add_response( hc, buf );
    defang( arg, defanged_arg, sizeof(defanged_arg) );
    (void) my_snprintf( buf, sizeof(buf), form, defanged_arg );
    add_response( hc, buf );
    if ( strstr("MSIE", hc->useragent ) )
	{
	int n;
	add_response( hc, "<!--\n" );
	for ( n = 0; n < 6; ++n )
	    add_response( hc, "Padding so that MSIE deigns to show this error instead of its own canned one.\n");
	add_response( hc, "-->\n" );
	}
    send_response_tail( hc );
    }


static void
send_response_tail( httpd_conn* hc )
    {
    char buf[1000];

    (void) my_snprintf( buf, sizeof(buf), "\
<HR>\n\
<ADDRESS><A HREF=\"%s\">%s</A></ADDRESS>\n\
</BODY>\n\
</HTML>\n",
	SERVER_ADDRESS, EXPOSED_SERVER_SOFTWARE );
    add_response( hc, buf );
    }


static void
defang( char* str, char* dfstr, int dfsize )
    {
    char* cp1;
    char* cp2;

    for ( cp1 = str, cp2 = dfstr;
	  *cp1 != '\0' && cp2 - dfstr < dfsize - 5;
	  ++cp1, ++cp2 )
	{
	switch ( *cp1 )
	    {
	    case '<':
	    *cp2++ = '&';
	    *cp2++ = 'l';
	    *cp2++ = 't';
	    *cp2 = ';';
	    break;
	    case '>':
	    *cp2++ = '&';
	    *cp2++ = 'g';
	    *cp2++ = 't';
	    *cp2 = ';';
	    break;
	    default:
	    *cp2 = *cp1;
	    break;
	    }
	}
    *cp2 = '\0';
    }


void
httpd_send_err( httpd_conn* hc, int status, char* title, char* extraheads, char* form, char* arg )
    {
    send_response( hc, status, title, extraheads, form, arg );
    }

char*
httpd_method_str( int method )
    {
    switch ( method )
	{
	case METHOD_GET: return "GET";
	case METHOD_HEAD: return "HEAD";
	case METHOD_POST: return "POST";
	default: return "UNKNOWN";
	}
    }

static int
hexit( char c )
    {
    if ( c >= '0' && c <= '9' )
	return c - '0';
    if ( c >= 'a' && c <= 'f' )
	return c - 'a' + 10;
    if ( c >= 'A' && c <= 'F' )
	return c - 'A' + 10;
    return 0;           /* shouldn't happen, we're guarded by isxdigit() */
    }


/* Copies and decodes a string.  It's ok for from and to to be the
** same string.
*/
static void
strdecode( char* to, char* from )
    {
    for ( ; *from != '\0'; ++to, ++from )
	{
	if ( from[0] == '%' && isxdigit( from[1] ) && isxdigit( from[2] ) )
	    {
	    *to = hexit( from[1] ) * 16 + hexit( from[2] );
	    from += 2;
	    }
	else
	    *to = *from;
	}
    *to = '\0';
    }


int
httpd_get_conn( httpd_server* hs, int listen_fd, httpd_conn* hc )
    {
    httpd_sockaddr sa;
    socklen_t sz;

    if ( ! hc->initialized )
	{
	hc->read_size = 0;
	httpd_realloc_str( &hc->read_buf, &hc->read_size, 500 );
	hc->maxdecodedurl =
	    hc->maxorigfilename = hc->maxexpnfilename = hc->maxencodings =
	    hc->maxpathinfo = hc->maxquery = hc->maxaccept =
	    hc->maxaccepte = hc->maxreqhost = hc->maxhostdir =
	    hc->maxremoteuser = hc->maxresponse = 0;
#ifdef TILDE_MAP_2
	hc->maxaltdir = 0;
#endif /* TILDE_MAP_2 */
	httpd_realloc_str( &hc->decodedurl, &hc->maxdecodedurl, 1 );
	httpd_realloc_str( &hc->origfilename, &hc->maxorigfilename, 1 );
	httpd_realloc_str( &hc->encodings, &hc->maxencodings, 0 );
	httpd_realloc_str( &hc->query, &hc->maxquery, 0 );
	httpd_realloc_str( &hc->accept, &hc->maxaccept, 0 );
	httpd_realloc_str( &hc->accepte, &hc->maxaccepte, 0 );
	httpd_realloc_str( &hc->reqhost, &hc->maxreqhost, 0 );
	httpd_realloc_str( &hc->hostdir, &hc->maxhostdir, 0 );
	httpd_realloc_str( &hc->remoteuser, &hc->maxremoteuser, 0 );
	httpd_realloc_str( &hc->response, &hc->maxresponse, 0 );
#ifdef TILDE_MAP_2
	httpd_realloc_str( &hc->altdir, &hc->maxaltdir, 0 );
#endif /* TILDE_MAP_2 */
	hc->initialized = 1;
	}

    /* Accept the new connection. */
    sz = sizeof(sa);
    hc->conn_fd = accept( listen_fd, &sa.sa, &sz );
    if ( hc->conn_fd < 0 )
	{
	if ( errno == EWOULDBLOCK )
	    return GC_NO_MORE;
	return GC_FAIL;
	}
    if ( ! sockaddr_check( &sa ) )
	{
	close( hc->conn_fd );
	hc->conn_fd = -1;
	return GC_FAIL;
	}
    (void) fcntl( hc->conn_fd, F_SETFD, 1 );
    hc->hs = hs;
    (void) memset( &hc->client_addr, 0, sizeof(hc->client_addr) );
    (void) memmove( &hc->client_addr, &sa, sockaddr_len( &sa ) );
    hc->read_idx = 0;
    hc->checked_idx = 0;
    hc->checked_state = CHST_FIRSTWORD;
    hc->method = METHOD_UNKNOWN;
    hc->status = 0;
    hc->bytes_to_send = 0;
    hc->bytes_sent = 0;
    hc->encodedurl = "";
    hc->decodedurl[0] = '\0';
    hc->protocol = "UNKNOWN";
    hc->origfilename[0] = '\0';
    hc->encodings[0] = '\0';
    hc->query[0] = '\0';
    hc->referer = "";
    hc->useragent = "";
    hc->accept[0] = '\0';
    hc->accepte[0] = '\0';
    hc->acceptl = "";
    hc->cookie = "";
    hc->contenttype = "";
    hc->reqhost[0] = '\0';
    hc->hdrhost = "";
    hc->hostdir[0] = '\0';
    hc->authorization = "";
    hc->remoteuser[0] = '\0';
    hc->response[0] = '\0';
#ifdef TILDE_MAP_2
    hc->altdir[0] = '\0';
#endif /* TILDE_MAP_2 */
    hc->responselen = 0;
    hc->if_modified_since = (time_t) -1;
    hc->range_if = (time_t) -1;
    hc->contentlength = -1;
    hc->type = "";
    hc->hostname = (char*) 0;
    hc->mime_flag = 1;
    hc->one_one = 0;
    hc->got_range = 0;
    hc->tildemapped = 0;
    hc->first_byte_index = 0;
    hc->last_byte_index = -1;
    hc->keep_alive = 0;
    hc->should_linger = 0;
    return GC_OK;
    }


/* Checks hc->read_buf to see whether a complete request has been read so far;
** either the first line has two words (an HTTP/0.9 request), or the first
** line has three words and there's a blank line present.
**
** hc->read_idx is how much has been read in; hc->checked_idx is how much we
** have checked so far; and hc->checked_state is the current state of the
** finite state machine.
*/
int
httpd_got_request( httpd_conn* hc )
    {
    char c;

    for ( ; hc->checked_idx < hc->read_idx; ++hc->checked_idx )
	{
	c = hc->read_buf[hc->checked_idx];
	switch ( hc->checked_state )
	    {
	    case CHST_FIRSTWORD:
	    switch ( c )
		{
		case ' ': case '\t':
		hc->checked_state = CHST_FIRSTWS;
		break;
		case '\012': case '\015':
		hc->checked_state = CHST_BOGUS;
		return GR_BAD_REQUEST;
		}
	    break;
	    case CHST_FIRSTWS:
	    switch ( c )
		{
		case ' ': case '\t':
		break;
		case '\012': case '\015':
		hc->checked_state = CHST_BOGUS;
		return GR_BAD_REQUEST;
		default:
		hc->checked_state = CHST_SECONDWORD;
		break;
		}
	    break;
	    case CHST_SECONDWORD:
	    switch ( c )
		{
		case ' ': case '\t':
		hc->checked_state = CHST_SECONDWS;
		break;
		case '\012': case '\015':
		/* The first line has only two words - an HTTP/0.9 request. */
		return GR_GOT_REQUEST;
		}
	    break;
	    case CHST_SECONDWS:
	    switch ( c )
		{
		case ' ': case '\t':
		break;
		case '\012': case '\015':
		hc->checked_state = CHST_BOGUS;
		return GR_BAD_REQUEST;
		default:
		hc->checked_state = CHST_THIRDWORD;
		break;
		}
	    break;
	    case CHST_THIRDWORD:
	    switch ( c )
		{
		case ' ': case '\t':
		hc->checked_state = CHST_THIRDWS;
		break;
		case '\012':
		hc->checked_state = CHST_LF;
		break;
		case '\015':
		hc->checked_state = CHST_CR;
		break;
		}
	    break;
	    case CHST_THIRDWS:
	    switch ( c )
		{
		case ' ': case '\t':
		break;
		case '\012':
		hc->checked_state = CHST_LF;
		break;
		case '\015':
		hc->checked_state = CHST_CR;
		break;
		default:
		hc->checked_state = CHST_BOGUS;
		return GR_BAD_REQUEST;
		}
	    break;
	    case CHST_LINE:
	    switch ( c )
		{
		case '\012':
		hc->checked_state = CHST_LF;
		break;
		case '\015':
		hc->checked_state = CHST_CR;
		break;
		}
	    break;
	    case CHST_LF:
	    switch ( c )
		{
		case '\012':
		/* Two newlines in a row - a blank line - end of request. */
		return GR_GOT_REQUEST;
		case '\015':
		hc->checked_state = CHST_CR;
		break;
		default:
		hc->checked_state = CHST_LINE;
		break;
		}
	    break;
	    case CHST_CR:
	    switch ( c )
		{
		case '\012':
		hc->checked_state = CHST_CRLF;
		break;
		case '\015':
		/* Two returns in a row - end of request. */
		return GR_GOT_REQUEST;
		default:
		hc->checked_state = CHST_LINE;
		break;
		}
	    break;
	    case CHST_CRLF:
	    switch ( c )
		{
		case '\012':
		/* Two newlines in a row - end of request. */
		return GR_GOT_REQUEST;
		case '\015':
		hc->checked_state = CHST_CRLFCR;
		break;
		default:
		hc->checked_state = CHST_LINE;
		break;
		}
	    break;
	    case CHST_CRLFCR:
	    switch ( c )
		{
		case '\012': case '\015':
		/* Two CRLFs or two CRs in a row - end of request. */
		return GR_GOT_REQUEST;
		default:
		hc->checked_state = CHST_LINE;
		break;
		}
	    break;
	    case CHST_BOGUS:
	    return GR_BAD_REQUEST;
	    }
	}
    return GR_NO_REQUEST;
    }


int
httpd_parse_request( httpd_conn* hc )
    {
    char* buf;
    char* method_str;
    char* url;
    char* protocol;
    char* reqhost;
    char* eol;
    char* cp;
    char* pi;

    hc->checked_idx = 0;	/* reset */
    method_str = bufgets( hc );
    url = strpbrk( method_str, " \t\012\015" );
    if ( url == (char*) 0 )
	{
	httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	return -1;
	}
    *url++ = '\0';
    url += strspn( url, " \t\012\015" );
    protocol = strpbrk( url, " \t\012\015" );
    if ( protocol == (char*) 0 )
	{
	protocol = "HTTP/0.9";
	hc->mime_flag = 0;
	}
    else
	{
	*protocol++ = '\0';
	protocol += strspn( protocol, " \t\012\015" );
	if ( *protocol != '\0' )
	    {
	    eol = strpbrk( protocol, " \t\012\015" );
	    if ( eol != (char*) 0 )
		*eol = '\0';
	    if ( strcasecmp( protocol, "HTTP/1.0" ) != 0 )
		hc->one_one = 1;
	    }
	}
    hc->protocol = protocol;

    /* Check for HTTP/1.1 absolute URL. */
    if ( strncasecmp( url, "http://", 7 ) == 0 )
	{
	if ( ! hc->one_one )
	    {
	    httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	    return -1;
	    }
	reqhost = url + 7;
	url = strchr( reqhost, '/' );
	if ( url == (char*) 0 )
	    {
	    httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	    return -1;
	    }
	*url = '\0';
	if ( strchr( reqhost, '/' ) != (char*) 0 || reqhost[0] == '.' )
	    {
	    httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	    return -1;
	    }
	httpd_realloc_str( &hc->reqhost, &hc->maxreqhost, strlen( reqhost ) );
	(void) strcpy( hc->reqhost, reqhost );
	*url = '/';
	}

    if ( *url != '/' )
	{
	httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	return -1;
	}

    if ( strcasecmp( method_str, httpd_method_str( METHOD_GET ) ) == 0 )
	hc->method = METHOD_GET;
    else if ( strcasecmp( method_str, httpd_method_str( METHOD_HEAD ) ) == 0 )
	hc->method = METHOD_HEAD;
    else if ( strcasecmp( method_str, httpd_method_str( METHOD_POST ) ) == 0 )
	hc->method = METHOD_POST;
    else
	{
	httpd_send_err( hc, 501, err501title, "", err501form, method_str );
	return -1;
	}

    hc->encodedurl = url;
    httpd_realloc_str(
	&hc->decodedurl, &hc->maxdecodedurl, strlen( hc->encodedurl ) );
    strdecode( hc->decodedurl, hc->encodedurl );

    httpd_realloc_str(
	&hc->origfilename, &hc->maxorigfilename, strlen( hc->decodedurl ) );
    (void) strcpy( hc->origfilename, &hc->decodedurl[1] );
    /* Special case for top-level URL. */
    if ( hc->origfilename[0] == '\0' )
	(void) strcpy( hc->origfilename, "." );

    /* Extract query string from encoded URL. */
    cp = strchr( hc->encodedurl, '?' );
    if ( cp != (char*) 0 )
	{
	++cp;
	httpd_realloc_str( &hc->query, &hc->maxquery, strlen( cp ) );
	(void) strcpy( hc->query, cp );
	/* Remove query from (decoded) origfilename. */
	cp = strchr( hc->origfilename, '?' );
	if ( cp != (char*) 0 )
	    *cp = '\0';
	}

    de_dotdot( hc->origfilename );
    if ( hc->origfilename[0] == '/' ||
	 ( hc->origfilename[0] == '.' && hc->origfilename[1] == '.' &&
	   ( hc->origfilename[2] == '\0' || hc->origfilename[2] == '/' ) ) )
	{
	httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	return -1;
	}

    if ( hc->mime_flag )
	{
	/* Read the MIME headers. */
	while ( ( buf = bufgets( hc ) ) != (char*) 0 )
	    {
	    if ( buf[0] == '\0' )
		break;
	    if ( strncasecmp( buf, "Referer:", 8 ) == 0 )
		{
		cp = &buf[8];
		cp += strspn( cp, " \t" );
		hc->referer = cp;
		}
	    else if ( strncasecmp( buf, "User-Agent:", 11 ) == 0 )
		{
		cp = &buf[11];
		cp += strspn( cp, " \t" );
		hc->useragent = cp;
		}
	    else if ( strncasecmp( buf, "Host:", 5 ) == 0 )
		{
		cp = &buf[5];
		cp += strspn( cp, " \t" );
		hc->hdrhost = cp;
		cp = strchr( hc->hdrhost, ':' );
		if ( cp != (char*) 0 )
		    *cp = '\0';
		if ( strchr( hc->hdrhost, '/' ) != (char*) 0 || hc->hdrhost[0] == '.' )
		    {
		    httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
		    return -1;
		    }
		}
	    else if ( strncasecmp( buf, "Accept:", 7 ) == 0 )
		{
		cp = &buf[7];
		cp += strspn( cp, " \t" );
		if ( hc->accept[0] != '\0' )
		    {
		    if ( strlen( hc->accept ) > 5000 )
			{
			continue;
			}
		    httpd_realloc_str(
			&hc->accept, &hc->maxaccept,
			strlen( hc->accept ) + 2 + strlen( cp ) );
		    (void) strcat( hc->accept, ", " );
		    }
		else
		    httpd_realloc_str(
			&hc->accept, &hc->maxaccept, strlen( cp ) );
		(void) strcat( hc->accept, cp );
		}
	    else if ( strncasecmp( buf, "Accept-Encoding:", 16 ) == 0 )
		{
		cp = &buf[16];
		cp += strspn( cp, " \t" );
		if ( hc->accepte[0] != '\0' )
		    {
		    if ( strlen( hc->accepte ) > 5000 )
			{
			continue;
			}
		    httpd_realloc_str(
			&hc->accepte, &hc->maxaccepte,
			strlen( hc->accepte ) + 2 + strlen( cp ) );
		    (void) strcat( hc->accepte, ", " );
		    }
		else
		    httpd_realloc_str(
			&hc->accepte, &hc->maxaccepte, strlen( cp ) );
		(void) strcpy( hc->accepte, cp );
		}
	    else if ( strncasecmp( buf, "Accept-Language:", 16 ) == 0 )
		{
		cp = &buf[16];
		cp += strspn( cp, " \t" );
		hc->acceptl = cp;
		}
	    else if ( strncasecmp( buf, "Cookie:", 7 ) == 0 )
		{
		cp = &buf[7];
		cp += strspn( cp, " \t" );
		hc->cookie = cp;
		}
	    else if ( strncasecmp( buf, "Range:", 6 ) == 0 )
		{
		/* Only support %d- and %d-%d, not %d-%d,%d-%d or -%d. */
		if ( strchr( buf, ',' ) == (char*) 0 )
		    {
		    char* cp_dash;
		    cp = strpbrk( buf, "=" );
		    if ( cp != (char*) 0 )
			{
			cp_dash = strchr( cp + 1, '-' );
			if ( cp_dash != (char*) 0 && cp_dash != cp + 1 )
			    {
			    *cp_dash = '\0';
			    hc->got_range = 1;
			    hc->first_byte_index = atoll( cp + 1 );
			    if ( hc->first_byte_index < 0 )
				hc->first_byte_index = 0;
			    if ( isdigit( (int) cp_dash[1] ) )
				{
				hc->last_byte_index = atoll( cp_dash + 1 );
				if ( hc->last_byte_index < 0 )
				    hc->last_byte_index = -1;
				}
			    }
			}
		    }
		}
	    else if ( strncasecmp( buf, "Content-Type:", 13 ) == 0 )
		{
		cp = &buf[13];
		cp += strspn( cp, " \t" );
		hc->contenttype = cp;
		}
	    else if ( strncasecmp( buf, "Content-Length:", 15 ) == 0 )
		{
		cp = &buf[15];
		hc->contentlength = atol( cp );
		}
	    else if ( strncasecmp( buf, "Authorization:", 14 ) == 0 )
		{
		cp = &buf[14];
		cp += strspn( cp, " \t" );
		hc->authorization = cp;
		}
	    else if ( strncasecmp( buf, "Connection:", 11 ) == 0 )
		{
		cp = &buf[11];
		cp += strspn( cp, " \t" );
		if ( strcasecmp( cp, "keep-alive" ) == 0 )
		    hc->keep_alive = 1;
		}
	    }
	}

    if ( hc->one_one )
	{
	/* Check that HTTP/1.1 requests specify a host, as required. */
	if ( hc->reqhost[0] == '\0' && hc->hdrhost[0] == '\0' )
	    {
	    httpd_send_err( hc, 400, httpd_err400title, "", httpd_err400form, "" );
	    return -1;
	    }

	/* If the client wants to do keep-alives, it might also be doing
	** pipelining.  There's no way for us to tell.  Since we don't
	** implement keep-alives yet, if we close such a connection there
	** might be unread pipelined requests waiting.  So, we have to
	** do a lingering close.
	*/
	if ( hc->keep_alive )
	    hc->should_linger = 1;
	}

    return 0;
    }


static char*
bufgets( httpd_conn* hc )
    {
    int i;
    char c;

    for ( i = hc->checked_idx; hc->checked_idx < hc->read_idx; ++hc->checked_idx )
	{
	c = hc->read_buf[hc->checked_idx];
	if ( c == '\012' || c == '\015' )
	    {
	    hc->read_buf[hc->checked_idx] = '\0';
	    ++hc->checked_idx;
	    if ( c == '\015' && hc->checked_idx < hc->read_idx &&
		 hc->read_buf[hc->checked_idx] == '\012' )
		{
		hc->read_buf[hc->checked_idx] = '\0';
		++hc->checked_idx;
		}
	    return &(hc->read_buf[i]);
	    }
	}
    return (char*) 0;
    }


static void
de_dotdot( char* file )
    {
    char* cp;
    char* cp2;
    int l;

    /* Collapse any multiple / sequences. */
    while ( ( cp = strstr( file, "//") ) != (char*) 0 )
	{
	for ( cp2 = cp + 2; *cp2 == '/'; ++cp2 )
	    continue;
	(void) strcpy( cp + 1, cp2 );
	}

    /* Remove leading ./ and any /./ sequences. */
    while ( strncmp( file, "./", 2 ) == 0 )
	(void) strcpy( file, file + 2 );
    while ( ( cp = strstr( file, "/./") ) != (char*) 0 )
	(void) strcpy( cp, cp + 2 );

    /* Alternate between removing leading ../ and removing xxx/../ */
    for (;;)
	{
	while ( strncmp( file, "../", 3 ) == 0 )
	    (void) strcpy( file, file + 3 );
	cp = strstr( file, "/../" );
	if ( cp == (char*) 0 )
	    break;
	for ( cp2 = cp - 1; cp2 >= file && *cp2 != '/'; --cp2 )
	    continue;
	(void) strcpy( cp2 + 1, cp + 4 );
	}

    /* Also elide any xxx/.. at the end. */
    while ( ( l = strlen( file ) ) > 3 &&
	    strcmp( ( cp = file + l - 3 ), "/.." ) == 0 )
	{
	for ( cp2 = cp - 1; cp2 >= file && *cp2 != '/'; --cp2 )
	    continue;
	if ( cp2 < file )
	    break;
	*cp2 = '\0';
	}
    }


void
httpd_close_conn( httpd_conn* hc, struct timeval* nowP )
    {
    if ( hc->conn_fd >= 0 )
	{
	(void) close( hc->conn_fd );
	hc->conn_fd = -1;
	}
    }

void
httpd_destroy_conn( httpd_conn* hc )
    {
    if ( hc->initialized )
	{
	free( (void*) hc->read_buf );
	free( (void*) hc->decodedurl );
	free( (void*) hc->origfilename );
	free( (void*) hc->encodings );
	free( (void*) hc->query );
	free( (void*) hc->accept );
	free( (void*) hc->accepte );
	free( (void*) hc->reqhost );
	free( (void*) hc->hostdir );
	free( (void*) hc->remoteuser );
	free( (void*) hc->response );
	free( (void*) hc->body_data );
#ifdef TILDE_MAP_2
	free( (void*) hc->altdir );
#endif /* TILDE_MAP_2 */
	hc->initialized = 0;
	}
    }

/* Figure out MIME encodings and type based on the filename.  Or not.
*/
static void
figure_mime( httpd_conn* hc )
    {
    hc->type = "text/plain; charset=%s";
    /* The last thing we do is actually generate the mime-encoding header. */
    hc->encodings[0] = '\0';
    }

char*
httpd_ntoa( httpd_sockaddr* saP )
    {
#ifdef USE_IPV6
    static char str[200];

    if ( getnameinfo( &saP->sa, sockaddr_len( saP ), str, sizeof(str), 0, 0, NI_NUMERICHOST ) != 0 )
	{
	str[0] = '?';
	str[1] = '\0';
	}
    else if ( IN6_IS_ADDR_V4MAPPED( &saP->sa_in6.sin6_addr ) && strncmp( str, "::ffff:", 7 ) == 0 )
	/* Elide IPv6ish prefix for IPv4 addresses. */
	(void) strcpy( str, &str[7] );

    return str;

#else /* USE_IPV6 */

    return inet_ntoa( saP->sa_in.sin_addr );

#endif /* USE_IPV6 */
    }


static int
sockaddr_check( httpd_sockaddr* saP )
    {
    switch ( saP->sa.sa_family )
	{
	case AF_INET: return 1;
#ifdef USE_IPV6
	case AF_INET6: return 1;
#endif /* USE_IPV6 */
	default:
	return 0;
	}
    }


static size_t
sockaddr_len( httpd_sockaddr* saP )
    {
    switch ( saP->sa.sa_family )
	{
	case AF_INET: return sizeof(struct sockaddr_in);
#ifdef USE_IPV6
	case AF_INET6: return sizeof(struct sockaddr_in6);
#endif /* USE_IPV6 */
	default:
	return 0;	/* shouldn't happen */
	}
    }


/* Some systems don't have snprintf(), so we make our own that uses
** either vsnprintf() or vsprintf().  If your system doesn't have
** vsnprintf(), it is probably vulnerable to buffer overruns.
** Upgrade!
*/
static int
my_snprintf( char* str, size_t size, const char* format, ... )
    {
    va_list ap;
    int r;

    va_start( ap, format );
#ifdef HAVE_VSNPRINTF
    r = vsnprintf( str, size, format, ap );
#else /* HAVE_VSNPRINTF */
    r = vsprintf( str, format, ap );
#endif /* HAVE_VSNPRINTF */
    va_end( ap );
    return r;
    }


#ifndef HAVE_ATOLL
static long long
atoll( const char* str )
    {
    long long value;
    long long sign;

    while ( isspace( *str ) )
	++str;
    switch ( *str )
	{
	case '-': sign = -1; ++str; break;
	case '+': sign = 1; ++str; break;
	default: sign = 1; break;
	}
    value = 0;
    while ( isdigit( *str ) )
	{
	value = value * 10 + ( *str - '0' );
	++str;
	}
    return sign * value;
    }
#endif /* HAVE_ATOLL */


/* Read the requested buffer completely, accounting for interruptions. */
int
httpd_read_fully( int fd, void* buf, size_t nbytes )
    {
    int nread;

    nread = 0;
    while ( nread < nbytes )
	{
	int r;

	r = read( fd, (char*) buf + nread, nbytes - nread );
	if ( r < 0 && ( errno == EINTR || errno == EAGAIN ) )
	    {
	    sleep( 1 );
	    continue;
	    }
	if ( r < 0 )
	    return r;
	if ( r == 0 )
	    break;
	nread += r;
	}

    return nread;
    }


/* Write the requested buffer completely, accounting for interruptions. */
int
httpd_write_fully( int fd, const void* buf, size_t nbytes )
    {
    int nwritten;

    nwritten = 0;
    while ( nwritten < nbytes )
	{
	int r;

	r = write( fd, (char*) buf + nwritten, nbytes - nwritten );
	if ( r < 0 && ( errno == EINTR || errno == EAGAIN ) )
	    {
	    sleep( 1 );
	    continue;
	    }
	if ( r < 0 )
	    return r;
	if ( r == 0 )
	    break;
	nwritten += r;
	}

    return nwritten;
    }

