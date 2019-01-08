/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <ncurses.h>
	#include <stdbool.h>
	#include <stddef.h>
/* libalx --------------------------------------------------------------------*/
	#include "libalx/alx_ncur.h"
/* Project -------------------------------------------------------------------*/
	#include "about.h"
	#include "proc_iface.h"
	#include "save.h"
	#include "start.h"
	#include "user_iface.h"
/* Module --------------------------------------------------------------------*/
	#include "menu_tui.h"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	menu_tui_continue	(void);
static	void	menu_tui_select		(void);
static	void	menu_tui_series		(void);
static	void	menu_tui_devel		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	menu_tui		(void)
{
	int	h;
	int	w;
	int	N;
	bool	wh;
	int	sw;

	alx_resume_curses();

	/* Menu dimensions & options */
	h	= 23;
	w	= 80;
	N	= 4;
	static const struct Alx_Menu	mnu[4]	= {
		{7, 4, "[0]	Exit program"},
		{2, 4, "[1]	Continue"},
		{4, 4, "[2]	Disclaimer of warranty"},
		{5, 4, "[3]	Terms and conditions"}
	};

	/* Menu */
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
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	int	N;
	int	w2;
	int	r2;
	bool	wh;
	int	sw;

	/* Menu dimensions & options */
	h	= 23;
	w	= 80;
	r	= 1;
	c	= (80 - w) / 2;
	N	= 6;
	static const struct Alx_Menu	mnu[6]	= {
		{8, 4, "[0]	Back"},
		{2, 4, "[1]	Start"},
		{3, 4, "[2]	Select"},
		{4, 4, "[3]	Series"},
		{5, 4, "[4]	Change file name"},
		{6, 4, "[5]	DEVEL"}
	};

	/* Input box */
	w2	= w - 8;
	r2	= r + h - 5;
	static const char  *const txt[]	= {"File name:"};

	/* Menu */
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
			wh	= false;
			break;

		case 1:
			alx_win_del(win);
			alx_pause_curses();
			start_switch();
			alx_resume_curses();
			break;

		case 2:
			alx_win_del(win);
			menu_tui_select();
			break;

		case 3:
			alx_win_del(win);
			menu_tui_series();
			break;

		case 4:
			save_clr();
			alx_w_getfname(saved_path, saved_name, true, w2, r2,
								txt[0], NULL);
			alx_win_del(win);
			break;

		case 5:
			alx_win_del(win);
			menu_tui_devel();
			break;
		}
	}

	/* Cleanup */
	alx_win_del(win);
}

static	void	menu_tui_select	(void)
{
	int	h;
	int	w;
	int	N;
	int	sw;

	/* Menu dimensions & options */
	h	= 23;
	w	= 80;
	N	= 3;
	static const struct Alx_Menu	mnu[3]	= {
		{6, 4, "[0]	Back"},
		{2, 4, "[1]	Single image"},
		{4, 4, "[2]	Series"}
	};

	/* Menu loop */
	sw	= alx_menu(h, w, N, mnu, "SELECT MODE:");

	/* Selection */
	switch (sw) {
	case 1:
		start_mode =	START_SINGLE;
		break;

	case 2:
		start_mode =	START_SERIES;
		break;
	}

}

static	void	menu_tui_series	(void)
{
	int	h;
	int	w;
	int	N;
	int	sw;

	/* Menu dimensions & options */
	h	= 23;
	w	= 80;
	N	= 6;
	static const struct Alx_Menu	mnu[6]	= {
		{8, 4, "[0]	Back"},
		{2, 4, "[1]	Label"},
		{3, 4, "[2]	Objects"},
		{4, 4, "[3]	Coins"},
		{5, 4, "[4]	Resistor"},
		{6, 4, "[5]	Lighters"}
	};

	/* Menu loop */
	sw	= alx_menu(h, w, N, mnu, "SELECT LEVEL:");

	/* Selection */
	switch (sw) {
	case 1:
		proc_mode	= PROC_MODE_LABEL_SERIES;
		break;
	case 2:
		proc_mode	= PROC_MODE_OBJECTS_SERIES;
		break;
	case 3:
		proc_mode	= PROC_MODE_COINS_SERIES;
		break;
	case 4:
		proc_mode	= PROC_MODE_RESISTOR_SERIES;
		break;
	case 5:
		proc_mode	= PROC_MODE_LIGHTERS_SERIES;
		break;
	}

}

static	void	menu_tui_devel		(void)
{
	WINDOW	*win;
	int	h;
	int	w;
	int	r;
	int	c;
	int	N;
	int	w2;
	int	r2;
	bool	wh;
	int	sw;

	h	= 23;
	w	= 80;
	r	= 1;
	c	= (80 - w) / 2;
	N	= 4;
	static const struct Alx_Menu	mnu[4]	= {
		{6, 4, "[0]	Back"},
		{2, 4, "[1]	Change process mode"},
		{3, 4, "[2]	Change log mode"},
		{4, 4, "[3]	Change user iface mode"}
	};

	/* Input box */
	w2	= w - 8;
	r2	= r + h - 5;
	static const char  *const txt[]	= {
		"Modes: 0=Auto; 1=Stop@prod; 2=Delay@step; 3=Stop@step",
		"Modes: 0=Results; 1=Operations; 2=All",
		"Modes: 1=CLUI; 2=TUI"
	};

	/* Menu */
	win	= newwin(h, w, r, c);

	/* Menu loop */
	wh	= true;
	while (wh) {
		/* Selection */
		sw	= alx_menu_2(win, N, mnu, "DEVELOPER OPTIONS:");

		switch (sw) {
		case 0:
			wh	= false;
			break;

		case 1:
			proc_debug		= alx_w_getint(w2, r2, txt[0],
						PROC_DBG_NO, 0,
						PROC_DBG_STOP_STEP, NULL);
			break;

		case 2:
			user_iface_log.visible	= alx_w_getint(w2, r2, txt[1],
								0, 2, 2, NULL);
			break;

		case 3:
			user_iface_mode		= alx_w_getint(w2, r2, txt[2],
								1, 2, 2, NULL);
			break;
		}
	}

	/* Cleanup */
	alx_win_del(win);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
