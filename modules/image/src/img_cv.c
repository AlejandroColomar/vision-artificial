/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* snprintf() */
	#include <stdio.h>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <cv.h>

/* Project -------------------------------------------------------------------*/
		/* user_iface_log */
	#include "user_iface.h"

/* Module --------------------------------------------------------------------*/
		/* data */
	#include "img_iface.h"

	#include "img_cv.h"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Filters */
static	void	img_cv_invert		(struct _IplImage  *imgptr);
static	void	img_cv_bgr2gray		(struct _IplImage  **imgptr2);
static	void	img_cv_component	(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_smooth		(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_threshold	(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_adaptive_thr	(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_dilate		(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_erode		(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_contours		(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_min_area_rect	(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_rotate_orto	(struct _IplImage  **imgptr2, void *data);
static	void	img_cv_rotate		(struct _IplImage  **imgptr2, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_cv_act	(struct _IplImage  **imgptr2, int action, void *data)
{
	switch (action) {
	case IMG_CV_ACT_INVERT:
		img_cv_invert(*imgptr2);
		break;

	case IMG_CV_ACT_BGR2GRAY:
		img_cv_bgr2gray(imgptr2);
		break;
	case IMG_CV_ACT_COMPONENT:
		img_cv_component(imgptr2, data);
		break;

	case IMG_CV_ACT_SMOOTH:
		img_cv_smooth(imgptr2, data);
		break;

	case IMG_CV_ACT_THRESHOLD:
		img_cv_threshold(imgptr2, data);
		break;
	case IMG_CV_ACT_ADAPTIVE_THRESHOLD:
		img_cv_adaptive_thr(imgptr2, data);
		break;

	case IMG_CV_ACT_DILATE:
		img_cv_dilate(imgptr2, data);
		break;
	case IMG_CV_ACT_ERODE:
		img_cv_erode(imgptr2, data);
		break;

	case IMG_CV_ACT_CONTOURS:
		img_cv_contours(imgptr2, data);
		break;
	case IMG_CV_ACT_MIN_AREA_RECT:
		img_cv_min_area_rect(imgptr2, data);
		break;

	case IMG_CV_ACT_ROTATE_ORTO:
		img_cv_rotate_orto(imgptr2, data);
		break;
	case IMG_CV_ACT_ROTATE:
		img_cv_rotate(imgptr2, data);
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

static	void	img_cv_component	(struct _IplImage  **imgptr2, void *data)
{
	struct _IplImage	*cmp_B;
	struct _IplImage	*cmp_G;
	struct _IplImage	*cmp_R;

	/* Data */
	struct Img_Iface_Data_Component	*data_cast;
	data_cast	= (struct Img_Iface_Data_Component *)data;

	/* Color component */
	int	cmp;
	cmp	= data_cast->cmp;

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

static	void	img_cv_smooth		(struct _IplImage  **imgptr2, void *data)
{
	struct _IplImage	*img_smth;

	/* Data */
	struct Img_Iface_Data_Smooth	*data_cast;
	data_cast	= (struct Img_Iface_Data_Smooth *)data;

	/* Smoothing method */
	int	method;
	method	= data_cast->method;
	/* Mask size */
	int	msk_siz;
	msk_siz	= data_cast->msk_siz;
	if (!(msk_siz % 2)) {
		msk_siz++;
	}

	/* Create structure for tmp */
	img_smth	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 1);

	/* Write smooth img into img_smth */
	cvSmooth(*imgptr2, img_smth, method, msk_siz, msk_siz, 0, 0);

	/* Write img_thr into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(img_smth);

	/* clean up */
	cvReleaseImage(&img_smth);	
}

static	void	img_cv_threshold	(struct _IplImage  **imgptr2, void *data)
{
	struct _IplImage	*img_thr;

	/* Data */
	struct Img_Iface_Data_Threshold	*data_cast;
	data_cast	= (struct Img_Iface_Data_Threshold *)data;

	/* Threshold type */
	int	thr_typ;
	thr_typ	= data_cast->thr_typ;
	/* Threshold value */
	int	thr_val;
	thr_val	= data_cast->thr_val;
	if (thr_val == -1) {
		thr_typ	|= CV_THRESH_OTSU;
	}

	/* Create structure for tmp */
	img_thr	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 1);

	/* Write thr img into img_thr */
	cvThreshold(*imgptr2, img_thr, thr_val, 0xFF, thr_typ);

	/* Write img_thr into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(img_thr);

	/* clean up */
	cvReleaseImage(&img_thr);	
}

static	void	img_cv_adaptive_thr	(struct _IplImage  **imgptr2, void *data)
{
	struct _IplImage	*img_thr;

	/* Data */
	struct Img_Iface_Data_Adaptive_Thr	*data_cast;
	data_cast	= (struct Img_Iface_Data_Adaptive_Thr *)data;

	/* Threshold type to apply */
	int	thr_typ;
	thr_typ	= data_cast->thr_typ;
	/* Neighbourhood size */
	int	nbh_val;
	nbh_val	= data_cast->nbh_val;
	if (!(nbh_val % 2)) {
		nbh_val++;
	}

	/* Create structure for tmp */
	img_thr	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 1);

	/* Write thr img into img_thr */
	cvAdaptiveThreshold(*imgptr2, img_thr, 0xFF,
			CV_ADAPTIVE_THRESH_GAUSSIAN_C, thr_typ, nbh_val, 0);

	/* Write img_thr into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(img_thr);

	/* clean up */
	cvReleaseImage(&img_thr);	
}

static	void	img_cv_dilate		(struct _IplImage  **imgptr2, void *data)
{
	struct _IplImage	*dilated;

	/* Data */
	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Dilate_Erode *)data;

	/* Iterations */
	int	i;
	i	= data_cast->i;

	/* Create structure for dilated */
	dilated	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth,
							(*imgptr2)->nChannels);

	/* Dilate */
	cvDilate(*imgptr2, dilated, NULL, i);

	/* Write dilated into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(dilated);

	/* clean up */
	cvReleaseImage(&dilated);
}

static	void	img_cv_erode		(struct _IplImage  **imgptr2, void *data)
{
	struct _IplImage	*eroded;

	/* Data */
	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Dilate_Erode *)data;

	/* Iterations */
	int	i;
	i	= data_cast->i;

	/* Create structure for eroded */
	eroded	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth,
							(*imgptr2)->nChannels);

	/* Erode */
	cvErode(*imgptr2, eroded, NULL, i);

	/* Write eroded into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(eroded);

	/* clean up */
	cvReleaseImage(&eroded);
}

static	void	img_cv_contours		(struct _IplImage  **imgptr2, void *data)
{
	/* Black image (3 channels) */
	struct _IplImage	*imgtmp;
	imgtmp	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth, 3);
	cvSet(imgtmp, CV_RGB(0, 0, 0), NULL);

	/* Data */
	struct Img_Iface_Data_Contours	*data_cast;
	data_cast	= (struct Img_Iface_Data_Contours *)data;

	/* Contours */
	struct CvMemStorage	**storage;
	storage		= data_cast->storage;
	struct CvSeq		**contours;
	contours	= data_cast->contours;

	/* Get contours */
	int	i;
	i	= cvFindContours(*imgptr2, *storage, contours, sizeof(CvContour),
			CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	/* Draw contours in color */
	cvDrawContours(imgtmp, *contours, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0),
							1, 1, 8, cvPoint(0, 0));

	/* imgptr2 was modified by cvFindContours */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(imgtmp);

	/* clean up */
	cvReleaseImage(&imgtmp);	
}

static	void	img_cv_min_area_rect	(struct _IplImage  **imgptr2, void *data)
{
	/* Data */
	struct Img_Iface_Data_MinARect	*data_cast;
	data_cast	= (struct Img_Iface_Data_MinARect *)data;

	/* Contours */
	struct CvSeq	**contours;
	contours	= data_cast->contours;
	/* Rotated rectangle */
	struct CvBox2D	*rect;
	rect		= data_cast->rect;

	/* Get rectangle */
	*rect	= cvMinAreaRect2(*contours, NULL);

	/* Draw rectangle */
	struct CvPoint2D32f	points[4];
	cvBoxPoints(*rect, points);
	cvLine(*imgptr2, cvPoint(points[0].x, points[0].y),
			cvPoint(points[1].x, points[1].y), CV_RGB(0, 0, 255),
								1, 8, 0);
	cvLine(*imgptr2, cvPoint(points[1].x, points[1].y),
			cvPoint(points[2].x, points[2].y), CV_RGB(0, 0, 255),
								1, 8, 0);
	cvLine(*imgptr2, cvPoint(points[2].x, points[2].y),
			cvPoint(points[3].x, points[3].y), CV_RGB(0, 0, 255),
								1, 8, 0);
	cvLine(*imgptr2, cvPoint(points[3].x, points[3].y),
			cvPoint(points[0].x, points[0].y), CV_RGB(0, 0, 255),
								1, 8, 0);
}

static	void	img_cv_rotate_orto	(struct _IplImage  **imgptr2, void *data)
{
	struct _IplImage	*rotated;
	struct _IplImage	*imgtmp;

	/* Data */
	struct Img_Iface_Data_Rotate_Orto	*data_cast;
	data_cast	= (struct Img_Iface_Data_Rotate_Orto *)data;

	/* Rotate n * pi/2 rad */
	int	n;
	n	= data_cast->n;

	switch (n) {
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

static	void	img_cv_rotate		(struct _IplImage  **imgptr2, void *data)
{
	struct _IplImage	*rotated;

	/* Data */
	struct Img_Iface_Data_Rotate	*data_cast;
	data_cast	= (struct Img_Iface_Data_Rotate *)data;

	/* Angle of rotation */
	struct CvPoint2D32f	center;
	center		= data_cast->center;
	double			angle;
	angle		= data_cast->angle;

	/* Create structure for dilated */
	rotated	= cvCreateImage(cvGetSize(*imgptr2), (*imgptr2)->depth,
							(*imgptr2)->nChannels);

	/* Init map_matrix */
	struct CvMat	*map_matrix;
	map_matrix	= cvCreateMat(2, 3, CV_32F);

	/* Get map_matrix */
	cv2DRotationMatrix(center, angle, 1, map_matrix);

	/* Rotate */
	cvWarpAffine(*imgptr2, rotated, map_matrix,
			CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

	/* Write rotated into imgptr2 */
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCloneImage(rotated);

	/* clean up */
	cvReleaseImage(&rotated);
	cvReleaseMat(&map_matrix);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
