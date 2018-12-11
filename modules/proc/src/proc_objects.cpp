/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* snprintf() */
	#include <cstdio>

/* Packages ------------------------------------------------------------------*/
		/* openCV */
	#include <opencv2/opencv.hpp>

/* libalx ------------------------------------------------------------------*/
		/* alx_maximum_u8() */
	#include "alx_math.hpp"

/* Project -------------------------------------------------------------------*/
		/* constants */
	#include "img_iface.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
	#include "proc_common.hpp"

	#include "proc_objects.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	COINS_MAX	1024


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
struct	Objects_Properties {
	/* position */
	double	x_pix;
	double	y_pix;
	double	x_mm;
	double	y_mm;

	/* Calibration */
	int8_t	value;

	/* Angle */
	double	angle;

	/* Area */
	double	area_pix2;
	double	area_mm2;

	/* Perimeter */
	double	perimeter_pix;
	double	perimeter_mm;

	/* type */
	double	ratio_p2_a;
	double	area_rect;
	double	ratio_a_arect;
	double	perimeter_rect;
	double	ratio_p_prect;

	/* diammeter */
	uint8_t	diameter_pix;
};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/

/* Static --------------------------------------------------------------------*/
static	std::vector <std::vector <cv::Point_ <int>>>	contours;
static	class cv::Mat					hierarchy;
static	class cv::RotatedRect				rect_rot [CONTOURS_MAX];
static	int						objects_n;
static	struct Objects_Properties			objects [CONTOURS_MAX];
static	double						ratio_mm_per_pix;
static	double						pattern_angle;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_objects		(int status);

static	void	pattern_segmentate	(void);
static	void	pattern_position_pix	(void);
static	void	pattern_len_pix		(void);
static	void	calibrate_mm_per_pix	(void);
static	int	pattern_angle_get	(void);

