/*
 * Copyright (c) 1998, 1999 Henry Spencer.  All rights reserved.
 * 
 * Development of this software was funded, in part, by Cray Research Inc.,
 * UUNET Communications Services Inc., Sun Microsystems Inc., and Scriptics
 * Corporation, none of whom are responsible for the results.  The author
 * thanks all of them. 
 * 
 * Redistribution and use in source and binary forms -- with or without
 * modification -- are permitted for any purpose, provided that
 * redistributions in source form retain this entire copyright notice and
 * indicate the origin and nature of any modifications.
 * 
 * I'd appreciate being given credit for this package in the documentation
 * of software which uses it, but that is not a requirement.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * HENRY SPENCER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Change for RLP integration:
 *
 * Reconfigured to point to RLP data types and helper methods.
 */

/* headers if any */
#include "bt_types.h"

/* overrides for regguts.h definitions, if any */
#define	FUNCPTR(name, args)	(*name)(args)
#define	MALLOC(n)		malloc(n)
#define	FREE(p)			free(VS(p))
#define	REALLOC(p,n)		realloc(VS(p),n)



/*
 * Do not insert extras between the "begin" and "end" lines -- this
 * chunk is automatically extracted to be fitted into regex.h.
 */
/* --- begin --- */
/* ensure certain things don't sneak in from system headers */
#ifdef __REG_WIDE_T
#undef __REG_WIDE_T
#endif
#ifdef __REG_WIDE_COMPILE
#undef __REG_WIDE_COMPILE
#endif
#ifdef __REG_WIDE_EXEC
#undef __REG_WIDE_EXEC
#endif
#ifdef __REG_REGOFF_T
#undef __REG_REGOFF_T
#endif
#ifdef __REG_VOID_T
#undef __REG_VOID_T
#endif
#ifdef __REG_CONST
#undef __REG_CONST
#endif
#ifdef __REG_NOFRONT
#undef __REG_NOFRONT
#endif
#ifdef __REG_NOCHAR
#undef __REG_NOCHAR
#endif
/* interface types */
#define	__REG_WIDE_T	Char16
#define	__REG_REGOFF_T	long	/* not really right, but good enough... */
#define	__REG_VOID_T	VOID
#define	__REG_CONST	CONST
/* names and declarations */
#define	__REG_WIDE_COMPILE	bt_regcomp
#define	__REG_WIDE_EXEC		bt_regexec
#define	__REG_NOFRONT		/* don't want regcomp() and regexec() */
#define	__REG_NOCHAR		/* or the char versions */
#define	regfree		bt_regfree
#define	regerror	bt_regerror
/* --- end --- */



/* internal character type and related */
typedef BT_Char16 chr;	/* the type itself */
typedef int pchr;		/* what it promotes to */
typedef unsigned uchr;		/* unsigned type that will hold a chr */
typedef int celt;		/* type to hold chr, MCCE number, or NOCELT */
#define	NOCELT	(-1)		/* celt value which is not valid chr or MCCE */
#define	CHR(c)	((chr)(c))	/* turn char literal into chr literal */
#define	DIGITVAL(c)	((c)-'0')	/* turn chr digit into its value */
#define	CHRBITS	16		/* bits in a chr; must not use sizeof */
#define	CHR_MIN	0x0000		/* smallest and largest chr; the value */
#define	CHR_MAX	0xffff		/*  CHR_MAX-CHR_MIN+1 should fit in uchr */

/* functions operating on chr */
#define	iscalnum(x)	( x < 0x80 && isalnum((char)x) )
#define	iscalpha(x)	( x < 0x80 && isalpha((char)x) )
#define	iscdigit(x)	( x < 0x80 && isdigit((char)x) )
#define	iscspace(x)	( x < 0x80 && isspace((char)x) )
#define toclower(x) ( x < 0x80 ? CHR(tolower((char)x)) : x )
#define tocupper(x) ( x < 0x80 ? CHR(toupper((char)x)) : x )
#define toctitle(x) tocupper(x)

/* name the external functions */
#define	compile		bt_regcomp
#define	exec	  bt_regexec

/* enable/disable debugging code (by whether REG_DEBUG is defined or not) */
#if 0		/* no debug unless requested by makefile */
#define	REG_DEBUG	/* */
#endif

/* and pick up the standard header */
#include "tcl_regex.h"
