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

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* img_load() & img_cleanup() & img_act() */
	#include "image.h"

	#include "img_iface.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
static	struct _IplImage	*image_copy;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Actions */
static	struct _IplImage	*img_iface_action	(int action);
static	struct _IplImage	*img_iface_save		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
struct _IplImage	*img_iface_load		(void)
{
	struct _IplImage	*imgptr;

	imgptr	= img_load();

	/* Make a static copy of imgptr so that it isn't modified by the user */
	cvReleaseImage(&image_copy);
	image_copy	= cvCloneImage(imgptr);
	return	image_copy;
}

void			img_iface_cleanup	(void)
{
	img_cleanup();

	cvReleaseImage(&image_copy);
}

struct _IplImage	*img_iface_act		(int action)
{
	struct _IplImage	*imgptr;

	imgptr	= img_iface_action(action);

	/* Make a static copy of imgptr so that it isn't modified by the user */
	if (imgptr != NULL) {
		cvReleaseImage(&image_copy);
		image_copy	= cvCloneImage(imgptr);
	} else {
		image_copy	= NULL;
	}

	return	image_copy;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Actions	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	struct _IplImage	*img_iface_action	(int action)
{
	struct _IplImage	*imgptr;

	switch (action) {
	case IMG_IFACE_ACT_INVERT:
		imgptr	= img_act(IMG_ACT_INVERT);
		break;

	case IMG_IFACE_ACT_APPLY:
		imgptr	= img_act(IMG_ACT_APPLY);
		break;

	case IMG_IFACE_ACT_SAVE:
		imgptr	= img_iface_save();
		break;

	case IMG_IFACE_ACT_DISCARD:
		imgptr	= img_act(IMG_ACT_DISCARD);
		break;

	default:
		/* Invalid action */
		imgptr	= NULL;
		break;
	}

	return	imgptr;
}

static	struct _IplImage	*img_iface_save		(void)
{
	struct _IplImage	*imgptr;

	img_act(IMG_ACT_APPLY);
	imgptr	= img_act(IMG_ACT_SAVE);

	return	imgptr;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
