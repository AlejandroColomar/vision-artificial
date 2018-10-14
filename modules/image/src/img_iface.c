/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* INFINITY */
	#include <math.h>
		/* snprintf() */
	#include <stdio.h>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <cv.h>

/* Project -------------------------------------------------------------------*/
		/* load_image_file() */
	#include "save.h"
		/* user_iface_log */
	#include "user_iface.h"

/* Module --------------------------------------------------------------------*/
		/* img_cv_act() */
	#include "img_cv.h"
		/* img_zb_act() */
	#include "img_zbar.h"
		/* OCR */
	#include "img_ocr.h"
		/* ORB */
	#include "img_orb.h"

	#include "img_iface.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	IMG_MEM_SIZE	(10)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
	char	img_ocr_text [OCR_TEXT_MAX];

/* Static --------------------------------------------------------------------*/
static	struct _IplImage	*image_copy_old;
static	struct _IplImage	*image_copy_tmp;
static	struct _IplImage	*image_copy_usr;
static	struct _IplImage	*image_mem [IMG_MEM_SIZE];
static	struct _IplImage	*image_ref;
static	struct CvMemStorage	*storage;
static	struct CvSeq		*contours;
static	struct CvBox2D		rectangle;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Actions */
static	void	img_iface_action	(int action, void *data);
	/* img_cv */
static	void	img_iface_invert	(void);
static	void	img_iface_bgr2gray	(void);
static	void	img_iface_component	(void *data);
static	void	img_iface_smooth	(void *data);
static	void	img_iface_threshold	(void *data);
static	void	img_iface_adaptive_thr	(void *data);
static	void	img_iface_dilate	(void *data);
static	void	img_iface_erode		(void *data);
static	void	img_iface_contours	(void *data);
static	void	img_iface_min_area_rect	(void *data);
static	void	img_iface_rotate_orto	(void *data);
static	void	img_iface_rotate	(void *data);

static	void	img_iface_dilate_erode	(void *data);
static	void	img_iface_erode_dilate	(void *data);
	/* img_zbar */
static	void	img_iface_decode	(void *data);
	/* img_ocr */
static	void	img_iface_read		(void *data);
	/* img_orb */
static	void	img_iface_align		(void);
	/* img_iface */
static	void	img_iface_apply		(void);
static	void	img_iface_discard	(void);
static	void	img_iface_save_mem	(void *data);
static	void	img_iface_load_mem	(void *data);
static	void	img_iface_save_ref	(void);
	/* save */
static	void	img_iface_save_file	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void			img_iface_init		(void)
{
	storage	= cvCreateMemStorage(0);
}

void			img_iface_cleanup_main	(void)
{
	int	i;

	for (i = 0; i < IMG_MEM_SIZE; i++) {
		cvReleaseImage(&image_mem[i]);
	}
	cvReleaseImage(&image_ref);
	cvReleaseMemStorage(&storage);
}

struct _IplImage	*img_iface_load		(void)
{
	/* Init */
	img_iface_init();

	/* Load file */
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

struct _IplImage	*img_iface_act		(int action, void *data)
{
	img_iface_action(action, data);

	/* Make a static copy of tmp so that it isn't modified by the user */
	cvReleaseImage(&image_copy_usr);
	image_copy_usr	= cvCloneImage(image_copy_tmp);