static	void	objects_segmentate	(void);
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
	/* Segmentate coins */
	{
		/* Measure time */
		clock_start();

		pattern_segmentate();

		/* Measure time */
		clock_stop("Segmentate pattern squares");
	}
	/* Find coins positions */
	{
		/* Measure time */
		clock_start();

		status	= objects_contours();
		if (status) {
			result_objects(status);
			return	status;
		}
		pattern_position_pix();

		/* Measure time */
		clock_stop("Find squares");
	}
	/* Calibrate size with the mean of the squares */
	{
		/* Measure time */
		clock_start();

		pattern_len_pix();
		calibrate_mm_per_pix();

		/* Measure time */
		clock_stop("Calibrate (mm per pix)");
	}
	/* Calibrate angle with the whole pattern */
	{
		/* Measure time */
		clock_start();

		status	= pattern_angle_get();
		if (status) {
			result_objects(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Calibrate (angle)");
	}

	status	= OBJECTS_OK;
	result_objects(status);
	return	status;
}

int	proc_objects			(void)
{
	int	status;

	proc_save_mem(0);
	/* Segmentate coins */
	{
		/* Measure time */
		clock_start();

		objects_segmentate();

		/* Measure time */
		clock_stop("Segmentate objects");
	}
	/* Find coins positions */
	{
		/* Measure time */
		clock_start();

		status	= objects_contours();
		if (status) {
			result_objects(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find objects positions");
	}
	/* Get objects properties in pixels */
	{
		/* Measure time */
		clock_start();

		objects_position_pix();
		objects_position_mm();

		/* Measure time */
		clock_stop("Objects positions");
	}
	/* Get objects properties in mm */
	{
		/* Measure time */
		clock_start();

		objects_size_pix();
		objects_size_mm();

		/* Measure time */
		clock_stop("Objects sizes");
	}
	/* Get objects properties in mm */
	{
		/* Measure time */
		clock_start();

		objects_shape();

		/* Measure time */
		clock_stop("Objects shapes");
	}
	/* Print properties of objects into log */
	{
		/* Measure time */
		clock_start();

		objects_log();

		/* Measure time */
		clock_stop("Objects properties (log)");
	}

	status	= OBJECTS_OK;
	result_objects(status);
	return	status;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_objects		(int status)
{
	/* Cleanup */

	/* Write result into log */
	switch (status) {
	case OBJECTS_OK:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  OK");
		break;
	case OBJECTS_NOK_OBJECTS:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK_OBJECTS");
		break;

	case OBJECTS_NOK_SIZE:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK_SIZE");
		break;
	default:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK");
		break;
	}
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

/* calibration ---------------------------------------------------------------*/
static	void	pattern_segmentate	(void)
{
	proc_load_mem(0);

	proc_cvt_color(cv::COLOR_BGR2GRAY);
	proc_threshold(cv::THRESH_BINARY_INV, IMG_IFACE_THR_OTSU);
	proc_save_mem(1);

	proc_distance_transform();
	proc_local_max();
	proc_dilate(6);
	/* Needs to be 3 because objects_contours() loads mem3 */
	proc_save_mem(3);
}

static	void	pattern_position_pix	(void)
{
	int	i;
	class cv::Rect_ <int>	rect [CONTOURS_MAX];

	/* Get position of each contour */
	for (i = 0; i < objects_n; i++) {
		proc_bounding_rect(&(contours[i]), &(rect[i]), true);

		objects[i].x_pix	= rect[i].x + rect[i].width / 2.0;
		objects[i].y_pix	= rect[i].y + rect[i].height / 2.0;
	}
}

static	void	pattern_len_pix		(void)
{
	int	i;

	proc_load_mem(3);

	/* Get pattern square side lenght in pixels */
	for (i = 0; i < objects_n; i++) {
		proc_pixel_value(objects[i].x_pix, objects[i].y_pix,
						&(objects[i].diameter_pix));
		objects[i].diameter_pix	*= 2;
	}
}

static	void	calibrate_mm_per_pix	(void)
{
	uint8_t	pattern_size_pix [CONTOURS_MAX];
	int	i;
	int	median_size;

	for (i = 0; i < objects_n; i++) {
		pattern_size_pix[i]	= objects[i].diameter_pix;
	}

	median_size		= alx_median_u8(objects_n, pattern_size_pix);
	ratio_mm_per_pix	= 10.0 / median_size;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"mm/pix	= %lf",
					ratio_mm_per_pix);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

static	int	pattern_angle_get	(void)
{
	int	status;

	proc_load_mem(1);

	proc_erode_dilate(2);
	proc_dilate(2);
	proc_contours(&contours, &hierarchy);
	proc_save_mem(2);

	/* If no contour is found, error:  NOK_PATTERN_ANGLE */
	if (contours.size() != 1) {
		status	= OBJECTS_NOK_PATTERN_ANGLE;
		return	status;
	}

	proc_min_area_rect(&(contours[0]), &(rect_rot[0]), true);
	pattern_angle	= -rect_rot[0].angle;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"pattern angle	= %lf DEG",
					pattern_angle);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	status	= OBJECTS_OK;
	return	status;
}

/* process -------------------------------------------------------------------*/
static	void	objects_segmentate	(void)
{
	proc_load_mem(0);

	proc_cvt_color(cv::COLOR_BGR2GRAY);
	proc_threshold(cv::THRESH_BINARY_INV, IMG_IFACE_THR_OTSU);
	proc_dilate_erode(3);
	proc_erode_dilate(3);
	proc_save_mem(1);

	proc_distance_transform();
	proc_threshold(cv::THRESH_BINARY_INV, 30);
	proc_distance_transform();
	proc_skeleton();
	proc_threshold(cv::THRESH_BINARY_INV, 10);
	proc_save_ref();
	proc_save_mem(2);

	proc_load_mem(1);
	proc_and_2ref();
	proc_distance_transform();
	proc_threshold(cv::THRESH_BINARY, 5);
	proc_save_mem(3);
}

static	int	objects_contours	(void)
{
	int	status;

	proc_load_mem(3);

	proc_contours(&contours, &hierarchy);
	objects_n	= contours.size();

	/* If no contour is found, error:  NOK_OBJECTS */
	if (!objects_n) {
		status	= OBJECTS_NOK_OBJECTS;
		return	status;
	}

	status	= OBJECTS_OK;
	return	status;
}

static	void	objects_position_pix	(void)
{
	int	i;

	/* Get position of each contour */
	for (i = 0; i < objects_n; i++) {
		proc_fit_ellipse(&(contours[i]), &(rect_rot[i]), true);

		objects[i].x_pix	= rect_rot[i].center.x;
		objects[i].y_pix	= rect_rot[i].center.y;

		objects[i].angle	= (-rect_rot[i].angle + 90.0) -
								pattern_angle;
		if (objects[i].angle < 0) {
			objects[i].angle	+= 180.0;
		}
	}
}

static	void	objects_position_mm	(void)
{
	int	i;

	/* Get position of each contour */
	for (i = 0; i < objects_n; i++) {
		objects[i].x_mm	= ratio_mm_per_pix * objects[i].x_pix;
		objects[i].y_mm	= ratio_mm_per_pix * objects[i].y_pix;
	}
}

static	void	objects_size_pix	(void)
{
	double	area [CONTOURS_MAX];
	double	perimeter [CONTOURS_MAX];
	int	i;

	/* Get size of each contour */
	proc_contours_size(&contours, area, perimeter);
	for (i = 0; i < objects_n; i++) {
		objects[i].area_pix2		= area[i];
		objects[i].perimeter_pix	= perimeter[i];
	}
}

static	void	objects_size_mm	(void)
{
	int	i;

	/* Get position of each contour */
	for (i = 0; i < objects_n; i++) {
		objects[i].area_mm2	= pow(ratio_mm_per_pix, 2) *
						objects[i].area_pix2;
		objects[i].perimeter_mm	= ratio_mm_per_pix *
						objects[i].perimeter_pix;
	}
}

static	void	objects_shape		(void)
{
	int	i;

	/* Get shape of each contour */
	for (i = 0; i < objects_n; i++) {
		proc_min_area_rect(&(contours[i]), &(rect_rot[i]), true);

		objects[i].ratio_p2_a	= pow(objects[i].perimeter_pix, 2) /
						objects[i].area_pix2;

		objects[i].area_rect		= rect_rot[i].size.width *
						rect_rot[i].size.height;
		objects[i].perimeter_rect	= 2.0 *
						(rect_rot[i].size.width +
						rect_rot[i].size.height);

		objects[i].ratio_p_prect	= objects[i].perimeter_pix /
						objects[i].perimeter_rect;
		objects[i].ratio_a_arect	= objects[i].area_pix2 /
						objects[i].area_rect;
	}
}

static	void	objects_log	(void)
{
	int	i;

	/* Get coins diameters in mm */
	for (i = 0; i < objects_n; i++) {
		if (objects[i].area_mm2 > 100) {
			/* Write diameters into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"Object[%i]:",
					i);
			user_iface_log.lvl[user_iface_log.len]	= 0;
			(user_iface_log.len)++;

			/* Write diameters into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"	pos:	(%.1lf, %.1lf) mm",
					objects[i].x_mm, objects[i].y_mm);
			user_iface_log.lvl[user_iface_log.len]	= 0;
			(user_iface_log.len)++;

			/* Write diameters into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"	ang	= %.1lf DEG",
					objects[i].angle);
			user_iface_log.lvl[user_iface_log.len]	= 0;
			(user_iface_log.len)++;

			/* Write diameters into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"	A	= %.1lf mm2",
					objects[i].area_mm2);
			user_iface_log.lvl[user_iface_log.len]	= 0;
			(user_iface_log.len)++;

			/* Write diameters into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"	P	= %.1lf mm",
					objects[i].perimeter_mm);
			user_iface_log.lvl[user_iface_log.len]	= 0;
			(user_iface_log.len)++;

			/* Write diameters into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"	p2/A	= %.2lf",
					objects[i].ratio_p2_a);
			user_iface_log.lvl[user_iface_log.len]	= 0;
			(user_iface_log.len)++;

			/* Write diameters into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"	p/p_r	= %.2lf",
					objects[i].ratio_p_prect);
			user_iface_log.lvl[user_iface_log.len]	= 0;
			(user_iface_log.len)++;

			/* Write diameters into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"	A/A_r	= %.2lf",
					objects[i].ratio_a_arect);
			user_iface_log.lvl[user_iface_log.len]	= 0;
			(user_iface_log.len)++;
		}
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
