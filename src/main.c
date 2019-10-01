/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include <stdio.h>

#include <libalx/base/stdio/seekc.h>
#include <libalx/extra/ncurses/common.h>

#include "vision-artificial/image/iface.h"
#include "vision-artificial/menu/iface.h"
#include "vision-artificial/menu/parse.h"
#include "vision-artificial/save/save.h"
#include "vision-artificial/share/share.h"
#include "vision-artificial/user/iface.h"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	init	(int argc, char *argv[]);
static	void	deinit	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	main	(int argc, char *argv[])
{

	init(argc, argv);

	print_share_file(SHARE_COPYRIGHT);

	menu_iface();

	deinit();

	return	0;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	init	(int argc, char *argv[])
{

	alx_ncurses_init();
	alx_ncurses_pause();

	save_init();

	user_iface_log.visible	= 2;

	parse(argc, argv);
}

static	void	deinit	(void)
{

	img_iface_deinit();

	alx_ncurses_resume();
	alx_ncurses_deinit();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
