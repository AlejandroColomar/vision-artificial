/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/proc/objects.hpp"

#include <cstddef>
#include <cstdio>

#include <opencv2/opencv.hpp>

#include "libalx/base/math/median.hpp"

#include "vision-artificial/image/iface.hpp"
#include "vision-artificial/proc/common.hpp"
#include "vision-artificial/user/iface.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define OBJECTS_MAX		(0xFFF)
#define PATTERN_SQUARE_LEN_MM	(10.0)

#define PATTERN_DILATE		(2)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/
struct	Point {
	uint16_t	x_pix;
	uint16_t	y_pix;

	double		x_mm;
	double		y_mm;
};

struct	Pattern_Square {
	struct Point	pos;
	uint8_t		len;
};

struct	Pattern_Properties {
	struct Point		center;
	struct Point		origin;

	double			angle;

	uint16_t		height;
	uint16_t		width;

	/* Squares (distance calibration) */
	struct Pattern_Square	square[OBJECTS_MAX];
};

struct	Objects_Properties {
	struct Point	pos;

	double		angle;

	double		area_pix2;
	double		area_mm2;
	double		perimeter_pix;
	double		perimeter_mm;

	double		ratio_p2_a;
	double		area_rect;
	double		ratio_a_arect;
	double		perimeter_rect;
	double		ratio_p_prect;
};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	class std::vector <class std::vector <class cv::Point_ <int>>>	contours;
static	class cv::Mat			hierarchy;
static	class cv::RotatedRect		rect_rot[OBJECTS_MAX];
static	class cv::Rect_ <int>		rect[OBJECTS_MAX];
static	struct Pattern_Properties	pattern;
static	ptrdiff_t			squares_n;
static	double				ratio_mm_pix;
static	ptrdiff_t			objects_n;
static	struct Objects_Properties	objects[OBJECTS_MAX];


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	result_objects		(int status);

static	void	pattern_bgr2gray	(void);
static	int	pattern_find		(void);
static	void	pattern_rotation_get	(void);
static	void	pattern_rotation_fix	(void);
static	void	pattern_dimensions_get	(void);
static	void	pattern_dimensions_fix	(void);
static	void	pattern_squares_find	(void);
static	void	pattern_squares_pos_get	(void);
static	void	pattern_squares_len_get	(void);
static	void	pattern_calib_mm_pix	(void);

