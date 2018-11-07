/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <inttypes.h>
		/* WINDOW & wgetch() & KEY_... & ... */
	#include <ncurses.h>

/* libalx --------------------------------------------------------------------*/
		/* alx_..._curses() & alx_ncur_prn_...() */
	#include "alx_ncur.h"

/* Project -------------------------------------------------------------------*/
		/*img_ocr_text */
	#include "img_iface.h"

/* Module --------------------------------------------------------------------*/
		/* user_iface_log */
	#include "user_iface.h"

	#include "user_tui.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
static	WINDOW	*win_log;
static	WINDOW	*win_help;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Log */
static	void	log_loop	(void);
	/* Input */
static	int	usr_input	(void);
	/* Help */
static	void	show_help	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	user_tui_init		(void)
{
	/* Use curses mode */
	alx_resume_curses();

	/* Dimensions: log */
	const int	h1 =	54;
	const int	w1 =	40;
	const int	r1 =	0;
	const int	c1 =	30;
	win_log		= newwin(h1, w1, r1, c1);

	/* Dimensions: help */
	const int	h2 =	54;
	const int	w2 =	40;
	const int	r2 =	0;
	const int	c2 =	0;
	win_help	= newwin(h2, w2, r2, c2);

	/* Activate keypad, don't echo input */
	keypad(win_log, true);
	noecho();
}

void	user_tui_cleanup	(void)
{
	/* Del wins & return to terminal mode */
	alx_win_del(win_log);
	alx_win_del(win_help);
	alx_pause_curses();
}

int	user_tui		(const char *title, const char *subtitle)
{
	int	action;

	show_help();
	user_tui_show_log(title, subtitle);
	action	= usr_input();

	return	action;
}

void	user_tui_fname		(const char *filepath, char *filename)
{
	/* Input box */
	int	w;
	int	r;
	w	= 75;
	r	= 10;

	/* Request name */
	alx_w_getfname(filepath, filename, false, w, r, "File name:",
			"Valid extensions: .bmp .dib .jpeg .png .pbm .pgm .ppm .tiff");
}

void	user_tui_show_log	(const char *title, const char *subtitle)
{
	/* Clear & box */
	werase(win_log);
	box(win_log, 0, 0);

	/* Title */
	alx_ncur_prn_title(win_log, title);
	/* Subtitle */
	alx_ncur_prn_subtitle(win_log, subtitle);

	/* Log */
	log_loop();

	/* Refresh */
	wrefresh(win_log);
}

double	user_tui_getdbl		(double m, double def, double M,
				const char *title, const char *help)
{
	/* Input box */
	int	w;
	int	r;
	w	= 75;
	r	= 10;

	/* Request int */
	double	R;
	R	= alx_w_getdbl(w, r, title, m, def, M, help);
	return	R;
}

int64_t	user_tui_getint		(double m, int64_t def, double M,
				const char *title, const char *help)
{
	/* Input box */
	int	w;
	int	r;
	w	= 75;
	r	= 10;

	/* Request int */
	int64_t	i;
	i	= alx_w_getint(w, r, title, m, def, M, help);
	return	i;
}

