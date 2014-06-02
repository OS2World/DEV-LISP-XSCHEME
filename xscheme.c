/* xscheme.c - xscheme main routine */
/*      Copyright (c) 1990, by David Michael Betz
        All Rights Reserved
        Permission is granted for unrestricted non-commercial use       */

#include "xscheme.h"

/* the program banner */
#define BANNER "XScheme V0.25, Copyright (c) 1991, David Betz"

/* global variables */
jmp_buf top_level;
int clargc;     /* command line argument count */
char **clargv;  /* array of command line arguments */

/* trace file pointer */
FILE *tfp=NULL;

/* external variables */
extern LVAL xlfun,xlenv,xlval;
extern LVAL s_stdin,s_stdout,s_stderr,s_unbound;
extern int trace;

/* main - the main routine */
main(argc,argv)
  int argc; char *argv[];
{
    int src,dst;
    LVAL code;
    char *p;

    /* process the arguments */
    for (src = dst = 1, clargv = argv, clargc = 1; src < argc; ++src) {

	/* handle options */
	if (argv[src][0] == '-') {
	    for (p = &argv[src][1]; *p != '\0'; )
	    	switch (*p++) {
		case 't':		/* root directory */
		    trace = TRUE;
		    break;
		default:
	    	    usage();
		}
	}

	/* handle a filename */
	else {
	    argv[dst++] = argv[src];
	    ++clargc;
	}
    }

    /* setup an initialization error handler */
    if (setjmp(top_level))
	exit(1);

    /* initialize */
    osinit(BANNER);

    /* restore the default workspace, otherwise create a new one */
    if (!xlirestore("xscheme.wks"))
	xlinitws(5000);

    /* do the initialization code first */
    code = xlenter("*INITIALIZE*");
    code = (boundp(code) ? getvalue(code) : NIL);

    /* trap errors */
    if (setjmp(top_level)) {
	code = xlenter("*TOPLEVEL*");
	code = (boundp(code) ? getvalue(code) : NIL);
	xlfun = xlenv = xlval = NIL;
	xlsp = xlstktop;
    }

    /* execute the main loop */
    if (code != NIL)
	xlexecute(code);
    wrapup();
}

usage()
{
    info("usage: xscheme [-t]\n");
    exit(1);
}

xlload() {}
xlcontinue() {}
xlbreak() { xltoplevel(); }
xlcleanup() {}

/* xltoplevel - return to the top level */
xltoplevel()
{
    stdputstr("[ back to top level ]\n");
    longjmp(top_level,1);
}

/* xlfail - report an error */
xlfail(msg)
  char *msg;
{
    xlerror(msg,s_unbound);
}

/* xlerror - report an error */
xlerror(msg,arg)
  char *msg; LVAL arg;
{
    /* display the error message */
    errputstr("Error: ");
    errputstr(msg);
    errputstr("\n");

    /* print the argument on a separate line */
    if (arg != s_unbound) {
	errputstr("  ");
	errprint(arg);
    }

    /* print the function where the error occurred */
    errputstr("happened in: ");
    errprint(xlfun);

    /* call the handler */
    callerrorhandler();
}

/* callerrorhandler - call the error handler */
callerrorhandler()
{
    extern jmp_buf bc_dispatch;

    /* invoke the error handler */
    if (xlval = getvalue(xlenter("*ERROR-HANDLER*"))) {
	oscheck();	/* an opportunity to break out of a bad handler */
	check(2);
	push(xlenv);
	push(xlfun);
	xlargc = 2;
	xlapply();
	longjmp(bc_dispatch,1);
    }

    /* no handler, just reset back to the top level */
    longjmp(top_level,1);
}

/* xlabort - print an error message and abort */
xlabort(msg)
  char *msg;
{
    /* display the error message */
    errputstr("Abort: ");
    errputstr(msg);
    errputstr("\n");

    /* print the function where the error occurred */
    errputstr("happened in: ");
    errprint(xlfun);

    /* reset back to the top level */
    oscheck();	/* an opportunity to break out */
    longjmp(top_level,1);
}

/* xlfatal - print a fatal error message and exit */
xlfatal(fmt,a1,a2,a3,a4)
  char *fmt;
{
    char buf[100];
    sprintf(buf,fmt,a1,a2,a3,a4);
    oserror(buf);
    exit(1);
}

/* info - display debugging information */
info(fmt,a1,a2,a3,a4)
  char *fmt;
{
    char buf[100],*p;
    sprintf(buf,fmt,a1,a2,a3,a4);
    for (p = buf; *p != '\0'; )
	ostputc(*p++);
}

/* wrapup - clean up and exit to the operating system */
wrapup()
{
    if (tfp)
	osclose(tfp);
    osfinish();
    exit(0);
}
