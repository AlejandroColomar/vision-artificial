/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* snprintf() */
	#include <cstdio>
		/* strcmp() */
	#include <cstring>

/* Packages ------------------------------------------------------------------*/
		/* openCV */
	#include <opencv2/opencv.hpp>
		/* zbar::ZBAR_EAN13 */
	#include <zbar.h>

/* Project -------------------------------------------------------------------*/
		/* zb_codes */
	#include "img_iface.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
	#include "proc_common.hpp"

	#include "proc_label.hpp"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/

/* Static --------------------------------------------------------------------*/
static	std::vector <std::vector <cv::Point_ <int>>>	contours;
static	class cv::Mat					hierarchy;
static	class cv::RotatedRect				rect;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_label		(int status);

static	int	label_find		(void);
static	void	label_align		(void);
static	int	find_cerdo		(void);
static	int	barcode_read		(void);
static	int	barcode_chk_prod	(void);
static	void	price_read		(void);
static	int	price_chk		(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_label		(void)
{
	int	status;

	proc_save_mem(0);
	/* Find label (position and angle) */
	{
		/* Measure time */
		clock_start();

		status	= label_find();
		if (status) {
			result_label(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find label");
	}
	/* Align label and extract green component */
	{
		/* Measure time */
		clock_start();

		label_align();

		/* Measure time */
		clock_stop("Align label");
	}
	/* Find "Cerdo" in aligned image */
	{
		/* Measure time */
		clock_start();

		status	= find_cerdo();
		if (status) {
			result_label(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find cerdo (OCR)");
	}
	/* Read barcode in original image */
	{
		/* Measure time */
		clock_start();

		status	= barcode_read();
		if (status) {
			result_label(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Read barcode (zbar)");
	}
	/* Check product code in barcode */
	{
		/* Measure time */
		clock_start();

		status	= barcode_chk_prod();
		if (status) {
			result_label(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Chk product code");
	}
	/* Read price in aligned image (green component) */
	{
		/* Measure time */
		clock_start();

		price_read();

		/* Measure time */
		clock_stop("Read price (OCR)");
	}
	/* Check label price with barcode price */
	{
		/* Measure time */
		clock_start();

		status	= price_chk();
		if (status) {
			result_label(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Check price");
	}

	status	= LABEL_OK;
	result_label(status);
	return	status;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_label		(int status)
{
	/* Cleanup */

	/* Write result into log */
	switch (status) {
	case LABEL_OK:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  OK");
		break;
	case LABEL_NOK_LABEL:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK_LABEL");
		break;
	case LABEL_NOK_CERDO:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK_CERDO");
		break;
	case LABEL_NOK_BCODE:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK_BCODE");
		break;
	case LABEL_NOK_PRODUCT:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK_PRODUCT");
		break;
	case LABEL_NOK_PRICE:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK_PRICE");
		break;
	default:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK");
		break;
	}
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

static	int	label_find		(void)
{
	int	status;

	proc_load_mem(0);

	proc_cmp(IMG_IFACE_CMP_BLUE);
	proc_smooth(IMGI_SMOOTH_MEDIAN, 7);
#if 0
	proc_adaptive_threshold(CV_ADAPTIVE_THRESH_MEAN_C,
					CV_THRESH_BINARY, 5);
#else
	proc_not();
#endif
	proc_smooth(IMGI_SMOOTH_MEAN, 21);
	proc_threshold(cv::THRESH_BINARY_INV, 2);
	proc_dilate_erode(100);
	proc_contours(&contours, &hierarchy);

	/* If no contour is found, error:  NOK_LABEL */
	if (!contours.size()) {
		status	= LABEL_NOK_LABEL;
		return	status;
	}

	proc_min_area_rect(&(contours[0]), &rect, true);

	/* If angle is < -45º, it is taking into acount the incorrect side */
	if (rect.angle < -45.0) {
		int	tmp;
		rect.angle		+= 90.0;
		tmp			= rect.size.width;
		rect.size.width		= rect.size.height;
		rect.size.height	= tmp;
	}

	status	= LABEL_OK;
	return	status;
}

static	void	label_align		(void)
{
	proc_load_mem(0);

	proc_rotate(rect.center.x, rect.center.y, rect.angle);
	proc_cmp(IMG_IFACE_CMP_GREEN);
	proc_save_mem(1);
}

static	int	find_cerdo		(void)
{
	int	status;

	proc_load_mem(1);

	int	x;
	int	y;
	int	w;
	int	h;
	x	= rect.center.x - (1.05 * rect.size.width / 2);
	if (x < 0) {
		x	= 0;
	}
	y	= rect.center.y - (1.47 * rect.size.height / 2);
	if (y < 0) {
		y	= 0;
	}
	w	= rect.size.width / 2;
	h	= rect.size.height * 0.20;
	proc_ROI(x, y, w, h);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_erode(1);
	proc_OCR(IMG_IFACE_OCR_LANG_ENG, IMG_IFACE_OCR_CONF_NONE);

	/* Compare Label text to "Cerdo". */
	bool	cerdo_nok;
	cerdo_nok	= strncmp(img_ocr_text, "Cerdo",
						strlen("Cerdo"));

	/* If string doesn't match, error:  NOK_CERDO */
	if (cerdo_nok) {
		status	= LABEL_NOK_CERDO;
		return	status;
	}

	status	= LABEL_OK;
	return	status;
}

static	int	barcode_read		(void)
{
	int	status;

	proc_load_mem(0);

	proc_cmp(IMG_IFACE_CMP_GREEN);
	proc_zbar(zbar::ZBAR_EAN13);

	/* Check that 1 and only 1 bcode is read. */
	if (zb_codes.n != 1) {
		status	= LABEL_NOK_BCODE;
		return	status;
	}

	status	= LABEL_OK;
	return	status;
}

static	int	barcode_chk_prod	(void)
{
	int	status;

	bool	prod_nok;
	prod_nok	= strncmp(zb_codes.arr[0].data, "2301703",
						strlen("2301703"));
	if (prod_nok) {
		status	= LABEL_NOK_PRODUCT;
		return	status;
	}

	status	= LABEL_OK;
	return	status;
}

static	void	price_read		(void)
{
	proc_load_mem(1);

	int	x;
	int	y;
	int	w;
	int	h;
	x	= rect.center.x + (0.33 * rect.size.width / 2);
	y	= rect.center.y + (0.64 * rect.size.height / 2);
	w	= rect.size.width * 0.225;
	h	= rect.size.height * 0.15;
	proc_ROI(x, y, w, h);
	proc_smooth(IMGI_SMOOTH_MEAN, 3);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_dilate_erode(1);
	proc_threshold(cv::THRESH_BINARY, 1);
	proc_OCR(IMG_IFACE_OCR_LANG_DIGITS, IMG_IFACE_OCR_CONF_PRICE);
}

static	int	price_chk		(void)
{
	int	status;

	char	price [80];

	/* Extract price from barcode */
	if (zb_codes.arr[0].data[8] != '0') {
		snprintf(price, 80, "%c%c.%c%c",
					zb_codes.arr[0].data[8],
					zb_codes.arr[0].data[9],
					zb_codes.arr[0].data[10],
					zb_codes.arr[0].data[11]);
	} else {
		snprintf(price, 80, "%c.%c%c",
					zb_codes.arr[0].data[9],
					zb_codes.arr[0].data[10],
					zb_codes.arr[0].data[11]);
	}

	/* Compare price from barcode and from text */
	bool	price_nok;
	price_nok	= strncmp(img_ocr_text, price, strlen(price));

	if (price_nok) {
		status	= LABEL_NOK_PRICE;
		return	status;
	}

	status	= LABEL_OK;
	return	status;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
