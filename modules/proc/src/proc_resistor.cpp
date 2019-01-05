/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
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
static	class std::vector <class std::vector <class cv::Point_ <int>>>	contours;
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
static	void	bands_code_deduce_0	(void);
static	void	bands_code_deduce_1	(void);
static	int	bands_code_deduce_no	(void);
static	void	resistor_value		(void);
static	int	resistor_tolerance	(void);
static	int	chk_std_value		(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
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
		bands_code_deduce_0();
		bands_code_deduce_1();
		status	= bands_code_deduce_no();
		if (status) {
			result_resistor(status);
			return	status;
		}

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
						"Resistance:	%.2E ± %i%% Ohm",
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
	/* Write result into log */
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
	case RESISTOR_NOK_COLOR:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistor:  NOK_COLOR");
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
	proc_rotate(rect_rot.center.x, rect_rot.center.y, rect_rot.angle);
	proc_save_mem(2);
	proc_load_mem(1);
	proc_rotate(rect_rot.center.x, rect_rot.center.y, rect_rot.angle);
	proc_save_mem(3);
}

static	void	resistor_dimensions_0	(void)
{
	proc_load_mem(3);

	proc_dilate_erode(10);
	proc_contours(&contours, &hierarchy);
	proc_bounding_rect(&(contours[0]), &rect, true);
}

static	void	resistor_crop_0		(void)
{
	int	x;
	int	y;
	int	w;
	int	h;

	proc_load_mem(2);

	w	= rect.width;
	h	= rect.height;
	x	= rect.x;
	y	= rect.y;
	proc_ROI(x, y, w, h);
	proc_save_mem(4);
}

