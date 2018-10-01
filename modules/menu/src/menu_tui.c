/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
	#include <ncurses.h>
	#include <stdbool.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
	#include "alx_ncur.h"

		/* printf_share_file() */
	#include "about.h"
		/* saved_name */
	#include "save.h"
		/* start_switch() */
	#include "start.h"

	#include "menu_tui.h"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	menu_tui_continue	(void);
static	void	menu_tui_select		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	menu_tui		(void)
{
	alx_resume_curses();

	/* Menu dimensions & options */
	int	h;
	int	w;
	h	= 10;
	w	= 34;
	int	N;
	N	= 4;
	struct Alx_Menu	mnu[4]	= {
		{7, 4, "[0]	Exit program"},
		{2, 4, "[1]	Continue"},
		{4, 4, "[2]	Disclaimer of warranty"},
		{5, 4, "[3]	Terms and conditions"}
	};

	/* Menu */
	bool	wh;
	int	sw;
	wh	= true;
	while (wh) {
		/* Menu loop */
		sw	= alx_menu(h, w, N, mnu, "MENU:");

		/* Selection */
		switch (sw) {
		case 0:
			wh	= false;
			break;

		case 1:
			menu_tui_continue();
			break;

		case 2:
			alx_pause_curses();
			print_share_file(SHARE_DISCLAIMER);
			getchar();
			alx_resume_curses();
			break;

		case 3:
			alx_pause_curses();
			print_share_file(SHARE_LICENSE);
			getchar();
			alx_resume_curses();
			break;
		}
	}

	alx_pause_curses();
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	menu_tui_continue	(void)
{
	/* Menu dimensions & options */
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	h	= 12;
	w	= 50;
	r	= 1;
	c	= (80 - w) / 2;
	int	N;
	N	= 3;
	struct Alx_Menu	mnu[3]	= {
		{5, 4, "[0]	Back"},
		{2, 4, "[1]	Start"},
		{3, 4, "[2]	Change file name"}
	};

	/* Input box */
	int	w2;
	int	r2;
	w2	= w - 8;
	r2	= r + h - 5;
	char	*txt[]	= {"File name:"};

	/* Menu */
	bool	wh;
	int	sw;
	wh	= true;
	while (wh) {
		/* Menu loop */
		win	= newwin(h, w, r, c);
		mvwprintw(win, mnu[1].r, mnu[1].c, "%s (File: \"%s\")",
							mnu[1].t, saved_name);
		wrefresh(win);
		sw	= alx_menu_2(win, N, mnu, "CONTINUE:");

		/* Selection */
		switch (sw) {
		case 0:
			alx_win_del(win);
			wh	= false;
			break;

		case 1:
			alx_win_del(win);
			alx_pause_curses();
			start_switch();
			alx_resume_curses();
			break;

		case 2:
			save_clr();
			alx_w_getfname(saved_path, saved_name, true, w2, r2,
								txt[0], NULL);
			alx_win_del(win);
			break;
		}
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
