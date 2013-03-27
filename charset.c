/* $XTermId: charset.c,v 1.76 2013/02/17 21:47:50 tom Exp $ */

/*
Copyright 2010-2012,2013 by Thomas E. Dickey
Copyright (c) 2001 by Juliusz Chroboczek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <charset.h>

#include <sys.h>
#include <parser.h>

static unsigned int
IdentityRecode(unsigned int n, const CharsetRec * self GCC_UNUSED)
{
    return n;
}

static int
NullReverse(unsigned int n GCC_UNUSED, const CharsetRec * self GCC_UNUSED)
{
    return -1;
}

static const CharsetRec Unknown94Charset =
{"Unknown (94)", T_94, 0, IdentityRecode, NullReverse, 0, 0, 0, 0, 0, 0};
static const CharsetRec Unknown96Charset =
{"Unknown (96)", T_96, 0, IdentityRecode, NullReverse, 0, 0, 0, 0, 0, 0};
static const CharsetRec Unknown9494Charset =
{"Unknown (94x94)", T_9494, 0, IdentityRecode, NullReverse, 0, 0, 0, 0, 0, 0};
static const CharsetRec Unknown9696Charset =
{"Unknown (96x96)", T_9696, 0, IdentityRecode, NullReverse, 0, 0, 0, 0, 0, 0};

/*
 * The "name" given is useful on the command-line.
 * The "xlfd" column is the name given in the X font-encoding package, where
 * that has a corresponding built-in table or ".enc" file.  Other names (such
 * as "dec-technical" are chosen for consistency with other entries in the
 * table.
 */
/* *INDENT-OFF* */
static FontencCharsetRec fontencCharsets[] =
{
    {"ISO 646 (1973)", T_94,    '@', "iso646.1973-0",    0x00,   0, 0},
    {"ASCII",          T_94,    'B', "iso8859-1",        0x00,   0, 0},	/* bug */
    {"JIS X 0201:GL",  T_94,    'J', "jisx0201.1976-0",  0x00,   0, 0},
    {"JIS X 0201:GR",  T_94,    'I', "jisx0201.1976-0",  0x80,   0, 0},
    {"DEC Special",    T_94,    '0', "dec-special",      0x00,   0, 0},
    {"DEC Technical",  T_94,    '>', "dec-technical",    0x00,   0, 0},

    {"ISO 8859-1",     T_96,    'A', "iso8859-1",        0x80,   0, 0},
    {"ISO 8859-2",     T_96,    'B', "iso8859-2",        0x80,   0, 0},
    {"ISO 8859-3",     T_96,    'C', "iso8859-3",        0x80,   0, 0},
    {"ISO 8859-4",     T_96,    'D', "iso8859-4",        0x80,   0, 0},
    {"ISO 8859-5",     T_96,    'L', "iso8859-5",        0x80,   0, 0},
    {"ISO 8859-6",     T_96,    'G', "iso8859-6",        0x80,   0, 0},
    {"ISO 8859-7",     T_96,    'F', "iso8859-7",        0x80,   0, 0},
    {"ISO 8859-8",     T_96,    'H', "iso8859-8",        0x80,   0, 0},
    {"ISO 8859-9",     T_96,    'M', "iso8859-9",        0x80,   0, 0},
    {"ISO 8859-10",    T_96,    'V', "iso8859-10",       0x80,   0, 0},
    {"ISO 8859-11",    T_96,    'T', "iso8859-11",       0x80,   0, 0},
    {"TIS 620",        T_96,    'T', "iso8859-11",       0x80,   0, 0},
    {"ISO 8859-13",    T_96,    'Y', "iso8859-13",       0x80,   0, 0},
    {"ISO 8859-14",    T_96,    '_', "iso8859-14",       0x80,   0, 0},
    {"ISO 8859-15",    T_96,    'b', "iso8859-15",       0x80,   0, 0},
    {"ISO 8859-16",    T_96,    'f', "iso8859-16",       0x80,   0, 0},
    {"KOI8-E",         T_96,    '@', "koi8-e",           0x80,   0, 0},
    {"TCVN",           T_96,    'Z', "tcvn-0",           0x80,   0, 0},

    {"GB 2312",        T_9494,  'A', "gb2312.1980-0",    0x0000, 0, 0},
    {"JIS X 0208",     T_9494,  'B', "jisx0208.1990-0",  0x0000, 0, 0},
    {"KSC 5601",       T_9494,  'C', "ksc5601.1987-0",   0x0000, 0, 0},
    {"JIS X 0212",     T_9494,  'D', "jisx0212.1990-0",  0x0000, 0, 0},

    {"GB 2312",        T_9696,  'A', "gb2312.1980-0",    0x0000, 0, 0},
    {"JIS X 0208",     T_9696,  'B', "jisx0208.1990-0",  0x0000, 0, 0},
    {"KSC 5601",       T_9696,  'C', "ksc5601.1987-0",   0x0000, 0, 0},
    {"JIS X 0212",     T_9696,  'D', "jisx0212.1990-0",  0x0000, 0, 0},

    {"CNS11643-1",     T_9494,  'G', "cns11643-1",       0x0000, 0, 0},
    {"CNS11643-2",     T_9494,  'H', "cns11643-2",       0x0000, 0, 0},
    {"CNS11643-3",     T_9494,  'I', "cns11643-3",       0x0000, 0, 0},

    {"KOI8-R",         T_128,   0,   "koi8-r",           0x80,   0, 0},
    {"KOI8-U",         T_128,   0,   "koi8-u",           0x80,   0, 0},
    {"KOI8-RU",        T_128,   0,   "koi8-ru",          0x80,   0, 0},
    {"CP 1250",        T_128,   0,   "microsoft-cp1250", 0x80,   0, 0},
    {"CP 1251",        T_128,   0,   "microsoft-cp1251", 0x80,   0, 0},
    {"CP 1252",        T_128,   0,   "microsoft-cp1252", 0x80,   0, 0},
    {"CP 1255",        T_128,   0,   "microsoft-cp1255", 0x80,   0, 0},

    {"CP 437",         T_128,   0,   "ibm-cp437",        0x80,   0, 0},
    {"CP 850",         T_128,   0,   "ibm-cp850",        0x80,   0, 0},
    {"CP 852",         T_128,   0,   "ibm-cp852",        0x80,   0, 0},
    {"CP 866",         T_128,   0,   "ibm-cp866",        0x80,   0, 0},

    {"Big 5",          T_94192, 0,   "big5.eten-0",      0x8000, 0, 0},
    {0,                0,       0,   0,                  0,      0, 0}
};
/* *INDENT-ON* */

