/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* errno */
	#include <cerrno>
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
		/* img_alx_act() */
	#include "img_alx.hpp"
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

	# define	WIN_NAME_IMG	"Image"
	# define	WIN_NAME_HIST	"Hist"
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
static	class cv::Mat					histogram_c0;
static	class cv::Mat					histogram_c1;
static	class cv::Mat					histogram_c2;
static	class cv::Mat					hist_img_c1;
static	class cv::Mat					hist_img_c3;
static	std::vector <std::vector <cv::Point_ <int>>>	contours;
static	double						area [CONTOURS_MAX];
static	double						perimeter [CONTOURS_MAX];
static	class cv::Mat					hierarchy;
static	class cv::Rect_ <int>				rectangle;
static	class cv::RotatedRect				rectangle_rot;
static	std::vector <class cv::Vec <float, 3>>		circles;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* img_alx */
static	void	img_iface_local_max		(void);

	/* img_cv */
		/* Core: The core functionality */
			/* Pixel */
static	void	img_iface_pixel_value		(void *data);
			/* ROI */
static	void	img_iface_set_ROI		(void *data);
static	void	img_iface_set_ROI_2rect		(void *data);
			/* Operations on Arrays */
static	void	img_iface_and_2ref		(void);
static	void	img_iface_not			(void);
static	void	img_iface_or_2ref		(void);
static	void	img_iface_component		(void *data);
		/* Imgproc: Image processing */
			/* Image filtering */
static	void	img_iface_dilate		(void *data);
static	void	img_iface_erode			(void *data);
static	void	img_iface_dilate_erode		(void *data);
static	void	img_iface_erode_dilate		(void *data);
static	void	img_iface_smooth		(void *data);
static	void	img_iface_sobel			(void *data);
			/* Geometric image transformations */
static	void	img_iface_rotate_orto		(void *data);
static	void	img_iface_rotate		(void *data);
static	void	img_iface_rotate_2rect		(void *data);
			/* Miscellaneous image transformations */
static	void	img_iface_adaptive_thr		(void *data);
static	void	img_iface_cvt_color		(void *data);
static	void	img_iface_distance_transform	(void);
static	void	img_iface_threshold		(void *data);
			/* Histograms */
static	void	img_iface_histogram		(void *data);
static	void	img_iface_histogram_c3		(void *data);
			/* Structural analysis and shape descriptors */
static	void	img_iface_contours		(void *data);
static	void	img_iface_contours_size		(void *data);
static	void	img_iface_bounding_rect		(void *data);
static	void	img_iface_fit_ellipse		(void *data);
static	void	img_iface_min_area_rect		(void *data);
			/* Feature detection */
static	void	img_iface_hough_circles		(void *data);

	/* img_zbar */
static	void	img_iface_decode		(void *data);
	/* img_ocr */
static	void	img_iface_read			(void *data);
	/* img_orb */
static	void	img_iface_align			(void);
	/* img_iface */
static	void	img_iface_apply			(void);
static	void	img_iface_discard		(void);
static	void	img_iface_save_mem		(void *data);
static	void	img_iface_load_mem		(void *data);
static	void	img_iface_save_ref		(void);
	/* save */
