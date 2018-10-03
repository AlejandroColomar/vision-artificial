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
		/* snprintf() */
	#include <stdio.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* load_image_file() */
	#include "save.h"
		/* user_iface_log */
	#include "user_iface.h"

		/* img_act() */
	#include "img_cv.h"

	#include "img_iface.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
static	struct _IplImage	*image_copy_old;
static	struct _IplImage	*image_copy_tmp;
static	struct _IplImage	*image_copy_usr;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Actions */
static	void	img_iface_action	(int action);
static	void	img_iface_invert	(void);
static	void	img_iface_bgr2gray	(void);
static	void	img_iface_apply		(void);
static	void	img_iface_save		(void);
static	void	img_iface_discard	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
struct _IplImage	*img_iface_load		(void)
{
	load_image_file();

	/* Make a static copy of image */
	cvReleaseImage(&image_copy_old);
	image_copy_old	= cvCloneImage(image);
	cvReleaseImage(&image_copy_tmp);
	image_copy_tmp	= cvCloneImage(image);

	/* Make a copy of the image so that it isn't modified by the user */
	cvReleaseImage(&image_copy_usr);
	image_copy_usr	= cvCloneImage(image_copy_tmp);
	return	image_copy_tmp;
}

void			img_iface_cleanup	(void)
{
	cvReleaseImage(&image_copy_old);
	cvReleaseImage(&image_copy_tmp);
	cvReleaseImage(&image_copy_usr);
}

struct _IplImage	*img_iface_act		(int action)
{
	img_iface_action(action);

	/* Make a static copy of tmp so that it isn't modified by the user */
	cvReleaseImage(&image_copy_usr);
	image_copy_usr	= cvCloneImage(image_copy_tmp);

	return	image_copy_usr;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Actions	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	img_iface_action	(int action)
{
	switch (action) {
	/* img_cv */
	case IMG_IFACE_ACT_INVERT:
		img_iface_invert();
		break;
	case IMG_IFACE_ACT_BGR2GRAY:
		img_iface_bgr2gray();
		break;

	/* img_iface */
	case IMG_IFACE_ACT_APPLY:
		img_iface_apply();
		break;
	case IMG_IFACE_ACT_SAVE:
		img_iface_save();
		break;
	case IMG_IFACE_ACT_DISCARD:
		img_iface_discard();
		break;

	default:
		/* Invalid action */
		break;
	}
}

static	void	img_iface_invert	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Invert color");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Filter: invert color */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_INVERT);
}

static	void	img_iface_bgr2gray	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "BGR -> Gray");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Filter: invert color */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_BGR2GRAY);
}

static	void	img_iface_apply		(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Apply changes");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write tmp into old */
	cvReleaseImage(&image_copy_old);
	image_copy_old	= cvCloneImage(image_copy_tmp);
}

static	void	img_iface_save		(void)
{
	/* Apply changes before saving */
	img_iface_apply();

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Save as...");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write into image struct (save.c) */
	cvReleaseImage(&image);
	image	= cvCloneImage(image_copy_old);

	/* Save into file */
	save_image_file();
}

static	void	img_iface_discard	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Discard changes");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Discard tmp image copy */
	cvReleaseImage(&image_copy_tmp);
	image_copy_tmp	= cvCloneImage(image_copy_old);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
