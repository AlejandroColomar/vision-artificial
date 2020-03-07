/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/image/iface.hpp"

#include <cerrno>
#include <cinttypes>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc.hpp>

#define ALX_NO_PREFIX
#include <libalx/base/compiler/unused.hpp>
#include <libalx/base/stdio/printf/sbprintf.hpp>
#include <libalx/extra/cv/alx/fill.hpp>
#include <libalx/extra/cv/alx/gray.hpp>
#include <libalx/extra/cv/alx/lines.hpp>
#include <libalx/extra/cv/alx/max.hpp>
#include <libalx/extra/cv/alx/mean.hpp>
#include <libalx/extra/cv/alx/median.hpp>
#include <libalx/extra/cv/alx/skeleton.hpp>
#include <libalx/extra/cv/core/array.hpp>
#include <libalx/extra/cv/core/contours.hpp>
#include <libalx/extra/cv/core/img.hpp>
#include <libalx/extra/cv/core/pixel.hpp>
#include <libalx/extra/cv/core/rect.hpp>
#include <libalx/extra/cv/core/roi.hpp>
#include <libalx/extra/cv/features2d/orb.hpp>
#include <libalx/extra/cv/imgproc/features.hpp>
#include <libalx/extra/cv/imgproc/filter.hpp>
#include <libalx/extra/cv/imgproc/geometric.hpp>
#include <libalx/extra/cv/imgproc/histogram.hpp>
#include <libalx/extra/cv/imgproc/miscellaneous.hpp>
#include <libalx/extra/cv/imgproc/shape.hpp>
#include <libalx/extra/cv/ximgproc/thinning.hpp>
#include <libalx/extra/ocr/ocr.hpp>
#include <libalx/extra/zbar/zbar.hpp>

#include "vision-artificial/image/calib3d.hpp"
#include "vision-artificial/image/cv.hpp"
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
static	void	img_iface_bkgd_mask		(void);
static	void	img_iface_bkgd_fill		(void);
static	void	img_iface_holes_mask		(void);
static	void	img_iface_holes_fill		(void);
static	void	img_iface_white_mask		(void);
static	void	img_iface_black_mask		(void);
static	void	img_iface_gray_mask		(void);
	/* cv */
		/* Core: The core functionality */
			/* Pixel */
static	void	img_iface_pixel_get		(void);
static	void	img_iface_pixel_set		(void);
			/* ROI */
static	void	img_iface_set_ROI		(void);
static	void	img_iface_set_ROI_2rect		(void);
			/* Operations on Arrays */
static	void	img_iface_and_2ref		(void);
static	void	img_iface_not			(void);
static	void	img_iface_or_2ref		(void);
static	void	img_iface_component		(void);
		/* Imgproc: Image processing */
			/* Image filtering */
static	void	img_iface_dilate		(void);
static	void	img_iface_erode			(void);
static	void	img_iface_dilate_erode		(void);
static	void	img_iface_erode_dilate		(void);
static	void	img_iface_smooth		(void);
static	void	img_iface_sobel			(void);
static	void	img_iface_border		(void);
			/* Geometric image transformations */
static	void	img_iface_mirror		(void);
static	void	img_iface_rotate_orto		(void);
static	void	img_iface_rotate		(void);
static	void	img_iface_rotate_2rect		(void);
			/* Miscellaneous image transformations */
static	void	img_iface_adaptive_thr		(void);
static	void	img_iface_cvt_color		(void);
static	void	img_iface_distance_transform	(void);
static	void	img_iface_threshold		(void);
			/* Histograms */
static	void	img_iface_histogram1D		(void);
static	void	img_iface_histogram3D		(void);
			/* Structural analysis and shape descriptors */
static	void	img_iface_contours		(void);
static	void	img_iface_contours_dimensions	(void);
static	void	img_iface_bounding_rect		(void);
static	void	img_iface_fit_ellipse		(void);
static	void	img_iface_min_area_rect		(void);
			/* Feature detection */
static	void	img_iface_hough_circles		(void);
	/* orb */
static	void	img_iface_align			(void);
	/* calib3d */