typedef struct _OtherCharset {
    const char *name;
    int (*init) (OtherStatePtr);
    unsigned int (*mapping) (unsigned int, OtherStatePtr);
    unsigned int (*reverse) (unsigned int, OtherStatePtr);
    int (*stack) (unsigned, OtherStatePtr);
} OtherCharsetRec, *OtherCharsetPtr;
/* *INDENT-OFF* */

static const OtherCharsetRec otherCharsets[] =
{
    {"GBK",        init_gbk,     mapping_gbk,     reverse_gbk,     stack_gbk},
    {"UTF-8",      init_utf8,    mapping_utf8,    reverse_utf8,    stack_utf8},
    {"SJIS",       init_sjis,    mapping_sjis,    reverse_sjis,    stack_sjis},
    {"BIG5-HKSCS", init_hkscs,   mapping_hkscs,   reverse_hkscs,   stack_hkscs},
    {"GB18030",    init_gb18030, mapping_gb18030, reverse_gb18030, stack_gb18030},
    {0, 0, 0, 0, 0}
};
/* *INDENT-ON* */

#define lcIgnore(c) \
	(c && (isspace(UChar(c)) || c == '-' || c == '_' || c == '/'))

int
lcStrCmp(const char *s, const char *t)
{
    int result = 0;

    while (*s || *t) {
	if (lcIgnore(*s))
	    s++;
	else if (lcIgnore(*t))
	    t++;
	else if (*s && *t && tolower(UChar(*s)) == tolower(UChar(*t))) {
	    s++;
	    t++;
	} else {
	    result = 1;
	    break;
	}
    }
    return result;
}

static int
compare1(const char *s, const char *t, size_t n)
{
    int result = 0;

    while (n && (*s || *t)) {
	--n;
	if (lcIgnore(*s))
	    s++;
	else if (lcIgnore(*t))
	    t++;
	else if (*s && *t && tolower(UChar(*s)) == tolower(UChar(*t))) {
	    s++;
	    t++;
	} else {
	    result = 1;
	    break;
	}
    }
    return result;
}

