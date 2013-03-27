/*
 * $XTermId: trace.c,v 1.6 2011/10/28 01:01:43 tom Exp $
 *
 * Copyright 2010,2011 by Thomas E. Dickey
 *
 * All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of the above listed
 * copyright holder(s) not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.
 *
 * THE ABOVE LISTED COPYRIGHT HOLDER(S) DISCLAIM ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <config.h>

#include <unistd.h>
#include <trace.h>

void
Trace(const char *fmt,...)
{
    static pid_t first_pid;
    static FILE *fp = 0;
    va_list ap;
    if (first_pid == 0)
	first_pid = getpid();
    if (fp == 0)
	fp = fopen("Trace.out", "w");
    va_start(ap, fmt);
    if (getpid() != first_pid)
	fprintf(fp, "child:");
    vfprintf(fp, fmt, ap);
    fflush(fp);
    va_end(ap);
}
