/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/image/iface.hpp"

#include <cerrno>
#include <cmath>
#include <cstddef>
#include <cstdio>

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "vision-artificial/image/alx.hpp"
#include "vision-artificial/image/calib3d.hpp"
#include "vision-artificial/image/cv.hpp"
#include "vision-artificial/image/ocr.hpp"
#include "vision-artificial/image/orb.hpp"
#include "vision-artificial/image/zbar.hpp"
#include "vision-artificial/save/save.hpp"
#include "vision-artificial/user/iface.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define IMG_MEM_SIZE	(20)

#define WIN_NAME_IMG	"Image"
#define WIN_NAME_HIST	"Hist"
#define WIN_TIMEOUT	(500)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
	char				img_ocr_text [OCR_TEXT_MAX];
	struct Img_Iface_ZB_Codes	zb_codes;
/* Static --------------------------------------------------------------------*/
static	class cv::Mat					image_copy_old;
static	class cv::Mat					image_copy_tmp;
static	class cv::Mat					image_mem[IMG_MEM_SIZE];
static	class cv::Mat					image_ref;
static	class cv::Mat					histogram_c0;
static	class cv::Mat					histogram_c1;
static	class cv::Mat					histogram_c2;
static	class cv::Mat					hist_img_c1;
static	class cv::Mat					hist_img_c3;
static	class std::vector <class std::vector <cv::Point_ <int>>>  contours;
static	double						area[CONTOURS_MAX];
static	double						perimeter[CONTOURS_MAX];
static	class cv::Mat					hierarchy;
static	class cv::Rect_ <int>				rectangle;
static	class cv::RotatedRect				rectangle_rot;
static	class std::vector <class cv::Vec <float, 3>>	circles;
static	class cv::Mat					intrinsic_mat;
static	class cv::Mat					dist_coefs;
static	class std::vector <class cv::Mat>		rvecs;
static	class std::vector <class cv::Mat>		tvecs;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
	/* alx */
static	void	img_iface_local_max		(void);
static	void	img_iface_skeleton		(void);
static	void	img_iface_lines_horizontal	(void);
static	void	img_iface_lines_vertical	(void);
static	void	img_iface_mean_horizontal	(void);
static	void	img_iface_mean_vertical		(void);
static	void	img_iface_median_horizontal	(void);
static	void	img_iface_median_vertical	(void);
	/* cv */
		/* Core: The core functionality */
			/* Pixel */
static	void	img_iface_pixel_get		(const void *data);
static	void	img_iface_pixel_set		(const void *data);
			/* ROI */
static	void	img_iface_set_ROI		(const void *data);
static	void	img_iface_set_ROI_2rect		(const void *data);
			/* Operations on Arrays */
static	void	img_iface_and_2ref		(void);
static	void	img_iface_not			(void);
static	void	img_iface_or_2ref		(void);
static	void	img_iface_component		(const void *data);
		/* Imgproc: Image processing */
			/* Image filtering */
static	void	img_iface_dilate		(const void *data);
static	void	img_iface_erode			(const void *data);
static	void	img_iface_dilate_erode		(const void *data);
static	void	img_iface_erode_dilate		(const void *data);
static	void	img_iface_smooth		(const void *data);
static	void	img_iface_sobel			(const void *data);
static	void	img_iface_border		(const void *data);
			/* Geometric image transformations */
static	void	img_iface_mirror		(const void *data);
static	void	img_iface_rotate_orto		(const void *data);
static	void	img_iface_rotate		(const void *data);
static	void	img_iface_rotate_2rect		(const void *data);
			/* Miscellaneous image transformations */
static	void	img_iface_adaptive_thr		(const void *data);
static	void	img_iface_cvt_color		(const void *data);
static	void	img_iface_distance_transform	(void);
static	void	img_iface_threshold		(const void *data);
			/* Histograms */
static	void	img_iface_histogram		(const void *data);
static	void	img_iface_histogram_c3		(const void *data);
			/* Structural analysis and shape descriptors */
static	void	img_iface_contours		(const void *data);
static	void	img_iface_contours_size		(const void *data);
static	void	img_iface_bounding_rect		(const void *data);
static	void	img_iface_fit_ellipse		(const void *data);
static	void	img_iface_min_area_rect		(const void *data);
			/* Feature detection */
static	void	img_iface_hough_circles		(const void *data);
	/* orb */
static	void	img_iface_align			(void);
	/* calib3d */
static	void	img_iface_calibrate		(const void *data);
static	void	img_iface_undistort		(const void *data);
	/* zbar */
