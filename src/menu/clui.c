/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/menu/clui.h"

#include <ctype.h>
#include <stdlib.h>

#include "libalx/base/stdio/get.h"

#include "vision-artificial/about/about.h"
#include "vision-artificial/ctrl/start.h"


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
//static	void	menu_clui_rand		(void);
//static	void	menu_clui_custom	(void);
static	void	menu_clui_load		(void);
static	void	menu_clui_start		(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	menu_clui	(void)
{
	unsigned char	c;

	c = alx_get_ch("ny", true, true, "Read 'Disclaimer of warranty'? (yes/NO): ",
								NULL, 1);
	if (tolower(c) == 'y') {
		printf(" >yes\n");
		print_share_file(SHARE_DISCLAIMER);
	} else {
		printf(" >NO\n");
	}

	c = alx_get_ch("ny", true, true, "Read 'License'? (yes/NO): ", NULL, 1);
	if (tolower(c) == 'y') {
		printf(" >yes\n");
		print_share_file(SHARE_LICENSE);
	} else {
		printf(" >NO\n");
	}
#if 0
	c = alx_get_ch("nt", true, true, "Game interface? (NCURSES/text): ",
								NULL, 1);
	if (tolower(c) == 't') {
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
 ******* static functions (definitions) ***************************************
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
	unsigned char	c;

	printf(" >>START:\n");
	start_switch();

	c = alx_get_ch("mle", true, true, "Load again? (MENU/load/exit): ",
								NULL, 1);
	if (tolower(c) == 'l') {
		printf(" >load\n");
		menu_clui_start();
	} else if (tolower(c) == 'e') {
		printf(" >exit!\n");
		exit(EXIT_SUCCESS);
	} else {
		printf(" >MENU\n");
		menu_clui();
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
