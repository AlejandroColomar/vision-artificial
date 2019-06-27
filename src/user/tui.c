/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/user/tui.h"

#include <inttypes.h>
#include <ncurses.h>

#include "libalx/base/stdio/wait.h"
#include "libalx/extra/ncurses/common.h"
#include "libalx/extra/ncurses/get.h"

#include "vision-artificial/image/iface.h"
#include "vision-artificial/user/iface.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
static	WINDOW	*win_log;
static	WINDOW	*win_help;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	log_loop	(void);
static	int	usr_input	(void);
static	void	show_help	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	user_tui_init		(void)
{
	int_fast8_t	h1, w1, r1, c1;
	int_fast8_t	h2, w2, r2, c2;

	alx_ncurses_resume();

	/* Dimensions: log */
	h1	= 54;
	w1	= 40;
	r1	= 0;
	c1	= 30;
	win_log	= newwin(h1, w1, r1, c1);

	/* Dimensions: help */
	h2	= 54;
	w2	= 40;
	r2	= 0;
	c2	= 0;
	win_help	= newwin(h2, w2, r2, c2);

	/* Activate keypad, don't echo input */
	keypad(win_log, true);
	noecho();
}

void	user_tui_cleanup	(void)
{

	/* Del wins & return to terminal mode */
	alx_ncurses_delwin(win_log);
	alx_ncurses_delwin(win_help);
	alx_ncurses_pause();
}

int	user_tui		(const char *restrict title,
				const char *restrict subtitle)
{

	show_help();
	user_tui_show_log(title, subtitle);
	return	usr_input();
}

void	user_tui_fname		(const char *restrict fpath,
				char *restrict fname)
{
	int	w, r;

	w	= 75;
	r	= 10;

	alx_ncurses_get_fname(fpath, fname, false, w, r, "File name:",
	"Valid extensions: .bmp .dib .jpeg .png .pbm .pgm .ppm .tiff", 2);
}

void	user_tui_show_log	(const char *restrict title,
				const char *restrict subtitle)
{

	werase(win_log);
	box(win_log, 0, 0);

	alx_ncurses_title(win_log, title);
	alx_ncurses_subtitle(win_log, subtitle);

	log_loop();
	wrefresh(win_log);
}

double	user_tui_getdbl		(double m, double def, double M,
				const char *restrict title,
				const char *restrict help)
{
	int	w, r;

	w	= 75;
	r	= 10;

	return	alx_ncurses_get_dbl(m, def, M, w, r, title, help, 2);
}

int	user_tui_getint		(int m, int def, int M,
				const char *restrict title,
				const char *restrict help)
{
	int	w, r;

	w	= 75;
	r	= 10;

	return	alx_ncurses_get_int(m, def, M, w, r, title, help, 2);
}