static unsigned int
FontencCharsetRecode(unsigned int n, const CharsetRec * self)
{
    const FontencCharsetRec *fc = (const FontencCharsetRec *) (self->data);
    unsigned result;

    result = MapCodeValue(n + fc->shift, fc->mapping);

    TRACE(("FontencCharsetRecode %#x ->%#x%s\n",
	   n,
	   result,
	   (n != result) ? " map" : ""));

    return result;
}

static int
FontencCharsetReverse(unsigned int i, const CharsetRec * self)
{
    const FontencCharsetRec *fc = (const FontencCharsetRec *) (self->data);
    unsigned n;
    int result = -1;

    n = fc->reverse->reverse(i, fc->reverse->data);
    if (n != 0 && n >= fc->shift) {
	n -= fc->shift;

#define IS_GL(n) ((n) >= 0x20 && (n) < 0x80)
	switch (self->type) {
	case T_94:
	case T_96:
	    if (IS_GL(n))
		result = (int) n;
	    break;
	case T_128:
	    if (n < 0x80)
		result = (int) n;
	    break;
	case T_9494:
	case T_9696:
	    if (IS_GL(n >> 8) && IS_GL(n & 0xFF))
		result = (int) n;
	    break;
	case T_94192:
	    if (IS_GL(n >> 8) && IS_GL(n & 0x7F))
		result = (int) n;
	    break;
	default:
	    abort();
	    /* NOTREACHED */
	}
#undef IS_GL
    }

    TRACE(("FontencCharsetReverse %#x ->%#x%s\n",
	   i,
	   result,
	   ((int) i != result) ? " map" : ""));

    return result;
}

static CharsetPtr cachedCharsets = NULL;

static CharsetPtr
getCachedCharset(unsigned final, int type, const char *name)
{
    CharsetPtr c;
    for (c = cachedCharsets; c; c = c->next) {
	if (((c->type == type && c->final == final) ||
	     (name && !lcStrCmp(c->name, name))) &&
	    (c->type != T_FAILED))
	    return c;
    }
    return NULL;
}

static void
cacheCharset(CharsetPtr c)
{
    c->next = cachedCharsets;
    cachedCharsets = c;
    VERBOSE(2, ("cachedCharset '%s'\n", c->name));
}

#ifdef USE_ICONV
static US_SIZE
cpSize(FontencCharsetPtr fc)
{
    US_SIZE result = usANY;

    switch (fc->type) {
    case T_94:
    case T_96:
    case T_128:
	result = us8BIT;
	break;
    case T_9494:
    case T_9696:
    case T_94192:
	result = us16BIT;
	break;
    }
    return result;
}
#endif

static CharsetPtr
getFontencCharset(unsigned final, int type, const char *name)
{
    FontencCharsetPtr fc;
    CharsetPtr c = NULL;
    FontMapPtr mapping;
    FontMapReversePtr reverse;
    CharsetPtr result = NULL;

    TRACE(("getFontencCharset(final %#x, type %d, name %s)\n", final, type, name));
    fc = fontencCharsets;
    while (fc->name) {
	if (((fc->type == type && fc->final == final) ||
	     (name && !lcStrCmp(fc->name, name))) &&
	    (fc->type != T_FAILED))
	    break;
	fc++;
    }

    if (!fc->name) {
	VERBOSE(2, ("...no match for '%s' in FontEnc charsets\n", NonNull(name)));
    } else if ((c = TypeCalloc(CharsetRec)) == 0) {
	VERBOSE(2, ("malloc failed\n"));
    } else if ((mapping = LookupMapping(fc->xlfd, cpSize(fc))) == NULL) {
	VERBOSE(2, ("...lookup mapping %s (%s) failed\n", NonNull(name), fc->xlfd));
	fc->type = T_FAILED;
    } else if ((reverse = LookupReverse(mapping)) == NULL) {
	VERBOSE(2, ("...lookup reverse %s failed\n", NonNull(name)));
	fc->type = T_FAILED;
    } else {
	fc->mapping = mapping;
	fc->reverse = reverse;

	c->name = fc->name;
	c->type = fc->type;
	c->final = fc->final;
	c->recode = FontencCharsetRecode;
	c->reverse = FontencCharsetReverse;
	c->data = fc;

	cacheCharset(c);
	result = c;
    }

    if (result == NULL && c != NULL)
	free(c);

    return result;
}

