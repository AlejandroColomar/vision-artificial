/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* getchar() */
	#include <cstdio>

/* libalx --------------------------------------------------------------------*/
	#include "alx_ncur.hpp"

/* Project -------------------------------------------------------------------*/
		/* about_init() & print_cpright() */
	#include "about.hpp"
	#include "img_iface.hpp"
	#include "menu_iface.hpp"
	#include "user_iface.hpp"
	#include "parser.hpp"
	#include "save.hpp"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	init_all	(int *argc, char *(*argv[]));
static	void	cleanup		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	main	(int argc, char *argv[])
{
	init_all(&argc, &argv);

	/* Print copyright () and wait for any key to continue */
	print_share_file(SHARE_COPYRIGHT);
	getchar();

	/* Menu () */
	menu_iface();

	cleanup();

	return	0;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	init_all	(int *argc, char *(*argv[]))
{
	/* Init curses */
	alx_start_curses();
	alx_pause_curses();

	/* Init modules */
	about_init();
	save_init();

	/* Modes */
	menu_iface_mode		= MENU_IFACE_TUI;
	user_iface_mode		= USER_IFACE_TUI;
	user_iface_log.visible	= 2;

	/* Parse command line options */
	parser(*argc, *argv);
}

static	void	cleanup		(void)
{
	/* Clean img buffers */
	img_iface_cleanup_main();

	/* End curses */
	alx_resume_curses();
	alx_end_curses();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
