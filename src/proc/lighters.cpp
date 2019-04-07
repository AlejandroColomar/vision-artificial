/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/proc/lighters.hpp"

#include <cstddef>
#include <cstdio>

#include <opencv2/opencv.hpp>

#include "libalx/base/stdlib/swap.hpp"

#include "vision-artificial/image/iface.hpp"
#include "vision-artificial/proc/common.hpp"
#include "vision-artificial/user/iface.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
enum	Lighter_Hood {
	HOOD_OK,
	HOOD_NOT_PRESENT,
	HOOD_NOT_OK
};


/******************************************************************************
 ******* structs / unions *****************************************************
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
	struct Point	pos;
	struct Size	size;
	double		angle;

	bool	fork;
	bool	valve;
	int	hood_;
	bool	hood;
	bool	wheel;

	bool	ok;
};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	class std::vector <class std::vector <class cv::Point_ <int>>>	contours_all;
static	class std::vector <class std::vector <class cv::Point_ <int>>>	contours_one;
static	class cv::Mat					hierarchy;
static	class cv::RotatedRect				rect_rot[CONTOURS_MAX];
static	class cv::Rect_ <int>				rect;
static	struct Lighter_Properties			lighter[CONTOURS_MAX];
static	ptrdiff_t					lighters_n;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	result_lighter		(int status);

static	void	lighters_bgr2gray	(void);
static	void	lighters_segment_full	(void);
static	int	lighters_find		(void);
static	void	lighter_segment_body	(int i);
static	void	lighter_rm_body		(int i);
static	void	lighter_crop_head	(int i);
static	void	lighter_chk_hood	(int i);
static	void	lighter_rm_hood		(void);
static	void	lighter_chk_fork	(int i);
static	void	lighter_chk_wheel	(int i);
static	void	lighter_chk_valve	(int i);
static	void	lighters_log		(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_lighter		(void)
{

	proc_save_mem(0);
	/* Segment lighters */
	clock_start();
	lighters_bgr2gray();
	lighters_segment_full();
	lighters_find();
	clock_stop("Segment lighters");

	/* Check each lighter */
	for (ptrdiff_t i = 0; i < lighters_n; i++) {
		clock_start();
		lighter[i].ok	= true;
		lighter_segment_body(i);
		lighter_rm_body(i);
		lighter_crop_head(i);
		lighter_chk_hood(i);
		if (lighter[i].hood_ != HOOD_NOT_PRESENT) {
			lighter_rm_hood();
		} else {
			proc_load_mem(7);
			proc_save_mem(8);
		}
		lighter_chk_fork(i);
		lighter_chk_wheel(i);
		lighter_chk_valve(i);
		clock_stop("Check parts");
	}

	/* Print results of lighters into log */
	clock_start();
	lighters_log();
	clock_stop("Lighters properties (log)");

	result_lighter(LIGHTER_OK);
	return	0;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	result_lighter		(int status)
{

	switch (status) {
	case LIGHTER_OK:
		user_iface_log_write(0, "Coin:  OK");
		break;
	case LIGHTER_NOK_LIGHTER:
		user_iface_log_write(0, "Coin:  NOK_LIGHTER");
		break;

	case LIGHTER_NOK_SIZE:
		user_iface_log_write(0, "Coin:  NOK_SIZE");
		break;
	default:
		user_iface_log_write(0, "Coin:  NOK");
	}
}

/* process -------------------------------------------------------------------*/
static	void	lighters_bgr2gray	(void)
{

	proc_load_mem(0);

	proc_cvt_color(cv::COLOR_BGR2GRAY);
	proc_not();
	proc_save_mem(1);

	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_border(100);
	proc_save_mem(3);

	proc_load_mem(1);
	proc_border(100);
	proc_save_mem(2);
}

static	void	lighters_segment_full	(void)
{

	proc_load_mem(3);
	proc_erode_dilate(1);
	proc_dilate_erode(2);
	proc_save_mem(4);
}

static	int	lighters_find		(void)
{

	proc_load_mem(4);
	proc_contours(&contours_all, &hierarchy);
	lighters_n	= contours_all.size();
	if (!lighters_n)
		return	LIGHTER_NOK_LIGHTER;

	for (ptrdiff_t i = 0; i < lighters_n; i++) {
		proc_min_area_rect(&contours_all[i], &rect_rot[i], true);
		/* If width > height,
		 * it is taking into acount the incorrect side */
		if (rect_rot[i].size.width > rect_rot[i].size.height) {
			rect_rot[i].angle	+= 90.0;
			ALX_SWAP(&rect_rot[i].size.width,
						&rect_rot[i].size.height);
		}
		lighter[i].pos.x	= rect_rot[i].center.x;
		lighter[i].pos.y	= rect_rot[i].center.y;
		lighter[i].angle	= -rect_rot[i].angle;
		lighter[i].size.h	= rect_rot[i].size.height;
		lighter[i].size.w	= rect_rot[i].size.width;
	}

	return	0;
}

static	void	lighter_segment_body	(int i)
{
	int	x, y;
	int	w, h;

	proc_load_mem(2);

	proc_rotate(lighter[i].pos.x, lighter[i].pos.y, -lighter[i].angle);
	w	= lighter[i].size.w * 1.25;
	h	= lighter[i].size.h * 1.25;
	x	= lighter[i].pos.x - (w / 2.0);
	y	= lighter[i].pos.y - (h / 2.0);
	proc_ROI(x, y, w, h);
	proc_threshold(cv::THRESH_BINARY, UINT8_MAX - 165);

	proc_erode_dilate(1);
	proc_dilate_erode(1);
	proc_erode_dilate(lighter[i].size.w * 0.43);
	proc_save_mem(5);

	proc_contours(&contours_one, &hierarchy);
	proc_bounding_rect(&contours_one[0], &rect, true);
}