static const OtherCharsetRec *
findOtherCharset(const char *name)
{
    const OtherCharsetRec *fc;
    fc = otherCharsets;
    while (fc->name) {
	if (name && !lcStrCmp(fc->name, name))
	    break;
	fc++;
    }
    return fc;
}

int
isOtherCharset(const char *name)
{
    const OtherCharsetRec *fc = findOtherCharset(name);
    int result = (fc->name != 0);
    if (!result) {
	result = (!lcStrCmp(name, "Big5") ||
		  !lcStrCmp(name, "JOHAB"));
    }
    return result;
}

static CharsetPtr
getOtherCharset(const char *name)
{
    const OtherCharsetRec *fc;
    CharsetPtr c = NULL;
    OtherStatePtr s = NULL;
    CharsetPtr result = NULL;

    fc = findOtherCharset(name);
    if (!fc->name) {
	VERBOSE(2, ("...no match for '%s' in Other charsets\n", NonNull(name)));
    } else if ((c = TypeCalloc(CharsetRec)) == NULL) {
	VERBOSE(2, ("malloc failed\n"));
    } else if ((s = TypeCalloc(OtherState)) == NULL) {
	VERBOSE(2, ("malloc failed\n"));
    } else {
	c->name = fc->name;
	c->type = T_OTHER;
	c->final = 0;
	c->data = fc;
	c->other_recode = fc->mapping;
	c->other_reverse = fc->reverse;
	c->other_stack = fc->stack;
	c->other_aux = s;

	if (!fc->init(s)) {
	    VERBOSE(2, ("...initialization %s failed\n", NonNull(name)));
	    c->type = T_FAILED;
	} else {
	    cacheCharset(c);
	    result = c;
	}
    }

    if (result == NULL) {
	if (c != NULL)
	    free(c);
	if (s != NULL)
	    free(s);
    }

    return result;
}

const CharsetRec *
getUnknownCharset(int type)
{
    TRACE(("getUnknownCharset(%d)\n", type));
    switch (type) {
    case T_94:
	VERBOSE(2, ("using unknown 94-charset\n"));
	return &Unknown94Charset;
    case T_96:
	VERBOSE(2, ("using unknown 96-charset\n"));
	return &Unknown96Charset;
    case T_9494:
	VERBOSE(2, ("using unknown 9494-charset\n"));
	return &Unknown9494Charset;
    case T_9696:
	VERBOSE(2, ("using unknown 9696-charset\n"));
	return &Unknown9696Charset;
    default:
	VERBOSE(2, ("using unknown 94-charset\n"));
	return &Unknown94Charset;
    }
}

const CharsetRec *
getCharset(unsigned final, int type)
{
    const CharsetRec *c;

    TRACE(("getCharset(final=%c, type=%d)\n", final, type));
    c = getCachedCharset(final, type, NULL);
    if (c)
	return c;

    c = getFontencCharset(final, type, NULL);
    if (c)
	return c;

    return getUnknownCharset(type);
}

