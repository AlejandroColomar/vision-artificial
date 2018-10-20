/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* errno */
<<<<<<< HEAD:modules/image/src/img_iface.cpp
	#include <cerrno>
=======
	#include <errno.h>
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/image/src/img_iface.c
		/* INFINITY */
	#include <cmath>
		/* snprintf() */
	#include <cstdio>

/* Packages ------------------------------------------------------------------*/
		/* openCV */
	#include <opencv2/opencv.hpp>

/* Project -------------------------------------------------------------------*/
		/* load_image_file() */
	#include "save.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
		/* img_cv_act() */
	#include "img_cv.hpp"
		/* img_zb_act() */
	#include "img_zbar.hpp"
		/* OCR */
	#include "img_ocr.hpp"
		/* ORB */
	#include "img_orb.hpp"

	#include "img_iface.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	IMG_MEM_SIZE	(10)
	# define	WIN_NAME	"Image"
	# define	WIN_TIMEOUT	(500)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
	char				img_ocr_text [OCR_TEXT_MAX];
	struct Img_Iface_ZB_Codes	zb_codes;

/* Static --------------------------------------------------------------------*/
static	class cv::Mat					image_copy_old;
static	class cv::Mat					image_copy_tmp;
static	class cv::Mat					image_mem [IMG_MEM_SIZE];
static	class cv::Mat					image_ref;
static	std::vector <std::vector <cv::Point_ <int>>>	contours;
static	class cv::Mat					hierarchy;
static	class cv::RotatedRect				rectangle;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
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
static	void	img_iface_contours_size	(void *data);
static	void	img_iface_min_area_rect	(void *data);
static	void	img_iface_rotate_orto	(void *data);
static	void	img_iface_rotate	(void *data);
static	void	img_iface_set_ROI	(void *data);

static	void	img_iface_dilate_erode	(void *data);
static	void	img_iface_erode_dilate	(void *data);
static	void	img_iface_rotate_2rect	(void *data);
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
void	img_iface_init		(void)
{
	cv::namedWindow(WIN_NAME, cv::WINDOW_NORMAL);
}

void	img_iface_cleanup_main	(void)
{
	int	i;

	for (i = 0; i < IMG_MEM_SIZE; i++) {
		image_mem[i].release();
	}
	image_ref.release();

	cv::destroyAllWindows();
}

void	img_iface_load		(void)
{
	/* Init */
	img_iface_init();

	/* Load file */
	errno	= 0;
	load_image_file();

	if (!errno) {
		/* Make a static copy of image */
<<<<<<< HEAD:modules/image/src/img_iface.cpp
		image.copyTo(image_copy_old);
		image.copyTo(image_copy_tmp);
=======
		cvReleaseImage(&image_copy_old);
		image_copy_old	= cvCloneImage(image);
		cvReleaseImage(&image_copy_tmp);
		image_copy_tmp	= cvCloneImage(image);

		/* Make a copy of the image so that it isn't modified by the user */
		cvReleaseImage(&image_copy_usr);
		image_copy_usr	= cvCloneImage(image_copy_tmp);
		return	image_copy_tmp;
	} else {
		return	NULL;
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/image/src/img_iface.c
	}
}

void	img_iface_cleanup	(void)
{
	image_copy_old.release();
	image_copy_tmp.release();
}

void	img_iface_act		(int action, void *data)
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
	case IMG_IFACE_ACT_CONTOURS_SIZE:
		img_iface_contours_size(data);
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
	case IMG_IFACE_ACT_SET_ROI:
		img_iface_set_ROI(data);
		break;

	case IMG_IFACE_ACT_DILATE_ERODE:
		img_iface_dilate_erode(data);
		break;
	case IMG_IFACE_ACT_ERODE_DILATE:
		img_iface_erode_dilate(data);
		break;
	case IMG_IFACE_ACT_ROTATE_2RECT:
		img_iface_rotate_2rect(data);
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

