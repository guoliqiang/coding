// Copyright 2006 Russ Cox.  All Rights Reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Hooks to make thttpd produce pprof-compatible outputs.

#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "google/malloc_extension.h"
#include "google/heap-profiler.h"
#include "google/profiler.h"

extern "C" {
#include "fdwatch.h"
#include "libhttpd.h"
#include "timers.h"
}

extern char *GetHeapProfile();

static char*
readfile(char *name, int *len)
{
	int fd;
	char *buf;
	int n, tot, m;

	if((fd = open(name, O_RDONLY)) < 0)
		return 0;
	m = (1<<20) - 1;
	if((buf = (char*)malloc(m + 1)) == 0){
		close(fd);
		return 0;
	}
	tot = 0;
	while(tot < m && ((n = read(fd, buf+tot, m - tot)) > 0))
		tot += n;
	buf[tot] = 0;
	if(len)
		*len = tot;
	close(fd);
	return buf;
}

static char profilefile[100];

static int
send_error(httpd_conn *hc)
{
	httpd_send_err( hc, 500, "Unepxected error", "", 
		"Unexpected error happened.", hc->encodedurl );
	return -1;
}

static int
send_prof_and_maps(httpd_conn *hc, char *prof, int len)
{
	if(prof == 0)
		return send_error(hc);
	if(len < 0)
		len = strlen(prof);
	
	char *maps = readfile("/proc/self/maps", 0);
	hc->body_data = (char*)malloc(len + 40 + (maps ? strlen(maps) : 0));
	if(hc->body_data == 0){
		fprintf(stderr, "no data\n");
		free(prof);
		free(maps);
		return send_error(hc);
	}
	memmove(hc->body_data, prof, len);
	if(maps){
		strcpy(hc->body_data + len, "\nMAPPED_LIBRARIES:\n");
		strcat(hc->body_data + len, maps);
		len += strlen(hc->body_data + len);
	}
	free(prof);
	free(maps);
	hc->bytes_to_send = len;
	return 0;
}

int
httpd_start_request(httpd_conn *hc, struct timeval *now)
{
	char *prof;
	int len;
	std::string s;

	len = -1;
	if(strcmp(hc->origfilename, "pprof/cmdline") == 0){
		free(hc->body_data);
		hc->body_data = readfile("/proc/self/cmdline", &len);
		if(hc->body_data == 0)
			return send_error(hc);
		hc->bytes_to_send = len;
		return 0;
	}

	if(strcmp(hc->origfilename, "pprof/heap") == 0){
		// TODO: Could call GetHeapProfile first,
		// and only fall back to GetHeapSample if 
		// the former returns an empty string?
		MallocExtension::instance()->GetHeapSample(&s);
		prof = strdup(s.c_str());
		return send_prof_and_maps(hc, prof, -1);
	}
	
	if(strcmp(hc->origfilename, "pprof/growth") == 0){
		std::string s;
		MallocExtension::instance()->GetHeapGrowthStacks(&s);
		prof = strdup(s.c_str());
		return send_prof_and_maps(hc, prof, -1);
	}
	
	if(strcmp(hc->origfilename, "pprof/heapstats") == 0){
		prof = (char*)malloc(1<<20);
		prof[0] = 0;
		MallocExtension::instance()->GetStats(prof, 1<<20);
		free(hc->body_data);
		hc->body_data = prof;
		hc->bytes_to_send = strlen(prof);
		return 0;
	}

	if(strcmp(hc->origfilename, "pprof/profile") == 0){
		snprintf(profilefile, sizeof profilefile, "/var/tmp/pprof.profile.%d", getpid());
		int n = 30;
		char *p = strstr(hc->query, "seconds=");
		if(p)
			n = atoi(p+8);
		ProfilerStart(profilefile);
		httpd_conn_sleep(hc, n);
		return 0;
	}
	
	httpd_send_err( hc, 404, httpd_err404title, "", 
		httpd_err404form, hc->encodedurl );
	return -1;
}

int
httpd_continue_request(httpd_conn *hc, struct timeval *now)
{
	char *prof;
	int len;
	
	ProfilerStop();
	ProfilerFlush();
	prof = readfile(profilefile, &len);
	unlink(profilefile);
	return send_prof_and_maps(hc, prof, len);
}

