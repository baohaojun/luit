/*
 * $XTermId: trace.h,v 1.7 2011/10/28 01:01:50 tom Exp $
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
#ifndef TRACE_H
#define TRACE_H

#include <luit.h>

void Trace(const char *fmt,...) GCC_PRINTFLIKE(1,2);

#ifdef OPT_TRACE
#define TRACE(params) Trace params
#if OPT_TRACE > 1
#define TRACE2(params) Trace params
#else
#define TRACE2(params)		/* nothing */
#endif
#else
#define TRACE(params)		/* nothing */
#define TRACE2(params)		/* nothing */
#endif

#define TRACE_ERR(msg) TRACE((msg ": %s\n", strerror(errno)))

#endif /* TRACE_H */
