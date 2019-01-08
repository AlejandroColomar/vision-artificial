/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* errno */
	#include <errno.h>
	#include <stddef.h>
		/* printf() */
	#include <stdio.h>
/* Project -------------------------------------------------------------------*/
		/* img_iface_load() */
	#include "img_iface.h"
		/* proc_iface_series() */
	#include "proc_iface.h"
		/* saved_name*/
	#include "save.h"
		/* user_iface() */
	#include "user_iface.h"
/* Module --------------------------------------------------------------------*/
	#include "start.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
int	start_mode;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	start_foo	(void);
static	void	start_single	(void);
static	void	start_series	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	start_switch	(void)
{

	switch (start_mode) {
	case START_FOO:
		start_foo();
		break;

	case START_SINGLE:
		start_single();
		break;

	case START_SERIES:
		start_series();
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	start_foo	(void)
{

	/* empty */
}

static	void	start_single	(void)
{

	errno	= 0;
	img_iface_load(NULL, saved_name);

	if (!errno) {
		user_iface_init();
		user_iface();
		user_iface_cleanup();
	} else {
		printf("errno:%i\n", errno);
	}

	img_iface_cleanup();
}

static	void	start_series	(void)
{
	int	tmp;

	tmp		= user_iface_mode;
	user_iface_mode	= USER_IFACE_CLUI;

	user_iface_init();
	proc_iface_series();
	user_iface_cleanup();

	user_iface_mode	= tmp;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