const CharsetRec *
getCharsetByName(const char *name)
{
    const CharsetRec *c;

    VERBOSE(2, ("getCharsetByName(%s)\n", NonNull(name)));
    TRACE(("getCharsetByName(%s)\n", NonNull(name)));

    if (name == NULL)
	return getUnknownCharset(T_94);

    c = getCachedCharset(0, 0, name);
    if (c)
	return c;

    c = getFontencCharset(0, 0, name);
    if (c)
	return c;

    c = getOtherCharset(name);
    if (c)
	return c;

    return getUnknownCharset(T_94);
}
/* *INDENT-OFF* */
static const LocaleCharsetRec localeCharsets[] =
{
    {"C",          0, 2, "ASCII", NULL,         "ISO 8859-1",    NULL,         NULL},
    {"POSIX",      0, 2, "ASCII", NULL,         "ISO 8859-1",    NULL,         NULL},
    {"US-ASCII",   0, 2, "ASCII", NULL,         "ISO 8859-1",    NULL,         NULL},

    {"ISO8859-1",  0, 2, "ASCII", NULL,         "ISO 8859-1",    NULL,         NULL},
    {"ISO8859-2",  0, 2, "ASCII", NULL,         "ISO 8859-2",    NULL,         NULL},
    {"ISO8859-3",  0, 2, "ASCII", NULL,         "ISO 8859-3",    NULL,         NULL},
    {"ISO8859-4",  0, 2, "ASCII", NULL,         "ISO 8859-4",    NULL,         NULL},
    {"ISO8859-5",  0, 2, "ASCII", NULL,         "ISO 8859-5",    NULL,         NULL},
    {"ISO8859-6",  0, 2, "ASCII", NULL,         "ISO 8859-6",    NULL,         NULL},
    {"ISO8859-7",  0, 2, "ASCII", NULL,         "ISO 8859-7",    NULL,         NULL},
    {"ISO8859-8",  0, 2, "ASCII", NULL,         "ISO 8859-8",    NULL,         NULL},
    {"ISO8859-9",  0, 2, "ASCII", NULL,         "ISO 8859-9",    NULL,         NULL},
    {"ISO8859-10", 0, 2, "ASCII", NULL,         "ISO 8859-10",   NULL,         NULL},
    {"ISO8859-11", 0, 2, "ASCII", NULL,         "ISO 8859-11",   NULL,         NULL},
    {"TIS620",     0, 2, "ASCII", NULL,         "ISO 8859-11",   NULL,         NULL},
    {"ISO8859-13", 0, 2, "ASCII", NULL,         "ISO 8859-13",   NULL,         NULL},
    {"ISO8859-14", 0, 2, "ASCII", NULL,         "ISO 8859-14",   NULL,         NULL},
    {"ISO8859-15", 0, 2, "ASCII", NULL,         "ISO 8859-15",   NULL,         NULL},
    {"ISO8859-16", 0, 2, "ASCII", NULL,         "ISO 8859-16",   NULL,         NULL},

    {"KOI8-E",     0, 2, "ASCII", NULL,         "KOI8-E",        NULL,         NULL},
    {"KOI8-R",     0, 2, "ASCII", NULL,         "KOI8-R",        NULL,         NULL},
    {"KOI8-U",     0, 2, "ASCII", NULL,         "KOI8-U",        NULL,         NULL},
    {"KOI8-RU",    0, 2, "ASCII", NULL,         "KOI8-RU",       NULL,         NULL},
    {"CP1250",     0, 2, "ASCII", NULL,         "CP 1250",       NULL,         NULL},
    {"CP1251",     0, 2, "ASCII", NULL,         "CP 1251",       NULL,         NULL},
    {"CP1252",     0, 2, "ASCII", NULL,         "CP 1252",       NULL,         NULL},
    {"CP1255",     0, 2, "ASCII", NULL,         "CP 1255",       NULL,         NULL},
    {"CP437",      0, 2, "ASCII", NULL,         "CP 437",        NULL,         NULL},
    {"CP850",      0, 2, "ASCII", NULL,         "CP 850",        NULL,         NULL},
    {"CP852",      0, 2, "ASCII", NULL,         "CP 852",        NULL,         NULL},
    {"CP866",      0, 2, "ASCII", NULL,         "CP 866",        NULL,         NULL},
    {"TCVN",       0, 2, "ASCII", NULL,         "TCVN",          NULL,         NULL},

    {"GB2312",     0, 1, "ASCII", "GB 2312",    NULL,            NULL,         NULL},
    {"eucJP",      0, 1, "ASCII", "JIS X 0208", "JIS X 0201:GR", "JIS X 0212", NULL},
    {"eucKR",      0, 1, "ASCII", "KSC 5601",   NULL,            NULL,         NULL},
    {"eucCN",      0, 1, "ASCII", "GB 2312",    NULL,            NULL,         NULL},
    {"eucTW",      0, 1, "ASCII", "CNS11643-1", "CNS11643-2",    "CNS11643-3", NULL},
    {"Big5",       0, 1, "ASCII", "Big 5",      NULL,            NULL,         NULL},

    {"gbk",        0, 1, NULL,    NULL,         NULL,            NULL,         "GBK"},
    {"UTF-8",      0, 1, NULL,    NULL,         NULL,            NULL,         "UTF-8"},
    {"SJIS",       0, 1, NULL,    NULL,         NULL,            NULL,         "SJIS"},
    {"Big5-HKSCS", 0, 1, NULL,    NULL,         NULL,            NULL,         "BIG5-HKSCS"},
    {"gb18030",    0, 1, NULL,    NULL,         NULL,            NULL,         "GB18030"},

    {0,            0, 0, 0,       0,            0,               0,            0}
};
/* *INDENT-ON* */