static	void	objects_bgr2gray	(void);
static	void	objects_rotation_fix	(void);
static	void	objects_dimensions_fix	(void);
static	void	objects_segment		(void);
static	int	objects_contours	(void);
static	void	objects_position_pix	(void);
static	void	objects_size_pix	(void);
static	void	objects_shape		(void);
static	void	objects_position_mm	(void);
static	void	objects_size_mm		(void);
static	void	objects_log		(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_objects_calibrate		(void)
{
	int	status;

	proc_save_mem(0);
	/* Calibrate angle with the whole pattern */
	clock_start();
	pattern_bgr2gray();
	status	= pattern_find();
	if (status) {
		result_objects(status);
		return	status;
	}
	pattern_rotation_get();
	pattern_rotation_fix();
	clock_stop("Calibrate: rotation");

	/* Segmentate pattern squares */
	clock_start();
	pattern_dimensions_get();
	pattern_dimensions_fix();
	clock_stop("Calibrate: dimensions");

	/* Find squares */
	clock_start();
	pattern_squares_find();
	pattern_squares_pos_get();
	pattern_squares_len_get();
	pattern_calib_mm_pix();
	clock_stop("Calibrate (mm per pix)");

	result_objects(OBJECTS_OK);
	return	0;
}

int	proc_objects			(void)
{
	int	status;

	proc_save_mem(0);
	/* Align image */
	clock_start();
	objects_bgr2gray();
	objects_rotation_fix();
	objects_dimensions_fix();
	clock_stop("Align image to pattern");

	/* Segment objects */
	clock_start();
	objects_segment();
	clock_stop("Segment objects");

	/* Find objects positions */
	clock_start();
	status	= objects_contours();
	if (status) {
		result_objects(status);
		return	status;
	}
	clock_stop("Find objects");

	/* Get objects properties in pixels */
	clock_start();
	objects_position_pix();
	objects_position_mm();
	clock_stop("Objects positions");

	/* Get objects properties in mm */
	clock_start();
	objects_size_pix();
	objects_size_mm();
	clock_stop("Objects sizes");

	/* Get objects properties in mm */
	clock_start();
	objects_shape();
	clock_stop("Objects shapes");

	/* Print properties of objects into log */
	clock_start();
	objects_log();
	clock_stop("Objects properties (log)");

	result_objects(OBJECTS_OK);
	return	0;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	result_objects		(int status)
{

	switch (status) {
	case OBJECTS_OK:
		user_iface_log_write(0, "Coin:  OK");
		break;
	case OBJECTS_NOK_OBJECTS:
		user_iface_log_write(0, "Coin:  NOK_OBJECTS");
		break;

	case OBJECTS_NOK_SIZE:
		user_iface_log_write(0, "Coin:  NOK_SIZE");
		break;
	default:
		user_iface_log_write(0, "Coin:  NOK");
	}
}

/* calibration ---------------------------------------------------------------*/
static	void	pattern_bgr2gray	(void)
{

	proc_load_mem(0);

	proc_cvt_color(cv::COLOR_BGR2GRAY);
	proc_save_mem(1);
}

static	int	pattern_find		(void)
{

	proc_load_mem(1);

	proc_threshold(cv::THRESH_BINARY_INV, IMG_IFACE_THR_OTSU);
	proc_erode_dilate(16);
	proc_dilate(2);
	proc_contours(&contours, &hierarchy);
	if (contours.size() != 1)
		return	OBJECTS_NOK_PATTERN;

	return	0;
}

static	void	pattern_rotation_get	(void)
{
	char	txt[LOG_LINE_LEN];

	proc_min_area_rect(&(contours[0]), &(rect_rot[0]), true);

	/* If angle is < -45º, it is taking into account the incorrect side */
	if (rect_rot[0].angle < -45.0)
		rect_rot[0].angle	+= 90.0;
	pattern.angle		= rect_rot[0].angle;
	pattern.center.x_pix	= rect_rot[0].center.x;
	pattern.center.y_pix	= rect_rot[0].center.y;

	snprintf(txt, LOG_LINE_LEN, 	"pattern angle		= %lf DEG",
							-pattern.angle);
	user_iface_log_write(0, txt);
	snprintf(txt, LOG_LINE_LEN, 	"pattern center (x)	= %i pix",
							pattern.center.x_pix);
	user_iface_log_write(0, txt);
	snprintf(txt, LOG_LINE_LEN, 	"pattern center (y)	= %i pix",
							pattern.center.y_pix);
	user_iface_log_write(0, txt);
}

static	void	pattern_rotation_fix	(void)
{

	proc_load_mem(1);

	/* invert image before rotation to avoid black bands */
	proc_not();
	proc_rotate(pattern.center.x_pix, pattern.center.y_pix, pattern.angle);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_save_mem(2);
}

static	void	pattern_dimensions_get	(void)
{

	proc_load_mem(2);

	proc_erode_dilate(16);
	proc_dilate(PATTERN_DILATE);
	proc_contours(&contours, &hierarchy);
	proc_bounding_rect(&(contours[0]), &(rect[0]), true);
}

static	void	pattern_dimensions_fix	(void)
{

	/* Need to correct also pattern dilatation (pattern_dimensions_get()) */
	pattern.origin.x_pix	= rect[0].x;
	pattern.origin.y_pix	= rect[0].y;
	pattern.width		= rect[0].width;
	pattern.height		= rect[0].height;

	proc_load_mem(2);
	proc_ROI(pattern.origin.x_pix, pattern.origin.y_pix,
						pattern.width, pattern.height);
	proc_save_mem(3);
}

static	void	pattern_squares_find	(void)
{

	proc_load_mem(3);

	proc_distance_transform();
	proc_local_max();
	proc_dilate(6);
	proc_save_mem(4);

	proc_contours(&contours, &hierarchy);
	squares_n	= contours.size();
}

static	void	pattern_squares_pos_get	(void)
{

	for (ptrdiff_t i = 0; i < squares_n; i++) {
		proc_bounding_rect(&(contours[i]), &(rect[i]), true);
		pattern.square[i].pos.x_pix	= rect[i].x + rect[i].width/2.0;
		pattern.square[i].pos.y_pix	= rect[i].y + rect[i].height/2.0;
	}
}

static	void	pattern_squares_len_get	(void)
{

	proc_load_mem(4);
	for (ptrdiff_t i = 0; i < squares_n; i++) {
		proc_pixel_get(pattern.square[i].pos.x_pix,
					pattern.square[i].pos.y_pix,
					&(pattern.square[i].len));
		pattern.square[i].len *= 2;
	}
}

static	void	pattern_calib_mm_pix	(void)
{
	uint8_t	pattern_len[OBJECTS_MAX];
	uint8_t	median_size;
	char	txt[LOG_LINE_LEN];

	for (ptrdiff_t i = 0; i < squares_n; i++)
		pattern_len[i]	= pattern.square[i].len;
	median_size	= alx_median_u8(squares_n, pattern_len);
	ratio_mm_pix	= PATTERN_SQUARE_LEN_MM / median_size;

	snprintf(txt, LOG_LINE_LEN, "mm/pix	= %lf", ratio_mm_pix);
	user_iface_log_write(0, txt);
}

/* process -------------------------------------------------------------------*/
static	void	objects_bgr2gray	(void)
{

	proc_load_mem(0);

	proc_cvt_color(cv::COLOR_BGR2GRAY);
	proc_save_mem(1);
}

static	void	objects_rotation_fix	(void)
{

	proc_load_mem(1);

	/* invert image before rotation to avoid black bands */
	proc_not();
	proc_rotate(pattern.center.x_pix, pattern.center.y_pix, pattern.angle);
	proc_save_mem(2);
}

static	void	objects_dimensions_fix	(void)
{

	/* Need to correct also pattern dilatation (pattern_dimensions_get()) */
	proc_load_mem(2);

	proc_ROI(pattern.origin.x_pix, pattern.origin.y_pix,
						pattern.width, pattern.height);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_save_mem(3);
}

static	void	objects_segment		(void)
{

	proc_load_mem(3);

	proc_dilate_erode(3);
	proc_erode_dilate(3);
	proc_save_mem(4);

	proc_distance_transform();
	proc_threshold(cv::THRESH_BINARY_INV, 30);
	proc_distance_transform();
	proc_skeleton();
	proc_threshold(cv::THRESH_BINARY_INV, 10);
	proc_save_ref();
	proc_save_mem(5);

	proc_load_mem(3);
	proc_and_2ref();
	proc_distance_transform();
	proc_threshold(cv::THRESH_BINARY, 10);
	proc_save_mem(6);
}

static	int	objects_contours	(void)
{

	proc_load_mem(6);

	proc_contours(&contours, &hierarchy);
	objects_n	= contours.size();
	if (!objects_n)
		return	OBJECTS_NOK_OBJECTS;

	return	0;
}

static	void	objects_position_pix	(void)
{

	for (ptrdiff_t i = 0; i < objects_n; i++) {
		proc_fit_ellipse(&(contours[i]), &(rect_rot[i]), true);
		objects[i].pos.x_pix	= rect_rot[i].center.x;
		objects[i].pos.y_pix	= rect_rot[i].center.y;
		objects[i].angle	= -rect_rot[i].angle + 90.0;
		if (objects[i].angle < 0)
			objects[i].angle	+= 180.0;
	}
}

static	void	objects_position_mm	(void)
{

	for (ptrdiff_t i = 0; i < objects_n; i++) {
		objects[i].pos.x_mm	= ratio_mm_pix * objects[i].pos.x_pix;
		objects[i].pos.y_mm	= ratio_mm_pix * objects[i].pos.y_pix;
	}
}

static	void	objects_size_pix	(void)
{
	double	area[OBJECTS_MAX];
	double	perimeter[OBJECTS_MAX];

	proc_contours_size(&contours, area, perimeter);
	for (ptrdiff_t i = 0; i < objects_n; i++) {
		objects[i].area_pix2		= area[i];
		objects[i].perimeter_pix	= perimeter[i];
	}
}

static	void	objects_size_mm	(void)
{

	for (ptrdiff_t i = 0; i < objects_n; i++) {
		objects[i].area_mm2	= pow(ratio_mm_pix, 2) *
						objects[i].area_pix2;
		objects[i].perimeter_mm	= ratio_mm_pix *
						objects[i].perimeter_pix;
	}
}

static	void	objects_shape		(void)
{

	for (ptrdiff_t i = 0; i < objects_n; i++) {
		proc_min_area_rect(&(contours[i]), &(rect_rot[i]), true);

		objects[i].ratio_p2_a	= pow(objects[i].perimeter_pix, 2) /
						objects[i].area_pix2;
		objects[i].area_rect	= rect_rot[i].size.width *
						rect_rot[i].size.height;
		objects[i].perimeter_rect = 2.0 * (rect_rot[i].size.width +
						rect_rot[i].size.height);
		objects[i].ratio_p_prect = objects[i].perimeter_pix /
						objects[i].perimeter_rect;
		objects[i].ratio_a_arect = objects[i].area_pix2 /
						objects[i].area_rect;
	}
}

static	void	objects_log	(void)
{
	char	txt[LOG_LINE_LEN];

	for (ptrdiff_t i = 0; i < objects_n; i++) {
		snprintf(txt, LOG_LINE_LEN, "Object[%ti]:", i);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	pos:	(%.1lf, %.1lf) mm",
						objects[i].pos.x_mm,
						objects[i].pos.y_mm);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	ang	= %.1lf DEG",
						objects[i].angle);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	A	= %.1lf mm2",
						objects[i].area_mm2);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	P	= %.1lf mm",
						objects[i].perimeter_mm);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	p2/A	= %.2lf",
						objects[i].ratio_p2_a);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	p/p_r	= %.2lf",
						objects[i].ratio_p_prect);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	A/A_r	= %.2lf",
						objects[i].ratio_a_arect);
		user_iface_log_write(0, txt);
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
