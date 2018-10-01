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

		/* user_clui() & ...save_name() */
	#include "user_clui.h"
		/* user_tui() & ...init() & ...cleanup() & ...save_name() */
	#include "user_tui.h"

	#include "user_iface.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	WIN_NAME	"Image"
	# define	WIN_TIMEOUT	(33)
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
		imgptr	= user_iface_act(user_action);
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


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
struct _IplImage	*user_iface_act	(int action)
{
	struct _IplImage	*imgptr;

	switch (action) {
	case USER_IFACE_ACT_QUIT:
		imgptr	= NULL;
		break;

	default:
		imgptr	= img_iface_act(action);
		break;
	}

	return	imgptr;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