void
reportCharsets(void)
{
    const LocaleCharsetRec *p;
    FontencCharsetPtr q;
    printf("Known locale encodings:\n\n");
    for (p = localeCharsets; p->name; p++) {
	if (p->other) {
	    printf("  %s (non-ISO-2022 encoding)\n", p->other);
	    continue;
	}
	printf("  %s: GL -> G%d, GR -> G%d", p->name, p->gl, p->gr);
	if (p->g0)
	    printf(", G0: %s", p->g0);
	if (p->g1)
	    printf(", G1: %s", p->g1);
	if (p->g2)
	    printf(", G2: %s", p->g2);
	if (p->g3)
	    printf(", G3: %s", p->g3);
	printf("\n");
    }

    printf("\n\nKnown charsets (not all may be available):\n\n");
    for (q = fontencCharsets; q->name; q++) {
	const char *csize = "";

	printf("  %s", q->name);
	switch (q->type) {
	case T_94:
	    csize = "94 codes";
	    break;
	case T_96:
	    csize = "96 codes";
	    break;
	case T_128:
	    csize = "128 codes";
	    break;
	case T_9494:
	    csize = "94x94 codes";
	    break;
	case T_9696:
	    csize = "94x96 codes";
	    break;
	case T_94192:
	    csize = "94x192 codes";
	    break;
	}
	if (q->final) {
	    printf(" (ISO 2022%s%s)", *csize ? ", " : "", csize);
	} else if (*csize) {
	    printf(" (%s)", csize);
	}
	printf("\n");
    }
}

#ifdef USE_ICONV
static LocaleCharsetRec fakeLocaleCharset;

static const LocaleCharsetRec *
findLocaleByCharset(const char *charset)
{
    const LocaleCharsetRec *lc;
    const LocaleCharsetRec *result = 0;

    for (lc = localeCharsets; lc->name != 0; ++lc) {
	if (lc->g1 == 0 && lc->g2 == 0)
	    continue;
	if ((lc->g3 != 0 && !lcStrCmp(charset, lc->g3))
	    || (lc->g2 != 0 && !lcStrCmp(charset, lc->g2))
	    || (lc->g1 != 0 && !lcStrCmp(charset, lc->g1))) {
	    result = lc;
	    break;
	}
    }
    TRACE(("findLocaleByCharset(%s) ->%s\n",
	   charset, result ? result->name : "?"));
    return result;
}

static const LocaleCharsetRec *
closestLocaleCharset(FontEncPtr enc)
{
    const LocaleCharsetRec *result = 0;

    if (enc != 0) {
	const FontencCharsetRec *fc = getFontencByName(enc->name);
	if (fc != 0) {
	    result = findLocaleByCharset(fc->name);
	} else {
	    result = findLocaleByCharset(enc->name);
	}
    }
    TRACE(("closestLocaleCharset(%s) ->%s\n",
	   enc ? enc->name : "?",
	   result ? result->name : "?"));
    return result;
}

static int
canFakeLocaleCharset(FontEncPtr enc)
{
    int result = 0;
    if (enc != 0
	&& enc->size <= 256
	&& enc->row_size == 0) {
	result = 1;
    }
    return result;
}
#endif

static const LocaleCharsetRec *
findLocaleCharset(const char *charset)
{
    const LocaleCharsetRec *p;
    const LocaleCharsetRec *result = 0;

    for (p = localeCharsets; p->name; p++) {
	if (lcStrCmp(p->name, charset) == 0) {
	    result = p;
	    break;
	}
    }
#ifdef USE_ICONV
    /*
     * The table is useful, but not complete.
     * If we can find a mapping for an 8-bit encoding, fake a table entry.
     */
    if (result == 0) {
	FontEncPtr enc = luitGetFontEnc(charset,
					(UM_MODE) ((int) umICONV
						   | (int) umFONTENC
						   | (int) umBUILTIN));
	if ((result = closestLocaleCharset(enc)) != 0) {
	    TRACE(("...matched a LocaleCharset record for %s\n", charset));
	} else if (canFakeLocaleCharset(enc)) {
	    LocaleCharsetRec *temp = &fakeLocaleCharset;

	    TRACE(("...fake a LocaleCharset record for %s\n", charset));

	    memset(temp, 0, sizeof(*temp));
	    temp->name = strmalloc(charset);
	    temp->gr = 2;
	    temp->g0 = "ASCII";
	    temp->g2 = temp->name;
	    result = temp;

	} else {
	    TRACE(("...do not know how to fake LocaleCharset for %s\n", charset));
	}
	luitFreeFontEnc(enc);
    }
#endif
    return result;
}