static	void	img_iface_decode		(const void *data);
	/* ocr */
static	void	img_iface_read			(const void *data);
	/* iface */
static	void	img_iface_apply			(void);
static	void	img_iface_discard		(void);
static	void	img_iface_save_mem		(const void *data);
static	void	img_iface_load_mem		(const void *data);
static	void	img_iface_save_ref		(void);
	/* save/save */
static	void	img_iface_save_file		(void);
static	void	img_iface_save_update		(void);


/******************************************************************************
 ******* global functions *****************************************************
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

void	img_iface_deinit	(void)
{

	for (ptrdiff_t i = 0; i < IMG_MEM_SIZE; i++)
		image_mem[i].release();
	image_ref.release();

	cv::destroyAllWindows();
}

void	img_iface_load		(const char *fpath, const char *fname)
{
	const char	*path;
	char		file_name[FILENAME_MAX];

	if (!fpath)
		path = saved_path;
	else
		path = fpath;

	if (!fname)
		user_iface_fname(fpath, file_name);
	else
		snprintf(file_name, FILENAME_MAX, "%s", fname);

	errno	= 0;
	load_image_file(path, file_name);
	if (errno)
		return;

	image.copyTo(image_copy_old);
	image.copyTo(image_copy_tmp);
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

void	img_iface_act		(int action, const void *data)
{

	switch (action) {
	/* img_alx */
	case IMG_IFACE_ACT_LOCAL_MAX:
		img_iface_local_max();
		break;
	case IMG_IFACE_ACT_SKELETON:
		img_iface_skeleton();
		break;
	case IMG_IFACE_ACT_LINES_HORIZONTAL:
		img_iface_lines_horizontal();
		break;
	case IMG_IFACE_ACT_LINES_VERTICAL:
		img_iface_lines_vertical();
		break;
	case IMG_IFACE_ACT_MEAN_HORIZONTAL:
		img_iface_mean_horizontal();
		break;
	case IMG_IFACE_ACT_MEAN_VERTICAL:
		img_iface_mean_vertical();
		break;
	case IMG_IFACE_ACT_MEDIAN_HORIZONTAL:
		img_iface_median_horizontal();
		break;
	case IMG_IFACE_ACT_MEDIAN_VERTICAL:
		img_iface_median_vertical();
		break;
	/* img_cv */
		/* Core: The core functionality */
			/* Pixel */
	case IMG_IFACE_ACT_PIXEL_GET:
		img_iface_pixel_get(data);
		break;
	case IMG_IFACE_ACT_PIXEL_SET:
		img_iface_pixel_set(data);
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
	case IMG_IFACE_ACT_BORDER:
		img_iface_border(data);
		break;
			/* Geometric image transformations */
	case IMG_IFACE_ACT_MIRROR:
		img_iface_mirror(data);
		break;
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
	/* img_calib3d */
	case IMG_IFACE_ACT_CALIBRATE:
		img_iface_calibrate(data);
		break;
	case IMG_IFACE_ACT_UNDISTORT:
		img_iface_undistort(data);
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
	case IMG_IFACE_ACT_SAVE_UPDT:
		img_iface_save_update();
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
/* alx -----------------------------------------------------------------------*/
static	void	img_iface_local_max		(void)
{

	if (image_copy_tmp.channels() != 1)
		goto err;

	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_LOCAL_MAX, NULL);

	user_iface_log_write(1, "Local maxima");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_skeleton		(void)
{

	if (image_copy_tmp.channels() != 1)
		goto err;

	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_SKELETON, NULL);

	user_iface_log_write(1, "Skeleton");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_lines_horizontal	(void)
{

	if (image_copy_tmp.channels() != 1)
		goto err;

	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_LINES_HORIZONTAL, NULL);

	user_iface_log_write(1, "Horizontal lines");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_lines_vertical	(void)
{

	if (image_copy_tmp.channels() != 1)
		goto err;

	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_LINES_VERTICAL, NULL);

	user_iface_log_write(1, "Vertical lines");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_mean_horizontal	(void)
{

	if (image_copy_tmp.channels() != 1)
		goto err;

	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_MEAN_HORIZONTAL, NULL);

	user_iface_log_write(1, "Horizontal mean");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_mean_vertical		(void)
{
	if (image_copy_tmp.channels() != 1)
		goto err;

	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_MEAN_VERTICAL, NULL);

	user_iface_log_write(1, "Vertical mean");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_median_horizontal	(void)
{

	if (image_copy_tmp.channels() != 1)
		goto err;

	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_MEDIAN_HORIZONTAL, NULL);

	user_iface_log_write(1, "Horizontal median");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_median_vertical	(void)
{

	if (image_copy_tmp.channels() != 1)
		goto err;

	img_alx_act(&image_copy_tmp, IMG_ALX_ACT_MEDIAN_VERTICAL, NULL);

	user_iface_log_write(1, "Vertical median");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* cv ------------------------------------------------------------------------*/
/* ----- Core: The core functionality */
/* ----- ------- Pixel */
static	void	img_iface_pixel_get		(const void *data)
{
	const	struct Img_Iface_Data_Pixel_Get	*data_cast;
	struct Img_Iface_Data_Pixel_Get		data_tmp;
	unsigned char	val;
	ptrdiff_t	tmp;
	char		txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.val	= &val;
		tmp		= image_copy_tmp.cols;
		data_tmp.x	= user_iface_getint(0, 0, tmp, "x:", NULL);
		tmp		= image_copy_tmp.rows;
		data_tmp.y	= user_iface_getint(0, 0, tmp, "y:", NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_PIXEL_GET, data);

	/* Write into user_iface_log_write */
	data_cast	= (const struct Img_Iface_Data_Pixel_Get *)data;
	snprintf(txt, LOG_LINE_LEN, "Pixel get:  (%ti, %ti): %i",
				data_cast->x, data_cast->y, *data_cast->val);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_pixel_set		(const void *data)
{
	const	struct Img_Iface_Data_Pixel_Set	*data_cast;
	struct Img_Iface_Data_Pixel_Set		data_tmp;
	ptrdiff_t	tmp;
	char		txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		tmp		= image_copy_tmp.cols;
		data_tmp.x	= user_iface_getint(0, 0, tmp, "x:", NULL);
		tmp		= image_copy_tmp.rows;
		data_tmp.y	= user_iface_getint(0, 0, tmp, "y:", NULL);
		data_tmp.val = user_iface_getint(0, 0, UINT8_MAX, "val:", NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_PIXEL_SET, data);

	data_cast	= (const struct Img_Iface_Data_Pixel_Set *)data;
	snprintf(txt, LOG_LINE_LEN, "Pixel get:  (%ti, %ti): %i",
				data_cast->x, data_cast->y, data_cast->val);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* ----- ------- ROI */
static	void	img_iface_set_ROI		(const void *data)
{
	const	struct Img_Iface_Data_SetROI	*data_cast;
	struct Img_Iface_Data_SetROI		data_tmp;
	ptrdiff_t	tmp;
	char		txt[LOG_LINE_LEN];

	if (!data) {
		tmp		= image_copy_tmp.cols;
		data_tmp.rect.x	= user_iface_getint(0, 0, tmp, "Origin:  x:",
									NULL);
		tmp		= image_copy_tmp.rows;
		data_tmp.rect.y	= user_iface_getint(0, 0, tmp, "Origin:  y:",
									NULL);
		tmp		= image_copy_tmp.cols - data_tmp.rect.x;
		data_tmp.rect.width = user_iface_getint(1, tmp, tmp, "Width:",
									NULL);
		tmp		= image_copy_tmp.rows - data_tmp.rect.y;
		data_tmp.rect.height = user_iface_getint(1, tmp, tmp,"Height:",
									NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_SET_ROI, data);

	data_cast	= (const struct Img_Iface_Data_SetROI *)data;
	snprintf(txt, LOG_LINE_LEN, "ROI: (%i,%i) w=%i,h=%i",
				data_cast->rect.x, data_cast->rect.y,
				data_cast->rect.width, data_cast->rect.height);
	user_iface_log_write(1, txt);
}

static	void	img_iface_set_ROI_2rect		(const void *data)
{
	const	struct Img_Iface_Data_SetROI	*data_cast;
	struct Img_Iface_Data_SetROI		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.rect.x		= rectangle.x;
		data_tmp.rect.y		= rectangle.y;
		data_tmp.rect.width	= rectangle.width;
		data_tmp.rect.height	= rectangle.height;
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_SET_ROI, data);

	data_cast	= (const struct Img_Iface_Data_SetROI *)data;
	snprintf(txt, LOG_LINE_LEN, "ROI: (%i,%i) w=%i,h=%i",
				data_cast->rect.x, data_cast->rect.y,
				data_cast->rect.width, data_cast->rect.height);
	user_iface_log_write(1, txt);
}

/* ----- ------- Operations on arrays */
static	void	img_iface_and_2ref		(void)
{

	if (image_copy_tmp.channels() != image_ref.channels())
		goto err;

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_AND_2REF,
						(const void *)&image_ref);

	user_iface_log_write(1, "Bitwise AND");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Channels do not match ref)");
}

static	void	img_iface_not			(void)
{

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_NOT, NULL);

	user_iface_log_write(1, "Invert color");
}