void	user_tui_show_ocr	(void)
{
	alx_pause_curses();

	printf("%s", img_ocr_text);
	getchar();

	alx_resume_curses();
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/* Log -----------------------------------------------------------------------*/
static	void	log_loop	(void)
{
	int	i;
	int	i_0;
	int	l;
	int	l_0;

	if (user_iface_log.len > 51) {
		i_0	= user_iface_log.len - 51;
		l_0	= 1;
		mvwprintw(win_log, 1, 10, "...");
	} else {
		i_0	= 0;
		l_0	= 0;
	}

	l	= l_0;
	for (i = i_0; i < user_iface_log.len; i++) {
		if (user_iface_log.lvl[i] <= user_iface_log.visible) {
			mvwprintw(win_log, (1 + l),
						(1 + 4 * user_iface_log.lvl[i]),
						user_iface_log.line[i]);
			l++;
		}
	}
}

/* Input ---------------------------------------------------------------------*/
static	int	usr_input	(void)
{
	int	action;
	wchar_t	ch;

	/* Interpret input */
	ch = wgetch(win_log);
	switch (ch) {
	case ' ':
		action	= USER_IFACE_ACT_APPLY;
		break;

		/* ASCII 0x08 is BS */
	case KEY_BACKSPACE:
	case 0x08:
		action	= USER_IFACE_ACT_DISCARD;
		break;

	case 'e':
		/* Exercises from class */
		ch = wgetch(win_log);

		switch (ch) {
		case '1':
			/* Label */
			ch = wgetch(win_log);

			switch (ch) {
			case '0':
				action	= USER_IFACE_ACT_PROC_LABEL;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		case '3':
			/* Resistor */
			ch = wgetch(win_log);

			switch (ch) {
			case '0':
				action	= USER_IFACE_ACT_PROC_RESISTOR;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
		break;

	case 'f':
		/* Use simple funtions */
		ch = wgetch(win_log);

		switch (ch) {
		case '1':
			/* img_cv */
			ch = wgetch(win_log);

			switch (ch) {
			case '0':
				/* Core: The core functionality */
				ch = wgetch(win_log);

				switch (ch) {
				case '0':
					/* Pixel */
					ch = wgetch(win_log);

					switch (ch) {
					case '0':
						action	= USER_IFACE_ACT_PIXEL_VALUE;
						break;
					default:
						action	= USER_IFACE_ACT_FOO;
						break;
					}
					break;
				case '1':
					/* ROI */
					ch = wgetch(win_log);

					switch (ch) {
					case '0':
						action	= USER_IFACE_ACT_SET_ROI;
						break;
					case '1':
						action	= USER_IFACE_ACT_SET_ROI_2RECT;
						break;
					default:
						action	= USER_IFACE_ACT_FOO;
						break;
					}
					break;
				case '2':
					/* Operations on Arrays */
					ch = wgetch(win_log);

					switch (ch) {
					case '0':
						action	= USER_IFACE_ACT_AND_2REF;
						break;
					case '1':
						action	= USER_IFACE_ACT_NOT;
						break;
					case '2':
						action	= USER_IFACE_ACT_OR_2REF;
						break;
					case '3':
						action	= USER_IFACE_ACT_COMPONENT;
						break;
					default:
						action	= USER_IFACE_ACT_FOO;
						break;
					}
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			case '1':
				/* Imgproc: Image processing */
				ch = wgetch(win_log);

				switch (ch) {
				case '0':
					/* Image filtering */
					ch = wgetch(win_log);

					switch (ch) {
					case '0':
						action	= USER_IFACE_ACT_DILATE;
						break;
					case '1':
						action	= USER_IFACE_ACT_ERODE;
						break;
					case '2':
						action	= USER_IFACE_ACT_DILATE_ERODE;
						break;
					case '3':
						action	= USER_IFACE_ACT_ERODE_DILATE;
						break;
					case '4':
						action	= USER_IFACE_ACT_SMOOTH;
						break;
					case '5':
						action	= USER_IFACE_ACT_SOBEL;
						break;
					default:
						action	= USER_IFACE_ACT_FOO;
						break;
					}
					break;
				case '1':
					/* Geometric image transformations */
					ch = wgetch(win_log);

					switch (ch) {
					case '0':
						action	= USER_IFACE_ACT_ROTATE_ORTO;
						break;
					case '1':
						action	= USER_IFACE_ACT_ROTATE;
						break;
					case '2':
						action	= USER_IFACE_ACT_ROTATE_2RECT;
						break;
					default:
						action	= USER_IFACE_ACT_FOO;
						break;
					}
					break;
				case '2':
					/* Miscellaneous image transformations */
					ch = wgetch(win_log);

					switch (ch) {
					case '0':
						/* Threshold */
						ch = wgetch(win_log);

						switch (ch) {
						case '0':
							action	= USER_IFACE_ACT_ADAPTIVE_THRESHOLD;
							break;
						case '1':
							action	= USER_IFACE_ACT_THRESHOLD;
							break;
						default:
							action	= USER_IFACE_ACT_FOO;
							break;
						}
						break;
					case '1':
						/* Color */
						ch = wgetch(win_log);

						switch (ch) {
						case '0':
							action	= USER_IFACE_ACT_CVT_COLOR;
							break;
						default:
							action	= USER_IFACE_ACT_FOO;
							break;
						}
						break;
					case '2':
						/* Transforms */
						ch = wgetch(win_log);

						switch (ch) {
						case '0':
							action	= USER_IFACE_ACT_DISTANCE_TRANSFORM;
							break;
						default:
							action	= USER_IFACE_ACT_FOO;
							break;
						}
						break;
					default:
						action	= USER_IFACE_ACT_FOO;
						break;
					}
					break;
				case '3':
					/* Histograms */
					ch = wgetch(win_log);

					switch (ch) {
					case '0':
						action	= USER_IFACE_ACT_HISTOGRAM;
						break;
					case '1':
						action	= USER_IFACE_ACT_HISTOGRAM_C3;
						break;
					default:
						action	= USER_IFACE_ACT_FOO;
						break;
					}
					break;
				case '4':
					/* Structural analysis and shape descriptors */
					ch = wgetch(win_log);

					switch (ch) {
					case '0':
						action	= USER_IFACE_ACT_CONTOURS;
						break;
					case '1':
						action	= USER_IFACE_ACT_CONTOURS_SIZE;
						break;
					case '2':
						action	= USER_IFACE_ACT_BOUNDING_RECT;
						break;
					case '3':
						action	= USER_IFACE_ACT_FIT_ELLIPSE;
						break;
					case '4':
						action	= USER_IFACE_ACT_MIN_AREA_RECT;
						break;
					default:
						action	= USER_IFACE_ACT_FOO;
						break;
					}
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		case '2':
			/* img_orb */
			ch = wgetch(win_log);

			switch (ch) {
			case '0':
				action	= USER_IFACE_ACT_ALIGN;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		case '3':
			/* img_zbar */
			ch = wgetch(win_log);

			switch (ch) {
			case '0':
				action	= USER_IFACE_ACT_DECODE;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		case '4':
			/* img_ocr */
			ch = wgetch(win_log);

			switch (ch) {
			case '0':
				action	= USER_IFACE_ACT_READ;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
		break;

	case 'l':
		action	= USER_IFACE_ACT_LOAD_MEM;
		break;

	case 'm':
		action	= USER_IFACE_ACT_SAVE_MEM;
		break;

	case 'q':
		action	= USER_IFACE_ACT_QUIT;
		break;

	case 'r':
		action	= USER_IFACE_ACT_SAVE_REF;
		break;

	case 's':
		action	= USER_IFACE_ACT_SAVE_FILE;
		break;

	case 'u':
		/* User iface actions */
		ch = wgetch(win_log);

		switch (ch) {
		case '1':
			action	= USER_IFACE_ACT_SHOW_OCR;
			break;
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
		break;

	case 'x':
		/* Special sequence "xyzzy" */
		ch = wgetch(win_log);
		if (ch == 'y') {
			ch = wgetch(win_log);
			if (ch == 'z') {

			ch = wgetch(win_log);
			if (ch == 'z') {

			ch = wgetch(win_log);
			if (ch == 'y') {
				action	= USER_IFACE_ACT_FOO;
			}
			}
			}
		}
		break;

	default:
		action	= USER_IFACE_ACT_FOO;
		break;
	}

	return	action;
}

/*	*	*	*	*	*	*	*	*	*
 *	*	* Help	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	show_help	(void)
{
	/* Clear */
	werase(win_help);

	int	r;
	int	c;

	r	= 0;
	c	= 0;
	mvwprintw(win_help, r++, c, "Apply:			%s",	"Space");
	mvwprintw(win_help, r++, c, "Discard:		%s",		"BS");
	mvwprintw(win_help, r++, c, "Save to mem:		%c",	'm');
	mvwprintw(win_help, r++, c, "Load from mem:		%c",	'l');
	mvwprintw(win_help, r++, c, "Save to ref:		%c",	'r');
	mvwprintw(win_help, r++, c, "Save to file:		%c",	's');
	mvwprintw(win_help, r++, c, "Functions:");
	mvwprintw(win_help, r++, c, " - Pixel value:		%s",	"f1000");
	mvwprintw(win_help, r++, c, " - Set ROI:		%s",	"f1010");
	mvwprintw(win_help, r++, c, " - Set ROI 2rect:	%s",		"f1011");
	mvwprintw(win_help, r++, c, " - Bitwise AND 2ref:	%s",	"f1020");
	mvwprintw(win_help, r++, c, " - Bitwise NOT:		%s",	"f1021");
	mvwprintw(win_help, r++, c, " - Bitwise OR 2ref:	%s",	"f1022");
	mvwprintw(win_help, r++, c, " - Component:		%s",	"f1023");
	mvwprintw(win_help, r++, c, " - Dilate:		%s",		"f1100");
	mvwprintw(win_help, r++, c, " - Erode:		%s",		"f1101");
	mvwprintw(win_help, r++, c, " - D-E:			%s",	"f1102");
	mvwprintw(win_help, r++, c, " - E-D:			%s",	"f1103");
	mvwprintw(win_help, r++, c, " - Smooth:		%s",		"f1104");
	mvwprintw(win_help, r++, c, " - Sobel:		%s",		"f1105");
	mvwprintw(win_help, r++, c, " - Rotate ortogonally:	%s",	"f1110");
	mvwprintw(win_help, r++, c, " - Rotate:		%s",		"f1111");
	mvwprintw(win_help, r++, c, " - Rotate 2rect_rot:	%s",	"f1112");
	mvwprintw(win_help, r++, c, " - Adaptive threshold:	%s",	"f11200");
	mvwprintw(win_help, r++, c, " - Threshold:		%s",	"f11201");
	mvwprintw(win_help, r++, c, " - Cvt color:		%s",	"f11210");
	mvwprintw(win_help, r++, c, " - Distance transform:	%s",	"f11220");
	mvwprintw(win_help, r++, c, " - Histogram:		%s",	"f1130");
	mvwprintw(win_help, r++, c, " - Histogram (3 chan):	%s",	"f1131");
	mvwprintw(win_help, r++, c, " - Contours:		%s",	"f1140");
	mvwprintw(win_help, r++, c, " - Contours size:	%s",		"f1141");
	mvwprintw(win_help, r++, c, " - Bounding rectangle:	%s",	"f1142");
	mvwprintw(win_help, r++, c, " - Fit ellipse:		%s",	"f1143");
	mvwprintw(win_help, r++, c, " - Min. area rectangle:	%s",	"f1144");
	mvwprintw(win_help, r++, c, " - Align 2ref (ORB):	%s",	"f20");
	mvwprintw(win_help, r++, c, " - Scan codes (ZBAR):	%s",	"f30");
	mvwprintw(win_help, r++, c, " - Scan text (OCR):	%s",	"f40");
	mvwprintw(win_help, r++, c, "Exercises:");
	mvwprintw(win_help, r++, c, " - Label:		%s",		"e10");
	mvwprintw(win_help, r++, c, " - Resistor:		%s",	"e30");
	mvwprintw(win_help, r++, c, "Other:");
	mvwprintw(win_help, r++, c, " - Show OCR:		%s",	"u1");
	mvwprintw(win_help, r++, c, "Quit:			%c",	'q');

	/* Refresh */
	wrefresh(win_help);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