static	void	lighter_rm_body		(int i)
{
	int	x, y;
	int	w, h;

	proc_load_mem(5);
	proc_not();
	proc_erode(5);
	proc_save_ref();

	proc_load_mem(3);
	proc_rotate(lighter[i].pos.x, lighter[i].pos.y, -lighter[i].angle);
	w	= lighter[i].size.w * 1.25;
	h	= lighter[i].size.h * 1.25;
	x	= lighter[i].pos.x - (w / 2.0);
	y	= lighter[i].pos.y - (h / 2.0);
	proc_ROI(x, y, w, h);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_and_2ref();
	proc_save_mem(6);
}

static	void	lighter_crop_head	(int i)
{
	int	x, y;
	int	w, h;

	proc_load_mem(6);

	w	= rect.width * 1.15;
	h	= rect.width * 0.65;
	x	= rect.x - rect.width * 0.075;
	y	= rect.y - rect.width * 0.65;
	proc_ROI(x, y, w, h);
	rect.x		= x;
	rect.y		= y;
	rect.width	= w;
	rect.height	= h;
	lighter[i].size.w	= w;
	lighter[i].size.h	= h;
	proc_save_mem(7);
}

static	void	lighter_chk_hood	(int i)
{
	uint8_t	val1;
	uint8_t	val2;
	proc_load_mem(7);

	proc_pixel_get(rect.width * 0.2, rect.height * 0.3, &val1);
	proc_pixel_set(rect.width * 0.2, rect.height * 0.3, UINT8_MAX / 2);
	proc_save_mem(9);

	if (val1) {
		proc_pixel_get(rect.width *0.2, rect.height *0.8, &val2);
		proc_pixel_set(rect.width *0.2, rect.height *0.8, UINT8_MAX /2);
		if (val2) {
			lighter[i].hood_	= HOOD_OK;
			lighter[i].hood		= true;
		} else {
			lighter[i].hood_	= HOOD_NOT_OK;
			lighter[i].hood		= false;
			lighter[i].ok		= false;
		}
	} else {
		lighter[i].hood_	= HOOD_NOT_PRESENT;
		lighter[i].hood		= false;
		lighter[i].ok		= false;
	}
}

static	void	lighter_rm_hood		(void)
{

	proc_load_mem(7);
	proc_border(2);
	proc_save_ref();

	proc_dilate_erode(2);
	proc_erode_dilate(rect.height * 0.25);
	proc_dilate(2);
	proc_not();
	proc_and_2ref();
	proc_save_mem(8);
}

static	void	lighter_chk_fork	(int i)
{
	uint8_t	val;

	proc_load_mem(8);

	proc_dilate_erode(2);
	proc_pixel_get(rect.width * 0.8, rect.height * 0.8, &val);
	proc_pixel_set(rect.width * 0.8, rect.height * 0.8, UINT8_MAX / 2);
	proc_save_mem(10);

	if (val) {
		lighter[i].fork	= true;
	} else {
		lighter[i].fork	= false;
		lighter[i].ok	= false;
	}
}

static	void	lighter_chk_wheel	(int i)
{
	uint8_t	val;

	proc_load_mem(8);

	proc_dilate(5);
	proc_pixel_get(rect.width * 0.58, rect.height * 0.19, &val);
	proc_pixel_set(rect.width * 0.58, rect.height * 0.19, UINT8_MAX / 2);
	proc_save_mem(11);

	if (val) {
		lighter[i].wheel	= true;
	} else {
		lighter[i].wheel	= false;
		lighter[i].ok		= false;
	}
}

static	void	lighter_chk_valve	(int i)
{
	uint8_t	val;

	proc_load_mem(8);

	if (lighter[i].hood_ == HOOD_NOT_PRESENT)
		proc_dilate(5);
	else
		proc_dilate(10);
	proc_pixel_get(rect.width * 0.07, rect.height * 0.92, &val);
	proc_pixel_set(rect.width * 0.07, rect.height * 0.92, UINT8_MAX / 2);
	proc_save_mem(12);

	if (val) {
		lighter[i].valve	= true;
	} else {
		lighter[i].valve	= false;
		lighter[i].ok		= false;
	}
}

static	void	lighters_log		(void)
{
	char	txt[LOG_LINE_LEN];

	for (ptrdiff_t i = 0; i < lighters_n; i++) {
		snprintf(txt, LOG_LINE_LEN, "Lighter[%ti]:", i);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	pos:	(%i, %i) pix",
					lighter[i].pos.x, lighter[i].pos.y);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	ang	= %.1lf DEG",
					lighter[i].angle);
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	hood	= %s",
					lighter[i].hood ? "ok" : "nok");
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	fork	= %s",
					lighter[i].fork ? "ok" : "nok");
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	wheel	= %s",
					lighter[i].wheel ? "ok" : "nok");
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	valve	= %s",
					lighter[i].valve ? "ok" : "nok");
		user_iface_log_write(0, txt);
		snprintf(txt, LOG_LINE_LEN, "	RESULT	= %s",
					lighter[i].ok ? "OK" : "NOK");
		user_iface_log_write(0, txt);
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