static	void	img_iface_or_2ref		(void)
{

	if (image_copy_tmp.channels() != image_ref.channels())
		goto err;

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_OR_2REF,
						(const void *)&image_ref);

	user_iface_log_write(1, "Bitwise OR");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Channels do not match ref)");
}

static	void	img_iface_component		(const void *data)
{
	const	struct Img_Iface_Data_Component	*data_cast;
	struct Img_Iface_Data_Component		data_tmp;
	ptrdiff_t	tmp;
	char		txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() < 2)
		goto err;

	if (!data) {
		tmp	= image_copy_tmp.channels() - 1;
		data_tmp.cmp = user_iface_getint(0, 0, tmp, "Component:", NULL);
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_COMPONENT, data);

	data_cast	= (const struct Img_Iface_Data_Component *)data;
	snprintf(txt, LOG_LINE_LEN, "Component %ti", data_cast->cmp);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be >= 2 channels)");
}

/* ----- Imgproc: Image processing */
/* ----- ------- Image filtering */
static	void	img_iface_dilate		(const void *data)
{
	const	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	struct Img_Iface_Data_Dilate_Erode		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.i = user_iface_getint(1, 1, INFINITY, "Iterations:",
									NULL);
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_DILATE, data);

	data_cast	= (const struct Img_Iface_Data_Dilate_Erode *)data;
	snprintf(txt, LOG_LINE_LEN, "Dilate i = %i", data_cast->i);
	user_iface_log_write(1, txt);
}

