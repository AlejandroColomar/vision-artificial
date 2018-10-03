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
		/* user_iface_log */
	#include "user_iface.h"

	#include "img_cv.h"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Filters */
static	void	img_cv_invert	(struct _IplImage  *imgptr);
static	void	img_cv_bgr2gray	(struct _IplImage  **imgptr2);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_cv_act	(struct _IplImage  **imgptr2, int action)
{
	switch (action) {
	case IMG_CV_ACT_INVERT:
		img_cv_invert(*imgptr2);
		break;

	case IMG_CV_ACT_BGR2GRAY:
		img_cv_bgr2gray(imgptr2);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Filters	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	img_cv_invert	(struct _IplImage  *imgptr)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Invert color");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	int	step;
	int	chan;
	char	*data;

	step	= imgptr->widthStep;
	chan	= imgptr->nChannels;
	data	= imgptr->imageData;

	int	i;
	int	j;
	int	k;

	for (i = 0; i < imgptr->height; i++) {
		for (j = 0; j < imgptr->width; j++) {
			for (k = 0; k < imgptr->nChannels; k++) {
				data[i*step + j*chan + k]	=
						~data[i*step + j*chan + k];
			}
		}
	}
}

static	void	img_cv_bgr2gray	(struct _IplImage  **imgptr2)
{
	struct _IplImage  *tmp;

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "BGR -> Gray");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Create structure for tmp */
	tmp	= cvCreateImage(cvGetSize(*imgptr2), IPL_DEPTH_8U, 1);

	/* Write gray img into tmp */
	cvCvtColor(*imgptr2, tmp, CV_BGR2GRAY);

	/* Write tmp into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(tmp);
	
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
