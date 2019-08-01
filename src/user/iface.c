/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/user/iface.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "libalx/base/stdio/get.h"

#include "vision-artificial/image/iface.h"
#include "vision-artificial/proc/iface.h"
#include "vision-artificial/user/tui.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define TITLE_SIZE	(80)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
struct User_Iface_Log	user_iface_log;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	user_iface_act		(int action);
static	void	user_iface_show_ocr	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	user_iface_init		(void)
{

	user_iface_log.len	= 0;
	user_iface_log.pos	= -1;

	user_tui_init();
}

void	user_iface_cleanup	(void)
{

	user_tui_cleanup();
	fflush(stdout);
}

void	user_iface		(void)
{
	char	title[TITLE_SIZE];
	char	subtitle[TITLE_SIZE];
	int	user_action;

	snprintf(title, TITLE_SIZE, "Title");
	snprintf(subtitle, TITLE_SIZE, "Subtitle");

	do {
		img_iface_show_img();

		user_action	= user_tui(title, subtitle);
		user_iface_act(user_action);
	} while (user_action != USER_IFACE_ACT_QUIT);
}

void	user_iface_show_log	(const char *restrict title,
				const char *restrict subtitle)
{

	user_tui_show_log(title, subtitle);
}

void	user_iface_fname	(const char *restrict filepath,
				char *restrict filename)
{

	user_tui_fname(filepath, filename);
}

double	user_iface_getdbl	(double m, double def, double M,
				const char *restrict title,
				const char *restrict help)
{

	return	user_tui_getint(m, def, M, title, help);
}

int	user_iface_getint	(double m, int64_t def, double M,
				const char *restrict title,
				const char *restrict help)
{

	return	user_tui_getdbl(m, def, M, title, help);
}

void	user_iface_log_write	(ptrdiff_t lvl, const char *restrict msg)
{

	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "%s",
									msg);
	user_iface_log.lvl[user_iface_log.len]	= lvl;
	user_iface_log.len++;
}

const char *user_iface_log_read	(void)
{

	user_iface_log.pos++;
	for (; user_iface_log.pos < user_iface_log.len; user_iface_log.pos++) {
		if (user_iface_log.lvl[user_iface_log.pos] <=
						user_iface_log.visible) {
			break;
		}
	}
	if (user_iface_log.pos == user_iface_log.len)
		return	NULL;
	return	&user_iface_log.line[user_iface_log.pos][0];
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	user_iface_act		(int action)
{

	if (action & USER_IFACE_ACT_USRI) {
		switch (action) {
		case USER_IFACE_ACT_SHOW_OCR:
			user_iface_show_ocr();
			break;
		case USER_IFACE_ACT_QUIT:
			/* do nothing */
			break;
		}
	} else if (action & USER_IFACE_ACT_PROC) {
		proc_iface_single(action);
	} else {
		img_iface_act(action);
	}
}

static	void	user_iface_show_ocr	(void)
{

	user_tui_show_ocr();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