static	void	img_iface_erode			(const void *data)
{
	const	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	struct Img_Iface_Data_Dilate_Erode		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.i = user_iface_getint(1, 1, INFINITY, "Iterations:",
									NULL);
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ERODE, data);

	data_cast	= (const struct Img_Iface_Data_Dilate_Erode *)data;
	snprintf(txt, LOG_LINE_LEN, "Erode i = %i", data_cast->i);
	user_iface_log_write(1, txt);
}

static	void	img_iface_dilate_erode		(const void *data)
{
	const	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	struct Img_Iface_Data_Dilate_Erode		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.i = user_iface_getint(1, 1, INFINITY, "Iterations:",
									NULL);
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_DILATE, data);
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ERODE, data);

	data_cast	= (const struct Img_Iface_Data_Dilate_Erode *)data;
	snprintf(txt, LOG_LINE_LEN, "Dilate-erode i = %i", data_cast->i);
	user_iface_log_write(1, txt);
}

static	void	img_iface_erode_dilate		(const void *data)
{
	const	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	struct Img_Iface_Data_Dilate_Erode		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.i = user_iface_getint(1, 1, INFINITY, "Iterations:",
									NULL);
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ERODE, data);
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_DILATE, data);

	data_cast	= (const struct Img_Iface_Data_Dilate_Erode *)data;
	snprintf(txt, LOG_LINE_LEN, "Erode-dilate i = %i", data_cast->i);
	user_iface_log_write(1, txt);
}

static	void	img_iface_smooth		(const void *data)
{
	const	struct Img_Iface_Data_Smooth	*data_cast;
	struct Img_Iface_Data_Smooth		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.method	= user_iface_getint(1, 3, 3,
				"Method: MEAN=1, GAUSS=2, MEDIAN=3", NULL);
		data_tmp.ksize	= user_iface_getint(3, 3, INFINITY,
				"Kernel size: 3, 5, 7, ...", NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_SMOOTH, data);

	data_cast	= (const struct Img_Iface_Data_Smooth *)data;
	snprintf(txt, LOG_LINE_LEN, "Smooth mth = %i [%i x %i]",
			data_cast->method, data_cast->ksize, data_cast->ksize);
	user_iface_log_write(1, txt);
}

