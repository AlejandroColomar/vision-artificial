/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
	#include <inttypes.h>
		/* WINDOW & wgetch() & KEY_... & ... */
	#include <ncurses.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* alx_..._curses() & alx_ncur_prn_...() */
	#include "alx_ncur.h"

		/*img_ocr_text */
	#include "img_ocr.h"

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
static	void	show_log	(const char *title, const char *subtitle);
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
	const int	h1 =	24;
	const int	w1 =	50;
	const int	r1 =	0;
	const int	c1 =	30;
	win_log		= newwin(h1, w1, r1, c1);

	/* Dimensions: help */
	const int	h2 =	24;
	const int	w2 =	30;
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
	show_log(title, subtitle);
	action	= usr_input();

	return	action;
}

void	user_tui_save_name	(const char *filepath, char *filename, int destsize)
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
/*	*	*	*	*	*	*	*	*	*
 *	*	* Log	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	show_log	(const char *title, const char *subtitle)
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

static	void	log_loop	(void)
{
	int	i;
	int	i_0;
	int	l;
	int	l_0;

	if ((user_iface_log.len - 21) > 0) {
		i_0	= user_iface_log.len - 21;
		l_0	= 1;
		mvwprintw(win_log, 1, 10, "...");
	} else {
		i_0	= 0;
		l_0	= 0;
	}

	for (i = i_0, l = l_0; i < user_iface_log.len; i++, l++) {
		mvwprintw(win_log, (1 + l), (1 + 4 * user_iface_log.lvl[i]), user_iface_log.line[i]);
	}
}

/*	*	*	*	*	*	*	*	*	*
 *	*	* Input	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
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
				action	= USER_IFACE_ACT_INVERT;
				break;
			case '1':
				action	= USER_IFACE_ACT_ROTATE;
				break;
			case '2':
				action	= USER_IFACE_ACT_BGR2GRAY;
				break;
			case '3':
				action	= USER_IFACE_ACT_COMPONENT;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		case '2':
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
		case '3':
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
	mvwprintw(win_help, r++, c, "Apply:		%s",	"Space");
	mvwprintw(win_help, r++, c, "Discard:	%s",		"Backspace");
	mvwprintw(win_help, r++, c, "Save to mem:	%c",	'm');
	mvwprintw(win_help, r++, c, "Load from mem:	%c",	'l');
	mvwprintw(win_help, r++, c, "Save to file:	%c",	's');
	mvwprintw(win_help, r++, c, "Functions:");
	mvwprintw(win_help, r++, c, " - Invert:	%s",	"f10");
	mvwprintw(win_help, r++, c, " - Rotate:	%s",	"f11");
	mvwprintw(win_help, r++, c, " - BGR -> Gray:	%s",	"f12");
	mvwprintw(win_help, r++, c, " - Component:	%s",	"f13");
	mvwprintw(win_help, r++, c, " - Scan codes:	%s",	"f20");
	mvwprintw(win_help, r++, c, " - Scan text:	%s",	"f30");
	mvwprintw(win_help, r++, c, "Exercises:");
	mvwprintw(win_help, r++, c, "Other:");
	mvwprintw(win_help, r++, c, " - Show OCR:	%s",	"u1");
	mvwprintw(win_help, r++, c, "Quit:		%c",	'q');

	/* Refresh */
	wrefresh(win_help);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