static	void	resistor_bkgd		(void)
{
	uint8_t	bkgd_hue;
	uint8_t	bkgd_sat;

	/* hue */
	proc_load_mem(4);
	proc_cmp(IMG_IFACE_CMP_HUE);
	proc_median_vertical();
	proc_median_horizontal();
	proc_pixel_get(0, 0, &bkgd_hue);

	/* saturation */
	proc_load_mem(4);
	proc_cmp(IMG_IFACE_CMP_SATURATION);
	proc_median_vertical();
	proc_median_horizontal();
	proc_pixel_get(0, 0, &bkgd_sat);

	if (bkgd_hue < 50) {
		/* Beige */
		bkgd	= 0;
	} else {
		/* Blue */
		if ((bkgd_hue < 90)  ||  (bkgd_sat < 100)) {
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
	proc_erode_dilate((rect.height * 0.67) / 2);
	proc_contours(&contours, &hierarchy);
	proc_bounding_rect(&(contours[0]), &rect, true);
}

static	void	resistor_crop_1		(void)
{
	int	x;
	int	y;
	int	w;
	int	h;

	proc_load_mem(2);

	w	= rect.width * 0.95;
	h	= rect.height * 0.8;
	x	= rect.x + w * (1.0 - 0.95) / 2.0;
	y	= rect.y + h * (1.0 - 0.8) / 2.0;
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
		proc_threshold(cv::THRESH_TOZERO_INV, 160);
		proc_threshold(cv::THRESH_TOZERO, 110);
		break;
	case 1:
		proc_threshold(cv::THRESH_TOZERO_INV, 180);
		proc_threshold(cv::THRESH_TOZERO, 30);
		break;
	case 2:
		proc_threshold(cv::THRESH_TOZERO_INV, 190);
		proc_threshold(cv::THRESH_TOZERO, 170);
		break;
	case 3:
		proc_threshold(cv::THRESH_TOZERO_INV, 210);
		proc_threshold(cv::THRESH_TOZERO, 120);
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
		proc_threshold(cv::THRESH_TOZERO_INV, 170);
		proc_threshold(cv::THRESH_TOZERO, 100);
		break;

	case 1:
		proc_threshold(cv::THRESH_TOZERO_INV, 180);
		proc_threshold(cv::THRESH_TOZERO, 100);
		break;
	case 2:
		proc_threshold(cv::THRESH_TOZERO_INV, 180);
		proc_threshold(cv::THRESH_TOZERO, 150);
		break;
	case 3:
		proc_threshold(cv::THRESH_TOZERO_INV, 190);
		proc_threshold(cv::THRESH_TOZERO, 90);
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
	if (rect.width * 0.95 > 80) {
		proc_dilate_erode(1);
	}
	proc_save_mem(8);
}

static	int	bands_find		(void)
{
	int	status;

	proc_load_mem(8);

	/* Contours */
	proc_contours(&contours, &hierarchy);
	if (contours.size() > 5) {
		proc_load_mem(8);
		proc_dilate_erode(1);
		proc_contours(&contours, &hierarchy);
	}

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

	status	= RESISTOR_OK;
	return	status;
}

static	void	bands_colors		(void)
{
	/* Hue */
	proc_load_mem(9);
	if (bands_n == 5) {
		proc_pixel_get(bands[0].x, bands[0].y, &(bands[0].h));
	}
	if (bands_n >= 3) {
		proc_pixel_get(bands[1].x, bands[1].y, &(bands[1].h));
	}
	proc_pixel_get(bands[2].x, bands[2].y, &(bands[2].h));
	if (bands_n >= 3) {
		proc_pixel_get(bands[3].x, bands[3].y, &(bands[3].h));
	}
	if (bands_n > 3) {
		proc_pixel_get(bands[4].x, bands[4].y, &(bands[4].h));
	}

	/* Saturation */
	proc_load_mem(10);
	if (bands_n == 5) {
		proc_pixel_get(bands[0].x, bands[0].y, &(bands[0].s));
	}
	if (bands_n >= 3) {
		proc_pixel_get(bands[1].x, bands[1].y, &(bands[1].s));
	}
	proc_pixel_get(bands[2].x, bands[2].y, &(bands[2].s));
	if (bands_n >= 3) {
		proc_pixel_get(bands[3].x, bands[3].y, &(bands[3].s));
	}
	if (bands_n > 3) {
		proc_pixel_get(bands[4].x, bands[4].y, &(bands[4].s));
	}

	/* Value */
	proc_load_mem(11);
	if (bands_n == 5) {
		proc_pixel_get(bands[0].x, bands[0].y, &(bands[0].v));
	}
	if (bands_n >= 3) {
		proc_pixel_get(bands[1].x, bands[1].y, &(bands[1].v));
	}
	proc_pixel_get(bands[2].x, bands[2].y, &(bands[2].v));
	if (bands_n >= 3) {
		proc_pixel_get(bands[3].x, bands[3].y, &(bands[3].v));
	}
	if (bands_n > 3) {
		proc_pixel_get(bands[4].x, bands[4].y, &(bands[4].v));
	}
}

static	void	bands_code		(void)
{
	int	i;

	/* Init to 0 */
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

	/*
	 * Not able to segmentate:
	 * q = 1 2
	 * w = 2 3
	 * e = 3 g
	 * r = 3 4
	 * t = 1 8
	 * y = 0 1
	 * u = 0 1 8
	 */

	if (band->h < 10) {
		// 1 2 3
		if (band->s < 90) {
			// 1 3
			if (band->v < 85) {
				ch	= '1';
			} else {
				ch	= '3';
			}
		} else if (band->s < 140) {
			// 1 2 3
			if (band->v < 60) {
				ch	= '1';
			} else if (band->v < 85) {
				// 1 2
				ch	= 'q';
			} else if (band->v < 90) {
				ch	= '2';
			} else if (band->v < 150) {
				// 2 3
				ch	= 'w';
			} else {
				ch	= '3';
			}
		} else if (band->s < 210) {
			// 2 3
			if (band->v < 90) {
				ch	= '2';
			} else if (band->v < 150) {
				// 2 3
				ch	= 'w';
			} else {
				ch	= '3';
			}
		} else {
			ch	= '3';
		}
	} else if (band->h < 35) {
		// 1 3 g
		if (band->s < 130) {
			// 1 3
			if (band->v < 85) {
				ch	= '1';
			} else {
				ch	= '3';
			}
		} else if (band->s < 140) {
			// 1 3 g
			if (band->v < 85) {
				ch	= '1';
			} else if (band->v < 120) {
				ch	= '3';
			} else {
				// 3 g
				ch	= 'e';
			}
		} else if (band->s < 180) {
			// 3 g
			if (band->v < 120) {
				ch	= '3';
			} else {
				// 3 g
				ch	= 'e';
			}
		} else {
			ch	= '3';
		}
	} else if (band->h < 50) {
		// 1 3
		if (band->s < 140) {
			// 1 3
			if (band->v < 85) {
				ch	= '1';
			} else {
				ch	= '3';
			}
		} else {
			ch	= '3';
		}
	} else if (band->h < 60) {
		// 1 3 4
		if (band->s < 140) {
			// 1 3
			if (band->v < 85) {
				ch	= '1';
			} else {
				ch	= '3';
			}
		} else if (band->s < 160) {
			ch	= '3';
		} else if (band->s < 180) {
			// 3 4
			if (band->v < 110) {
				// 3 4
				ch	= 'r';
			} else {
				ch	= '3';
			}
		} else {
			ch	= '3';
		}
	} else if (band->h < 80) {
		// 1 5
		if (band->s < 165) {
			ch	= '1';
		} else {
			ch	= '5';
		}
	} else if (band->h < 90) {
		ch	= '1';
	} else if (band->h < 100) {
		// 0 1 8 9
		if (band->s < 40) {
			// 1 8 9
			if (band->v < 55) {
				ch	= '1';
			} else if (band->v < 80) {
				// 1 8
				ch	= 't';
			} else if (band->v < 125) {
				ch	= '1';
			} else {
				ch	= '9';
			}
		} else if (band->s < 70) {
			// 0 1 8 9
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 55) {
				// 0 1
				ch	= 'y';
			} else if (band->v < 75) {
				// 0 1 8
				ch	= 'u';
			} else if (band->v < 80) {
				// 1 8
				ch	= 't';
			} else if (band->v < 125) {
				ch	= '1';
			} else {
				ch	= '9';
			}
		} else if (band->s < 100) {
			// 0 1 8
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 55) {
				// 0 1
				ch	= 'y';
			} else if (band->v < 75) {
				// 0 1 8
				ch	= 'u';
			} else if (band->v < 80) {
				// 1 8
				ch	= 't';
			} else {
				ch	= '1';
			}
		} else if (band->s < 140) {
			// 0 1
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else {
				ch	= '1';
			}
		} else {
			ch	= '0';
		}
	} else if (band->h < 110) {
		// 0 1 6 8
		if (band->s < 40) {
			// 1 8
			if (band->v < 55) {
				ch	= '1';
			} else if (band->v < 80) {
				// 1 8
				ch	= 't';
			} else {
				ch	= '1';
			}
		} else if (band->s < 100) {
			// 0 1 8
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 55) {
				// 0 1
				ch	= 'y';
			} else if (band->v < 75) {
				// 0 1 8
				ch	= 'u';
			} else if (band->v < 80) {
				// 1 8
				ch	= 't';
			} else {
				ch	= '1';
			}
		} else if (band->s < 140) {
			// 0 1
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else {
				ch	= '1';
			}
		} else if (band->s < 200) {
			ch	= '0';
		} else {
			ch	= '6';
		}
	} else if (band->h < 120) {
		// 0 1 6 7
		if (band->s < 40) {
			ch	= '1';
		} else if (band->s < 90) {
			// 0 1
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else {
				ch	= '1';
			}
		} else if (band->s < 110) {
			// 0 1 7
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else if (band->v < 120) {
				ch	= '1';
			} else {
				ch	= '7';
			}
		} else if (band->s < 140) {
			// 0 1
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else {
				ch	= '1';
			}
		} else if (band->s < 200) {
			ch	= '0';
		} else {
			ch	= '6';
		}
	} else if (band->h < 130) {
		// 0 1 7
		if (band->s < 40) {
			ch	= '1';
		} else if (band->s < 90) {
			// 0 1
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else {
				ch	= '1';
			}
		} else if (band->s < 110) {
			// 0 1 7
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else if (band->v < 120) {
				ch	= '1';
			} else {
				ch	= '7';
			}
		} else if (band->s < 140) {
			// 0 1
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else {
				ch	= '1';
			}
		} else {
			ch	= '0';
		}
	} else if (band->h < 140) {
		// 0 1
		if (band->s < 40) {
			ch	= '1';
		} else if (band->s < 140) {
			// 0 1
			if (band->v < 45) {
				ch	= '0';
			} else if (band->v < 75) {
				// 0 1
				ch	= 'y';
			} else {
				ch	= '1';
			}
		} else {
			ch	= '0';
		}
	} else if (band->h < 150) {
		ch	= '0';
	} else {
		ch	= '2';
	}

	return	ch;
}