static	void	img_iface_sobel			(const void *data)
{
	const	struct Img_Iface_Data_Sobel	*data_cast;
	struct Img_Iface_Data_Sobel		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.dx	= user_iface_getint(0, 1, 10,
				"Order of the derivative x", NULL);
		data_tmp.dy	= user_iface_getint(0, 1, 10,
				"Order of the derivative y", NULL);
		data_tmp.ksize	= user_iface_getint(-1, 3, 7,
				"Size of the extended Sobel kernel (Scharr: -1)",
									NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_SOBEL, data);

	data_cast	= (const struct Img_Iface_Data_Sobel *)data;
	snprintf(txt, LOG_LINE_LEN, "Sobel dx = %i; dy = %i [ks = %i]",
				data_cast->dx, data_cast->dy, data_cast->ksize);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_border		(const void *data)
{
	const	struct Img_Iface_Data_Border	*data_cast;
	struct Img_Iface_Data_Border		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.size = user_iface_getint(1, 1, INT16_MAX, "Size",NULL);
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_BORDER, data);

	data_cast	= (const struct Img_Iface_Data_Border *)data;
	snprintf(txt, LOG_LINE_LEN, "Border size = %ti", data_cast->size);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* ----- ------- Geometric image transformations */
static	void	img_iface_mirror		(const void *data)
{
	const	struct Img_Iface_Data_Mirror	*data_cast;
	struct Img_Iface_Data_Mirror		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.axis = user_iface_getint(0, 1, 1, "Axis:  0=x;  1=y",
									NULL);
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_MIRROR, data);

	data_cast	= (const struct Img_Iface_Data_Mirror *)data;
	snprintf(txt, LOG_LINE_LEN, "Mirror axis: %i", data_cast->axis);
	user_iface_log_write(1, txt);
}

static	void	img_iface_rotate_orto		(const void *data)
{
	const	struct Img_Iface_Data_Rotate_Orto	*data_cast;
	struct Img_Iface_Data_Rotate_Orto		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.n	= user_iface_getint(1, 1, 3,
				"Rotate (counterclockwise) n * pi/2 rad;  n:",
									NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ROTATE_ORTO, data);

	data_cast	= (const struct Img_Iface_Data_Rotate_Orto *)data;
	snprintf(txt, LOG_LINE_LEN, "Rotate %i * pi/2 rad", data_cast->n);
	user_iface_log_write(1, txt);
}

static	void	img_iface_rotate		(const void *data)
{
	const	struct Img_Iface_Data_Rotate	*data_cast;
	struct Img_Iface_Data_Rotate		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.center.x = user_iface_getdbl(0, 0, INFINITY,
							"Center:  x:", NULL);
		data_tmp.center.y = user_iface_getdbl(0, 0, INFINITY,
							"Center:  y:", NULL);
		data_tmp.angle	= user_iface_getdbl(-INFINITY, 0, INFINITY,
							"Angle: (deg)", NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ROTATE, data);

	data_cast	= (const struct Img_Iface_Data_Rotate *)data;
	snprintf(txt, LOG_LINE_LEN, "Rotate (%.2f,%.2f) %lfº",
				data_cast->center.x, data_cast->center.y,
				data_cast->angle);
	user_iface_log_write(1, txt);
}

static	void	img_iface_rotate_2rect		(const void *data)
{
	struct Img_Iface_Data_Rotate	data_tmp;

	if (!data) {
		data_tmp.center.x	= rectangle_rot.center.x;
		data_tmp.center.y	= rectangle_rot.center.y;
		data_tmp.angle		= rectangle_rot.angle;
		/* If angle is < -45º,
		 * it is taking into acount the incorrect side */
		if (data_tmp.angle < -45.0)
			data_tmp.angle += 90.0;
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ROTATE, data);

	user_iface_log_write(1, "Rotate to rectangle");
}

/* ----- ------- Miscellaneous image transformations */
static	void	img_iface_adaptive_thr		(const void *data)
{
	const	struct Img_Iface_Data_Adaptive_Thr	*data_cast;
	struct Img_Iface_Data_Adaptive_Thr		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.method	= user_iface_getint(0, 1, 1,
					"Method: MEAN=0, GAUSS=1", NULL);
		data_tmp.thr_typ = user_iface_getint(0, 0, 1,
					"Type: BIN=0, BIN_INV=1", NULL);
		data_tmp.ksize	= user_iface_getint(3, 3, INFINITY,
					"Kernel size: 3, 5, 7, ...", NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ADAPTIVE_THRESHOLD, data);

	data_cast	= (const struct Img_Iface_Data_Adaptive_Thr *)data;
	snprintf(txt, LOG_LINE_LEN, "Threshold mth=%i, typ=%i, ks=%i",
					data_cast->method, data_cast->thr_typ,
					data_cast->ksize);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_cvt_color		(const void *data)
{
	const	struct Img_Iface_Data_Cvt_Color	*data_cast;
	struct Img_Iface_Data_Cvt_Color		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 3)
		goto err;

	if (!data) {
		data_tmp.method	=user_iface_getint(0, 0, cv::COLOR_COLORCVT_MAX,
				"Method: BGR2GRAY = 6, BGR2HSV = 40", NULL);
		data		= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_CVT_COLOR, data);

	data_cast	= (const struct Img_Iface_Data_Cvt_Color *)data;
	snprintf(txt, LOG_LINE_LEN, "Convert color %i", data_cast->method);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 3 channels)");
}