static	void	img_iface_calibrate		(void);
static	void	img_iface_undistort		(void);
	/* zbar */
static	void	img_iface_decode		(void);
	/* ocr */
static	void	img_iface_read			(void);
	/* iface */
static	void	img_iface_apply			(void);
static	void	img_iface_discard		(void);
static	void	img_iface_save_mem		(void);
static	void	img_iface_load_mem		(void);
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
		ALX_UNUSED(sbprintf(file_name, NULL, "%s", fname));

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

void	img_iface_act		(int action)
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
	case IMG_IFACE_ACT_BKGD_MASK:
		img_iface_bkgd_mask();
		break;
	case IMG_IFACE_ACT_BKGD_FILL:
		img_iface_bkgd_fill();
		break;
	case IMG_IFACE_ACT_HOLES_MASK:
		img_iface_holes_mask();
		break;
	case IMG_IFACE_ACT_HOLES_FILL:
		img_iface_holes_fill();
		break;
	case IMG_IFACE_ACT_WHITE_MASK:
		img_iface_white_mask();
		break;
	case IMG_IFACE_ACT_BLACK_MASK:
		img_iface_black_mask();
		break;
	case IMG_IFACE_ACT_GRAY_MASK:
		img_iface_gray_mask();
		break;
	/* img_cv */
		/* Core: The core functionality */
			/* Pixel */
	case IMG_IFACE_ACT_PIXEL_GET:
		img_iface_pixel_get();
		break;
	case IMG_IFACE_ACT_PIXEL_SET:
		img_iface_pixel_set();
		break;
			/* ROI */
	case IMG_IFACE_ACT_SET_ROI:
		img_iface_set_ROI();
		break;
	case IMG_IFACE_ACT_SET_ROI_2RECT:
		img_iface_set_ROI_2rect();
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
		img_iface_component();
		break;
		/* Imgproc: Image processing */
			/* Image filtering */
	case IMG_IFACE_ACT_DILATE:
		img_iface_dilate();
		break;
	case IMG_IFACE_ACT_ERODE:
		img_iface_erode();
		break;
	case IMG_IFACE_ACT_DILATE_ERODE:
		img_iface_dilate_erode();
		break;
	case IMG_IFACE_ACT_ERODE_DILATE:
		img_iface_erode_dilate();
		break;
	case IMG_IFACE_ACT_SMOOTH:
		img_iface_smooth();
		break;
	case IMG_IFACE_ACT_SOBEL:
		img_iface_sobel();
		break;
	case IMG_IFACE_ACT_BORDER:
		img_iface_border();
		break;
			/* Geometric image transformations */
	case IMG_IFACE_ACT_MIRROR:
		img_iface_mirror();
		break;
	case IMG_IFACE_ACT_ROTATE_ORTO:
		img_iface_rotate_orto();
		break;
	case IMG_IFACE_ACT_ROTATE:
		img_iface_rotate();
		break;
	case IMG_IFACE_ACT_ROTATE_2RECT:
		img_iface_rotate_2rect();
		break;
			/* Miscellaneous image transformations */
	case IMG_IFACE_ACT_ADAPTIVE_THRESHOLD:
		img_iface_adaptive_thr();
		break;
	case IMG_IFACE_ACT_CVT_COLOR:
		img_iface_cvt_color();
		break;
	case IMG_IFACE_ACT_DISTANCE_TRANSFORM:
		img_iface_distance_transform();
		break;
	case IMG_IFACE_ACT_THRESHOLD:
		img_iface_threshold();
		break;
			/* Histograms */
	case IMG_IFACE_ACT_HISTOGRAM:
		img_iface_histogram1D();
		break;
	case IMG_IFACE_ACT_HISTOGRAM_C3:
		img_iface_histogram3D();
		break;
			/* Structural analysis and shape descriptors */
	case IMG_IFACE_ACT_CONTOURS:
		img_iface_contours();
		break;
	case IMG_IFACE_ACT_CONTOURS_SIZE:
		img_iface_contours_dimensions();
		break;
	case IMG_IFACE_ACT_BOUNDING_RECT:
		img_iface_bounding_rect();
		break;
	case IMG_IFACE_ACT_FIT_ELLIPSE:
		img_iface_fit_ellipse();
		break;
	case IMG_IFACE_ACT_MIN_AREA_RECT:
		img_iface_min_area_rect();
		break;
			/* Feature detection */
	case IMG_IFACE_ACT_HOUGH_CIRCLES:
		img_iface_hough_circles();
		break;
	/* img_orb */
	case IMG_IFACE_ACT_ALIGN:
		img_iface_align();
		break;
	/* img_calib3d */
	case IMG_IFACE_ACT_CALIBRATE:
		img_iface_calibrate();
		break;
	case IMG_IFACE_ACT_UNDISTORT:
		img_iface_undistort();
		break;
	/* img_zbar */
	case IMG_IFACE_ACT_DECODE:
		img_iface_decode();
		break;
	/* img_ocr */
	case IMG_IFACE_ACT_READ:
		img_iface_read();
		break;
	/* img_iface */
	case IMG_IFACE_ACT_APPLY:
		img_iface_apply();
		break;
	case IMG_IFACE_ACT_DISCARD:
		img_iface_discard();
		break;
	case IMG_IFACE_ACT_SAVE_MEM:
		img_iface_save_mem();
		break;
	case IMG_IFACE_ACT_LOAD_MEM:
		img_iface_load_mem();
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

	if (alx::CV::local_max(&image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Local maxima");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_skeleton		(void)
{

	if (alx::CV::thinning(&image_copy_tmp, cv::ximgproc::THINNING_ZHANGSUEN))
		goto err;

	user_iface_log_write(1, "Skeleton");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_lines_horizontal	(void)
{

	if (alx::CV::lines_horizontal(&image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Horizontal lines");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_lines_vertical	(void)
{

	if (alx::CV::lines_vertical(&image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Vertical lines");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_mean_horizontal	(void)
{

	if (alx::CV::mean_horizontal(&image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Horizontal mean");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_mean_vertical		(void)
{

	if (alx::CV::mean_vertical(&image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Vertical mean");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_median_horizontal	(void)
{

	if (alx::CV::median_horizontal(&image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Horizontal median");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_median_vertical	(void)
{

	if (alx::CV::median_vertical(&image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Vertical median");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_bkgd_mask	(void)
{

	if (alx::CV::bkgd_mask(&image_copy_tmp))
		goto err;
	user_iface_log_write(1, "Bkgd mask");
	return;
err:	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_bkgd_fill	(void)
{

	if (alx::CV::bkgd_fill(&image_copy_tmp))
		goto err;
	user_iface_log_write(1, "Bkgd fill");
	return;
err:	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_holes_mask	(void)
{

	if (alx::CV::holes_mask(&image_copy_tmp))
		goto err;
	user_iface_log_write(1, "Holes mask");
	return;
err:	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_holes_fill	(void)
{

	if (alx::CV::holes_fill(&image_copy_tmp))
		goto err;
	user_iface_log_write(1, "Holes fill");
	return;
err:	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_white_mask	(void)
{
	uint8_t	s_tolerance, l_tolerance;
	char	txt[LOG_LINE_LEN];

	s_tolerance	= user_iface_getint(0, 0, UINT8_MAX, "S tolerance:", NULL);
	l_tolerance	= user_iface_getint(0, 0, UINT8_MAX, "L tolerance:", NULL);
	if (alx::CV::white_mask(&image_copy_tmp, s_tolerance, l_tolerance))
		goto err;

	if (sbprintf(txt, NULL, "White mask: S_tol=%" PRIu8 "; L_tol=%" PRIu8 "",
					s_tolerance, l_tolerance) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_black_mask	(void)
{
	uint8_t	s_tolerance, l_tolerance;
	char	txt[LOG_LINE_LEN];

	s_tolerance	= user_iface_getint(0, 0, UINT8_MAX, "S tolerance:", NULL);
	l_tolerance	= user_iface_getint(0, 0, UINT8_MAX, "L tolerance:", NULL);
	if (alx::CV::black_mask(&image_copy_tmp, s_tolerance, l_tolerance))
		goto err;

	if (sbprintf(txt, NULL, "Black mask: S_tol=%" PRIu8 "; L_tol=%" PRIu8 "",
					s_tolerance, l_tolerance) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_gray_mask	(void)
{
	uint8_t	s_tolerance, l_tolerance;
	char	txt[LOG_LINE_LEN];

	s_tolerance	= user_iface_getint(0, 0, UINT8_MAX, "S tolerance:", NULL);
	l_tolerance	= user_iface_getint(0, 0, UINT8_MAX, "L tolerance:", NULL);
	if (alx::CV::gray_mask(&image_copy_tmp, s_tolerance, l_tolerance))
		goto err;

	if (sbprintf(txt, NULL, "Gray mask: S_tol=%" PRIu8 "; L_tol=%" PRIu8 "",
					s_tolerance, l_tolerance) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* cv ------------------------------------------------------------------------*/
/* ----- Core: The core functionality */
/* ----- ------- Pixel */
static	void	img_iface_pixel_get		(void)
{
	unsigned char	val;
	ptrdiff_t	x, y;
	char		txt[LOG_LINE_LEN];

	x	= user_iface_getint(0, 0, image_copy_tmp.cols, "x:", NULL);
	y	= user_iface_getint(0, 0, image_copy_tmp.rows, "y:", NULL);

	if (alx::CV::pixel_get_u8(&image_copy_tmp, &val, x, y))
		goto err;

	if (sbprintf(txt, NULL, "Pixel get:  (%ti, %ti): %hhu", x, y, val) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_pixel_set		(void)
{
	unsigned char	val;
	ptrdiff_t	x, y;
	char		txt[LOG_LINE_LEN];

	x	= user_iface_getint(0, 0, image_copy_tmp.cols, "x:", NULL);
	y	= user_iface_getint(0, 0, image_copy_tmp.rows, "y:", NULL);
	val	= user_iface_getint(0, 0, UINT8_MAX, "val:", NULL);

	if (alx::CV::pixel_set_u8(&image_copy_tmp, val, x, y))
		goto err;

	if (sbprintf(txt, NULL, "Pixel set:  (%ti, %ti): %hhu", x, y, val) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* ----- ------- ROI */
static	void	img_iface_set_ROI		(void)
{
	class cv::Rect_ <int>	rect;
	ptrdiff_t	tmp;
	char		txt[LOG_LINE_LEN];

	tmp		= image_copy_tmp.cols;
	rect.x		= user_iface_getint(0, 0, tmp, "Origin:  x:", NULL);
	tmp		= image_copy_tmp.rows;
	rect.y		= user_iface_getint(0, 0, tmp, "Origin:  y:", NULL);
	tmp		= image_copy_tmp.cols - rect.x;
	rect.width	= user_iface_getint(1, tmp, tmp, "Width:", NULL);
	tmp		= image_copy_tmp.rows - rect.y;
	rect.height	= user_iface_getint(1, tmp, tmp, "Height:", NULL);

	alx::CV::roi_set(&image_copy_tmp, &rect);

	if (sbprintf(txt, NULL, "ROI: (%i,%i) w=%i,h=%i",
				rect.x, rect.y, rect.width, rect.height) < 0)
		return;
	user_iface_log_write(1, txt);
}

static	void	img_iface_set_ROI_2rect		(void)
{
	char	txt[LOG_LINE_LEN];

	alx::CV::roi_set(&image_copy_tmp, &rectangle);

	if (sbprintf(txt, NULL, "ROI: (%i,%i) w=%i,h=%i",
				rectangle.x, rectangle.y,
				rectangle.width, rectangle.height) < 0)
		return;
	user_iface_log_write(1, txt);
}

/* ----- ------- Operations on arrays */
static	void	img_iface_and_2ref		(void)
{

	if (alx::CV::and_2ref(&image_copy_tmp, &image_ref))
		goto err;

	user_iface_log_write(1, "Bitwise AND");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Channels do not match ref)");
}

static	void	img_iface_not			(void)
{

	alx::CV::invert(&image_copy_tmp);

	user_iface_log_write(1, "Invert color");
}

static	void	img_iface_or_2ref		(void)
{

	if (alx::CV::or_2ref(&image_copy_tmp, &image_ref))
		goto err;

	user_iface_log_write(1, "Bitwise OR");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Channels do not match ref)");
}

static	void	img_iface_component		(void)
{
	ptrdiff_t	cmp, tmp;
	char		txt[LOG_LINE_LEN];

	tmp	= image_copy_tmp.channels() - 1;
	cmp	= user_iface_getint(0, 0, tmp, "Component:", NULL);

	if (alx::CV::component(&image_copy_tmp, cmp))
		goto err;

	if (sbprintf(txt, NULL, "Component %ti", cmp) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (channels)");
}

/* ----- Imgproc: Image processing */
/* ----- ------- Image filtering */
static	void	img_iface_dilate		(void)
{
	ptrdiff_t	i;
	char		txt[LOG_LINE_LEN];

	i	= user_iface_getint(1, 1, INFINITY, "Iterations:", NULL);

	if (alx::CV::dilate(&image_copy_tmp, i))
		goto err;

	if (sbprintf(txt, NULL, "Dilate i = %ti", i) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (iterations)");
}

static	void	img_iface_erode			(void)
{
	ptrdiff_t	i;
	char		txt[LOG_LINE_LEN];

	i	= user_iface_getint(1, 1, INFINITY, "Iterations:", NULL);

	if (alx::CV::erode(&image_copy_tmp, i))
		goto err;

	if (sbprintf(txt, NULL, "Erode i = %ti", i) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (iterations)");
}

static	void	img_iface_dilate_erode		(void)
{
	ptrdiff_t	i;
	char		txt[LOG_LINE_LEN];

	i	= user_iface_getint(1, 1, INFINITY, "Iterations:", NULL);

	if (alx::CV::dilate_erode(&image_copy_tmp, i))
		goto err;

	if (sbprintf(txt, NULL, "Dilate-erode i = %ti", i) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (iterations)");
}

static	void	img_iface_erode_dilate		(void)
{
	ptrdiff_t	i;
	char		txt[LOG_LINE_LEN];

	i	= user_iface_getint(1, 1, INFINITY, "Iterations:", NULL);

	if (alx::CV::erode_dilate(&image_copy_tmp, i))
		goto err;

	if (sbprintf(txt, NULL, "Erode-dilate i = %ti", i) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (iterations)");
}

static	void	img_iface_smooth		(void)
{
	int		method;
	ptrdiff_t	ksize;
	char		txt[LOG_LINE_LEN];

	method	= user_iface_getint(1, 3, 3,
				"Method: MEAN=1, GAUSS=2, MEDIAN=3", NULL);
	ksize	= user_iface_getint(3, 3, INFINITY,
				"Kernel size: 3, 5, 7, ...", NULL);

	if (alx::CV::smooth(&image_copy_tmp, method, ksize))
		goto err;

	if (sbprintf(txt, NULL, "Smooth mth = %i [%ti x %ti]",
						method, ksize, ksize) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input");
}

static	void	img_iface_sobel			(void)
{
	ptrdiff_t	dx, dy, ksize;
	char		txt[LOG_LINE_LEN];

	dx	= user_iface_getint(0, 1, 10,
			"Order of the derivative x", NULL);
	dy	= user_iface_getint(0, 1, 10,
			"Order of the derivative y", NULL);
	ksize	= user_iface_getint(-1, 3, 7,
			"Size of the extended Sobel kernel (Scharr: -1)", NULL);

	if (alx::CV::sobel(&image_copy_tmp, dx, dy, ksize))
		goto err;

	if (sbprintf(txt, NULL, "Sobel dx = %ti; dy = %ti [ks = %ti]",
						dx, dy, ksize) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_border		(void)
{
	ptrdiff_t	size;
	uint8_t		val;
	char		txt[LOG_LINE_LEN];

	size	= user_iface_getint(1, 1, INT16_MAX, "Size",NULL);
	val	= user_iface_getint(0, 0, UINT8_MAX, "Value",NULL);

	if (alx::CV::border(&image_copy_tmp, size, val))
		goto err;

	if (sbprintf(txt, NULL, "Border size = %ti; val = %" PRIu8 "", size,
								val) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* ----- ------- Geometric image transformations */
static	void	img_iface_mirror		(void)
{
	int	axis;
	char	txt[LOG_LINE_LEN];

	axis	= user_iface_getint(0, 1, 1, "Axis:  0=x;  1=y", NULL);

	if (alx::CV::mirror(&image_copy_tmp, axis))
		goto err;

	if (sbprintf(txt, NULL, "Mirror axis: %i", axis) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input");
}

static	void	img_iface_rotate_orto		(void)
{
	int	n;
	char	txt[LOG_LINE_LEN];

	n	= user_iface_getint(1, 1, 3,
			"Rotate (counterclockwise) n * pi/2 rad;  n:", NULL);

	if (alx::CV::rotate_orto(&image_copy_tmp, n))
		goto err;

	if (sbprintf(txt, NULL, "Rotate %i * pi/2 rad", n) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input");
}

static	void	img_iface_rotate		(void)
{
	double		x, y;
	double		angle;
	char		txt[LOG_LINE_LEN];

	x	= user_iface_getdbl(0, 0, INFINITY, "Center:  x:", NULL);
	y	= user_iface_getdbl(0, 0, INFINITY, "Center:  y:", NULL);
	angle = user_iface_getdbl(-INFINITY, 0, INFINITY, "Angle: (deg)", NULL);

	if (alx::CV::rotate(&image_copy_tmp, x, y, angle))
		goto err;

	if (sbprintf(txt, NULL, "Rotate (%.2lf,%.2lf) %lfº", x, y, angle) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input");
}

static	void	img_iface_rotate_2rect		(void)
{

	if (alx::CV::rotate_2rect(&image_copy_tmp, &rectangle_rot))
		goto err;

	user_iface_log_write(1, "Rotate to rectangle");
	return;
err:
	user_iface_log_write(1, "! Invalid input");
}

/* ----- ------- Miscellaneous image transformations */
static	void	img_iface_adaptive_thr		(void)
{
	int		method, thr_typ;
	ptrdiff_t	ksize;
	char		txt[LOG_LINE_LEN];

	method	= user_iface_getint(0, 1, 1, "Method: MEAN=0, GAUSS=1", NULL);
	thr_typ = user_iface_getint(0, 0, 1, "Type: BIN=0, BIN_INV=1", NULL);
	ksize	= user_iface_getint(3, 3, INFINITY,
					"Kernel size: 3, 5, 7, ...", NULL);

	if (alx::CV::adaptive_thr(&image_copy_tmp, method, thr_typ, ksize))
		goto err;

	if (sbprintf(txt, NULL, "Threshold mth=%i, typ=%i, ks=%ti",
						method, thr_typ, ksize) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_cvt_color		(void)
{
	int	method;
	char	txt[LOG_LINE_LEN];

	method	= user_iface_getint(0, 0, cv::COLOR_COLORCVT_MAX,
				"Method: BGR2GRAY = 6, BGR2HSV = 40, BGR2HLS = 52", NULL);

	if (alx::CV::cvt_color(&image_copy_tmp, method))
		goto err;

	if (sbprintf(txt, NULL, "Convert color %i", method) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input");
}

static	void	img_iface_distance_transform	(void)
{

	if (alx::CV::distance_transform_8b(&image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Distance transform");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_threshold		(void)
{
	int	thr_typ, thr_val;
	char	txt[LOG_LINE_LEN];

	thr_typ	= user_iface_getint(0, 0, 4,
			"Type: BIN=0, BIN_INV=1, TRUNC=2, TOZ=3, TOZ_INV=4",
									NULL);
	thr_val = user_iface_getint(-1, 0, UINT8_MAX,
			"Value: 0 to 255 (or -1 for Otsu's algorithm)", NULL);

	if (alx::CV::threshold(&image_copy_tmp, thr_typ, thr_val))
		goto err;

	if (sbprintf(txt, NULL, "Threshold typ = %i, val = %i",
							thr_typ, thr_val) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

/* ----- ------- Histograms */
static	void	img_iface_histogram1D		(void)
{

	if (alx::CV::histogram1D(&hist_img_c1, &image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Histogram");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_histogram3D		(void)
{

	if (alx::CV::histogram1D(&hist_img_c3, &image_copy_tmp))
		goto err;

	user_iface_log_write(1, "Histogram (3 channels)");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 3 channels)");
}

/* ----- ------- Structural analysis and shape descriptors */
static	void	img_iface_contours		(void)
{
	char	txt[LOG_LINE_LEN];

	if (alx::CV::contours(&image_copy_tmp, &contours))
		goto err;

	if (sbprintf(txt, NULL, "Contours n = %zu", contours.size()) < 0)
		return;
	user_iface_log_write(1, txt);
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_contours_dimensions	(void)
{
	char	txt[LOG_LINE_LEN];

	user_iface_log_write(1, "Contours size:");
	for (size_t i = 0; i < contours.size(); i++) {
		alx::CV::contour_dimensions(&contours[i],
						&area[i], &perimeter[i]);
		if (sbprintf(txt, NULL, "cnt[%zu]: A = %lf; P = %lf;",
						i, area[i], perimeter[i]) < 0)
			return;
		user_iface_log_write(2, txt);
	}
}

static	void	img_iface_bounding_rect		(void)
{

	if(!contours.size())
		goto err;

	alx::CV::bounding_rect(&rectangle, &contours[0]);
	alx::CV::draw_rect(&image_copy_tmp, &rectangle);

	user_iface_log_write(1, "Bounding rectangle");
	return;
err:
	user_iface_log_write(1, "! Invalid input (No contours saved)");
}

static	void	img_iface_fit_ellipse		(void)
{

	if(!contours.size())
		goto err;

	alx::CV::fit_ellipse(&rectangle_rot, &contours[0]);
	alx::CV::draw_rect_rot(&image_copy_tmp, &rectangle_rot);

	user_iface_log_write(1, "Fit ellipse");
	return;
err:
	user_iface_log_write(1, "! Invalid input (No contours saved)");
}

static	void	img_iface_min_area_rect		(void)
{

	if(!contours.size())
		goto err;

	alx::CV::min_area_rect(&rectangle_rot, &contours[0]);
	alx::CV::draw_rect_rot(&image_copy_tmp, &rectangle_rot);

	user_iface_log_write(1, "Min area rectangle");
	return;
err:
	user_iface_log_write(1, "! Invalid input (No contours saved)");
}

/* ----- ------- Feature detection */
static	void	img_iface_hough_circles		(void)
{
	struct Img_Iface_Data_Hough_Circles	data;
	char	txt[LOG_LINE_LEN];

	if (image_copy_tmp.channels() != 1)
		goto err;

	data.circles	= &circles;
	data.dist_min	= user_iface_getdbl(0, 5, INFINITY,
						"Minimum distance:", NULL);
	data.param_1	= user_iface_getdbl(0, 200, INFINITY, "param 1:", NULL);
	data.param_2	= user_iface_getdbl(0, 100, INFINITY, "param 2:", NULL);
	data.radius_min	= user_iface_getint(0, 10, INFINITY,
						"Minimum radius", NULL);
	data.radius_max	= user_iface_getint(0, 0, INFINITY,
						"Maximum radius", NULL);

	img_cv_act(&image_copy_tmp, IMG_CV_ACT_HOUGH_CIRCLES, &data);

	if (sbprintf(txt, NULL, "Circles n = %zu",data.circles->size()) < 0)
		return;
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

	alx::CV::orb_align(&image_ref, &image_copy_tmp, NULL);

	user_iface_log_write(1, "Align to reference");
	return;
err:
	user_iface_log_write(1, "! Reference is NULL");
}

/* calib3d -------------------------------------------------------------------*/
static	void	img_iface_calibrate		(void)
{
	struct Img_Iface_Data_Calibrate	data;

	if (image_copy_tmp.channels() != 1)
		goto err;

	data.intrinsic_mat	= &intrinsic_mat;
	data.dist_coefs		= &dist_coefs;
	data.rvecs		= &rvecs;
	data.tvecs		= &tvecs;

	img_calib3d_act(&image_copy_tmp, IMG_CALIB3D_ACT_CALIBRATE, &data);

	user_iface_log_write(1, "Calibrate");
	return;
err:
	user_iface_log_write(1, "! Invalid input (Must be 1 channel)");
}

static	void	img_iface_undistort		(void)
{
	struct Img_Iface_Data_Undistort	data;

	data.intrinsic_mat	= &intrinsic_mat;
	data.dist_coefs		= &dist_coefs;

	img_calib3d_act(&image_copy_tmp, IMG_CALIB3D_ACT_UNDISTORT, &data);

	user_iface_log_write(1, "Undistort");
}

/* zbar ----------------------------------------------------------------------*/
static	void	img_iface_decode		(void)
{
	void		*imgdata;
	ptrdiff_t	rows, cols;
	int		type;
	char		bcode_type[BUFSIZ];
	char		bcode_data[BUFSIZ];
	char		txt[LOG_LINE_LEN];

	type	= user_iface_getint(0, 0, INT_MAX,
					"Type of code: (0 for all)", NULL);

	alx_cv_extract_imgdata(&image_copy_tmp, &imgdata, &rows, &cols,
					NULL, NULL, NULL, NULL, NULL);
	if (alx_zbar_read(ARRAY_SIZE(bcode_data), bcode_data, bcode_type,
					imgdata, rows, cols, type))
		goto err;

	if (sbprintf(txt, NULL, "Detect bcodes (type = %i)", type) < 0)
		return;
	user_iface_log_write(1, txt);

	if (sbprintf(txt, NULL, "%s -- '%s'", bcode_type, bcode_data) < 0)
		return;
	user_iface_log_write(2, txt);
	return;
err:
	user_iface_log_write(2, "! No code detected");
}

/* ocr -----------------------------------------------------------------------*/
static	void	img_iface_read			(void)
{
	void		*imgdata;
	ptrdiff_t	w, h, B_per_pix, B_per_line;
	int		lang, conf;
	char		txt[LOG_LINE_LEN];

	lang = user_iface_getint(alx::ocr::LANG_ENG, 0,
				alx::ocr::LANG_DIGITS_COMMA,
				"Language: ENG = 0, SPA = 1, CAT = 2", NULL);
	conf = user_iface_getint(alx::ocr::CONF_NONE, 1,
				alx::ocr::CONF_PRICE_USD,
				"Config: none = 0, Price = 1", NULL);

	alx_cv_extract_imgdata(&image_copy_tmp, &imgdata, NULL, NULL, &w, &h,
					&B_per_pix, &B_per_line, NULL);
	if (alx_ocr_read(ARRAY_SIZE(img_ocr_text), img_ocr_text, imgdata, w, h,
					B_per_pix, B_per_line, lang, conf))
		goto err;

	if (sbprintf(txt, NULL, "OCR (lang = %i) [conf = %i]", lang, conf) < 0)
		return;
	user_iface_log_write(1, txt);
	if (img_ocr_text[0] == '\0')
		goto err;
	return;
err:
	user_iface_log_write(2, "! No text detected");
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

static	void	img_iface_save_mem		(void)
{
	ptrdiff_t	m;
	char		txt[LOG_LINE_LEN];

	m = user_iface_getint(0, 0, IMG_MEM_SIZE - 1, "mem[m];  m:", NULL);

	image_mem[m].release();
	image_copy_tmp.copyTo(image_mem[m]);

	if (sbprintf(txt, NULL, "Save to mem[%ti]", m) < 0)
		return;
	user_iface_log_write(1, txt);
}

static	void	img_iface_load_mem		(void)
{
	ptrdiff_t	m;
	char		txt[LOG_LINE_LEN];

	m = user_iface_getint(0, 0, IMG_MEM_SIZE - 1, "mem[m];  m:", NULL);

	if (image_mem[m].empty())
		goto err;

	image_copy_tmp.release();
	image_mem[m].copyTo(image_copy_tmp);

	if (sbprintf(txt, NULL, "Load from mem[%ti]", m) < 0)
		return;
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
