/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
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
	sscanf(buff, " %c", &ch);
	if (ch == 'y' || ch == 'Y') {
		puts (" >yes");
		print_share_file(SHARE_DISCLAIMER);
	} else {
		puts (" >NO");
	}

	ch	= 'n';
	printf("Read 'License'? (yes/NO): ");
	fgets(buff, BUFF_SIZE, stdin);
	sscanf(buff, " %c", &ch);
	if (ch == 'y' || ch == 'Y') {
		puts (" >yes");
		print_share_file(SHARE_LICENSE);
	} else {
		puts (" >NO");
	}
#if 0
	printf("Game interface? (NCURSES/text): ");
	scanf(" %c%*s ", &ch);
	if (ch == 't' || ch == 'T') {
		puts (" >text");
		// FIXME
	} else {
		puts (" >NCURSES");
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
	/* File name */ // FIXME
	alx_w_getfname(USER_SAVED_DIR, saved_name, "File name:", saved_name, NULL);
#endif
	menu_clui_start();
}

static	void	menu_clui_start		(void)
{
	puts(" >>START:");
	start_switch();

	char	buff [BUFF_SIZE];
	char	ch;

	ch	= 'm';
	printf("Load again? (MENU/load/exit): ");
	fgets(buff, BUFF_SIZE, stdin);
	sscanf(buff, " %c", &ch);
	if (ch == 'p' || ch == 'P') {
		puts (" >load");
		menu_clui_start();
	} else if (ch == 'e' || ch == 'E') {
		puts (" >exit!");
	} else {
		puts (" >MENU");
		menu_clui();
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