static	void	img_iface_distance_transform	(void)
{

	if (image_copy_tmp.channels() != 1)
		goto err;

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_DISTANCE_TRANSFORM, NULL);

	user_iface_log_write(1, "Distance transform");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_threshold		(const void *data)
{
	const	struct Img_Iface_Data_Threshold	*data_cast;
	struct Img_Iface_Data_Threshold		data_tmp;
	char	txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.thr_typ = user_iface_getint(0, 0, 4,
				"Type: BIN=0, BIN_INV=1, TRUNC=2, TOZ=3, TOZ_INV=4",
									NULL);
		data_tmp.thr_val = user_iface_getint(-1, 0, UINT8_MAX,
				"Value: 0 to 255 (or -1 for Otsu's algorithm)",
									NULL);
		data	= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_THRESHOLD, data);

	data_cast	= (const struct Img_Iface_Data_Threshold *)data;
	snprintf(txt, LOG_LINE_LEN, "Threshold typ = %i, val = %i",
					data_cast->thr_typ, data_cast->thr_val);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* ----- ------- Histograms */
static	void	img_iface_histogram		(const void *data)
{
	struct Img_Iface_Data_Histogram	data_tmp;

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.hist_c0	= &histogram_c0;
		data_tmp.hist_img	= &hist_img_c1;
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_HISTOGRAM, data);

	user_iface_log_write(1, "Histogram");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_histogram_c3		(const void *data)
{
	struct Img_Iface_Data_Histogram	data_tmp;

	if (image_copy_tmp.channels() != 3)
		goto err;

	if (!data) {
		data_tmp.hist_c0	= &histogram_c0;
		data_tmp.hist_c1	= &histogram_c1;
		data_tmp.hist_c2	= &histogram_c2;
		data_tmp.hist_img	= &hist_img_c3;
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_HISTOGRAM_C3, data);

	user_iface_log_write(1, "Histogram (3 channels)");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 3 channels)");
}

/* ----- ------- Structural analysis and shape descriptors */
static	void	img_iface_contours		(const void *data)
{
	const	struct Img_Iface_Data_Contours	*data_cast;
	struct Img_Iface_Data_Contours		data_tmp;
	ptrdiff_t	tmp;
	char		txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.contours	= &contours;
		data_tmp.hierarchy	= &hierarchy;
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_CONTOURS, data);

	data_cast	= (const struct Img_Iface_Data_Contours *)data;
	tmp		= data_cast->contours->size();
	snprintf(txt, LOG_LINE_LEN, "Contours n = %ti", tmp);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_contours_size		(const void *data)
{
	const	struct Img_Iface_Data_Contours_Size	*data_cast;
	struct Img_Iface_Data_Contours_Size		data_tmp;
	ptrdiff_t	tmp;
	char		txt[LOG_LINE_LEN];

	if (!data) {
		data_tmp.contours	= &contours;
		data_tmp.area		= area;
		data_tmp.perimeter	= perimeter;
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_CONTOURS_SIZE, data);

	data_cast	= (const struct Img_Iface_Data_Contours_Size *)data;
	user_iface_log_write(1, "Contours size:");
	tmp		= data_cast->contours->size();
	for (ptrdiff_t i = 0; i < tmp; i++) {
		snprintf(txt, LOG_LINE_LEN, "cnt[%ti]: A = %lf; P = %lf;",
				i, data_cast->area[i], data_cast->perimeter[i]);
		user_iface_log_write(2, txt);
	}
}

static	void	img_iface_bounding_rect		(const void *data)
{
	struct Img_Iface_Data_Bounding_Rect	data_tmp;

	if (!data) {
		if(!contours.size())
			goto err;
		data_tmp.contour	= &(contours[0]);
		data_tmp.rect		= &rectangle;
		data_tmp.show		= true;
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_BOUNDING_RECT, data);

	user_iface_log_write(1, "Bounding rectangle");
	return;
err:
	user_iface_log_write(1, "! Invalid input (No contours saved)");
}