	return	image_copy_usr;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/* Actions -------------------------------------------------------------------*/
static	void	img_iface_action	(int action, void *data)
{
	switch (action) {
	/* img_cv */
	case IMG_IFACE_ACT_INVERT:
		img_iface_invert();
		break;
	case IMG_IFACE_ACT_BGR2GRAY:
		img_iface_bgr2gray();
		break;
	case IMG_IFACE_ACT_COMPONENT:
		img_iface_component(data);
		break;
	case IMG_IFACE_ACT_SMOOTH:
		img_iface_smooth(data);
		break;
	case IMG_IFACE_ACT_THRESHOLD:
		img_iface_threshold(data);
		break;
	case IMG_IFACE_ACT_ADAPTIVE_THRESHOLD:
		img_iface_adaptive_thr(data);
		break;
	case IMG_IFACE_ACT_DILATE:
		img_iface_dilate(data);
		break;
	case IMG_IFACE_ACT_ERODE:
		img_iface_erode(data);
		break;
	case IMG_IFACE_ACT_CONTOURS:
		img_iface_contours(data);
		break;
	case IMG_IFACE_ACT_MIN_AREA_RECT:
		img_iface_min_area_rect(data);
		break;
	case IMG_IFACE_ACT_ROTATE_ORTO:
		img_iface_rotate_orto(data);
		break;
	case IMG_IFACE_ACT_ROTATE:
		img_iface_rotate(data);
		break;

	case IMG_IFACE_ACT_DILATE_ERODE:
		img_iface_dilate_erode(data);
		break;
	case IMG_IFACE_ACT_ERODE_DILATE:
		img_iface_erode_dilate(data);
		break;

	/* img_zbar */
	case IMG_IFACE_ACT_DECODE:
		img_iface_decode(data);
		break;

	/* img_ocr */
	case IMG_IFACE_ACT_READ:
		img_iface_read(data);
		break;

	/* img_orb */
	case IMG_IFACE_ACT_ALIGN:
		img_iface_align();
		break;

	/* img_iface */
	case IMG_IFACE_ACT_APPLY:
		img_iface_apply();
		break;
	case IMG_IFACE_ACT_DISCARD:
		img_iface_discard();
		break;
	case IMG_IFACE_ACT_SAVE_MEM:
		img_iface_save_mem(data);
		break;
	case IMG_IFACE_ACT_LOAD_MEM:
		img_iface_load_mem(data);
		break;
	case IMG_IFACE_ACT_SAVE_REF:
		img_iface_save_ref();
		break;

	/* save */
	case IMG_IFACE_ACT_SAVE_FILE:
		img_iface_save_file();
		break;

	default:
		/* Invalid action */
		break;
	}
}

/* img_cv --------------------------------------------------------------------*/
static	void	img_iface_invert	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Invert color");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: invert color */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_INVERT, NULL);
}

static	void	img_iface_bgr2gray	(void)
{
	/* Must have 3 channels */
	if (image_copy_tmp->nChannels != 3) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		return;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"BGR -> Gray");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: BGR to gray */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_BGR2GRAY, NULL);
}

static	void	img_iface_component	(void *data)
{
	/* Must have 3 channels */
	if (image_copy_tmp->nChannels != 3) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Component	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Component: B = 0, G = 1, R = 2");
		data_tmp.cmp	= user_iface_getint(0, 0, 2, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Component	*data_cast;
	data_cast	= (struct Img_Iface_Data_Component *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Component %i",
						data_cast->cmp);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: extract component */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_COMPONENT, data);
}

