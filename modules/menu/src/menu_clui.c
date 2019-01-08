/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
	#include "libalx/alx_input.h"

	#include "about.h"
//	#include "save.h"
	#include "start.h"

	#include "menu_clui.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	BUFF_SIZE	(1024)


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
//static	void	menu_clui_rand		(void);
//static	void	menu_clui_custom	(void);
static	void	menu_clui_load		(void);
static	void	menu_clui_start		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	menu_clui	(void)
{
	char	buff [BUFF_SIZE];
	char	c;

	c	= 'n';
	printf("Read 'Disclaimer of warranty'? (yes/NO): ");
	if (!fgets(buff, BUFF_SIZE, stdin)) {
		return;
	}
	if (sscanf(buff, " %c", &c)  !=  1) {
		return;
	}
	if (c == 'y' || c == 'Y') {
		printf(" >yes\n");
		print_share_file(SHARE_DISCLAIMER);
	} else {
		printf(" >NO\n");
	}

	c	= 'n';
	printf("Read 'License'? (yes/NO): ");
	if (!fgets(buff, BUFF_SIZE, stdin)) {
		return;
	}
	if (sscanf(buff, " %c", &c)  !=  1) {
		return;
	}
	if (c == 'y' || c == 'Y') {
		printf(" >yes\n");
		print_share_file(SHARE_LICENSE);
	} else {
		printf(" >NO\n");
	}
#if 0
	printf("Game interface? (NCURSES/text): ");
	if (!fgets(buff, BUFF_SIZE, stdin)) {
		return;
	}
	if (sscanf(buff, " %c", &c)  !=  1) {
		return;
	}
	if (c == 't' || c == 'T') {
		printf(" >text\n");
		// FIXME
	} else {
		printf(" >NCURSES\n");
		// FIXME
	}
#endif
	menu_clui_load();
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	menu_clui_load		(void)
{
#if 0
	/* File name */ /* FIXME */
	alx_w_getfname(USER_SAVED_DIR, saved_name, "File name:", saved_name, NULL);
#endif
	menu_clui_start();
}

static	void	menu_clui_start		(void)
{
	printf(" >>START:\n");
	start_switch();

	char	buff [BUFF_SIZE];
	char	c;

	c	= 'm';
	printf("Load again? (MENU/load/exit): ");
	if (!fgets(buff, BUFF_SIZE, stdin)) {
		return;
	}
	if (sscanf(buff, " %c", &c)  !=  1) {
		return;
	}
	if (c == 'p' || c == 'P') {
		printf(" >load\n");
		menu_clui_start();
	} else if (c == 'e' || c == 'E') {
		printf(" >exit!\n");
	} else {
		printf(" >MENU\n");
		menu_clui();
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