static	void	img_iface_fit_ellipse		(const void *data)
{
	struct Img_Iface_Data_MinARect	data_tmp;

	if (!data) {
		if(!contours.size())
			goto err;
		data_tmp.contour	= &(contours[0]);
		data_tmp.rect		= &rectangle_rot;
		data_tmp.show		= true;
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_FIT_ELLIPSE, data);

	user_iface_log_write(1, "Fit ellipse");
	return;
err:
	user_iface_log_write(1, "! Invalid input (No contours saved)");
}

static	void	img_iface_min_area_rect		(const void *data)
{
	struct Img_Iface_Data_MinARect	data_tmp;

	if (!data) {
		if(!contours.size())
			goto err;
		data_tmp.contour	= &(contours[0]);
		data_tmp.rect		= &rectangle_rot;
		data_tmp.show		= true;
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_MIN_AREA_RECT, data);

	user_iface_log_write(1, "Min area rectangle");
	return;
err:
	user_iface_log_write(1, "! Invalid input (No contours saved)");
}

/* ----- ------- Feature detection */
static	void	img_iface_hough_circles		(const void *data)
{
	const	struct Img_Iface_Data_Hough_Circles	*data_cast;
	struct Img_Iface_Data_Hough_Circles		data_tmp;
	ptrdiff_t	tmp;
	char		txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.circles	= &circles;
		data_tmp.dist_min	= user_iface_getdbl(0, 5, INFINITY,
						"Minimum distance:", NULL);
		data_tmp.param_1	= user_iface_getdbl(0, 200, INFINITY,
						"param 1:", NULL);
		data_tmp.param_2	= user_iface_getdbl(0, 100, INFINITY,
						"param 2:", NULL);
		data_tmp.radius_min	= user_iface_getint(0, 10, INFINITY,
						"Minimum radius", NULL);
		data_tmp.radius_max	= user_iface_getint(0, 0, INFINITY,
						"Maximum radius", NULL);
		data			= (const void *)&data_tmp;
	}

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_HOUGH_CIRCLES, data);

	data_cast	= (const struct Img_Iface_Data_Hough_Circles *)data;
	tmp		= data_cast->circles->size();
	snprintf(txt, LOG_LINE_LEN, "Circles n = %ti", tmp);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* orb -----------------------------------------------------------------------*/
static	void	img_iface_align			(void)
{

	if (image_ref.empty())
		goto err;

	img_orb_act(&image_ref, &image_copy_tmp, IMG_ORB_ACT_ALIGN);

	user_iface_log_write(1, "Align to reference");
	return;
err:
	user_iface_log_write(1, "! Reference is NULL");
}