static	void	bands_code_deduce_0	(void)
{
	/*
	 * Not able to segmentate:
	 * q = 1 2
	 * w = 2 3
	 * e = 3 g
	 * r = 3 4
	 * t = 1 8
	 * y = 0 1
	 * u = 0 1 8
	 */

	/* Band 0 (hundreds) */
	switch (code[0]) {
	case 'e':
		code[0]	= '3';
		break;
	case 'y':
		code[0]	= '1';
		break;
	case 'u':
		code[0]	= 't';
		break;
	}

	/* Band 2 (units) */
	switch (code[2]) {
	case 'e':
		code[2]	= '3';
		break;
	}

	/* Band 3 (multiplier) */
	switch (code[3]) {
	case 't':
		code[3]	= '1';
		break;
	case 'u':
		code[3]	= 'y';
		break;
	}

	/* Band 4 (tolerance) */
	switch (code[4]) {
	case 'w':
		code[4]	= '2';
		break;
	case 'e':
		code[4]	= 'g';
		break;
	case 't':
		code[4]	= '1';
		break;
	case 'y':
		code[4]	= '1';
		break;
	case 'u':
		code[4]	= '1';
		break;
	}

	/* Write bands' code into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Code:		\"%s\"",
						code);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

static	void	bands_code_deduce_1	(void)
{
	/*
	 * Not able to segmentate:

	 * q = 1 2
	 * w = 2 3
	 * e = 3 g
	 * r = 3 4
	 * t = 1 8
	 * y = 0 1

	 * u = 0 1 8
	 */

	/* Band 0 (hundreds) */
	switch (code[0]) {
	case 'q':
		switch (code[1]) {
		case '1':
		case '3':
		case '5':
		case '6':
		case '8':
			code[0]	= '1';
			break;
		case '2':
		case '4':
		case '7':
			code[0]	= '2';
			break;
		case '0':
			// q
			break;
		}
		break;
	case 'w':
		switch (code[1]) {
		case '2':
		case '4':
		case '7':
			code[0]	= '2';
			break;
		case '3':
		case '6':
		case '9':
			code[0]	= '3';
			break;
		case '0':
			// w
			break;
		}
		break;
	case 'r':
		switch (code[1]) {
		case '0':
		case '6':
		case '9':
			code[0]	= '3';
			break;
		case '7':
			code[0]	= '4';
			break;
		case '3':
			// r
			break;
		}
		break;
	case 't':
		switch (code[1]) {
		case '0':
		case '1':
		case '3':
		case '5':
		case '6':
		case '8':
			code[0]	= '1';
			break;
		case '2':
			// t
			break;
		}
		break;
	}

	/* Band 1 (tens) */
	switch (code[1]) {
	case 'q':
		switch (code[0]) {
		case '5':
		case '9':
			code[1]	= '1';
			break;
		case '2':
		case '6':
		case '8':
			code[1]	= '2';
			break;
		case '1':
			// q
			break;
		}
		break;
	case 'w':
		switch (code[0]) {
		case '2':
		case '6':
		case '8':
			code[1]	= '2';
			break;
		case '3':
		case '4':
			code[1]	= '3';
			break;
		case '1':
			// w
			break;
		}
		break;
	case 't':
		switch (code[0]) {
		case '5':
		case '9':
			code[1]	= '1';
			break;
		case '6':
			code[1]	= '8';
			break;
		case '1':
			// t
			break;
		}
		break;
	case 'y':
		switch (code[0]) {
		case '2':
		case '3':
			code[1]	= '0';
			break;
		case '5':
		case '9':
			code[1]	= '1';
			break;
		case '1':
			// y
			break;
		}
		break;
	case 'u':
		switch (code[0]) {
		case '2':
		case '3':
			code[1]	= '0';
			break;
		case '5':
		case '9':
			code[1]	= '1';
			break;
		case '6':
			code[1]	= '8';
			break;
		case '1':
			// u
			break;
		}
		break;
	}

	/* Band 2 (units) */
	switch (code[2]) {
	case 'y':
		code[2]	= '0';
		break;
	case 'u':
		code[2]	= '0';
		break;
	}

	/* Band 3 (multiplier) */
	switch (code[3]) {
	case 'y':
		code[3]	= '0';
		break;
	}

	/* Band 4 (tolerance) */
	switch (code[4]) {
	case 'q':
		code[4]	= '2';
		break;
	}

	/* Write bands' code into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Code:		\"%s\"",
						code);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

static	int	bands_code_deduce_no	(void)
{
	int	status;
	int	i;

	/*
	 * Not able to segmentate:
	 * q = 1 2
	 * w = 2 3
	 * e = 3 g
	 * r = 3 4
	 * t = 1 8
	 * y = 0 1
	 * u = 0 1 8
	 */

	for (i = 0; i < 5; i++) {
		switch (code[i]) {
		case 'q':
		case 'w':
		case 'e':
		case 'r':
		case 't':
		case 'y':
		case 'u':
			code[i]	= '?';
			break;
		}
	}

	for (i = 0; i < 5; i++) {
		if (code[i] == '?') {
			status	= RESISTOR_NOK_COLOR;
			return	status;
		}
	}

	/* Write bands' code into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Code:		\"%s\"",
						code);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	status	= RESISTOR_OK;
	return	status;
}

static	void	resistor_value		(void)
{
	int	power;

	/* Base value */
	base	= code[2] - '0';
	if (code[1] != 'n') {
		base	+= (code[1] - '0') * 10;
	}
	if (code[0] != 'n') {
		base	+= (code[0] - '0') * 100;
	}

	/* Calculate resistance */
	if ((code[3] > '0')  &&  (code[3] < '9')) {
		power	= code[3] - '0';
	} else if (code[3] == 'g') {
		power	= -1;
	} else if (code[3] == 's') {
		power	= -2;
	} else {
		// error
		return;
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
	bool	std_value_nok		= true;
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
