/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/proc/label.hpp"

#include <cstdio>
#include <cstring>

#include <opencv2/opencv.hpp>
#include <zbar.h>

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


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	class std::vector <class std::vector <class cv::Point_ <int>>>	contours;
static	class cv::Mat						hierarchy;
static	class cv::RotatedRect					rect;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	result_label	(int status);

static	int	label_find	(void);
static	void	label_align	(void);
static	int	find_cerdo	(void);
static	int	barcode_read	(void);
static	int	barcode_chk_prod(void);
static	void	price_read	(void);
static	int	price_chk	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_label	(void)
{
	int	status;

	proc_save_mem(0);

	/* Find label (position and angle) */
	clock_start();
	status	= label_find();
	if (status) {
		result_label(status);
		return	status;
	}
	clock_stop("Find label");

	/* Align label and extract green component */
	clock_start();
	label_align();
	clock_stop("Align label");

	/* Find "Cerdo" in aligned image */
	clock_start();
	status	= find_cerdo();
	if (status) {
		result_label(status);
		return	status;
	}
	clock_stop("Find cerdo (OCR)");

	/* Read barcode in original image */
	clock_start();
	status	= barcode_read();
	if (status) {
		result_label(status);
		return	status;
	}
	clock_stop("Read barcode (zbar)");

	/* Check product code in barcode */
	clock_start();
	status	= barcode_chk_prod();
	if (status) {
		result_label(status);
		return	status;
	}
	clock_stop("Chk product code");

	/* Read price in aligned image (green component) */
	clock_start();
	price_read();
	clock_stop("Read price (OCR)");

	/* Check label price with barcode price */
	clock_start();
	status	= price_chk();
	if (status) {
		result_label(status);
		return	status;
	}
	clock_stop("Check price");

	result_label(LABEL_OK);
	return	0;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	result_label	(int status)
{

	switch (status) {
	case LABEL_OK:
		user_iface_log_write(0, "Label:  OK");
		break;
	case LABEL_NOK_LABEL:
		user_iface_log_write(0, "Label:  NOK_LABEL");
		break;
	case LABEL_NOK_CERDO:
		user_iface_log_write(0, "Label:  NOK_CERDO");
		break;
	case LABEL_NOK_BCODE:
		user_iface_log_write(0, "Label:  NOK_BCODE");
		break;
	case LABEL_NOK_PRODUCT:
		user_iface_log_write(0, "Label:  NOK_PRODUCT");
		break;
	case LABEL_NOK_PRICE:
		user_iface_log_write(0, "Label:  NOK_PRICE");
		break;
	default:
		user_iface_log_write(0, "Label:  NOK");
		break;
	}
}

static	int	label_find	(void)
{

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

	if (!contours.size())
		return	LABEL_NOK_LABEL;

	proc_min_area_rect(&contours[0], &rect, true);
	/* If angle is < -45º, it is taking into acount the incorrect side */
	if (rect.angle < -45.0) {
		rect.angle		+= 90.0;
		ALX_SWAP(&rect.size.width, &rect.size.height);
	}

	return	0;
}

static	void	label_align	(void)
{
	proc_load_mem(0);

	proc_rotate(rect.center.x, rect.center.y, rect.angle);
	proc_cmp(IMG_IFACE_CMP_GREEN);
	proc_save_mem(1);
}

static	int	find_cerdo	(void)
{
	int	x, y;
	int	w, h;

	proc_load_mem(1);

	x	= rect.center.x - (1.05 * rect.size.width / 2);
	if (x < 0)
		x	= 0;
	y	= rect.center.y - (1.47 * rect.size.height / 2);
	if (y < 0)
		y	= 0;
	w	= rect.size.width / 2;
	h	= rect.size.height * 0.20;
	proc_ROI(x, y, w, h);
	proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
	proc_erode(1);
	proc_OCR(IMG_IFACE_OCR_LANG_ENG, IMG_IFACE_OCR_CONF_NONE);

	if (strncmp(img_ocr_text, "Cerdo", strlen("Cerdo")))
		return	LABEL_NOK_CERDO;
	return	0;
}

static	int	barcode_read	(void)
{

	proc_load_mem(0);

	proc_cmp(IMG_IFACE_CMP_GREEN);
	proc_zbar(zbar::ZBAR_EAN13);
	if (zb_codes.n != 1)
		return	LABEL_NOK_BCODE;
	return	0;
}

static	int	barcode_chk_prod(void)
{

	if (strncmp(zb_codes.arr[0].data, "2301703", strlen("2301703")))
		return	LABEL_NOK_PRODUCT;
	return	LABEL_OK;
}

static	void	price_read	(void)
{
	int	x, y;
	int	w, h;

	proc_load_mem(1);

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

static	int	price_chk	(void)
{
	char	price[80];

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
	if (strncmp(img_ocr_text, price, strlen(price)))
		return	LABEL_NOK_PRICE;
	return	0;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