static	void	img_iface_smooth	(void *data)
{
	/* Must have 1 channel */
	if (image_copy_tmp->nChannels != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Smooth	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Method: MEAN=1, GAUSS=2, MEDIAN=3");
		data_tmp.method		= user_iface_getint(0, 3, 4, title, NULL);

		snprintf(title, 80, "Mask size: 3, 5, 7, ...");
		data_tmp.msk_siz	= user_iface_getint(3, 3, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Smooth	*data_cast;
	data_cast	= (struct Img_Iface_Data_Smooth *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Smooth mth=%i [%ix%i]",
						data_cast->method,
						data_cast->msk_siz,
						data_cast->msk_siz);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: smooth */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_SMOOTH, data);
}

static	void	img_iface_threshold	(void *data)
{
	/* Must have 1 channel */
	if (image_copy_tmp->nChannels != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Threshold	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Type: BIN=0, BIN_INV=1, TRUNC=2, TOZ=3, TOZ_INV=4");
		data_tmp.thr_typ	= user_iface_getint(0, 0, 4, title, NULL);

		snprintf(title, 80, "Value: 0 to 255 (or -1 for Otsu's algorithm)");
		data_tmp.thr_val	= user_iface_getint(-1, 0, 255, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Threshold	*data_cast;
	data_cast	= (struct Img_Iface_Data_Threshold *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Threshold typ=%i, val=%i",
						data_cast->thr_typ,
						data_cast->thr_val);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: threshold */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_THRESHOLD, data);
}

static	void	img_iface_adaptive_thr	(void *data)
{
	/* Must have 1 channel */
	if (image_copy_tmp->nChannels != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Adaptive_Thr	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Method: MEAN=0, GAUSS=1");
		data_tmp.method		= user_iface_getint(0, 1, 1, title, NULL);

		snprintf(title, 80, "Type: BIN=0, BIN_INV=1");
		data_tmp.thr_typ	= user_iface_getint(0, 0, 1, title, NULL);

		snprintf(title, 80, "Neighbourhood size: 3, 5, 7, ...");
		data_tmp.nbh_val	= user_iface_getint(3, 3, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Adaptive_Thr	*data_cast;
	data_cast	= (struct Img_Iface_Data_Adaptive_Thr *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Threshold mth=%i, typ=%i, nbh=%i",
						data_cast->method,
						data_cast->thr_typ,
						data_cast->nbh_val);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: adaptive threshold */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ADAPTIVE_THRESHOLD, data);
}

static	void	img_iface_dilate	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Dilate_Erode	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Iterations:");
		data_tmp.i	= user_iface_getint(1, 1, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Dilate_Erode *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Dilate i=%i",
						data_cast->i);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Dilate */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_DILATE, data);
}

static	void	img_iface_erode		(void *data)
{
	/* Data */
	struct Img_Iface_Data_Dilate_Erode	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Iterations:");
		data_tmp.i	= user_iface_getint(1, 1, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Dilate_Erode *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Erode i=%i",
						data_cast->i);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Erode */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ERODE, data);
}

static	void	img_iface_contours	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Contours	data_tmp;
	if (!data) {
		data_tmp.storage	= &storage;
		cvClearMemStorage(*(data_tmp.storage));

		data_tmp.contours	= &contours;
		*(data_tmp.contours)	= NULL;

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Contours");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Contours */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_CONTOURS, data);
}

static	void	img_iface_min_area_rect	(void *data)
{
	/* Data */
	struct Img_Iface_Data_MinARect	data_tmp;
	if (!data) {
		data_tmp.contours	= &contours;

		data_tmp.rect		= &rectangle;

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Min area rectangle");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Enclosing rectangle */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_MIN_AREA_RECT, data);
}

static	void	img_iface_rotate_orto	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Rotate_Orto	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Rotate (counterclockwise) n * pi/2 rad;  n:");
		data_tmp.n	= user_iface_getint(1, 1, 3, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Rotate_Orto	*data_cast;
	data_cast	= (struct Img_Iface_Data_Rotate_Orto *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Rotate %i * pi/2 rad",
						data_cast->n);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Rotate ortogonally */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ROTATE_ORTO, data);
}

static	void	img_iface_rotate	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Rotate	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Center:  x:");
		data_tmp.center.x	= user_iface_getdbl(0, 0, INFINITY, title, NULL);

		snprintf(title, 80, "Center:  y:");
		data_tmp.center.y	= user_iface_getdbl(0, 0, INFINITY, title, NULL);

		snprintf(title, 80, "Angle: (deg)");
		data_tmp.angle		= user_iface_getdbl(-INFINITY, 0, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Rotate	*data_cast;
	data_cast	= (struct Img_Iface_Data_Rotate *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Rotate (%f,%f) angle=%lfº",
						data_cast->center.x,
						data_cast->center.y,
						data_cast->angle);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Rotate ortogonally */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ROTATE, data);
}

static	void	img_iface_dilate_erode	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Dilate_Erode	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Iterations:");
		data_tmp.i	= user_iface_getint(1, 1, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Dilate_Erode *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Dilate-erode i=%i",
						data_cast->i);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Dilate */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_DILATE, data);
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ERODE, data);
}

static	void	img_iface_erode_dilate	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Dilate_Erode	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Iterations:");
		data_tmp.i	= user_iface_getint(1, 1, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Dilate_Erode *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Erode-dilate i=%i",
						data_cast->i);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Dilate */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ERODE, data);
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_DILATE, data);
}

