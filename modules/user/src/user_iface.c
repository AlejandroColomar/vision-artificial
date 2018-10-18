/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* opencv */
	#include <cv.h>
		/* opencv gui */
	#include <highgui.h>
		/* snprintf() & fflush() */
	#include <stdio.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* img_iface_act() */
	#include "img_iface.h"
		/* proc_iface() */
	#include "proc.h"

		/* user_clui() & ...save_name() */
	#include "user_clui.h"
		/* user_tui() & ...init() & ...cleanup() & ...save_name() */
	#include "user_tui.h"

	#include "user_iface.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	TITLE_SIZE	(20)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
int			user_iface_mode;
struct User_Iface_Log	user_iface_log;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
struct _IplImage	*user_iface_act	(int action);
void			user_iface_show_ocr	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	user_iface_init		(void)
{
	user_iface_log.len	= 0;

	cvNamedWindow(WIN_NAME, CV_WINDOW_NORMAL);

	switch (user_iface_mode) {
	case USER_IFACE_CLUI:
		break;

	case USER_IFACE_TUI:
		user_tui_init();
		break;
	}
}

void	user_iface_cleanup	(void)
{
	switch (user_iface_mode) {
	case USER_IFACE_CLUI:
		break;

	case USER_IFACE_TUI:
		user_tui_cleanup();
		break;
	}

	cvDestroyWindow(WIN_NAME);

	fflush(stdout);
}

void	user_iface		(struct _IplImage  *imgptr)
{
	char	title[TITLE_SIZE];
	char	subtitle[TITLE_SIZE];
	int	user_action;

	snprintf(title, TITLE_SIZE, "Title");
	snprintf(subtitle, TITLE_SIZE, "Subtitle");

	do {
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

		/* Request user action */
		switch (user_iface_mode) {
		case USER_IFACE_CLUI:
			user_action	= user_clui(title, subtitle);
			break;

		case USER_IFACE_TUI:
			user_action	= user_tui(title, subtitle);
			break;
		}

		/* Do action */
		switch (user_action & 0xF000) {
		case USER_IFACE_ACT_PROC:
			proc_iface(user_action);
			break;
		default:
			user_iface_act(user_action);
			break;
		}

		imgptr	= img_iface_show();
	} while (user_action != USER_IFACE_ACT_QUIT);
}

void	user_iface_save_name	(const char *filepath, char *filename, int destsize)
{
	switch (user_iface_mode) {
	case USER_IFACE_CLUI:
		user_clui_save_name(filepath, filename, destsize);
		break;

	case USER_IFACE_TUI:
		user_tui_save_name(filepath, filename, destsize);
		break;
	}
}

int64_t	user_iface_getint	(double m, int64_t def, double M,
				const char *title, const char *help)
{
	int64_t	i;

	switch (user_iface_mode) {
	case USER_IFACE_CLUI:
		// FIXME
		i	= 1;
		break;

	case USER_IFACE_TUI:
		i	= user_tui_getint(m, def, M, title, help);
		break;
	}

	return	i;
}

double	user_iface_getdbl	(double m, double def, double M,
				const char *title, const char *help)
{
	int64_t	i;

	switch (user_iface_mode) {
	case USER_IFACE_CLUI:
		// FIXME
		i	= 1;
		break;

	case USER_IFACE_TUI:
		i	= user_tui_getdbl(m, def, M, title, help);
		break;
	}

	return	i;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
struct _IplImage	*user_iface_act		(int action)
{
	struct _IplImage	*imgptr;

	if (action & USER_IFACE_ACT_USRI) {
		imgptr	= NULL;

		switch (action) {
		case USER_IFACE_ACT_SHOW_OCR:
			user_iface_show_ocr();
			break;
		case USER_IFACE_ACT_QUIT:
			/* do nothing */
			break;
		}
	} else {
		img_iface_act(action, NULL);
	}

	return	imgptr;
}

void			user_iface_show_ocr	(void)
{
	switch (user_iface_mode) {
	case USER_IFACE_CLUI:
		// FIXME
		break;

	case USER_IFACE_TUI:
		user_tui_show_ocr();
		break;
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