void	img_iface_show		(void)
{
	cv::imshow(WIN_NAME, image_copy_tmp);
	cv::waitKey(WIN_TIMEOUT);
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
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
	if (image_copy_tmp.channels() != 3) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
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
	if (image_copy_tmp.channels() != 3) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
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
	if (image_copy_tmp.channels() != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Smooth	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Method: MEAN=1, GAUSS=2, MEDIAN=3");
		data_tmp.method		= user_iface_getint(1, 3, 3, title, NULL);

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
	if (image_copy_tmp.channels() != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
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
	if (image_copy_tmp.channels() != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
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
	/* Must have 1 channel */
	if (image_copy_tmp.channels() != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Contours	data_tmp;
	if (!data) {
		data_tmp.contours	= &contours;
		data_tmp.hierarchy	= &hierarchy;

		data	= (void *)&data_tmp;
	}

	/* Contours */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_CONTOURS, data);

	/* Write into log */
	struct Img_Iface_Data_Contours	*data_cast;
	data_cast	= (struct Img_Iface_Data_Contours *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"Contours n=%i",
					(int)data_cast->contours->size());
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;
}

static	void	img_iface_contours_size	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Contours_Size	data_tmp;
	if (!data) {
		data_tmp.contours	= &contours;

		data	= (void *)&data_tmp;
	}

	/* Contours */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_CONTOURS_SIZE, data);

	/* Write into log */
	struct Img_Iface_Data_Contours_Size	*data_cast;
	data_cast	= (struct Img_Iface_Data_Contours_Size *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Contours size:");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;
	int	i;
	for (i = 0; i < data_cast->contours->size(); i++) {
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"cnt[%i]: A=%lf; P=%lf;",
							i,
							data_cast->area[i],
							data_cast->perimeter[i]);
		user_iface_log.lvl[user_iface_log.len]	= 2;
		(user_iface_log.len)++;
	}
}

static	void	img_iface_min_area_rect	(void *data)
{
	/* Data */
	struct Img_Iface_Data_MinARect	data_tmp;
	if (!data) {
		data_tmp.contour	= &(contours[0]);

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
						"Rotate (%.2f,%.2f) %lfº",
						data_cast->center.x,
						data_cast->center.y,
						data_cast->angle);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Rotate ortogonally */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ROTATE, data);
}

