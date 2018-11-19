/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* pow() */
	#include <cmath>
		/* snprintf() & fflush() */
	#include <cstdio>

/* Packages ------------------------------------------------------------------*/
		/* openCV */
	#include <opencv2/opencv.hpp>

/* Project -------------------------------------------------------------------*/
		/* CONSTANTS */
	#include "img_iface.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
	#include "proc_common.hpp"

	#include "proc_resistor.hpp"


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
struct	Resistor_Bands {
	/* position */
	int		x;
	int		y;

	/* value */
	unsigned char	h;
	unsigned char	s;
	unsigned char	v;
};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/

/* Static --------------------------------------------------------------------*/
static	std::vector <std::vector <cv::Point_ <int>>>	contours;
static	class cv::Mat					hierarchy;
static	class cv::RotatedRect				rect_rot;
static	class cv::Rect_ <int>				rect;
static	int						bkgd;
static	int						bands_n;
static	struct Resistor_Bands				bands [5];
static	char						code [6];
static	int						base;
static	float						resistance;
static	int						tolerance;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_resistor		(int status);

static	int	resistor_find		(void);
static	void	resistor_align		(void);
static	void	resistor_dimensions_0	(void);
static	void	resistor_crop_0		(void);
static	void	resistor_bkgd		(void);
static	void	resistor_dimensions_1	(void);
static	void	resistor_crop_1		(void);
static	void	separate_bkgd_bands_h	(void);
static	void	separate_bkgd_bands_s	(void);
static	void	separate_bkgd_bands_v	(void);
static	void	bkgd_find		(void);
static	int	bands_find		(void);
static	void	bands_colors		(void);
static	void	bands_code		(void);
static	char	band_hsv2code		(struct Resistor_Bands  *band);
static	void	resistor_value		(void);
static	int	resistor_tolerance	(void);
static	int	chk_std_value		(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
static	int	proc_resistor_		(void)
{
	int	status;


	proc_save_mem(0);
	/* Find resistor (position and angle) */
	{
		/* Measure time */
		clock_start();

		status	= resistor_find();
		if (status) {
			result_resistor(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find resistor");
	}
	/* Align resistor, find its dimensions, and crop */
	{
		/* Measure time */
		clock_start();

		resistor_align();
		resistor_dimensions_0();
		resistor_crop_0();

		/* Measure time */
		clock_stop("Align, dimensions, & crop");
	}
	/* Find backgroung color */
	{
		/* Measure time */
		clock_start();

		resistor_bkgd();

		/* Measure time */
		clock_stop("Background color");
	}
	/* Crop more */
	{
		/* Measure time */
		clock_start();

		resistor_dimensions_1();
		resistor_crop_1();

		/* Measure time */
		clock_stop("Crop more");
	}
	/* Separate background (BK) and lines (WH) */
	{
		/* Measure time */
		clock_start();

		separate_bkgd_bands_h();
		separate_bkgd_bands_s();
		separate_bkgd_bands_v();
		bkgd_find();

		/* Measure time */
		clock_stop("Separate bkgd from bands");
	}
	/* Find bands:  contours -> rectangles */
	{
		/* Measure time */
		clock_start();

		status	= bands_find();
		if (status) {
			result_resistor(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find bands");
	}
	/* Read values on the center of each band */
	{
		/* Measure time */
		clock_start();

		bands_colors();

		/* Measure time */
		clock_stop("Bands' colors");
	}
	/* Interpret colors */
	{
		/* Measure time */
		clock_start();

		bands_code();

		/* Measure time */
		clock_stop("Interpret colors");
	}
	/* Calculate resistor value & tolerance */
	{
		/* Measure time */
		clock_start();

		resistor_value();
		status	= resistor_tolerance();
		if (status) {
			result_resistor(status);
			return	status;
		}

		/* Write resistor value into log */
		if (bands_n != 1) {
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistance:	%.2E ± %i% Ohm",
						resistance, tolerance);
		} else {
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistance:	0 Ohm");
		}
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		/* Measure time */
		clock_stop("Calculate resistance & tolerance");
	}
	/* Check STD value */
	{
		/* Measure time */
		clock_start();

		status	= chk_std_value();
		if (status) {
			result_resistor(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Chk STD values");
	}

	status	= RESISTOR_OK;
	result_resistor(status);
	return	status;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_resistor		(int status)
{
	/* Cleanup */

	/* Write result into log */
	char	result [LOG_LINE_LEN];
	switch (status) {
	case RESISTOR_OK:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistor:  OK");
		break;
	case RESISTOR_NOK_RESISTOR:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistor:  NOK_RESISTOR");
		break;
	case RESISTOR_NOK_BANDS:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistor:  NOK_BANDS");
		break;
	case RESISTOR_NOK_STD_VALUE:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistor:  NOK_STD_VALUE");
		break;
	case RESISTOR_NOK_TOLERANCE:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistor:  NOK_TOLERANCE");
		break;
	default:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistor:  NOK");
		break;
	}
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

int	proc_resistor		(void)
{
	int	status;

	proc_save_mem(0);
	/* Find resistor (position and angle) */
	{
		/* Measure time */
		clock_start();

		status	= resistor_find();
		if (status) {
			result_resistor(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find resistor");
	}
	/* Align resistor, find its dimensions, and crop */
	{
		/* Measure time */
		clock_start();

		resistor_align();
		resistor_dimensions_0();
		resistor_crop_0();

		/* Measure time */
		clock_stop("Align, dimensions, & crop");
	}
	/* Find backgroung color */
	{
		/* Measure time */
		clock_start();

		resistor_bkgd();

		/* Measure time */
		clock_stop("Background color");
	}
	/* Crop more */
	{
		/* Measure time */
		clock_start();

		resistor_dimensions_1();
		resistor_crop_1();

		/* Measure time */
		clock_stop("Crop more");
	}
	/* Separate background (BK) and bands (WH) */
	{
		/* Measure time */
		clock_start();

		separate_bkgd_bands_h();
		separate_bkgd_bands_s();
#if 0
		separate_bkgd_bands_v();
		bkgd_find();

		/* Measure time */
		clock_stop("Separate bkgd from bands");
	}
	/* Find bands:  contours -> rectangles */
	{
		/* Measure time */
		clock_start();

		status	= bands_find();
		if (status) {
			result_resistor(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find bands");
	}
	/* Read values on the center of each band */
	{
		/* Measure time */
		clock_start();

		bands_colors();

		/* Measure time */
		clock_stop("Bands' colors");
	}
	/* Interpret colors */
	{
		/* Measure time */
		clock_start();

		bands_code();

		/* Measure time */
		clock_stop("Interpret colors");
	}
	/* Calculate resistor value & tolerance */
	{
		/* Measure time */
		clock_start();

		resistor_value();
		status	= resistor_tolerance();
		if (status) {
			result_resistor(status);
			return	status;
		}

		/* Write resistor value into log */
		if (bands_n != 1) {
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistance:	%.2E ± %i% Ohm",
						resistance, tolerance);
		} else {
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistance:	0 Ohm");
		}
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		/* Measure time */
		clock_stop("Calculate resistance & tolerance");
	}
	/* Check STD value */
	{
		/* Measure time */
		clock_start();

		status	= chk_std_value();
		if (status) {
			result_resistor(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Chk STD values");
#endif
	}

	proc_apply();
	proc_save_file();

	status	= -1;
	result_resistor(status);
	return	status;
}

static	int	resistor_find		(void)
{
	int	status;

	proc_load_mem(0);

	/* BGR -> HSV */
	proc_cvt_color(cv::COLOR_BGR2HSV);
	proc_save_mem(19);

	proc_cmp(IMG_IFACE_CMP_SATURATION);
	proc_smooth(IMGI_SMOOTH_MEDIAN, 7);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_save_mem(1);
	proc_dilate_erode(10);
	proc_contours(&contours, &hierarchy);

	/* If no contour is found, error:  NOK_RESISTOR */
	if (!contours.size()) {
		status	= RESISTOR_NOK_RESISTOR;
		return	status;
	}

	proc_min_area_rect(&(contours[0]), &rect_rot, true);

	/* If angle is < -45º, it is taking into acount the incorrect side */
	if (rect_rot.angle < -45.0) {
		rect_rot.angle	+= 90.0;
	}

	status	= RESISTOR_OK;
	return	status;
}

static	void	resistor_align		(void)
{
	proc_load_mem(19);
	proc_rotate(&rect_rot);
	proc_save_mem(2);
	proc_load_mem(1);
	proc_rotate(&rect_rot);
	proc_save_mem(3);
}

static	void	resistor_dimensions_0	(void)
{
	proc_load_mem(3);

	proc_contours(&contours, &hierarchy);
	proc_bounding_rect(&(contours[0]), &rect, true);
}

static	void	resistor_crop_0		(void)
{
	proc_load_mem(2);

	int	x;
	int	y;
	int	w;
	int	h;
	w	= rect.width;
	h	= rect.height;
	x	= rect.x;
	y	= rect.y;
	proc_ROI(x, y, w, h);
	proc_save_mem(4);
}

static	void	resistor_bkgd		(void)
{
	/* hue */
	uint8_t	bkgd_hue;
	proc_load_mem(4);
	proc_cmp(IMG_IFACE_CMP_HUE);
	proc_median_vertical();
	proc_median_horizontal();
	proc_pixel_value(0, 0, &bkgd_hue);

	/* saturation */
	uint8_t	bkgd_sat;
	proc_load_mem(4);
	proc_cmp(IMG_IFACE_CMP_SATURATION);
	proc_median_vertical();
	proc_median_horizontal();
	proc_pixel_value(0, 0, &bkgd_sat);

	if (bkgd_hue < 50) {
		/* Beige */
		bkgd	= 0;
	} else {
		/* Blue */
		if ((bkgd_hue <= 90)  ||  (bkgd_sat <= 140)) {
			/* Teal blue */
			bkgd	= 1;
		} else if (bkgd_hue >= 105) {
			/* Dark blue */
			bkgd	= 2;
		} else {
			/* Normal blue */
			bkgd	= 3;
		}
	}
}

static	void	resistor_dimensions_1	(void)
{
	proc_load_mem(3);

	proc_dilate_erode(10);
	proc_erode_dilate(rect.height / 1.9 - 9);
	proc_contours(&contours, &hierarchy);
	proc_bounding_rect(&(contours[0]), &rect, true);
}

static	void	resistor_crop_1		(void)
{
	proc_load_mem(2);

	int	x;
	int	y;
	int	w;
	int	h;
	w	= rect.width * 0.9;
	h	= rect.height * 0.9;
	x	= rect.x + w * (1.0 - 0.9) / 2.0;
	y	= rect.y + h * (1.0 - 0.9) / 2.0;
	proc_ROI(x, y, w, h);
	proc_save_mem(4);
}

static	void	separate_bkgd_bands_h	(void)
{
	proc_load_mem(4);

	proc_cmp(IMG_IFACE_CMP_HUE);
	proc_median_vertical();
	proc_save_mem(9);

	switch (bkgd) {
	case 0:
		proc_threshold(cv::THRESH_TOZERO_INV, 20);
		proc_threshold(cv::THRESH_TOZERO, 5);
		break;
	case 1:
		proc_threshold(cv::THRESH_TOZERO_INV, 100);
		proc_threshold(cv::THRESH_TOZERO, 70);
		break;
	case 2:
		proc_threshold(cv::THRESH_TOZERO_INV, 115);
		proc_threshold(cv::THRESH_TOZERO, 100);
		break;
	case 3:
		proc_threshold(cv::THRESH_TOZERO_INV, 110);
		proc_threshold(cv::THRESH_TOZERO, 90);
		break;
	}
	proc_threshold(cv::THRESH_BINARY_INV, 1);
	proc_save_mem(5);
}

static	void	separate_bkgd_bands_s	(void)
{
	proc_load_mem(4);

	proc_cmp(IMG_IFACE_CMP_SATURATION);
	proc_median_vertical();
	proc_save_mem(10);

	switch (bkgd) {

	case 0:
		proc_threshold(cv::THRESH_TOZERO, 100);
		break;
	case 1:
		proc_threshold(cv::THRESH_TOZERO, 95);
		break;
	case 2:
		proc_threshold(cv::THRESH_TOZERO, 200);
		break;
	case 3:
		proc_threshold(cv::THRESH_TOZERO, 130);
		break;
	}
	proc_threshold(cv::THRESH_BINARY_INV, 1);
	proc_save_mem(6);
}

static	void	separate_bkgd_bands_v	(void)
{
	proc_load_mem(4);

	proc_cmp(IMG_IFACE_CMP_VALUE);
	proc_median_vertical();
	proc_save_mem(11);

	switch (bkgd) {
	case 0:
		proc_threshold(cv::THRESH_TOZERO_INV, 130);
		proc_threshold(cv::THRESH_TOZERO, 65);
		break;

	case 1:
		proc_threshold(cv::THRESH_TOZERO_INV, 155);
		proc_threshold(cv::THRESH_TOZERO, 60);
		break;
	case 2:

		proc_threshold(cv::THRESH_TOZERO_INV, 140);
		proc_threshold(cv::THRESH_TOZERO, 60);
		break;
	case 3:
		proc_threshold(cv::THRESH_TOZERO_INV, 165);
		proc_threshold(cv::THRESH_TOZERO, 45);
		break;
	}
	proc_threshold(cv::THRESH_BINARY_INV, 1);
	proc_save_mem(7);
}

static	void	bkgd_find		(void)
{
	/* Merge the components:  H | S | V */
	proc_load_mem(7);
	proc_save_ref();
	proc_load_mem(6);
	proc_or_2ref();
	proc_save_ref();
	proc_load_mem(5);
	proc_or_2ref();
	proc_dilate_erode(1);
	proc_save_mem(8);
}

static	int	bands_find		(void)
{
	int	status;

	proc_load_mem(8);

	/* Contours */
	proc_contours(&contours, &hierarchy);

	bands_n	= contours.size();
	if ((bands_n == 0)  ||  (bands_n == 2)  ||  (bands_n > 5)) {
		status	= RESISTOR_NOK_BANDS;
		return	status;
	}

	/* Band 0 (hundreds) */
	if (bands_n == 5) {
		proc_bounding_rect(&(contours[4]), &rect, true);
		bands[0].x	= rect.x + rect.width / 2.0;
		bands[0].y	= rect.y + rect.height / 2.0;
	}

	/* Band 1 (tens) */
	if (bands_n >= 3) {
		if (bands_n > 3) {
			proc_bounding_rect(&(contours[3]), &rect, true);
		} else {
			proc_bounding_rect(&(contours[2]), &rect, true);
		}
		bands[1].x	= rect.x + rect.width / 2.0;
		bands[1].y	= rect.y + rect.height / 2.0;
	}

	/* Band 2 (units) */
	if (bands_n > 3) {
		proc_bounding_rect(&(contours[2]), &rect, true);
	} else if (bands_n == 3) {
		proc_bounding_rect(&(contours[1]), &rect, true);
	} else {	/* bands_n == 1 */
		proc_bounding_rect(&(contours[0]), &rect, true);
	}
	bands[2].x	= rect.x + rect.width / 2.0;
	bands[2].y	= rect.y + rect.height / 2.0;

	/* Band 3 (multiplier) */
	if (bands_n >= 3) {
		if (bands_n > 3) {
			proc_bounding_rect(&(contours[1]), &rect, true);
		} else {
			proc_bounding_rect(&(contours[0]), &rect, true);
		}
		bands[3].x	= rect.x + rect.width / 2.0;
		bands[3].y	= rect.y + rect.height / 2.0;
	}

	/* Band 4 (tolerance) */
	if (bands_n > 3) {
		proc_bounding_rect(&(contours[0]), &rect, true);
		bands[4].x	= rect.x + rect.width / 2.0;
		bands[4].y	= rect.y + rect.height / 2.0;
	}
}

static	void	bands_colors		(void)
{
	/* Hue */
	proc_load_mem(9);
	if (bands_n == 5) {
		proc_pixel_value(bands[0].x, bands[0].y, &(bands[0].h));
	}
	if (bands_n >= 3) {
		proc_pixel_value(bands[1].x, bands[1].y, &(bands[1].h));
	}
	proc_pixel_value(bands[2].x, bands[2].y, &(bands[2].h));
	if (bands_n >= 3) {
		proc_pixel_value(bands[3].x, bands[3].y, &(bands[3].h));
	}
	if (bands_n > 3) {
		proc_pixel_value(bands[4].x, bands[4].y, &(bands[4].h));
	}

	/* Saturation */
	proc_load_mem(10);
	if (bands_n == 5) {
		proc_pixel_value(bands[0].x, bands[0].y, &(bands[0].s));
	}
	if (bands_n >= 3) {
		proc_pixel_value(bands[1].x, bands[1].y, &(bands[1].s));
	}
	proc_pixel_value(bands[2].x, bands[2].y, &(bands[2].s));
	if (bands_n >= 3) {
		proc_pixel_value(bands[3].x, bands[3].y, &(bands[3].s));
	}
	if (bands_n > 3) {
		proc_pixel_value(bands[4].x, bands[4].y, &(bands[4].s));
	}

	/* Value */
	proc_load_mem(11);
	if (bands_n == 5) {
		proc_pixel_value(bands[0].x, bands[0].y, &(bands[0].v));
	}
	if (bands_n >= 3) {
		proc_pixel_value(bands[1].x, bands[1].y, &(bands[1].v));
	}
	proc_pixel_value(bands[2].x, bands[2].y, &(bands[2].v));
	if (bands_n >= 3) {
		proc_pixel_value(bands[3].x, bands[3].y, &(bands[3].v));
	}
	if (bands_n > 3) {
		proc_pixel_value(bands[4].x, bands[4].y, &(bands[4].v));
	}
}

static	void	bands_code		(void)
{
	/* Init to 0 */
	int	i;
	for (i = 0; i < 6; i++) {
		code[i]	= '\0';
	}

	/* Band 0 (hundreds) */
	if (bands_n == 5) {
		code[0]	= band_hsv2code(&bands[0]);
	} else {
		code[0]	= 'n';
	}


	/* Band 1 (tens) */
	if (bands_n != 1) {
		code[1]	= band_hsv2code(&bands[1]);
	} else {
		code[1]	= 'n';
	}

	/* Band 2 (units) */
	code[2]	= band_hsv2code(&bands[2]);

	/* Band 3 (multiplier) */
	if (bands_n != 1) {
		code[3]	= band_hsv2code(&bands[3]);
	} else {
		code[3]	= 'n';
	}

	/* Band 4 (tolerance) */
	if (bands_n > 3) {
		code[4]	= band_hsv2code(&bands[4]);
	} else {
		code[4]	= 'n';
	}

	/* Write bands' code into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Code:		\"%s\"",
						code);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

static	char	band_hsv2code		(struct Resistor_Bands  *band)
{
	char	ch;

	if (band->v < 24) {
		ch	= '0';
	} else if (band->v < 28) {
		if ((band->h < 90)  ||  (band->h > 135)) {
			ch	= '1';
		} else if (band->s < 78) {
			ch	= '1';
		} else {
			ch	= '0';
		}
	} else if (band->v < 50) {
		if ((band->h > 120)  ||  (band->h < 10)) {
			ch	= '1';
		} else if (band->h < 85) {
			ch	= '5';
		} else {
			ch	= '8';
		}
	} else if (band->v < 70) {
		if (band->h < 90) {
			ch	= '3';
		} else {
			ch	= '2';
		}
	} else if (band->v < 100) {
		if (band->h < 10) {
			ch	= '3';
		} else if (band->h < 40) {
			ch	= 'g';
		} else if (band->h < 85) {
			ch	= '4';
		} else if (band->h < 140) {
			ch	= '6';
		} else {
			ch	= '2';
		}
	} else {
		if (band->h < 45) {
			ch	= '3';
		} else if (band->h < 105) {
			ch	= '9';
		} else if (band->h < 140) {
			ch	= '7';
		} else {
			ch	= '2';
		}
	}

	return	ch;
}

static	void	resistor_value		(void)
{
	/* Base value */
	base	= code[2] - '0';
	if (code[1] != 'n') {
		base	+= (code[1] - '0') * 10;
	}
	if (code[0] != 'n') {
		base	+= (code[0] - '0') * 100;
	}

	/* Calculate resistance */
	int	power;
	if ((code[3] > '0')  &&  (code[3] < '9')) {
		power	= code[3] - '0';
	} else if (code[3] == 'g') {
		power	= -1;
	} else if (code[3] == 's') {
		power	= -2;
	}
	resistance	= base * pow(10, power);
}

static	int	resistor_tolerance	(void)
{
	int	status;

	if (bands_n != 1) {
		switch (code[4]) {
		case '1':
			tolerance	= 1;
			break;
		case '2':
			tolerance	= 2;
			break;
		case '4':
		case 'g':
		case 'n':
			tolerance	= 5;
			break;
		case '8':
			tolerance	= 10;
			break;
		default:
			status	= RESISTOR_NOK_TOLERANCE;
			return	status;
		}
	}

	status	= RESISTOR_OK;
	return	status;
}

static	int	chk_std_value		(void)
{
	int	status;

	/* Check that base value is a standard value */
	int	std_values_10 [12]	= {10,12,15,18,22,27,33,39,47,56,68,82};
	int	std_values_5 [12]	= {11,13,16,20,24,30,36,43,51,62,75,91};

	bool	std_value_nok;
	std_value_nok	= true;

	int	i;
	if (bands_n != 1) {
		for (i = 0; i < 12; i++) {
			if (base == std_values_10[i]) {
				std_value_nok	= false;
			}
			if (base == (std_values_10[i] * 10)) {
				std_value_nok	= false;
			}
		}
		if (tolerance <= 5) {
			for (i = 0; i < 12; i++) {
				if (base == std_values_5[i]) {
					std_value_nok	= false;
				}
				if (base == (std_values_5[i] * 10)) {
					std_value_nok	= false;
				}
			}
		}
	} else {
		if (base == 0) {
			std_value_nok	= false;
		}
	}

	if (std_value_nok) {
		status	= RESISTOR_NOK_STD_VALUE;
		return	status;
	}

	status	= RESISTOR_OK;
	return	status;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
