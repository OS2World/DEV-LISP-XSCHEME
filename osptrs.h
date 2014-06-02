/* osptrs.h - table entries for machine specific functions */

#ifdef MACINTOSH
{	"HIDEPEN",				xhidepen	},
{	"SHOWPEN",				xshowpen	},
{	"GETPEN",				xgetpen		},
{	"PENSIZE",				xpensize	},
{	"PENMODE",				xpenmode	},
{	"PENPAT",				xpenpat		},
{	"PENNORMAL",				xpennormal	},
{	"MOVETO",				xmoveto		},
{	"MOVE",					xmove		},
{	"LINETO",				xlineto		},
{	"LINE",					xline		},
{	"SHOW-GRAPHICS",			xshowgraphics	},
{	"HIDE-GRAPHICS",			xhidegraphics	},
{	"CLEAR-GRAPHICS",			xcleargraphics	},
#endif

#ifdef MSDOS
{	"INT86",				xint86		},
{	"INBYTE",				xinbyte		},
{	"OUTBYTE",				xoutbyte	},
{	"SYSTEM",				xsystem		},
{	"GET-KEY",				xgetkey		},
#endif

#ifdef __OS2__
{	"SYSTEM",				xsystem		},
{	"GET-KEY",				xgetkey		},
#endif

#ifdef UNIX
{	"SYSTEM",				xsystem		},
#endif