static	void	img_iface_save_file		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_iface_init		(void)
{
	cv::namedWindow(WIN_NAME_HIST, cv::WINDOW_NORMAL);
	cv::namedWindow(WIN_NAME_IMG, cv::WINDOW_NORMAL);

	hist_img_c1.release();
	hist_img_c1	= cv::Mat::zeros(cv::Size(256, 100), CV_8UC3);
	hist_img_c3.release();
	hist_img_c3	= cv::Mat::zeros(cv::Size(256 * 3, 100), CV_8UC3);
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

void	img_iface_load		(const char *fpath, const char *fname)
{
	char	file_path [FILENAME_MAX];
	char	file_name [FILENAME_MAX];

	/* Init */
	img_iface_init();

	/* Set file_path */
	if (!fpath) {
		snprintf(file_path, FILENAME_MAX, "%s", saved_path);
	} else {
		snprintf(file_path, FILENAME_MAX, "%s", fpath);
	}

	/* Set file_name */
	if (!fname) {
		/* Request file name */
		user_iface_fname(file_path, file_name);
	} else {
		snprintf(file_name, FILENAME_MAX, "%s", fname);
	}

	/* Load file */
	errno	= 0;
	load_image_file(file_path, file_name);

	if (!errno) {
		/* Make a static copy of image */
		image.copyTo(image_copy_old);
		image.copyTo(image_copy_tmp);
	}
}

void	img_iface_cleanup	(void)
{
	image_copy_old.release();
	image_copy_tmp.release();
	histogram_c0.release();
	histogram_c1.release();
	histogram_c2.release();
	hist_img_c1.release();
	hist_img_c3.release();
}

void	img_iface_act		(int action, void *data)
{
	switch (action) {
	/* img_alx */
	case IMG_IFACE_ACT_LOCAL_MAX:
		img_iface_local_max();
		break;

	/* img_cv */
		/* Core: The core functionality */
			/* Pixel */
	case IMG_IFACE_ACT_PIXEL_VALUE:
		img_iface_pixel_value(data);
		break;
			/* ROI */
	case IMG_IFACE_ACT_SET_ROI:
		img_iface_set_ROI(data);
		break;
	case IMG_IFACE_ACT_SET_ROI_2RECT:
		img_iface_set_ROI_2rect(data);
		break;
			/* Operations on Arrays */
	case IMG_IFACE_ACT_AND_2REF:
		img_iface_and_2ref();
		break;
	case IMG_IFACE_ACT_NOT:
		img_iface_not();
		break;
	case IMG_IFACE_ACT_OR_2REF:
		img_iface_or_2ref();
		break;
	case IMG_IFACE_ACT_COMPONENT:
		img_iface_component(data);
		break;
		/* Imgproc: Image processing */
			/* Image filtering */
	case IMG_IFACE_ACT_DILATE:
		img_iface_dilate(data);
		break;
	case IMG_IFACE_ACT_ERODE:
		img_iface_erode(data);
		break;
	case IMG_IFACE_ACT_DILATE_ERODE:
		img_iface_dilate_erode(data);
		break;
	case IMG_IFACE_ACT_ERODE_DILATE:
		img_iface_erode_dilate(data);
		break;
	case IMG_IFACE_ACT_SMOOTH:
		img_iface_smooth(data);
		break;
	case IMG_IFACE_ACT_SOBEL:
		img_iface_sobel(data);
		break;
			/* Geometric image transformations */
	case IMG_IFACE_ACT_ROTATE_ORTO:
		img_iface_rotate_orto(data);
		break;
	case IMG_IFACE_ACT_ROTATE:
		img_iface_rotate(data);
		break;
	case IMG_IFACE_ACT_ROTATE_2RECT:
		img_iface_rotate_2rect(data);
		break;
			/* Miscellaneous image transformations */
	case IMG_IFACE_ACT_ADAPTIVE_THRESHOLD:
		img_iface_adaptive_thr(data);
		break;
	case IMG_IFACE_ACT_CVT_COLOR:
		img_iface_cvt_color(data);
		break;
	case IMG_IFACE_ACT_DISTANCE_TRANSFORM:
		img_iface_distance_transform();
		break;
	case IMG_IFACE_ACT_THRESHOLD:
		img_iface_threshold(data);
		break;
			/* Histograms */
	case IMG_IFACE_ACT_HISTOGRAM:
		img_iface_histogram(data);
		break;
	case IMG_IFACE_ACT_HISTOGRAM_C3:
		img_iface_histogram_c3(data);
		break;
			/* Structural analysis and shape descriptors */
	case IMG_IFACE_ACT_CONTOURS:
		img_iface_contours(data);
		break;
	case IMG_IFACE_ACT_CONTOURS_SIZE:
		img_iface_contours_size(data);
		break;
	case IMG_IFACE_ACT_BOUNDING_RECT:
		img_iface_bounding_rect(data);
		break;
	case IMG_IFACE_ACT_FIT_ELLIPSE:
		img_iface_fit_ellipse(data);
		break;
	case IMG_IFACE_ACT_MIN_AREA_RECT:
		img_iface_min_area_rect(data);
		break;
			/* Feature detection */
	case IMG_IFACE_ACT_HOUGH_CIRCLES:
		img_iface_hough_circles(data);
		break;

	/* img_orb */
	case IMG_IFACE_ACT_ALIGN:
		img_iface_align();
		break;

	/* img_zbar */
	case IMG_IFACE_ACT_DECODE:
		img_iface_decode(data);
		break;

	/* img_ocr */
	case IMG_IFACE_ACT_READ:
		img_iface_read(data);
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

void	img_iface_show_img	(void)
{
	cv::imshow(WIN_NAME_IMG, image_copy_tmp);
	cv::waitKey(WIN_TIMEOUT);
}

void	img_iface_show_hist_c1	(void)
{
	cv::imshow(WIN_NAME_HIST, hist_img_c1);
	cv::waitKey(WIN_TIMEOUT);
}

void	img_iface_show_hist_c3	(void)
{
	cv::imshow(WIN_NAME_HIST, hist_img_c3);
	cv::waitKey(WIN_TIMEOUT);
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/* img_alx --------------------------------------------------------------------*/
static	void	img_iface_local_max		(void)
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

	/* Local maxima */
	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_LOCAL_MAX, NULL);

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Local maxima");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;
}

/* img_cv --------------------------------------------------------------------*/
/* ----- Core: The core functionality */
/* ----- ------- Pixel */
static	void	img_iface_pixel_value		(void *data)
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
	struct Img_Iface_Data_Pixel_Value	data_tmp;
	unsigned char				val;
	if (!data) {
		data_tmp.val		= &val;

		/* Ask user */
		char	title [80];
		snprintf(title, 80, "x:");
		data_tmp.x		= user_iface_getint(0, 0,
					image_copy_tmp.cols,
					title, NULL);

		snprintf(title, 80, "y:");
		data_tmp.y		= user_iface_getint(0, 0,
					image_copy_tmp.rows,
					title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Contours size */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_PIXEL_VALUE, data);

	/* Write into log */
	struct Img_Iface_Data_Pixel_Value	*data_cast;
	data_cast	= (struct Img_Iface_Data_Pixel_Value *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Pixel value:  (%i, %i): %i",
						data_cast->x,
						data_cast->y,
						*(data_cast->val));
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;
}

/* ----- ------- ROI */
static	void	img_iface_set_ROI		(void *data)
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

static	void	img_iface_set_ROI_2rect		(void *data)
{
	/* Data */
	struct Img_Iface_Data_SetROI	data_tmp;
	if (!data) {
		data_tmp.rect.x		= rectangle.x;
		data_tmp.rect.y		= rectangle.y;
		data_tmp.rect.width	= rectangle.width;
		data_tmp.rect.height	= rectangle.height;

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

/* ----- ------- Operations on arrays */
static	void	img_iface_and_2ref		(void)
{
	/* Must have same channels */
	if (image_copy_tmp.channels() != image_ref.channels()) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Bitwise AND");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Bitwise AND to reference */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_AND_2REF, (void *)&image_ref);
}

static	void	img_iface_not			(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Invert color");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Bitwise NOT */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_NOT, NULL);
}

static	void	img_iface_or_2ref		(void)
{
	/* Must have same channels */
	if (image_copy_tmp.channels() != image_ref.channels()) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Bitwise OR");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Bitwise OR to reference */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_OR_2REF, (void *)&image_ref);
}

static	void	img_iface_component		(void *data)
{
	/* Must have at least 3 channels */
	if (image_copy_tmp.channels() < 3) {
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
		snprintf(title, 80, "Component:");
		data_tmp.cmp	= user_iface_getint(0, 0,
						image_copy_tmp.channels() - 1,
						title, NULL);

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

/* ----- Imgproc: Image processing */
/* ----- ------- Image filtering */
static	void	img_iface_dilate		(void *data)
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

static	void	img_iface_erode			(void *data)
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

static	void	img_iface_dilate_erode		(void *data)
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

static	void	img_iface_erode_dilate		(void *data)
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

static	void	img_iface_smooth		(void *data)
{
	/* Data */
	struct Img_Iface_Data_Smooth	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Method: MEAN=1, GAUSS=2, MEDIAN=3");
		data_tmp.method		= user_iface_getint(1, 3, 3, title, NULL);

		snprintf(title, 80, "Kernel size: 3, 5, 7, ...");
		data_tmp.ksize	= user_iface_getint(3, 3, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Smooth	*data_cast;
	data_cast	= (struct Img_Iface_Data_Smooth *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Smooth mth=%i [%ix%i]",
						data_cast->method,
						data_cast->ksize,
						data_cast->ksize);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: smooth */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_SMOOTH, data);
}

static	void	img_iface_sobel			(void *data)
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
	struct Img_Iface_Data_Sobel	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Order of the derivative x");
		data_tmp.dx	= user_iface_getint(0, 1, 10, title, NULL);

		snprintf(title, 80, "Order of the derivative y");
		data_tmp.dy	= user_iface_getint(0, 1, 10, title, NULL);

		snprintf(title, 80, "Size of the extended Sobel kernel (-1 -> Scharr");
		data_tmp.ksize	= user_iface_getint(-1, 3, 7, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Sobel	*data_cast;
	data_cast	= (struct Img_Iface_Data_Sobel *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Sobel dx=%i;dy=%i [ks=%i]",
						data_cast->dx,
						data_cast->dy,
						data_cast->ksize);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: sobel */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_SOBEL, data);
}

/* ----- ------- Geometric image transformations */
static	void	img_iface_rotate_orto		(void *data)
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

static	void	img_iface_rotate		(void *data)
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

static	void	img_iface_rotate_2rect		(void *data)
{
	/* Data */
	struct Img_Iface_Data_Rotate	data_tmp;
	if (!data) {
		data_tmp.center.x	= rectangle_rot.center.x;
		data_tmp.center.y	= rectangle_rot.center.y;
		data_tmp.angle		= rectangle_rot.angle;

		/* If angle is < -45º, it is taking into acount the incorrect side */
		if (data_tmp.angle < -45.0) {
			data_tmp.angle	+= 90.0;
		}

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

/* ----- ------- Miscellaneous image transformations */
static	void	img_iface_adaptive_thr		(void *data)
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

		snprintf(title, 80, "Kernel size: 3, 5, 7, ...");
		data_tmp.ksize	= user_iface_getint(3, 3, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Adaptive_Thr	*data_cast;
	data_cast	= (struct Img_Iface_Data_Adaptive_Thr *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Threshold mth=%i, typ=%i, ks=%i",
						data_cast->method,
						data_cast->thr_typ,
						data_cast->ksize);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: adaptive threshold */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ADAPTIVE_THRESHOLD, data);
}

static	void	img_iface_cvt_color		(void *data)
{
	/* Must have at least 3 channels */
	if (image_copy_tmp.channels() < 3) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Cvt_Color	data_tmp;
	if (!data) {
		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Method: BGR2GRAY = 6, BGR2HSV = 40");
		data_tmp.method	= user_iface_getint(0, 0,
							cv::COLOR_COLORCVT_MAX,
							title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	struct Img_Iface_Data_Cvt_Color	*data_cast;
	data_cast	= (struct Img_Iface_Data_Cvt_Color *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Convert color %i",
						data_cast->method);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Filter: BGR to gray */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_CVT_COLOR, data);
}

static	void	img_iface_distance_transform	(void)
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

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Distance transform");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Distance transform */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_DISTANCE_TRANSFORM, NULL);
}

static	void	img_iface_threshold		(void *data)
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

/* ----- ------- Histograms */
static	void	img_iface_histogram		(void *data)
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
	struct Img_Iface_Data_Histogram	data_tmp;
	if (!data) {
		data_tmp.hist_c0		= &histogram_c0;
		data_tmp.hist_img	= &hist_img_c1;

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Histogram");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Histogram */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_HISTOGRAM, data);
}

static	void	img_iface_histogram_c3		(void *data)
{
	/* Must have 1 channel */
	if (image_copy_tmp.channels() != 3) {
		/* Write into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"! Invalid input");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

		return;
	}

	/* Data */
	struct Img_Iface_Data_Histogram	data_tmp;
	if (!data) {
		data_tmp.hist_c0	= &histogram_c0;
		data_tmp.hist_c1	= &histogram_c1;
		data_tmp.hist_c2	= &histogram_c2;
		data_tmp.hist_img	= &hist_img_c3;

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Histogram (3 channels)");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Histogram */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_HISTOGRAM_C3, data);
}

/* ----- ------- Structural analysis and shape descriptors */
static	void	img_iface_contours		(void *data)
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

static	void	img_iface_contours_size		(void *data)
{
	/* Data */
	struct Img_Iface_Data_Contours_Size	data_tmp;
	if (!data) {
		data_tmp.contours	= &contours;
		data_tmp.area		= area;
		data_tmp.perimeter	= perimeter;

		data	= (void *)&data_tmp;
	}

	/* Contours size */
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

static	void	img_iface_bounding_rect		(void *data)
{
	/* Data */
	struct Img_Iface_Data_Bounding_Rect	data_tmp;
	if (!data) {
		if(!contours.size()) {
			/* Write into log */
			snprintf(user_iface_log.line[user_iface_log.len],
							LOG_LINE_LEN,
							"! Invalid input");
			user_iface_log.lvl[user_iface_log.len]	= 1;
			(user_iface_log.len)++;

			return;
		}
		data_tmp.contour	= &(contours[0]);
		data_tmp.rect		= &rectangle;
		data_tmp.show		= true;

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Bounding rectangle");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Enclosing rectangle */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_BOUNDING_RECT, data);
}

static	void	img_iface_fit_ellipse		(void *data)
{
	/* Data */
	struct Img_Iface_Data_MinARect	data_tmp;
	if (!data) {
		if(!contours.size()) {
			/* Write into log */
			snprintf(user_iface_log.line[user_iface_log.len],
							LOG_LINE_LEN,
							"! Invalid input");
			user_iface_log.lvl[user_iface_log.len]	= 1;
			(user_iface_log.len)++;

			return;
		}
		data_tmp.contour	= &(contours[0]);

		data_tmp.rect		= &rectangle_rot;

		data	= (void *)&data_tmp;
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Fit ellipse");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Enclosing rectangle */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_FIT_ELLIPSE, data);
}

static	void	img_iface_min_area_rect		(void *data)
{
	/* Data */
	struct Img_Iface_Data_MinARect	data_tmp;
	if (!data) {
		if(!contours.size()) {
			/* Write into log */
			snprintf(user_iface_log.line[user_iface_log.len],
							LOG_LINE_LEN,
							"! Invalid input");
			user_iface_log.lvl[user_iface_log.len]	= 1;
			(user_iface_log.len)++;

			return;
		}
		data_tmp.contour	= &(contours[0]);
		data_tmp.rect		= &rectangle_rot;
		data_tmp.show		= true;

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

/* ----- ------- Feature detection */
static	void	img_iface_hough_circles		(void *data)
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
	struct Img_Iface_Data_Hough_Circles	data_tmp;
	if (!data) {
		data_tmp.circles	= &circles;

		/* Ask user */
		char	title [80];
		snprintf(title, 80, "Minimum distance:");
		data_tmp.dist_min	= user_iface_getdbl(0, 5, INFINITY, title, NULL);

		snprintf(title, 80, "param 1:");
		data_tmp.param_1	= user_iface_getdbl(0, 200, INFINITY, title, NULL);

		snprintf(title, 80, "param 2:");
		data_tmp.param_2	= user_iface_getdbl(0, 100, INFINITY, title, NULL);

		snprintf(title, 80, "Minimum radius");
		data_tmp.radius_min	= user_iface_getint(0, 10, INFINITY, title, NULL);

		snprintf(title, 80, "Maximum radius");
		data_tmp.radius_max	= user_iface_getint(0, 0, INFINITY, title, NULL);

		data	= (void *)&data_tmp;
	}

	/* Circles */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_HOUGH_CIRCLES, data);

	/* Write into log */
	struct Img_Iface_Data_Hough_Circles	*data_cast;
	data_cast	= (struct Img_Iface_Data_Hough_Circles *)data;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Circles n=%i",
						(int)data_cast->circles->size());
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;
}

/* img_zbar ------------------------------------------------------------------*/
static	void	img_iface_decode		(void *data)
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
static	void	img_iface_read			(void *data)
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
static	void	img_iface_align			(void)
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
static	void	img_iface_apply			(void)
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

static	void	img_iface_discard		(void)
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

static	void	img_iface_save_mem		(void *data)
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
	image_mem[x].release();
	image_copy_tmp.copyTo(image_mem[x]);
}

static	void	img_iface_load_mem		(void *data)
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

static	void	img_iface_save_ref		(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save to reference");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write into ref */
	image_ref.release();
	image_copy_tmp.copyTo(image_ref);
	image_ref	= image_copy_tmp;
}

/* save ----------------------------------------------------------------------*/
static	void	img_iface_save_file		(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Save as...");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write into image struct (save.c) */
	image.release();
	image_copy_tmp.copyTo(image);

	/* Save into file */
	save_image_file(NULL, NULL);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
