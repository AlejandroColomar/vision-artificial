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
static	void	img_cv_invert		(struct _IplImage  *imgptr);
static	void	img_cv_bgr2gray		(struct _IplImage  **imgptr2);
static	void	img_cv_component	(struct _IplImage  **imgptr2);
static	void	img_cv_threshold	(struct _IplImage  **imgptr2);
static	void	img_cv_rotate		(struct _IplImage  **imgptr2);


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

	case IMG_CV_ACT_COMPONENT:
		img_cv_component(imgptr2);
		break;

	case IMG_CV_ACT_THRESHOLD:
		img_cv_threshold(imgptr2);
		break;

	case IMG_CV_ACT_ROTATE:
		img_cv_rotate(imgptr2);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Filters	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	img_cv_invert		(struct _IplImage  *imgptr)
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

static	void	img_cv_bgr2gray		(struct _IplImage  **imgptr2)
{
	struct _IplImage  *gray;

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "BGR -> Gray");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Create structure for tmp */
	gray	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 1);

	/* Write gray img into gray */
	cvCvtColor(*imgptr2, gray, CV_BGR2GRAY);

	/* Write gray into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(gray);

	/* clean up */
	cvReleaseImage(&gray);	
}

static	void	img_cv_component	(struct _IplImage  **imgptr2)
{
	struct _IplImage  *cmp_B;
	struct _IplImage  *cmp_G;
	struct _IplImage  *cmp_R;

	/* Ask user which component to extract */
	char	title [80];
	int	cmp;
	snprintf(title, 80, "Component: B = 0, G = 1, R = 2");
	cmp	= user_iface_getint(0, 0, 2, title, NULL);

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Component %i", cmp);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Create structure for tmp */
	cmp_B	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 1);
	cmp_G	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 1);
	cmp_R	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 1);

	/* Write gray img into tmp */
	cvSplit(*imgptr2, cmp_B, cmp_G, cmp_R, NULL);

	/* Write tmp into imgptr2 */
	cvReleaseImage(imgptr2);
	switch (cmp) {
	case 0:
		*imgptr2	= cvCloneImage(cmp_B);
		break;
	case 1:
		*imgptr2	= cvCloneImage(cmp_G);
		break;
	case 2:
		*imgptr2	= cvCloneImage(cmp_R);
		break;
	}

	/* clean up */
	cvReleaseImage(&cmp_B);
	cvReleaseImage(&cmp_G);
	cvReleaseImage(&cmp_R);	
}

static	void	img_cv_threshold	(struct _IplImage  **imgptr2)
{
	struct _IplImage  *img_thr;

	/* Ask user which threshold type to apply */
	char	title [80];
	int	thr_typ;
	snprintf(title, 80, "Type: BIN=0, BIN_INV=1, TRUNC=2, TOZ=3, TOZ_INV=4");
	thr_typ	= user_iface_getint(0, 0, 4, title, NULL);

	/* Ask user which threshold value to apply */
	int	thr_val;
	snprintf(title, 80, "Value: 0 to 255 (or -1 for Otsu's algorithm)");
	thr_val	= user_iface_getint(-1, 0, 255, title, NULL);
	if (thr_val == -1) {
		thr_typ	|= CV_THRESH_OTSU;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Threshold typ=%i, val=%i",
						thr_typ, thr_val);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Create structure for tmp */
	img_thr	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 1);

	/* Write gray img into gray */
	cvThreshold(*imgptr2, img_thr, thr_val, 0xFF, thr_typ);

	/* Write B & W img into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(img_thr);

	/* clean up */
	cvReleaseImage(&img_thr);	
}

static	void	img_cv_rotate		(struct _IplImage  **imgptr2)
{
	struct _IplImage  *rotated;
	struct _IplImage  *imgtmp;

	/* Ask user how to rotate */
	char	title [80];
	int	rot;
	snprintf(title, 80, "Rotate (counterclockwise) n * pi/2 rad;  n:");
	rot	= user_iface_getint(1, 1, 3, title, NULL);

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Rotate %i * pi/2 rad", rot);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	switch (rot) {
	case 1:
		/* Init structures */
		imgtmp	= cvCreateImage(cvSize((*imgptr2)->height, (*imgptr2)->width),
				(*imgptr2)->depth, (*imgptr2)->nChannels);
		rotated	= cvCreateImage(cvSize((*imgptr2)->height, (*imgptr2)->width),
				(*imgptr2)->depth, (*imgptr2)->nChannels);

		/* Rotate: transpose and flip around horizontal axis: flip_mode=0 */
		cvTranspose(*imgptr2, imgtmp);
		cvFlip(imgtmp, rotated, 0);
		break;

	case 2:
		/* Init structures */
		imgtmp	= NULL;
		rotated	= cvCreateImage(cvGetSize(*imgptr2),
				(*imgptr2)->depth, (*imgptr2)->nChannels);

		/* Rotate: Flip both axises: flip_mode=-1 */
		cvFlip(*imgptr2, rotated, -1);
		break;

	case 3:
		/* Init structures */
		imgtmp	= cvCreateImage(cvSize((*imgptr2)->height, (*imgptr2)->width),
				(*imgptr2)->depth, (*imgptr2)->nChannels);
		rotated	= cvCreateImage(cvSize((*imgptr2)->height, (*imgptr2)->width),
				(*imgptr2)->depth, (*imgptr2)->nChannels);

		/* Rotate: transpose and flip around vertical axis: flip_mode=1 */
		cvTranspose(*imgptr2, imgtmp);
		cvFlip(imgtmp, rotated, 1);
		break;
	}

	/* Write rotated into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(rotated);

	/* clean up */
	cvReleaseImage(&imgtmp);
	cvReleaseImage(&rotated);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