static const LocaleCharsetRec *
matchLocaleCharset(const char *charset)
{
    static const struct {
	const char *source;
	const char *target;
	size_t source_len;
	size_t target_len;
    } prefixes[] = {
#define DATA(source, target) { source, target, sizeof(source)-1, sizeof(target)-1 }
	DATA("ISO-", "ISO "),
	    DATA("DEC ", "DEC-"),
	    DATA("IBM-CP", "CP "),	/* "ibm-cp866" -> "cp 866" (iconv) */
	    DATA("IBM", "CP "),
	    DATA("MICROSOFT-CP", "CP "),
	    DATA("MICROSOFT", "CP "),
	    DATA("CP-", "CP "),
	    DATA("ANSI", "CP "),	/* e.g., Solaris ANSI1251 */
#undef DATA
    };

    const LocaleCharsetRec *p = 0;

    TRACE(("matchLocaleCharset(%s)\n", charset));
    if (*charset != '\0') {
	char *euro;
	char source[MAX_KEYWORD_LENGTH + 1];

	sprintf(source, "%.*s", MAX_KEYWORD_LENGTH, charset);
	if ((euro = strrchr(source, '@')) != 0 && !strcmp(euro, "@euro")) {
	    Warning("the euro character may not be supported\n");
	    *euro = 0;
	}

	p = findLocaleCharset(source);

	if (p == 0) {
	    size_t have = strlen(source);
	    size_t n;
	    char target[MAX_KEYWORD_LENGTH + 80];

	    for (n = 0; n < SizeOf(prefixes); ++n) {
		if (have > prefixes[n].source_len
		    && have < MAX_KEYWORD_LENGTH
		    && !compare1(source,
				 prefixes[n].source,
				 prefixes[n].source_len)) {
		    strcpy(target, prefixes[n].target);
		    strcpy(target + prefixes[n].target_len,
			   source + prefixes[n].source_len);
		    if ((p = findLocaleCharset(target)) != 0) {
			break;
		    }
		}
	    }
	}
    }
    return p;
}

int
getLocaleState(const char *locale,
	       const char *charset,
	       int *gl_return, int *gr_return,
	       const CharsetRec * *g0_return,
	       const CharsetRec * *g1_return,
	       const CharsetRec * *g2_return,
	       const CharsetRec * *g3_return,
	       const CharsetRec * *other_return)
{
    int result = 0;
    char *resolved = 0;
    const LocaleCharsetRec *p;

    TRACE(("getLocaleState(locale=%s, charset=%s)\n", locale, NonNull(charset)));
    if (!charset) {
	if (ignore_locale) {
	    charset = locale;
	} else {
	    resolved = resolveLocale(locale);
	    if (!resolved)
		return -1;
	    if ((charset = strrchr(resolved, '.')) != 0) {
		charset++;
	    } else {
		charset = resolved;
	    }
	}
    }

    if ((p = matchLocaleCharset(charset)) != 0) {
	*gl_return = p->gl;
	*gr_return = p->gr;
	*g0_return = getCharsetByName(p->g0);
	*g1_return = getCharsetByName(p->g1);
	*g2_return = getCharsetByName(p->g2);
	*g3_return = getCharsetByName(p->g3);
	if (p->other)
	    *other_return = getCharsetByName(p->other);
	else
	    *other_return = NULL;
    } else {
	result = -1;
    }

    if (resolved != 0)
	free(resolved);

    TRACE(("...getLocaleState ->%d\n", result));
    return result;
}

#ifdef USE_ICONV
/*
 * Given either a charset name, or the xlfd field (which is more likely to
 * match iconv), return a pointer to the entry in fontencCharsets which matches.
 */
