/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <cstddef>
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

	#include "proc_lighters.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
struct	Point {
	uint16_t	x;
	uint16_t	y;
};

struct	Size {
	uint16_t	h;
	uint16_t	w;
};

struct	Lighter_Properties {
	/* position */
	struct Point	pos;
	struct Size	size;
	double		angle;

	bool	fork;
	bool	valve;
	bool	hood;
	bool	wheel;
};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/

/* Static --------------------------------------------------------------------*/
static	class std::vector <class std::vector <class cv::Point_ <int>>>	contours_all;
static	class std::vector <class std::vector <class cv::Point_ <int>>>	contours_one;
static	class cv::Mat					hierarchy;
static	class cv::RotatedRect				rect_rot [CONTOURS_MAX];
static	class cv::Rect_ <int>				rect;
static	struct Lighter_Properties			lighter [CONTOURS_MAX];
static	unsigned					lighters_n;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_lighter		(int status);

static	void	lighters_bgr2gray	(void);
static	void	lighters_segment_full	(void);
static	int	lighters_find		(void);
static	void	lighter_segment_body	(int i);
static	void	lighter_crop_head	(int i);
static	void	lighter_segment_head	(int i);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_lighter		(void)
{
	int	status;
	int	i;

	proc_save_mem(0);
	/* Segment lighters */
	{
		/* Measure time */
		clock_start();

		lighters_bgr2gray();
		lighters_segment_full();
		lighters_find();

		/* Measure time */
		clock_stop("Segment lighten");
	}
	for (i = 0; (unsigned)i < lighters_n; i++) {
		/* Segment lighter */
		{
			/* Measure time */
			clock_start();

			lighter_segment_body(i);

			/* Measure time */
			clock_stop("Segment lighten");
		}
		/* Segment interest zone */
		{
			/* Measure time */
			clock_start();

			lighter_crop_head(i);
			lighter_segment_head(i);

			/* Measure time */
			clock_stop("Segment interest zone");
		}
	}

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

/* process -------------------------------------------------------------------*/
static	void	lighters_bgr2gray	(void)
{
	proc_load_mem(0);

	proc_cvt_color(cv::COLOR_BGR2GRAY);
	proc_not();
	proc_border(100);
	proc_save_mem(1);
}

static	void	lighters_segment_full	(void)
{
	proc_load_mem(1);

	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_erode_dilate(1);
	proc_dilate_erode(1);
	proc_save_mem(2);
}

static	int	lighters_find		(void)
{
	int	status;
	int	i;
	int	tmp;

	proc_load_mem(2);

	proc_contours(&contours_all, &hierarchy);
	lighters_n	= contours_all.size();

	/* If no contour is found, error:  NOK_RESISTOR */
	if (!lighters_n) {
		status	= LIGHTER_NOK_LIGHTER;
		return	status;
	}

	for (i = 0; (unsigned)i < lighters_n; i++) {
		proc_min_area_rect(&(contours_all[i]), &rect_rot[i], true);

		/* If width > height, it is taking into acount the incorrect side */
		if (rect_rot[i].size.width > rect_rot[i].size.height) {
			rect_rot[i].angle	+= 90.0;
			tmp			= rect_rot[i].size.width;
			rect_rot[i].size.width	= rect_rot[i].size.height;
			rect_rot[i].size.height	= tmp;
		}

		lighter[i].pos.x	= rect_rot[i].center.x;
		lighter[i].pos.y	= rect_rot[i].center.y;
		lighter[i].angle	= rect_rot[i].angle;
		lighter[i].size.h	= rect_rot[i].size.height;
		lighter[i].size.w	= rect_rot[i].size.width;
	}

	status	= LIGHTER_OK;
	return	status;
}

static	void	lighter_segment_body	(int i)
{
	int	x;
	int	y;
	int	w;
	int	h;

	proc_load_mem(1);

	proc_rotate(lighter[i].pos.x, lighter[i].pos.y, lighter[i].angle);
	w	= lighter[i].size.w * 1.25;
	h	= lighter[i].size.h * 1.25;
	x	= lighter[i].pos.x - (w / 2.0);
	y	= lighter[i].pos.y - (h / 2.0);
	proc_ROI(x, y, w, h);
	proc_threshold(cv::THRESH_BINARY, 255 - 165);

	proc_erode_dilate(1);
	proc_dilate_erode(1);
	proc_erode_dilate(lighter[i].size.w * 0.43);
	proc_save_mem(3);

	proc_contours(&contours_one, &hierarchy);
	proc_bounding_rect(&(contours_one[0]), &rect, true);
}

static	void	lighter_crop_head	(int i)
{
	int	x;
	int	y;
	int	w;
	int	h;

	proc_load_mem(3);
	proc_not();
	proc_erode(1);
	proc_save_ref();

	proc_load_mem(1);
	proc_rotate(lighter[i].pos.x, lighter[i].pos.y, lighter[i].angle);
	w	= lighter[i].size.w * 1.25;
	h	= lighter[i].size.h * 1.25;
	x	= lighter[i].pos.x - (w / 2.0);
	y	= lighter[i].pos.y - (h / 2.0);
	proc_ROI(x, y, w, h);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_and_2ref();
//
	w	= rect.width * 1.15;
	h	= rect.width * 0.65;
	x	= rect.x - rect.width * 0.075;
	y	= rect.y - rect.width * 0.65;
	proc_ROI(x, y, w, h);
	rect.x		= x;
	rect.y		= y;
	rect.width	= w;
	rect.height	= h;
	proc_save_mem(8);

proc_apply();
proc_save_file();
}

static	void	lighter_segment_head	(int i)
{
	proc_load_mem(8);
/*
	proc_smooth(IMGI_SMOOTH_MEDIAN, 5);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_dilate_erode(5);
	proc_erode_dilate(rect[0].height * 0.25);

	proc_contours(&contours, &hierarchy);
	proc_bounding_rect(&(contours[0]), &(rect[0]), true);
*/
	proc_save_mem(9);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