/* img_zbar ------------------------------------------------------------------*/
static	void	img_iface_decode	(void *data)
{
	/* Must have 1 channel */
	if (image_copy_tmp->nChannels != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Decode	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Type of code: (0 for all)");
		data_tmp.code_type	= user_iface_getint(0, 0, INT_MAX, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Decode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Decode *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Detect codes c=%i",
						data_cast->code_type);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Decode */
	img_zb_act(&image_copy_tmp, IMG_ZB_ACT_DECODE, data);

	/* Results */
	if (!img_zb_code_n) {
		/* No text found */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! No code detected");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;
	} else {
		/* Write results into log */
		int	i;
		for (i = 0; i < img_zb_code_n; i++) {
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"%s -- '%s'",
							img_zb_code[i].sym_name,
							img_zb_code[i].data);
			user_iface_log.lvl[user_iface_log.len]	= 1;
			(user_iface_log.len)++;
		}
	}
}

/* img_ocr -------------------------------------------------------------------*/
static	void	img_iface_read		(void *data)
{
	/* Data */
	struct Img_Iface_Data_Read	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Language: ENG = 0, SPA = 1, CAT = 2");
		data_tmp.lang	= user_iface_getint(0, 1, 2, title, NULL);

		snprintf(title, 80, "Config: none = 0, Price = 1");
		data_tmp.conf	= user_iface_getint(0, 1, 2, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Read	*data_cast;
	data_cast	= (struct Img_Iface_Data_Read *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"OCR (lang=%i) [c=%i]",
						data_cast->lang,
						data_cast->conf);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* OCR */
	img_ocr_act(&image_copy_tmp, IMG_OCR_ACT_READ, data);

	/* Results */
	if (img_ocr_text[0] == '\0') {
		/* No text found */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! No text detected");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;
	}
}

/* img_orb -------------------------------------------------------------------*/
static	void	img_iface_align		(void)
{
	/* Must have defined a reference */
	if (!image_ref) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Reference is NULL");
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		return;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Align to reference");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Align to reference image_ref */
	img_orb_act(image_ref, &image_copy_tmp, IMG_ORB_ACT_ALIGN);
}

/* img_iface -----------------------------------------------------------------*/
static	void	img_iface_apply		(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Apply changes");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write tmp into old */
	cvReleaseImage(&image_copy_old);
	image_copy_old	= cvCloneImage(image_copy_tmp);
}

static	void	img_iface_discard	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Discard changes");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Discard tmp image copy */
	cvReleaseImage(&image_copy_tmp);
	image_copy_tmp	= cvCloneImage(image_copy_old);
}

static	void	img_iface_save_mem	(void *data)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save to memory");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Apply changes before saving */
	img_iface_apply();

	/* Which memory to use */
	int	x;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "mem_X;  X:");
		x	= user_iface_getint(0, 0, IMG_MEM_SIZE - 1, title, NULL);
	} else {
		x	= *(int *)data;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save to mem_%i", x);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write into mem */
	cvReleaseImage(&image_mem[x]);
	image_mem[x]	= cvCloneImage(image_copy_old);
}

static	void	img_iface_load_mem	(void *data)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Load from memory");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Discard tmp image copy */
	cvReleaseImage(&image_copy_tmp);

	/* Which memory to use */
	int	x;
	if (!data) {
		/* Ask user which memory to use */
		char	title [80];
		snprintf(title, 80, "mem_X;  X:");
		x	= user_iface_getint(0, 0, IMG_MEM_SIZE - 1, title, NULL);
	} else {
		x	= *(int *)data;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Load from mem_%i", x);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Load from mem */
	image_copy_tmp	= cvCloneImage(image_mem[x]);
}

static	void	img_iface_save_ref	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save to reference");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Apply changes before saving */
	img_iface_apply();

	/* Write into ref */
	cvReleaseImage(&image_ref);
	image_ref	= cvCloneImage(image_copy_old);
}

/* save ----------------------------------------------------------------------*/
static	void	img_iface_save_file	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save to file");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Apply changes before saving */
	img_iface_apply();

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save as...");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write into image struct (save.c) */
	cvReleaseImage(&image);
	image	= cvCloneImage(image_copy_old);

	/* Save into file */
	save_image_file();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