const FontencCharsetRec *
getFontencByName(const char *encoding_name)
{
    const FontencCharsetRec *result = 0;
    const FontencCharsetRec *fc;
    char *gr_special;

    for (fc = fontencCharsets; fc->name != 0; ++fc) {
	if (!lcStrCmp(encoding_name, fc->name)
	    || (strstr(fc->name, ":GL") == 0
		&& !lcStrCmp(encoding_name, fc->xlfd))) {
	    result = fc;
	    break;
	}
    }

    /*
     * Luit treats ":GR" specially in its charset tables, essentially to
     * distinguish the case it uses for JIS X 201 from other possibilities.
     */
    if (result == 0
	&& strchr(encoding_name, ':') == 0
	&& (gr_special = malloc(strlen(encoding_name) + 4)) != 0) {
	sprintf(gr_special, "%s:GR", encoding_name);
	result = getFontencByName(gr_special);
	free(gr_special);
    }
    TRACE(("getFontencByName(%s) ->%s\n",
	   encoding_name,
	   result ? result->name : "?"));
    return result;
}

/*
 * Check (for EUC-JP specifically, but generally...) for a charset which
 * is part of a composite charset using G2/G3 via single-shifts.
 */
const char *
getCompositeCharset(const char *encoding_name)
{
    const char *result = 0;
    const FontencCharsetRec *fc;
    const LocaleCharsetRec *lc;

    if ((fc = getFontencByName(encoding_name)) != 0) {
	if ((lc = findLocaleByCharset(fc->name)) != 0) {
	    result = lc->name;
	    TRACE(("getCompositeCharset(%s) ->%s\n", encoding_name, result));
	}
    }
    return result;
}

static const char *
selectPart(const LocaleCharsetRec * data, unsigned g)
{
    const char *result = 0;
    switch (g) {
    case 0:
	result = data->g0;
	break;
    case 1:
	result = data->g1;
	break;
    case 2:
	result = data->g2;
	break;
    case 3:
	result = data->g3;
	break;
    }
    return result;
}

/*
 * Given a composite name returned by getCompositeCharset, return a pointer to
 * the data which describes the encoding used for a given shift.
 */
const FontencCharsetRec *
getCompositePart(const char *composite_name, unsigned g)
{
    const FontencCharsetRec *result = 0;
    const LocaleCharsetRec *lc;
    const char *part_name;

    for (lc = localeCharsets; lc->name; ++lc) {
	if (!lcStrCmp(composite_name, lc->name)) {
	    if ((part_name = selectPart(lc, g)) != 0) {
		const FontencCharsetRec *fc;
		for (fc = fontencCharsets; fc->name != 0; ++fc) {
		    if (!lcStrCmp(part_name, fc->name)) {
			result = fc;
			break;
		    }
		}
	    }
	    break;
	}
    }
    return result;
}
#endif

#ifdef NO_LEAKS
static int
isUnknownCharsetPtr(CharsetPtr p)
{
    return (p == &Unknown94Charset
	    || p == &Unknown96Charset
	    || p == &Unknown9494Charset
	    || p == &Unknown9696Charset);
}

static void
destroyFontencCharsetPtr(FontencCharsetPtr p)
{
#ifdef USE_ICONV
    if (p->reverse) {
	luitDestroyReverse(p->reverse);
    }
#else
    p->mapping = 0;

    /*
     * This should, but does not work -
     *     FontMapReverseFree(p->reverse)
     *
     * The iteration for map[] is based on reading the source of
     * FontMapReverse().
     */
    if (p->reverse) {
	int n;
	unsigned **map = p->reverse->data;
	for (n = 0; n < 256; ++n) {
	    if (map[n])
		free(map[n]);
	}
	free(p->reverse->data);
	free(p->reverse);
	p->reverse = 0;
    }
#endif
}

static void
destroyCharset(CharsetPtr p)
{
    if (!isUnknownCharsetPtr(p)) {
	if (p->type == T_OTHER) {
	    free(p->other_aux);
	} else {
	    destroyFontencCharsetPtr((FontencCharsetPtr) p->data);
	}
	free(p);
    }
}

void
charset_leaks(void)
{
    while (cachedCharsets != 0) {
	CharsetPtr next = cachedCharsets->next;
	destroyCharset(cachedCharsets);
	cachedCharsets = next;
    }
#ifdef USE_ICONV
    if (fakeLocaleCharset.name != 0) {
	free((void *) fakeLocaleCharset.name);
	fakeLocaleCharset.name = 0;
    }
#endif
}
#endif /* NO_LEAKS */
