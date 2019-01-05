/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
	#include "alx_input.h"

	#include "about.h"
//	#include "save.h"
		/* start_switch() */
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
	char	ch;

	ch	= 'n';
	printf("Read 'Disclaimer of warranty'? (yes/NO): ");
	fgets(buff, BUFF_SIZE, stdin);
	if (sscanf(buff, " %c", &ch)) {
		return;
	}
	if (ch == 'y' || ch == 'Y') {
		printf(" >yes\n");
		print_share_file(SHARE_DISCLAIMER);
	} else {
		printf(" >NO\n");
	}

	ch	= 'n';
	printf("Read 'License'? (yes/NO): ");
	fgets(buff, BUFF_SIZE, stdin);
	if (sscanf(buff, " %c", &ch)) {
		return;
	}
	if (ch == 'y' || ch == 'Y') {
		printf(" >yes\n");
		print_share_file(SHARE_LICENSE);
	} else {
		printf(" >NO\n");
	}
#if 0
	printf("Game interface? (NCURSES/text): ");
	if (sscanf(buff, " %c", &ch)) {
		return;
	}
	if (ch == 't' || ch == 'T') {
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
	char	ch;

	ch	= 'm';
	printf("Load again? (MENU/load/exit): ");
	fgets(buff, BUFF_SIZE, stdin);
	if (sscanf(buff, " %c", &ch)) {
		return;
	}
	if (ch == 'p' || ch == 'P') {
		printf(" >load\n");
		menu_clui_start();
	} else if (ch == 'e' || ch == 'E') {
		printf(" >exit!\n");
	} else {
		printf(" >MENU\n");
		menu_clui();
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