void	user_tui_show_ocr	(void)
{

	alx_ncurses_pause();

	printf("%s", img_ocr_text);
	alx_wait4enter();

	alx_ncurses_resume();
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	log_loop	(void)
{
	ptrdiff_t	i_0;
	int_fast8_t	l_0;
	int_fast8_t	l;

	if (user_iface_log.len > 51) {
		i_0	= user_iface_log.len - 51;
		l_0	= 1;
		mvwprintw(win_log, 1, 10, "...");
	} else {
		i_0	= 0;
		l_0	= 0;
	}

	l	= l_0 + 1;
	for (ptrdiff_t i = i_0; i < user_iface_log.len; i++) {
		if (user_iface_log.lvl[i] <= user_iface_log.visible) {
			mvwprintw(win_log, l, (1 + 4 * user_iface_log.lvl[i]),
					"%.*s",
					40 - 2 - 4 * user_iface_log.lvl[i],
					user_iface_log.line[i]);
			l++;
		}
	}
}

static	int	usr_input	(void)
{
	int	c;

	c = wgetch(win_log);
	switch (c) {
	case ' ':
		return	USER_IFACE_ACT_APPLY;
	case KEY_BACKSPACE:
	/* ASCII 0x08 is BS */
	case 0x08:
		return	USER_IFACE_ACT_DISCARD;
	case 'e':
		/* Exercises from class */
		c = wgetch(win_log);

		switch (c) {
		case '1':
			/* Label */
			c = wgetch(win_log);

			switch (c) {
			case '1':
				return	USER_IFACE_ACT_PROC_LABEL_SERIES;
			}
			break;
		case '2':
			/* Objects */
			c = wgetch(win_log);

			switch (c) {
			case '0':
				return	USER_IFACE_ACT_PROC_OBJECTS_CALIB;
			case '1':
				return	USER_IFACE_ACT_PROC_OBJECTS_SERIES;
			}
			break;
		case '3':
			/* Coins */
			c = wgetch(win_log);

			switch (c) {
			case '1':
				return	USER_IFACE_ACT_PROC_COINS_SERIES;
			}
			break;
		case '4':
			/* Resistor */
			c = wgetch(win_log);

			switch (c) {
			case '1':
				return	USER_IFACE_ACT_PROC_RESISTOR_SERIES;
			}
			break;
		case '5':
			/* Lighters */
			c = wgetch(win_log);

			switch (c) {
			case '1':
				return	USER_IFACE_ACT_PROC_LIGHTERS_SERIES;
			}
			break;
		}
		break;
	case 'f':
		/* Use simple funtions */
		c = wgetch(win_log);

		switch (c) {
		case '0':
			/* img_alx */
			c = wgetch(win_log);

			switch (c) {
			case '0':
				/* Distance transform postprocessing */
				c = wgetch(win_log);

				switch (c) {
				case '0':
					return	USER_IFACE_ACT_LOCAL_MAX;
				case '1':
					return	USER_IFACE_ACT_SKELETON;
				}
				break;
			case '1':
				/* Lines */
				c = wgetch(win_log);

				switch (c) {
				case '0':
					return	USER_IFACE_ACT_LINES_HORIZONTAL;
				case '1':
					return	USER_IFACE_ACT_LINES_VERTICAL;
				}
				break;
			case '2':
				/* Smooth */
				c = wgetch(win_log);

				switch (c) {
				case '0':
					return	USER_IFACE_ACT_MEAN_HORIZONTAL;
				case '1':
					return	USER_IFACE_ACT_MEAN_VERTICAL;
				case '2':
					return	USER_IFACE_ACT_MEDIAN_HORIZONTAL;
				case '3':
					return	USER_IFACE_ACT_MEDIAN_VERTICAL;
				}
				break;
			}
			break;
		case '1':
			/* img_cv */
			c = wgetch(win_log);

			switch (c) {
			case '0':
				/* Core: The core functionality */
				c = wgetch(win_log);

				switch (c) {
				case '0':
					/* Pixel */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_PIXEL_GET;
					case '1':
						return	USER_IFACE_ACT_PIXEL_SET;
					}
					break;
				case '1':
					/* ROI */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_SET_ROI;
					case '1':
						return	USER_IFACE_ACT_SET_ROI_2RECT;
					}
					break;
				case '2':
					/* Operations on Arrays */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_AND_2REF;
					case '1':
						return	USER_IFACE_ACT_NOT;
					case '2':
						return	USER_IFACE_ACT_OR_2REF;
					case '3':
						return	USER_IFACE_ACT_COMPONENT;
					}
					break;
				}
				break;
			case '1':
				/* Imgproc: Image processing */
				c = wgetch(win_log);

				switch (c) {
				case '0':
					/* Image filtering */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_DILATE;
					case '1':
						return	USER_IFACE_ACT_ERODE;
					case '2':
						return	USER_IFACE_ACT_DILATE_ERODE;
					case '3':
						return	USER_IFACE_ACT_ERODE_DILATE;
					case '4':
						return	USER_IFACE_ACT_SMOOTH;
					case '5':
						return	USER_IFACE_ACT_SOBEL;
					case '6':
						return	USER_IFACE_ACT_BORDER;
					}
					break;
				case '1':
					/* Geometric image transformations */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_MIRROR;
					case '1':
						return	USER_IFACE_ACT_ROTATE_ORTO;
					case '2':
						return	USER_IFACE_ACT_ROTATE;
					case '3':
						return	USER_IFACE_ACT_ROTATE_2RECT;
					}
					break;
				case '2':
					/* Miscellaneous image transformations */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_ADAPTIVE_THRESHOLD;
					case '1':
						return	USER_IFACE_ACT_CVT_COLOR;
					case '2':
						return	USER_IFACE_ACT_DISTANCE_TRANSFORM;
					case '3':
						return	USER_IFACE_ACT_THRESHOLD;
					}
					break;
				case '3':
					/* Histograms */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_HISTOGRAM;
					case '1':
						return	USER_IFACE_ACT_HISTOGRAM_C3;
					}
					break;
				case '4':
					/* Structural analysis and shape descriptors */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_CONTOURS;
					case '1':
						return	USER_IFACE_ACT_CONTOURS_SIZE;
					case '2':
						return	USER_IFACE_ACT_BOUNDING_RECT;
					case '3':
						return	USER_IFACE_ACT_FIT_ELLIPSE;
					case '4':
						return	USER_IFACE_ACT_MIN_AREA_RECT;
					}
					break;
				case '5':
					/* Feature detection */
					c = wgetch(win_log);

					switch (c) {
					case '0':
						return	USER_IFACE_ACT_HOUGH_CIRCLES;
					}
					break;
				}
				break;
			}
			break;
		case '2':
			/* img_orb */
			c = wgetch(win_log);

			switch (c) {
			case '0':
				return	USER_IFACE_ACT_ALIGN;
			}
			break;
		case '3':
			/* img_calib3d */
			c = wgetch(win_log);

			switch (c) {
			case '0':
				return	USER_IFACE_ACT_CALIBRATE;
			case '1':
				return	USER_IFACE_ACT_UNDISTORT;
			}
			break;
		case '4':
			/* img_zbar */
			c = wgetch(win_log);

			switch (c) {
			case '0':
				return	USER_IFACE_ACT_DECODE;
			}
			break;
		case '5':
			/* img_ocr */
			c = wgetch(win_log);

			switch (c) {
			case '0':
				return	USER_IFACE_ACT_READ;
			}
			break;
		}
		break;
	case 'l':
		return	USER_IFACE_ACT_LOAD_MEM;
	case 'm':
		return	USER_IFACE_ACT_SAVE_MEM;
	case 'q':
		return	USER_IFACE_ACT_QUIT;
	case 'r':
		return	USER_IFACE_ACT_SAVE_REF;
	case 's':
		return	USER_IFACE_ACT_SAVE_FILE;
	case 'u':
		/* User iface actions */
		c = wgetch(win_log);

		switch (c) {
		case '1':
			return	USER_IFACE_ACT_SHOW_OCR;
		}
		break;

	case 'x':

		/* Special sequence "xyzzy" */
		c = wgetch(win_log);
		if (c == 'y') {
			c = wgetch(win_log);
			if (c == 'z') {

			c = wgetch(win_log);
			if (c == 'z') {

			c = wgetch(win_log);
			if (c == 'y') {
				return	USER_IFACE_ACT_FOO;
			}
			}
			}
		}
		break;
	}

	return	USER_IFACE_ACT_FOO;
}

