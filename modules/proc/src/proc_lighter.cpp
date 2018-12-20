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

	#include "proc_lighter.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	PATTERN_SQUARE_LEN_MM	(10.0)

	# define	PATTERN_DILATE		(2)


/******************************************************************************
 ******* structs **************************************************************
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
	/* center */
	struct Point		center;
	struct Point		origin;

	/* Angle */
	double			angle;

	/* Size */
	uint16_t		height;
	uint16_t		width;

	/* Squares (distance calibration) */
	struct Pattern_Square	square [CONTOURS_MAX];
	int			squares_n;
};

struct	Lighter_Properties {
	/* position */
	struct Point	pos;

	/* Angle */
	double		angle;

	/* Area */
	double		area_pix2;
	double		area_mm2;

	/* Perimeter */
	double		perimeter_pix;
	double		perimeter_mm;

	/* type */
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
static	std::vector <std::vector <cv::Point_ <int>>>	contours;
static	class cv::Mat					hierarchy;
static	class cv::RotatedRect				rect_rot [1];
static	class cv::Rect_ <int>				rect [CONTOURS_MAX];
static	struct Pattern_Properties			pattern;
static	double						ratio_mm_pix;
static	struct Lighter_Properties			lighter;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_lighter		(int status);

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

static	void	lighter_bgr2gray	(void);
static	void	lighter_rotation_fix	(void);
static	void	lighter_dimensions_fix	(void);
static	void	lighter_segment_full	(void);
static	int	lighter_align		(void);
static	void	lighter_segment_base	(void);
static	void	lighter_crop		(void);
static	int	lighter_contours	(void);
static	void	lighter_position_pix	(void);
static	void	lighter_size_pix	(void);
static	void	lighter_shape		(void);
static	void	lighter_position_mm	(void);
static	void	lighter_size_mm		(void);
static	void	lighter_log		(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_lighter_calibrate	(void)
{
	int	status;

	proc_save_mem(0);
	/* Calibrate angle with the whole pattern */
	{
		/* Measure time */
		clock_start();

		pattern_bgr2gray();
		status	= pattern_find();
		if (status) {
			result_lighter(status);
			return	status;
		}
		pattern_rotation_get();
		pattern_rotation_fix();

		/* Measure time */
		clock_stop("Calibrate: rotation");
	}
	/* Segmentate pattern squares */
	{
		/* Measure time */
		clock_start();

		pattern_dimensions_get();
		pattern_dimensions_fix();

		/* Measure time */
		clock_stop("Calibrate: dimensions");
	}
	/* Find squares */
	{
		/* Measure time */
		clock_start();

		pattern_squares_find();
		pattern_squares_pos_get();
		pattern_squares_len_get();
		pattern_calib_mm_pix();

		/* Measure time */
		clock_stop("Calibrate (mm per pix)");
	}

	status	= LIGHTER_OK;
	result_lighter(status);
	return	status;
}

int	proc_lighter		(void)
{
	int	status;

	proc_save_mem(0);
	/* Align image */
	{
		/* Measure time */
		clock_start();

		lighter_bgr2gray();
		lighter_rotation_fix();
		lighter_dimensions_fix();

		/* Measure time */
		clock_stop("Align image to pattern");
	}
	/* Segment lighter */
	{
		/* Measure time */
		clock_start();

		lighter_segment_full();
		lighter_align();
		lighter_segment_base();
		lighter_crop();

		/* Measure time */
		clock_stop("Segment lighter");
	}
#if 0	/* Find lighters positions */
	{
		/* Measure time */
		clock_start();

		status	= lighter_contours();
		if (status) {
			result_lighter(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find lighter");
	}
	/* Get lighter properties in pixels */
	{
		/* Measure time */
		clock_start();

		lighter_position_pix();
		lighter_position_mm();

		/* Measure time */
		clock_stop("Lighter position");
	}
	/* Get lighter properties in mm */
	{
		/* Measure time */
		clock_start();

		lighter_size_pix();
		lighter_size_mm();

		/* Measure time */
		clock_stop("Lighter size");
	}
	/* Get lighter properties in mm */
	{
		/* Measure time */
		clock_start();

		lighter_shape();

		/* Measure time */
		clock_stop("Lighter shape");
	}
	/* Print properties of lighter into log */
	{
		/* Measure time */
		clock_start();

		lighter_log();

		/* Measure time */
		clock_stop("Lighter properties (log)");
	}
#endif
	status	= LIGHTER_OK;
	result_lighter(status);
	return	status;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_lighter		(int status)
{
	/* Cleanup */

	/* Write result into log */
	switch (status) {
	case LIGHTER_OK:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  OK");
		break;
	case LIGHTER_NOK_LIGHTER:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK_LIGHTER");
		break;

	case LIGHTER_NOK_SIZE:
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
static	void	pattern_bgr2gray	(void)
{
	proc_load_mem(0);

	proc_cvt_color(cv::COLOR_BGR2GRAY);
	proc_save_mem(1);
}

static	int	pattern_find		(void)
{
	int	status;

	proc_load_mem(1);

	proc_threshold(cv::THRESH_BINARY_INV, IMG_IFACE_THR_OTSU);
	proc_erode_dilate(16);
	proc_dilate(2);
	proc_contours(&contours, &hierarchy);

	/* If no contour is found, error:  NOK_PATTERN */
	if (contours.size() != 1) {
		status	= LIGHTER_NOK_PATTERN;
		return	status;
	}

	status	= LIGHTER_OK;
	return	status;
}

static	void	pattern_rotation_get	(void)
{
	proc_min_area_rect(&(contours[0]), &(rect_rot[0]), true);

	/* If angle is < -45º, it is taking into account the incorrect side */
	if (rect_rot[0].angle < -45.0) {
		rect_rot[0].angle	+= 90.0;
	}
	pattern.angle		= rect_rot[0].angle;
	pattern.center.x_pix	= rect_rot[0].center.x;
	pattern.center.y_pix	= rect_rot[0].center.y;

	/* Write pattern properties into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"pattern angle		= %lf DEG",
					-pattern.angle);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"pattern center (x)	= %i pix",
					pattern.center.x_pix);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"pattern center (y)	= %i pix",
					pattern.center.y_pix);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
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
	pattern.squares_n	= contours.size();
}

static	void	pattern_squares_pos_get	(void)
{
	int	i;

	/* Get position of each contour */
	for (i = 0; i < pattern.squares_n; i++) {
		proc_bounding_rect(&(contours[i]), &(rect[i]), true);

		pattern.square[i].pos.x_pix	= rect[i].x + rect[i].width/2.0;
		pattern.square[i].pos.y_pix	= rect[i].y + rect[i].height/2.0;
	}
}

static	void	pattern_squares_len_get	(void)
{
	int	i;

	proc_load_mem(4);

	/* Get pattern square side lenght in pixels */
	for (i = 0; i < pattern.squares_n; i++) {
		proc_pixel_value(pattern.square[i].pos.x_pix,
				pattern.square[i].pos.y_pix,
				&(pattern.square[i].len));
		pattern.square[i].len	*= 2;
	}
}

static	void	pattern_calib_mm_pix	(void)
{
	int	i;
	uint8_t	pattern_len [CONTOURS_MAX];
	uint8_t	median_size;

	for (i = 0; i < pattern.squares_n; i++) {
		pattern_len[i]	= pattern.square[i].len;
	}

	median_size	= alx_median_u8(pattern.squares_n, pattern_len);
	ratio_mm_pix	= PATTERN_SQUARE_LEN_MM / median_size;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"mm/pix	= %lf",
					ratio_mm_pix);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

/* process -------------------------------------------------------------------*/
static	void	lighter_bgr2gray	(void)
{
	proc_load_mem(0);

	proc_cvt_color(cv::COLOR_BGR2GRAY);
	proc_save_mem(1);
}

static	void	lighter_rotation_fix	(void)
{
	proc_load_mem(1);

	/* invert image before rotation to avoid black bands */
	proc_not();
	proc_rotate(pattern.center.x_pix, pattern.center.y_pix, pattern.angle);
	proc_save_mem(2);
}

static	void	lighter_dimensions_fix	(void)
{
	/* Need to correct also pattern dilatation (pattern_dimensions_get()) */
	proc_load_mem(2);

	proc_ROI(pattern.origin.x_pix+200, pattern.origin.y_pix+200,
					pattern.width-400, pattern.height-200);
	proc_save_mem(3);
}

static	void	lighter_segment_full	(void)
{
	proc_load_mem(3);

	proc_smooth(IMGI_SMOOTH_MEDIAN, 5);
	proc_threshold(cv::THRESH_BINARY, 80);
	proc_save_mem(4);
}

static	int	lighter_align		(void)
{
	int	status;

	proc_load_mem(4);

	proc_dilate_erode(2 / ratio_mm_pix);
	proc_erode_dilate(4.5 / ratio_mm_pix);
	proc_contours(&contours, &hierarchy);

	/* If no contour is found, error:  NOK_RESISTOR */
	if (contours.size() != 1) {
		status	= LIGHTER_NOK_LIGHTER;
		return	status;
	}

	proc_min_area_rect(&(contours[0]), &rect_rot[0], true);

	/* If width > height, it is taking into acount the incorrect side */
	if (rect_rot[0].size.width > rect_rot[0].size.height) {
		int	tmp;
		rect_rot[0].angle	+= 90.0;
		tmp			= rect_rot[0].size.width;
		rect_rot[0].size.width	= rect_rot[0].size.height;
		rect_rot[0].size.height	= tmp;
	}

	proc_rotate(rect_rot[0].center.x, rect_rot[0].center.y, rect_rot[0].angle);

	proc_save_mem(5);

	status	= LIGHTER_OK;
	return	status;
}

static	void	lighter_segment_base	(void)
{
	proc_load_mem(3);

	proc_rotate(rect_rot[0].center.x, rect_rot[0].center.y, rect_rot[0].angle);
	proc_smooth(IMGI_SMOOTH_MEDIAN, 5);
	proc_threshold(cv::THRESH_BINARY, 65);
	proc_erode_dilate(rect_rot[0].size.width * 0.43);
//	proc_dilate_erode(rect_rot[0].size.width * 0.2);

	proc_contours(&contours, &hierarchy);
	proc_bounding_rect(&(contours[0]), &(rect[0]), true);

	proc_save_mem(6);
}

static	void	lighter_crop		(void)
{
	int	x;
	int	y;
	int	w;
	int	h;

	proc_load_mem(3);

	proc_rotate(rect_rot[0].center.x, rect_rot[0].center.y, rect_rot[0].angle);
	w	= rect[0].width * 1.2;
	h	= rect[0].width * 0.9;
	x	= rect[0].x - rect[0].width * 0.1;
	y	= rect[0].y - h * 0.9;
	proc_ROI(x, y, w, h);
	proc_save_mem(7);
}

static	int	lighter_contours	(void)
{
	int	status;

	proc_load_mem(6);

	proc_contours(&contours, &hierarchy);

	/* If no contour is found, error:  NOK_LIGHTER */
	if (!contours.size()) {
		status	= LIGHTER_NOK_LIGHTER;
		return	status;
	}

	status	= LIGHTER_OK;
	return	status;
}

static	void	lighter_position_pix	(void)
{
	proc_fit_ellipse(&(contours[0]), &(rect_rot[0]), true);

	lighter.pos.x_pix	= rect_rot[0].center.x;
	lighter.pos.y_pix	= rect_rot[0].center.y;

	lighter.angle	= -rect_rot[0].angle + 90.0;
	if (lighter.angle < 0) {
		lighter.angle	+= 180.0;
	}
}

static	void	lighter_position_mm	(void)
{
	lighter.pos.x_mm	= ratio_mm_pix * lighter.pos.x_pix;
	lighter.pos.y_mm	= ratio_mm_pix * lighter.pos.y_pix;
}

static	void	lighter_size_pix	(void)
{
	double	area;
	double	perimeter;

	/* Get size */
	proc_contours_size(&contours, &area, &perimeter);
	lighter.area_pix2	= area;
	lighter.perimeter_pix	= perimeter;
}

static	void	lighter_size_mm	(void)
{
	/* Get size */
	lighter.area_mm2	= pow(ratio_mm_pix, 2) * lighter.area_pix2;
	lighter.perimeter_mm	= ratio_mm_pix * lighter.perimeter_pix;
}

static	void	lighter_shape		(void)
{
	/* Get shape */
	proc_min_area_rect(&(contours[0]), &(rect_rot[0]), true);

	lighter.ratio_p2_a	= pow(lighter.perimeter_pix, 2) /
						lighter.area_pix2;

	lighter.area_rect	= rect_rot[0].size.width *
						rect_rot[0].size.height;
	lighter.perimeter_rect	= 2.0 * (rect_rot[0].size.width +
						rect_rot[0].size.height);

	lighter.ratio_p_prect	= lighter.perimeter_pix /
						lighter.perimeter_rect;
	lighter.ratio_a_arect	= lighter.area_pix2 / lighter.area_rect;
}

static	void	lighter_log	(void)
{
	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
				"Lighter:");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
				"	pos:	(%.1lf, %.1lf) mm",
				lighter.pos.x_mm,
				lighter.pos.y_mm);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
				"	ang	= %.1lf DEG",
				lighter.angle);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
				"	A	= %.1lf mm2",
				lighter.area_mm2);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
				"	P	= %.1lf mm",
				lighter.perimeter_mm);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
				"	p2/A	= %.2lf",
				lighter.ratio_p2_a);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
				"	p/p_r	= %.2lf",
				lighter.ratio_p_prect);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write diameters into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
				"	A/A_r	= %.2lf",
				lighter.ratio_a_arect);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
