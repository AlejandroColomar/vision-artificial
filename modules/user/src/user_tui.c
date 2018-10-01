/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* WINDOW & wgetch() & KEY_... & ... */
	#include <ncurses.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* alx_..._curses() & alx_ncur_prn_...() */
	#include "alx_ncur.h"

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

int	user_tui		(const char *title, const char *subtitle)
{
	int	action;

	show_log(title, subtitle);
	show_help();
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

void	user_tui_cleanup	(void)
{
	/* Del wins & return to terminal mode */
	alx_win_del(win_log);
	alx_win_del(win_help);
	alx_pause_curses();
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

	for (i = 0; i < user_iface_log.len; i++) {
		mvwprintw(win_log, i+1, 0, user_iface_log.line[i]);
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

	case 's':
		action	= USER_IFACE_ACT_SAVE;
		break;

		/* ASCII 0x08 is BS */
	case KEY_BACKSPACE:
	case 0x08:
		action	= USER_IFACE_ACT_DISCARD;
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

	case 'f':
		/* Use simple funtions */
		ch = wgetch(win_log);

		switch (ch) {
		case '0':
			action	= USER_IFACE_ACT_INVERT;
			break;
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
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

	case 'q':
		action	= USER_IFACE_ACT_QUIT;
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
	mvwprintw(win_help, r++, c, "Apply:	Space");
	mvwprintw(win_help, r++, c, "Save:	%c", 's');
	mvwprintw(win_help, r++, c, "Discard:	Backspace");
	mvwprintw(win_help, r++, c, "Functions:	%cX", 'f');
	mvwprintw(win_help, r++, c, "	 - Invert:	%c0", 'f');
	mvwprintw(win_help, r++, c, "Exercises:	%cX", 'e');
	mvwprintw(win_help, r++, c, "Quit:	%c", 'c');

	/* Refresh */
	wrefresh(win_help);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