static	void	show_help	(void)
{
	int	r, c;

	werase(win_help);
	r	= 0;
	c	= 0;
	mvwprintw(win_help, r++, c, "Apply:			%s",	"Space");
	mvwprintw(win_help, r++, c, "Discard:		%s",		"BS");
	mvwprintw(win_help, r++, c, "Save to mem:		%c",	'm');
	mvwprintw(win_help, r++, c, "Load from mem:		%c",	'l');
	mvwprintw(win_help, r++, c, "Save to ref:		%c",	'r');
	mvwprintw(win_help, r++, c, "Save to file:		%c",	's');
	mvwprintw(win_help, r++, c, "Functions:");
	mvwprintw(win_help, r++, c, " - Local maxima:	%s",		"f000");
	mvwprintw(win_help, r++, c, " - Skeleton:		%s",	"f001");
	mvwprintw(win_help, r++, c, " - Horizontal lines:	%s",	"f010");
	mvwprintw(win_help, r++, c, " - Vertical lines:	%s",		"f011");
	mvwprintw(win_help, r++, c, " - Horizontal mean:	%s",	"f020");
	mvwprintw(win_help, r++, c, " - Vertical mean:	%s",		"f021");
	mvwprintw(win_help, r++, c, " - Horizontal median:	%s",	"f022");
	mvwprintw(win_help, r++, c, " - Vertical median:	%s",	"f023");
	mvwprintw(win_help, r++, c, " - Pixel get:		%s",	"f1000");
	mvwprintw(win_help, r++, c, " - Pixel set:		%s",	"f1001");
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
	mvwprintw(win_help, r++, c, " - Border:		%s",		"f1106");
	mvwprintw(win_help, r++, c, " - Mirror:		%s",		"f1110");
	mvwprintw(win_help, r++, c, " - Rotate ortogonally:	%s",	"f1111");
	mvwprintw(win_help, r++, c, " - Rotate:		%s",		"f1112");
	mvwprintw(win_help, r++, c, " - Rotate 2rect_rot:	%s",	"f1113");
	mvwprintw(win_help, r++, c, " - Adaptive threshold:	%s",	"f1120");
	mvwprintw(win_help, r++, c, " - Cvt color:		%s",	"f1121");
	mvwprintw(win_help, r++, c, " - Distance transform:	%s",	"f1122");
	mvwprintw(win_help, r++, c, " - Threshold:		%s",	"f1123");
	mvwprintw(win_help, r++, c, " - Histogram:		%s",	"f1130");
	mvwprintw(win_help, r++, c, " - Histogram (3 chan):	%s",	"f1131");
	mvwprintw(win_help, r++, c, " - Contours:		%s",	"f1140");
	mvwprintw(win_help, r++, c, " - Contours size:	%s",		"f1141");
	mvwprintw(win_help, r++, c, " - Bounding rectangle:	%s",	"f1142");
	mvwprintw(win_help, r++, c, " - Fit ellipse:		%s",	"f1143");
	mvwprintw(win_help, r++, c, " - Min. area rectangle:	%s",	"f1144");
	mvwprintw(win_help, r++, c, " - Hough circles:	%s",		"f1150");
	mvwprintw(win_help, r++, c, " - Align 2ref (ORB):	%s",	"f20");
	mvwprintw(win_help, r++, c, " - Calibrate (Calib3d):	%s",	"f30");
	mvwprintw(win_help, r++, c, " - Undistort (Calib3d):	%s",	"f31");
	mvwprintw(win_help, r++, c, " - Scan codes (ZBAR):	%s",	"f40");
	mvwprintw(win_help, r++, c, " - Scan text (OCR):	%s",	"f50");
	mvwprintw(win_help, r++, c, "Exercises:");
	mvwprintw(win_help, r++, c, " - Label:		%s",		"e11");
	mvwprintw(win_help, r++, c, " - Objects (calib):	%s",	"e20");
	mvwprintw(win_help, r++, c, " - Objects:		%s",	"e21");
	mvwprintw(win_help, r++, c, " - Coins:		%s",		"e31");
	mvwprintw(win_help, r++, c, " - Resistor:		%s",	"e41");
	mvwprintw(win_help, r++, c, " - Lighters:		%s",	"e51");
	mvwprintw(win_help, r++, c, "Other:");
	mvwprintw(win_help, r++, c, " - Show OCR:		%s",	"u1");
	mvwprintw(win_help, r++, c, "Quit:			%c",	'q');
	wrefresh(win_help);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