/* calib3d -------------------------------------------------------------------*/
static	void	img_iface_calibrate		(const void *data)
{
	struct Img_Iface_Data_Calibrate	data_tmp;

	if (image_copy_tmp.channels() != 1)
		goto err;

	if (!data) {
		data_tmp.intrinsic_mat	= &intrinsic_mat;
		data_tmp.dist_coefs	= &dist_coefs;
		data_tmp.rvecs		= &rvecs;
		data_tmp.tvecs		= &tvecs;
		data			= (const void *)&data_tmp;
	}

	img_calib3d_act(&image_copy_tmp, IMG_CALIB3D_ACT_CALIBRATE, data);

	user_iface_log_write(1, "Calibrate");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_undistort		(const void *data)
{
	struct Img_Iface_Data_Undistort	data_tmp;

	if (!data) {
		data_tmp.intrinsic_mat	= &intrinsic_mat;
		data_tmp.dist_coefs	= &dist_coefs;
		data			= (const void *)&data_tmp;
	}

	img_calib3d_act(&image_copy_tmp, IMG_CALIB3D_ACT_UNDISTORT, data);

	user_iface_log_write(1, "Undistort");
}

/* zbar ----------------------------------------------------------------------*/
static	void	img_iface_decode		(const void *data)
{
	struct Img_Iface_Data_Decode	data_tmp;
	const	struct Img_Iface_Data_Decode	*data_cast;
	char	txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err_chan;

	if (!data) {
		data_tmp.code_type	= user_iface_getint(0, 0, INT_MAX,
					"Type of code: (0 for all)", NULL);
		data			= (const void *)&data_tmp;
	}

	img_zb_act(&image_copy_tmp, IMG_ZB_ACT_DECODE, data);

	data_cast	= (const struct Img_Iface_Data_Decode *)data;
	snprintf(txt, LOG_LINE_LEN, "Detect codes c = %i",data_cast->code_type);
	user_iface_log_write(1, txt);

	if (!zb_codes.n)
		goto err_zb;
	for (ptrdiff_t i = 0; i < zb_codes.n; i++) {
		snprintf(txt, LOG_LINE_LEN, "%s -- '%s'",
				zb_codes.arr[i].sym_name, zb_codes.arr[i].data);
		user_iface_log_write(2, txt);
	}
	return;
err_zb:
	user_iface_log_write(2, "! No code detected");
	return;
err_chan:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* ocr -----------------------------------------------------------------------*/
static	void	img_iface_read			(const void *data)
{
	struct Img_Iface_Data_Read		data_tmp;
	const struct Img_Iface_Data_Read	*data_cast;
	char	txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err_chan;

	data_cast	= (const struct Img_Iface_Data_Read *)data;

	if (!data) {
		data_tmp.lang	= user_iface_getint(0, 1, 2,
				"Language: ENG = 0, SPA = 1, CAT = 2", NULL);
		data_tmp.conf	= user_iface_getint(0, 1, 2,
				"Config: none = 0, Price = 1", NULL);
	} else {
		data_tmp.lang	= data_cast->lang;
		data_tmp.conf	= data_cast->conf;
	}
	/* Adapt image data */
	data_tmp.img.data	= image_copy_tmp.data;
	data_tmp.img.width	= image_copy_tmp.size().width;
	data_tmp.img.height	= image_copy_tmp.size().height;
	data_tmp.img.B_per_pix	= image_copy_tmp.channels();
	data_tmp.img.B_per_line	= image_copy_tmp.step1();
	data	= (const void *)&data_tmp;

	img_ocr_act(IMG_OCR_ACT_READ, data);

	snprintf(txt, LOG_LINE_LEN, "OCR (lang = %i) [c = %i]",
					data_cast->lang, data_cast->conf);
	user_iface_log_write(1, txt);
	if (img_ocr_text[0] == '\0')
		goto err_ocr;
	return;
err_ocr:
	user_iface_log_write(2, "! No text detected");
	return;
err_chan:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* iface ---------------------------------------------------------------------*/
static	void	img_iface_apply			(void)
{

	image_copy_old.release();
	image_copy_tmp.copyTo(image_copy_old);

	user_iface_log_write(1, "Apply changes");
}

static	void	img_iface_discard		(void)
{

	image_copy_tmp.release();
	image_copy_old.copyTo(image_copy_tmp);

	user_iface_log_write(1, "Discard changes");
}

static	void	img_iface_save_mem		(const void *data)
{
	ptrdiff_t	m;
	char		txt[LOG_LINE_LEN];

	if (!data)
		m = user_iface_getint(0, 0, IMG_MEM_SIZE -1,"mem[m];  m:",NULL);
	else
		m = *(const ptrdiff_t *)data;

	image_mem[m].release();
	image_copy_tmp.copyTo(image_mem[m]);

	snprintf(txt, LOG_LINE_LEN, "Save to mem[%ti]", m);
	user_iface_log_write(1, txt);
}

static	void	img_iface_load_mem		(const void *data)
{
	ptrdiff_t	m;
	char		txt[LOG_LINE_LEN];

	if (!data)
		m = user_iface_getint(0, 0, IMG_MEM_SIZE -1,"mem[m];  m:",NULL);
	else
		m = *(const ptrdiff_t *)data;

	if (image_mem[m].empty())
		goto err;

	image_copy_tmp.release();
	image_mem[m].copyTo(image_copy_tmp);

	snprintf(txt, LOG_LINE_LEN, "Load from mem[%ti]", m);
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Empty memory");
}

static	void	img_iface_save_ref		(void)
{

	image_ref.release();
	image_copy_tmp.copyTo(image_ref);

	user_iface_log_write(1, "Save to reference");
}

/* save/save -----------------------------------------------------------------*/
static	void	img_iface_save_file		(void)
{

	user_iface_log_write(1, "Save as...");

	image.release();
	image_copy_tmp.copyTo(image);
	save_image_file(NULL, NULL);
}

static	void	img_iface_save_update		(void)
{

	image.release();
	image_copy_tmp.copyTo(image);

	user_iface_log_write(1, "Save: update img");
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