static	void	img_iface_set_ROI	(void *data)
{
	/* Data */
	struct Img_Iface_Data_SetROI	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Origin:  x:");
		data_tmp.rect.x		= user_iface_getint(0, 0,
					image_copy_tmp.cols,
					title, NULL);

		snprintf(title, 80, "Origin:  y:");
		data_tmp.rect.y		= user_iface_getint(0, 0,
					image_copy_tmp.rows,
					title, NULL);

		snprintf(title, 80, "Width:");
		data_tmp.rect.width	= user_iface_getint(1, 1,
					image_copy_tmp.cols - data_tmp.rect.x,
					title, NULL);

		snprintf(title, 80, "Height:");
		data_tmp.rect.height	= user_iface_getint(1, 1,
					image_copy_tmp.rows - data_tmp.rect.y,
					title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_SetROI	*data_cast;
	data_cast	= (struct Img_Iface_Data_SetROI *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"ROI: (%i,%i) w=%i,h=%i",
						data_cast->rect.x,
						data_cast->rect.y,
						data_cast->rect.width,
						data_cast->rect.height);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Set ROI */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_SET_ROI, data);
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

static	void	img_iface_rotate_2rect	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Rotate	data_tmp;
	if (!data) {
		data_tmp.center.x	= rectangle.center.x;
		data_tmp.center.y	= rectangle.center.y;
		data_tmp.angle		= rectangle.angle;

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Rotate	*data_cast;
	data_cast	= (struct Img_Iface_Data_Rotate *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Rotate to rectangle");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Rotate ortogonally */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ROTATE, data);
}

/* img_zbar ------------------------------------------------------------------*/
static	void	img_iface_decode	(void *data)
{
	/* Must have 1 channel */
	if (image_copy_tmp.channels() != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Decode	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Type of code: (0 for all)");
		data_tmp.code_type	= (enum zbar::zbar_symbol_type_e)
						user_iface_getint(0, 0, INT_MAX,
								title, NULL);

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
	if (!zb_codes.n) {
		/* No text found */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! No code detected");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;
	} else {
		/* Write results into log */
		int	i;
		for (i = 0; i < zb_codes.n; i++) {
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"%s -- '%s'",
							zb_codes.arr[i].sym_name,
							zb_codes.arr[i].data);
			user_iface_log.lvl[user_iface_log.len]	= 2;
			(user_iface_log.len)++;
		}
	}
}

/* img_ocr -------------------------------------------------------------------*/
static	void	img_iface_read		(void *data)
{
	/* Must have 1 channel */
	if (image_copy_tmp.channels() != 1) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

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

	/* Adapt image data */
	data_cast->img.data		= image_copy_tmp.data;
	data_cast->img.width		= image_copy_tmp.size().width;
	data_cast->img.height		= image_copy_tmp.size().height;
	data_cast->img.B_per_pix	= image_copy_tmp.channels();
	data_cast->img.B_per_line	= image_copy_tmp.step1();

	/* OCR */
	img_ocr_act(IMG_OCR_ACT_READ, data);

	/* Results */
	if (img_ocr_text[0] == '\0') {
		/* No text found */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! No text detected");
		user_iface_log.lvl[user_iface_log.len]	= 2;
		(user_iface_log.len)++;
	}
}

/* img_orb -------------------------------------------------------------------*/
static	void	img_iface_align		(void)
{
	/* Must have defined a reference */
	if (image_ref.empty()) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Reference is NULL");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Align to reference");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Align to reference image_ref */
	img_orb_act(&image_ref, &image_copy_tmp, IMG_ORB_ACT_ALIGN);
}

/* img_iface -----------------------------------------------------------------*/
static	void	img_iface_apply		(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Apply changes");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write tmp into old */
	image_copy_old.release();
	image_copy_tmp.copyTo(image_copy_old);
}

static	void	img_iface_discard	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Discard changes");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Discard tmp image copy */
	image_copy_tmp.release();
	image_copy_old.copyTo(image_copy_tmp);
}

static	void	img_iface_save_mem	(void *data)
{
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
<<<<<<< HEAD:modules/image/src/img_iface.cpp
	image_mem[x].release();
	image_copy_tmp.copyTo(image_mem[x]);
=======
	cvReleaseImage(&image_mem[x]);
	image_mem[x]	= cvCloneImage(image_copy_tmp);
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/image/src/img_iface.c
}

static	void	img_iface_load_mem	(void *data)
{
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

	if (!image_mem[x].empty()) {
		/* Discard tmp image copy */
		image_copy_tmp.release();

		/* Load from mem */
		image_mem[x].copyTo(image_copy_tmp);
	}
}

static	void	img_iface_save_ref	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save to reference");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write into ref */
<<<<<<< HEAD:modules/image/src/img_iface.cpp
	image_ref.release();
	image_copy_tmp.copyTo(image_ref);
	image_ref	= image_copy_tmp;
=======
	cvReleaseImage(&image_ref);
	image_ref	= cvCloneImage(image_copy_tmp);
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/image/src/img_iface.c
}

/* save ----------------------------------------------------------------------*/
static	void	img_iface_save_file	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save as...");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write into image struct (save.c) */
<<<<<<< HEAD:modules/image/src/img_iface.cpp
	image.release();
	image_copy_tmp.copyTo(image);
=======
	cvReleaseImage(&image);
	image	= cvCloneImage(image_copy_tmp);
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/image/src/img_iface.c

	/* Save into file */
	save_image_file();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
