/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/menu/tui.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ncurses.h>

#include <libalx/base/compiler/size.h>
#include <libalx/base/stdio/seekc.h>
#include <libalx/extra/ncurses/common.h>
#include <libalx/extra/ncurses/get.h>
#include <libalx/extra/ncurses/menu.h>

#include "vision-artificial/ctrl/start.h"
#include "vision-artificial/save/save.h"
#include "vision-artificial/share/share.h"
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


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	menu_continue	(void);
static	void	menu_devel	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	menu_tui		(void)
{
	static const struct Alx_Ncurses_Menu	mnu[]	= {
		{7, 4, "[0]	Exit program"},
		{2, 4, "[1]	Continue"},
		{4, 4, "[2]	Disclaimer of warranty"},
		{5, 4, "[3]	Terms and conditions"}
	};
	int_fast8_t	h, w;
	bool		wh;
	int_fast8_t	sw;

	alx_ncurses_resume();

	h	= 23;
	w	= 80;

	wh	= true;
	while (wh) {
		sw	= alx_ncurses_menu(h, w, ARRAY_SIZE(mnu), mnu, "MENU:");

		switch (sw) {
		case 0:
			wh = false;
			break;
		case 1:
			menu_continue();
			break;
		case 2:
			alx_ncurses_pause();
			print_share_file(SHARE_DISCLAIMER);
			alx_ncurses_resume();
			break;
		case 3:
			alx_ncurses_pause();
			print_share_file(SHARE_LICENSE);
			alx_ncurses_resume();
			break;
		}
	}

	alx_ncurses_pause();
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	menu_continue	(void)
{
	static const struct Alx_Ncurses_Menu	mnu[]	= {
		{6, 4, "[0]	Back"},
		{2, 4, "[1]	Start"},
		{3, 4, "[2]	Change file name"},
		{4, 4, "[3]	DEVEL"}
	};
	static const char  *const txt[]	= {"File name:"};
	WINDOW		*win;
	int_fast8_t	h, w;
	int_fast8_t	r, c;
	int_fast8_t	w2, r2;
	bool		wh;
	int_fast8_t	sw;

	h	= 23;
	w	= 80;
	r	= 1;
	c	= (80 - w) / 2;

	/* Input box */
	w2	= w - 8;
	r2	= r + h - 5;

	wh	= true;
	while (wh) {
		win	= newwin(h, w, r, c);
		mvwprintw(win, mnu[1].r, mnu[1].c, "%s (File: \"%s\")",
							mnu[1].t, saved_name);
		sw = alx_ncurses_menu_w(win, ARRAY_SIZE(mnu), mnu, "CONTINUE:");

		switch (sw) {
		case 0:
			wh	= false;
			break;
		case 1:
			alx_ncurses_delwin(win);
			alx_ncurses_pause();
			start_switch();
			printf("Press ENTER to continue");
			alx_wait4enter();
			alx_ncurses_resume();
			break;
		case 2:
			save_reset_fpath();
			alx_ncurses_get_fname(saved_path, saved_name, true,
						w2, r2, txt[0], NULL, 2);
			alx_ncurses_delwin(win);
			break;
		case 3:
			alx_ncurses_delwin(win);
			menu_devel();
			break;
		}
	}

	alx_ncurses_delwin(win);
}

static	void	menu_devel	(void)
{
	static const struct Alx_Ncurses_Menu	mnu[]	= {
		{4, 4, "[0]	Back"},
		{2, 4, "[1]	Change log mode"}
	};
	static const char  *const txt[]	= {
		"Modes: 0=Results; 1=Operations; 2=All"
	};
	WINDOW		*win;
	int_fast8_t	h, w;
	int_fast8_t	r, c;
	int_fast8_t	w2, r2;
	bool		wh;
	int_fast8_t	sw;

	h	= 23;
	w	= 80;
	r	= 1;
	c	= (80 - w) / 2;

	/* Input box */
	w2	= w - 8;
	r2	= r + h - 5;

	win	= newwin(h, w, r, c);

	wh	= true;
	while (wh) {
		sw	= alx_ncurses_menu_w(win, ARRAY_SIZE(mnu), mnu,
							"DEVELOPER OPTIONS:");
		switch (sw) {
		case 0:
			wh	= false;
			break;
		case 1:
			user_iface_log.visible = alx_ncurses_get_u8(0, 2, 2,
						w2, r2, txt[0], NULL, 2);
			break;
		}
	}

	alx_ncurses_delwin(win);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
