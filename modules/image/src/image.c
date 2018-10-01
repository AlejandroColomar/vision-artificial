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
		/* load_image_file() */
	#include "save.h"

	#include "image.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
static	struct _IplImage	*image_copy_old;
static	struct _IplImage	*image_copy_tmp;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Examples */
static	void	img_invert	(void);
	/* Apply / Save / Discard */
static	void	img_apply	(void);
static	void	img_save	(void);
static	void	img_discard	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
struct _IplImage	*img_load	(void)
{
	load_image_file();

	/* Make a static copy of image */
	cvReleaseImage(&image_copy_old);
	image_copy_old	= cvCloneImage(image);
	cvReleaseImage(&image_copy_tmp);
	image_copy_tmp	= cvCloneImage(image);

	return	image_copy_tmp;
}

void			img_cleanup	(void)
{
	cvReleaseImage(&image_copy_old);
	cvReleaseImage(&image_copy_tmp);
}

struct _IplImage	*img_act	(int action)
{
	switch (action) {
	case IMG_ACT_INVERT:
		img_invert();
		break;

	case IMG_ACT_APPLY:
		img_apply();
		break;

	case IMG_ACT_SAVE:
		img_save();
		break;

	case IMG_ACT_DISCARD:
		img_discard();
		break;
	}

	return	image_copy_tmp;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/

/*	*	*	*	*	*	*	*	*	*
 *	*	* Examples	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	img_invert	(void)
{
	int	step;
	int	chan;
	char	*data;

	step	= image_copy_tmp->widthStep;
	chan	= image_copy_tmp->nChannels;
	data	= image_copy_tmp->imageData;

	int	i;
	int	j;
	int	k;

	for (i = 0; i < image_copy_tmp->height; i++) {
		for (j = 0; j < image_copy_tmp->width; j++) {
			for (k = 0; k < image_copy_tmp->nChannels; k++) {
				data[i*step + j*chan + k]	= ~data[i*step + j*chan + k];
			}
		}
	}
}

/*	*	*	*	*	*	*	*	*	*
 *	*	* Apply / Save / Discard	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	img_apply	(void)
{
	cvReleaseImage(&image_copy_old);
	image_copy_old	= cvCloneImage(image_copy_tmp);
}

static	void	img_save	(void)
{
	/* Write into image (save.c) */
	cvReleaseImage(&image);
	image	= cvCloneImage(image_copy_old);

	save_image_file();
}

static	void	img_discard	(void)
{
	cvReleaseImage(&image_copy_tmp);
	image_copy_tmp	= cvCloneImage(image_copy_old);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
