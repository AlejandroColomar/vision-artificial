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
		/* strcmp() */
	#include <cstring>
		/* clock_t & clock() & CLOCKS_PER_SEC */
	#include <ctime>

/* Packages ------------------------------------------------------------------*/
		/* openCV */
	#include <opencv2/opencv.hpp>
		/* zbar::ZBAR_EAN13 */
	#include <zbar.h>

/* libalx -------------------------------------------------------------------*/
		/* alx_sscan_fname() */
	#include "alx_input.hpp"

/* Project -------------------------------------------------------------------*/
		/* img_iface_act() */
	#include "img_iface.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"
		/* saved_path */
	#include "save.hpp"

/* Module --------------------------------------------------------------------*/
	#include "proc.hpp"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
	int	proc_debug;
	int	proc_mode;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	int	proc_label		(void);
static	void	result_label		(int status);

static	int	proc_coins		(void);
static	void	result_coins		(int status);

static	int	proc_resistor		(void);
static	void	result_resistor		(int status);

static	void	proc_save_mem		(int n);
static	void	proc_load_mem		(int n);
static	void	proc_save_ref		(void);

static	void	proc_local_max		(void);

static	void	proc_pixel_value	(int x, int y, unsigned char *val);
static	void	proc_ROI		(int x, int y, int w, int h);
static	void	proc_and_2ref		(void);
static	void	proc_not		(void);
static	void	proc_or_2ref		(void);
static	void	proc_cmp		(int cmp);
static	void	proc_dilate		(int size);
static	void	proc_erode		(int size);
static	void	proc_dilate_erode	(int size);
static	void	proc_erode_dilate	(int size);
static	void	proc_smooth		(int method, int ksize);
static	void	proc_rotate		(class cv::RotatedRect  *rect);
static	void	proc_adaptive_threshold	(int method, int type, int ksize);
static	void	proc_cvt_color		(int method);
static	void	proc_distance_transform	(void);
static	void	proc_threshold		(int type, int ksize);
static	void	proc_contours		(
			std::vector <std::vector <class cv::Point_ <int>>>  *contours,
			class cv::Mat  *hierarchy);
static	void	proc_contours_size		(
			std::vector <std::vector <class cv::Point_ <int>>>  *contours,
			double  *area,
			double  *perimeter);
static	void	proc_fit_ellipse	(
			std::vector <class cv::Point_ <int>>  *contour,
			class cv::RotatedRect  *rect,
			bool show);
static	void	proc_min_area_rect	(
			std::vector <class cv::Point_ <int>>  *contour,
			class cv::RotatedRect  *rect,
			bool show);

static	void	proc_OCR		(int lang, int conf);
static	void	proc_zbar		(int type);

static	void	proc_show_img		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	proc_iface_single	(int action)
{
	int	error;
	clock_t	time_0;
	clock_t	time_1;
	double	time_tot;

	/* Init timer */
	time_0	= clock();

	/* Process */
	switch (action) {
	case PROC_MODE_LABEL:
		error	= proc_label();
		break;
	case PROC_MODE_COINS:
		error	= proc_coins();
		break;
	case PROC_MODE_RESISTOR:
		error	= proc_resistor();
		break;
	}

	/* End timer */
	time_1	= clock();

	/* Calculate time in seconds */
	time_tot	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;

	/* Write time into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Time:  %.3lf", time_tot);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	return	error;
}

void	proc_iface_series	(void)
{
	bool	file_error;
	int	num_len;
	char	file_basename [FILENAME_MAX];
	char	file_ext [80];
	char	file_name [FILENAME_MAX];
	bool	proc_error;
	char	save_error_as [FILENAME_MAX];

	switch (proc_mode) {
	case PROC_MODE_LABEL:
		snprintf(file_basename, 80, "b");
		num_len	= 4;
		snprintf(file_ext, 80, ".BMP");
		break;
	case PROC_MODE_COINS:
		snprintf(file_basename, 80, "c");
		num_len	= 4;
		snprintf(file_ext, 80, ".jpeg");
		break;
	case PROC_MODE_RESISTOR:
		snprintf(file_basename, 80, "res");
		num_len	= 4;
		snprintf(file_ext, 80, ".jpeg");
		break;
	}

	bool	wh;
	int	i;
	wh	= true;
	for (i = 0; wh; i++) {
		snprintf(file_name, FILENAME_MAX, "%s%04i%s",
						file_basename, i, file_ext);

		file_error	= alx_sscan_fname(labels_path, file_name,
						true, file_name);

		if (file_error) {
			wh	= false;
		} else {
			errno	= 0;
			img_iface_load(labels_path, file_name);

			if (!errno) {
				/* Process */
				proc_error	= proc_iface_single(proc_mode);

				if (proc_error) {
					/* Save failed image into file */
					proc_show_img();
					snprintf(save_error_as, FILENAME_MAX,
							"%s%04i_err%s",
							file_basename, i,
							file_ext);
					save_image_file(labels_fail_path,
							save_error_as);
				}

				/* Show log */
				char	txt_tmp [80];
				snprintf(txt_tmp, 80, "%04i", i);
				user_iface_show_log(txt_tmp, "Item");
			} else {
				printf("errno:%i\n", errno);
			}
		}
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	int	proc_label		(void)
{
	int	status;

	double	times;
	clock_t	time_0;
	clock_t	time_1;

	std::vector <std::vector <cv::Point_ <int>>>	contours;
	class cv::Mat					hierarchy;
	class cv::RotatedRect				rect;
	int	x;
	int	y;
	int	w;
	int	h;

	char	price [80];

	proc_save_mem(0);
	/* Find label (position and angle) */
	{
		/* Measure time */
		time_0		= clock();

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
			result_label(status);
			return	status;
		}

		proc_min_area_rect(&(contours[0]), &rect, true);

		/* If angle is < -45º, it is taking into acount the incorrect side */
		if (rect.angle < -45.0) {
			int	tmp;
			rect.angle		= rect.angle + 90.0;
			tmp			= rect.size.width;
			rect.size.width		= rect.size.height;
			rect.size.height	= tmp;
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time0:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Align label and extract green component */
	{
		/* Measure time */
		time_0		= clock();

		proc_load_mem(0);
		proc_rotate(&rect);
		proc_cmp(IMG_IFACE_CMP_GREEN);
		proc_save_mem(1);

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time1:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Find "Cerdo" in aligned image */
	{
		/* Measure time */
		time_0		= clock();

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
		if (cerdo_nok) {
			status	= LABEL_NOK_CERDO;
			result_label(status);
			return	status;
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time2:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Read barcode in original image */
	{
		/* Measure time */
		time_0		= clock();

		proc_load_mem(0);
		proc_cmp(IMG_IFACE_CMP_GREEN);
		proc_zbar(zbar::ZBAR_EAN13);

		/* Check that 1 and only 1 bcode is read. */
		if (zb_codes.n != 1) {
			status	= LABEL_NOK_BCODE;
			result_label(status);
			return	status;
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time3:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Check product code in barcode */
	{
		/* Measure time */
		time_0		= clock();

		bool	prod_nok;
		prod_nok	= strncmp(zb_codes.arr[0].data, "2301703",
							strlen("2301703"));
		if (prod_nok) {
			status	= LABEL_NOK_PRODUCT;
			result_label(status);
			return	status;
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time4:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Read price in aligned image (green component) */
	{
		/* Measure time */
		time_0		= clock();

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

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time5:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Extract price from barcode */
	{
		/* Measure time */
		time_0		= clock();

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

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time6:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Check label price with barcode price */
	{
		/* Measure time */
		time_0		= clock();

		bool	price_nok;
		price_nok	= strncmp(img_ocr_text, price, strlen(price));
		if (price_nok) {
			status	= LABEL_NOK_PRICE;
			result_label(status);
			return	status;
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time7:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}

	status	= LABEL_OK;
	result_label(status);
	return	status;
}

static	void	result_label		(int status)
{
	/* Cleanup */

	/* Write result into log */
	char	result [LOG_LINE_LEN];
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

static	int	proc_coins		(void)
{
	int	status;

	double	times;
	clock_t	time_0;
	clock_t	time_1;

	std::vector <std::vector <cv::Point_ <int>>>	contours;
	class cv::Mat					hierarchy;
	class cv::RotatedRect				rect;
	int	x;
	int	y;
	int	w;
	int	h;

	struct	{
		/* found? */
		bool	found;

		/* position */
		int	x;
		int	y;
	} blobs [5];

	struct	{
		/* found? */
		bool		found;

		/* blob num */
		int		blob;

		/* position */
		int		x;
		int		y;

		/* value */
		unsigned char	h;
		unsigned char	s;
		unsigned char	v;
	} bands [5];

	char	code [6]	= {'\0', '\0', '\0', '\0', '\0', '\0'};
	float	resistance;
	int	tolerance;

	int	i;
	int	j;

	proc_save_mem(0);
	/* Find coins */
	{
		/* Measure time */
		time_0		= clock();

		proc_cmp(IMG_IFACE_CMP_BLUE);
//		proc_smooth(IMGI_SMOOTH_MEDIAN, 11);
		proc_threshold(cv::THRESH_BINARY_INV, IMG_IFACE_THR_OTSU);
//		proc_threshold(cv::THRESH_BINARY_INV, 100);
		proc_distance_transform();
		proc_local_max();
		proc_dilate(8);
		proc_save_mem(1);

		proc_contours(&contours, &hierarchy);

		/* If no contour is found, error:  NOK_COINS */
		if (!contours.size()) {
			status	= COINS_NOK_COINS;
			result_coins(status);
			return	status;
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time0:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
#if 0	/* Align resistor and crop */
	{
		/* Measure time */
		time_0		= clock();

		proc_load_mem(0);
		proc_rotate(&rect);
		x	= rect.center.x - (0.8 * rect.size.width / 2.0);
		if (x < 0) {
			x	= 0;
		}
		y	= rect.center.y - (0 * rect.size.height / 2.0);
		if (y < 0) {
			y	= 0;
		}
		w	= rect.size.width * 0.8;
		h	= rect.size.height * 0.3;
		proc_ROI(x, y, w, h);
		proc_save_mem(1);

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time1:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Separate background (BK) and lines (WH) */
	{
		/* Measure time */
		time_0		= clock();

		proc_smooth(IMGI_SMOOTH_MEDIAN, 5);
		proc_cvt_color(cv::COLOR_BGR2HSV);
		proc_smooth(IMGI_SMOOTH_MEDIAN, 5);
		proc_save_mem(2);

		/* hue */
		proc_cmp(IMG_IFACE_CMP_HUE);
		proc_save_mem(3);
		proc_not();
		proc_threshold(cv::THRESH_TOZERO_INV, 255 - 15);
		proc_threshold(cv::THRESH_TOZERO, 255 - 23);
		proc_threshold(cv::THRESH_BINARY_INV, 1);
		proc_save_mem(4);

		/* saturation */
		proc_load_mem(2);
		proc_cmp(IMG_IFACE_CMP_SATURATION);
		proc_save_mem(5);
		proc_threshold(cv::THRESH_TOZERO_INV, 163);
		proc_threshold(cv::THRESH_TOZERO, 50);
		proc_threshold(cv::THRESH_BINARY_INV, 1);
		proc_save_mem(6);

		/* value */
		proc_load_mem(2);
		proc_cmp(IMG_IFACE_CMP_VALUE);
		proc_save_mem(7);
		proc_threshold(cv::THRESH_TOZERO_INV, 240);
		proc_threshold(cv::THRESH_TOZERO, 100);
		proc_threshold(cv::THRESH_BINARY_INV, 1);
		proc_save_mem(8);

		/* Merge the components:  H | S | V */
		proc_save_ref();
		proc_load_mem(6);
		proc_or_2ref();
		proc_save_ref();
		proc_load_mem(4);
		proc_or_2ref();
		proc_dilate_erode(1);
		proc_save_mem(9);

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time2:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Find bands:  contours -> rectangles -> positions */
	{
		/* Measure time */
		time_0		= clock();

		/* Contours */
		proc_contours(&contours, &hierarchy);

		bool	bands_nok;
		bands_nok	= contours.size() != 4;
		if (bands_nok) {
			status	= RESISTOR_NOK_BANDS;
			result_label(status);
			return	status;
		}

		for (i = 0; i < contours.size(); i++) {
			blobs[i].found	= true;
			proc_min_area_rect(&(contours[i]), &rect, true);
			blobs[i].x	= rect.center.x;
			blobs[i].y	= rect.center.y;
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time3:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

	}
	/* Read values of HSV on the center of each band */
	{
		/* Measure time */
		time_0		= clock();

		/* Sort blobs into bands (sort x) */
		float	tmp_x;
		int	tmp_i;
		for (i = 0; i < contours.size(); i++) {
			tmp_x	= INFINITY;
			for (j = 0; j < contours.size(); j++) {
				if ((blobs[j].x < tmp_x)  &&  (blobs[j].found)) {
					tmp_x	= blobs[j].x;
					tmp_i	= j;
				}
			}
			blobs[tmp_i].found	= false;
			bands[i].blob		= tmp_i;
			bands[i].x		= blobs[tmp_i].x;
			bands[i].y		= blobs[tmp_i].y;
		}

		/* Hue */
		proc_load_mem(3);
		for (i = 0; i < contours.size(); i++) {
			proc_pixel_value(bands[i].x, bands[i].y, &(bands[i].h));
		}
		/* Saturation */
		proc_load_mem(5);
		for (i = 0; i < contours.size(); i++) {
			proc_pixel_value(bands[i].x, bands[i].y, &(bands[i].s));
		}
		/* Value */
		proc_load_mem(7);
		for (i = 0; i < contours.size(); i++) {
			proc_pixel_value(bands[i].x, bands[i].y, &(bands[i].v));
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time4:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

	}
	/* Interpret colors */
	{
		/* Measure time */
		time_0		= clock();

		for (i = 0; (i < contours.size())  &&  (i < 5); i++) {
			if (bands[i].s < 100) {
				if (bands[i].v < 50) {
					code[i]	= '0';
				} else if (bands[i].v > 200) {
					code[i]	= '9';
				} else {
					code[i]	= '8';
				}
			} else {
				if (bands[i].h < 20) {
					if (bands[i].v > 220) {
						code[i]	= '3';
					} else if (bands[i].v < 100) {
						code[i]	= '1';
					} else {
						code[i]	= '2';
					}
				} else if (bands[i].h < 50) {
					code[i]	= '4';
				} else if (bands[i].h < 93) {
					code[i]	= '5';
				} else if (bands[i].h < 127) {
					code[i]	= '6';
				} else if (bands[i].h < 161) {
					code[i]	= '7';
				} else {
					if (bands[i].v < 100) {
						code[i]	= '1';
					} else {
						code[i]	= '2';
					}
				}
			}
		}

#if 0
		if (contours.size() == 3) {
			/* Precission band not detected:  gold */
			code[3]	= '4';
		}
#endif

		/* Write bands' code into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Code:		\"%s\"",
							code);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time5:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Calculate resistor value */
	{
		/* Measure time */
		time_0		= clock();

		/* Base value */
		int	base;
		base	= 10 * (code[0] - '0') + (code[1] - '0');

		/* Check that base value is a standard value */
		int	std_values [12]	= {10,12,15,18,22,27,33,39,47,56,68,82};
		bool	std_value_nok;
		std_value_nok	= true;
		for (i = 0; i < 12; i++) {
			if (base == std_values[i]) {
				std_value_nok	= false;
			}
		}
		if (std_value_nok) {
			status	= RESISTOR_NOK_STD_VALUE;
			result_label(status);
			return	status;
		}

		/* Calculate resistance */
		int	power;
		power		= code[2] - '0';
		resistance	= base * pow(10, power);

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time6:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Calculate resistor tolerance */
	{
		/* Measure time */
		time_0		= clock();

		switch (code[3]) {
		case '1':
			tolerance	= 1;
			break;
		case '2':
			tolerance	= 2;
			break;
		case '4':
			tolerance	= 5;
			break;
		case '8':
			tolerance	= 10;
			break;
		default:
			status	= RESISTOR_NOK_TOLERANCE;
			result_label(status);
			return	status;
		}

		/* Write resistance value into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistance:	%.2E ± %i% Ohm",
						resistance, tolerance);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time7:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
#endif
	status	= COINS_OK;
	result_coins(status);
	return	status;
}

static	void	result_coins		(int status)
{
	/* Cleanup */

	/* Write result into log */
	char	result [LOG_LINE_LEN];
	switch (status) {
	case COINS_OK:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  OK");
		break;
	case COINS_NOK_COINS:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK_COINS");
		break;
	case COINS_NOK_OVERLAP:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK_OVERLAP");
		break;
	default:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:  NOK");
		break;
	}
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

static	int	proc_resistor		(void)
{
	int	status;

	double	times;
	clock_t	time_0;
	clock_t	time_1;

	std::vector <std::vector <cv::Point_ <int>>>	contours;
	class cv::Mat					hierarchy;
	class cv::RotatedRect				rect;
	int	x;
	int	y;
	int	w;
	int	h;

	struct	{
		/* found? */
		bool	found;

		/* position */
		int	x;
		int	y;
	} blobs [5];

	struct	{
		/* found? */
		bool		found;

		/* blob num */
		int		blob;

		/* position */
		int		x;
		int		y;

		/* value */
		unsigned char	h;
		unsigned char	s;
		unsigned char	v;
	} bands [5];

	char	code [6]	= {'\0', '\0', '\0', '\0', '\0', '\0'};
	float	resistance;
	int	tolerance;

	int	i;
	int	j;

	proc_save_mem(0);
	/* Find resistor (position and angle) */
	{
		/* Measure time */
		time_0		= clock();

		proc_cvt_color(cv::COLOR_BGR2HSV);
		proc_cmp(IMG_IFACE_CMP_SATURATION);
		proc_smooth(IMGI_SMOOTH_MEDIAN, 7);
		proc_smooth(IMGI_SMOOTH_MEAN, 7);
		proc_threshold(cv::THRESH_BINARY, 75);
		proc_dilate_erode(5);
		proc_contours(&contours, &hierarchy);

		/* If no contour is found, error:  NOK_RESISTOR */
		if (!contours.size()) {
			status	= RESISTOR_NOK_RESISTOR;
			result_resistor(status);
			return	status;
		}

#if 1
		proc_min_area_rect(&(contours[0]), &rect, true);

		/* If angle is < -45º, it is taking into acount the incorrect side */
		if (rect.angle < -45.0) {
			int	tmp;
			rect.angle		= rect.angle + 90.0;
			tmp			= rect.size.width;
			rect.size.width		= rect.size.height;
			rect.size.height	= tmp;
		}
#else
		proc_fit_ellipse(&(contours[0]), &rect, true);

		/* If height > width, it is taking into acount the incorrect side */
		if (rect.size.height > rect.size.width) {
			int	tmp;
			rect.angle		= rect.angle - 90.0;
			tmp			= rect.size.width;
			rect.size.width		= rect.size.height;
			rect.size.height	= tmp;
		}
#endif

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time0:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Align resistor and crop */
	{
		/* Measure time */
		time_0		= clock();

		proc_load_mem(0);
		proc_rotate(&rect);
		x	= rect.center.x - (0.8 * rect.size.width / 2.0);
		if (x < 0) {
			x	= 0;
		}
		y	= rect.center.y - (0 * rect.size.height / 2.0);
		if (y < 0) {
			y	= 0;
		}
		w	= rect.size.width * 0.8;
		h	= rect.size.height * 0.3;
		proc_ROI(x, y, w, h);
		proc_save_mem(1);

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time1:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Separate background (BK) and lines (WH) */
	{
		/* Measure time */
		time_0		= clock();

		proc_smooth(IMGI_SMOOTH_MEDIAN, 5);
		proc_cvt_color(cv::COLOR_BGR2HSV);
		proc_smooth(IMGI_SMOOTH_MEDIAN, 5);
		proc_save_mem(2);

		/* hue */
		proc_cmp(IMG_IFACE_CMP_HUE);
		proc_save_mem(3);
		proc_not();
		proc_threshold(cv::THRESH_TOZERO_INV, 255 - 15);
		proc_threshold(cv::THRESH_TOZERO, 255 - 23);
		proc_threshold(cv::THRESH_BINARY_INV, 1);
		proc_save_mem(4);

		/* saturation */
		proc_load_mem(2);
		proc_cmp(IMG_IFACE_CMP_SATURATION);
		proc_save_mem(5);
		proc_threshold(cv::THRESH_TOZERO_INV, 163);
		proc_threshold(cv::THRESH_TOZERO, 50);
		proc_threshold(cv::THRESH_BINARY_INV, 1);
		proc_save_mem(6);

		/* value */
		proc_load_mem(2);
		proc_cmp(IMG_IFACE_CMP_VALUE);
		proc_save_mem(7);
		proc_threshold(cv::THRESH_TOZERO_INV, 240);
		proc_threshold(cv::THRESH_TOZERO, 100);
		proc_threshold(cv::THRESH_BINARY_INV, 1);
		proc_save_mem(8);

		/* Merge the components:  H | S | V */
		proc_save_ref();
		proc_load_mem(6);
		proc_or_2ref();
		proc_save_ref();
		proc_load_mem(4);
		proc_or_2ref();
		proc_dilate_erode(1);
		proc_save_mem(9);

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time2:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Find bands:  contours -> rectangles -> positions */
	{
		/* Measure time */
		time_0		= clock();

		/* Contours */
		proc_contours(&contours, &hierarchy);

		bool	bands_nok;
		bands_nok	= contours.size() != 4;
		if (bands_nok) {
			status	= RESISTOR_NOK_BANDS;
			result_label(status);
			return	status;
		}

		for (i = 0; i < contours.size(); i++) {
			blobs[i].found	= true;
			proc_min_area_rect(&(contours[i]), &rect, true);
			blobs[i].x	= rect.center.x;
			blobs[i].y	= rect.center.y;
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time3:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

	}
	/* Read values of HSV on the center of each band */
	{
		/* Measure time */
		time_0		= clock();

		/* Sort blobs into bands (sort x) */
		float	tmp_x;
		int	tmp_i;
		for (i = 0; i < contours.size(); i++) {
			tmp_x	= INFINITY;
			for (j = 0; j < contours.size(); j++) {
				if ((blobs[j].x < tmp_x)  &&  (blobs[j].found)) {
					tmp_x	= blobs[j].x;
					tmp_i	= j;
				}
			}
			blobs[tmp_i].found	= false;
			bands[i].blob		= tmp_i;
			bands[i].x		= blobs[tmp_i].x;
			bands[i].y		= blobs[tmp_i].y;
		}

		/* Hue */
		proc_load_mem(3);
		for (i = 0; i < contours.size(); i++) {
			proc_pixel_value(bands[i].x, bands[i].y, &(bands[i].h));
		}
		/* Saturation */
		proc_load_mem(5);
		for (i = 0; i < contours.size(); i++) {
			proc_pixel_value(bands[i].x, bands[i].y, &(bands[i].s));
		}
		/* Value */
		proc_load_mem(7);
		for (i = 0; i < contours.size(); i++) {
			proc_pixel_value(bands[i].x, bands[i].y, &(bands[i].v));
		}

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time4:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

	}
	/* Interpret colors */
	{
		/* Measure time */
		time_0		= clock();

		for (i = 0; (i < contours.size())  &&  (i < 5); i++) {
			if (bands[i].s < 100) {
				if (bands[i].v < 50) {
					code[i]	= '0';
				} else if (bands[i].v > 200) {
					code[i]	= '9';
				} else {
					code[i]	= '8';
				}
			} else {
				if (bands[i].h < 20) {
					if (bands[i].v > 220) {
						code[i]	= '3';
					} else if (bands[i].v < 100) {
						code[i]	= '1';
					} else {
						code[i]	= '2';
					}
				} else if (bands[i].h < 50) {
					code[i]	= '4';
				} else if (bands[i].h < 93) {
					code[i]	= '5';
				} else if (bands[i].h < 127) {
					code[i]	= '6';
				} else if (bands[i].h < 161) {
					code[i]	= '7';
				} else {
					if (bands[i].v < 100) {
						code[i]	= '1';
					} else {
						code[i]	= '2';
					}
				}
			}
		}

#if 0
		if (contours.size() == 3) {
			/* Precission band not detected:  gold */
			code[3]	= '4';
		}
#endif

		/* Write bands' code into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Code:		\"%s\"",
							code);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time5:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Calculate resistor value */
	{
		/* Measure time */
		time_0		= clock();

		/* Base value */
		int	base;
		base	= 10 * (code[0] - '0') + (code[1] - '0');

		/* Check that base value is a standard value */
		int	std_values [12]	= {10,12,15,18,22,27,33,39,47,56,68,82};
		bool	std_value_nok;
		std_value_nok	= true;
		for (i = 0; i < 12; i++) {
			if (base == std_values[i]) {
				std_value_nok	= false;
			}
		}
		if (std_value_nok) {
			status	= RESISTOR_NOK_STD_VALUE;
			result_label(status);
			return	status;
		}

		/* Calculate resistance */
		int	power;
		power		= code[2] - '0';
		resistance	= base * pow(10, power);

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time6:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
	/* Calculate resistor tolerance */
	{
		/* Measure time */
		time_0		= clock();

		switch (code[3]) {
		case '1':
			tolerance	= 1;
			break;
		case '2':
			tolerance	= 2;
			break;
		case '4':
			tolerance	= 5;
			break;
		case '8':
			tolerance	= 10;
			break;
		default:
			status	= RESISTOR_NOK_TOLERANCE;
			result_label(status);
			return	status;
		}

		/* Write resistance value into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Resistance:	%.2E ± %i% Ohm",
						resistance, tolerance);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;

		/* Measure time */
		time_1	= clock();
		times	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
		/* Write time into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Time7:  %.3lf", times);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}

	status	= RESISTOR_OK;
	result_resistor(status);
	return	status;
}

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

static	void	proc_save_mem		(int n)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_MEM, (void *)&n);
}

static	void	proc_load_mem		(int n)
{
	img_iface_act(IMG_IFACE_ACT_LOAD_MEM, (void *)&n);

	proc_show_img();
}

static	void	proc_save_ref		(void)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_REF, NULL);
}

static	void	proc_local_max		(void)
{
	img_iface_act(IMG_IFACE_ACT_LOCAL_MAX, NULL);

	proc_show_img();
}

static	void	proc_pixel_value	(int x, int y, unsigned char *val)
{
	struct Img_Iface_Data_Pixel_Value	data;
	data.x		= x;
	data.y		= y;
	data.val	= val;
	img_iface_act(IMG_IFACE_ACT_PIXEL_VALUE, (void *)&data);
}

static	void	proc_ROI		(int x, int y, int w, int h)
{
	struct Img_Iface_Data_SetROI		data;
	data.rect.x		= x;
	data.rect.y		= y;
	data.rect.width		= w;
	data.rect.height	= h;
	img_iface_act(IMG_IFACE_ACT_SET_ROI, (void *)&data);

	proc_show_img();
}

static	void	proc_and_2ref		(void)
{
	img_iface_act(USER_IFACE_ACT_AND_2REF, NULL);

	proc_show_img();
}

static	void	proc_not		(void)
{
	img_iface_act(USER_IFACE_ACT_NOT, NULL);

	proc_show_img();
}

static	void	proc_or_2ref		(void)
{
	img_iface_act(USER_IFACE_ACT_OR_2REF, NULL);

	proc_show_img();
}

static	void	proc_cmp		(int cmp)
{
	struct Img_Iface_Data_Component		data;
	data.cmp	= cmp;
	img_iface_act(IMG_IFACE_ACT_COMPONENT, (void *)&data);

	proc_show_img();
}

static	void	proc_dilate		(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_DILATE, (void *)&data);

	proc_show_img();
}

static	void	proc_erode		(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_ERODE, (void *)&data);

	proc_show_img();
}

static	void	proc_dilate_erode	(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_DILATE_ERODE, (void *)&data);

	proc_show_img();
}

static	void	proc_erode_dilate	(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_ERODE_DILATE, (void *)&data);

	proc_show_img();
}

static	void	proc_smooth		(int method, int ksize)
{
	struct Img_Iface_Data_Smooth		data;
	data.method	= method;
	data.ksize	= ksize;
	img_iface_act(IMG_IFACE_ACT_SMOOTH, (void *)&data);

	proc_show_img();
}

static	void	proc_rotate		(class cv::RotatedRect  *rect)
{
	struct Img_Iface_Data_Rotate		data;
	data.center.x	= rect->center.x;
	data.center.y	= rect->center.y;
	data.angle	= rect->angle;
	img_iface_act(IMG_IFACE_ACT_ROTATE, (void *)&data);

	proc_show_img();
}

static	void	proc_adaptive_threshold	(int method, int type, int ksize)
{
	struct Img_Iface_Data_Adaptive_Thr	data;
	data.method	= method;
	data.thr_typ	= type;
	data.ksize	= ksize;
	img_iface_act(USER_IFACE_ACT_ADAPTIVE_THRESHOLD, (void *)&data);

	proc_show_img();
}

static	void	proc_cvt_color		(int method)
{
	struct Img_Iface_Data_Cvt_Color	data;
	data.method	= method;
	img_iface_act(IMG_IFACE_ACT_CVT_COLOR, (void *)&data);

	proc_show_img();
}

static	void	proc_threshold		(int type, int size)
{
	struct Img_Iface_Data_Threshold		data;
	data.thr_typ	= type;
	data.thr_val	= size;
	img_iface_act(IMG_IFACE_ACT_THRESHOLD, (void *)&data);

	proc_show_img();
}

static	void	proc_distance_transform	(void)
{
	img_iface_act(IMG_IFACE_ACT_DISTANCE_TRANSFORM, NULL);

	proc_show_img();
}

static	void	proc_contours		(
			std::vector <std::vector <class cv::Point_ <int>>>  *contours,
			class cv::Mat  *hierarchy)
{
	struct Img_Iface_Data_Contours		data;
	data.contours	= contours;
	data.hierarchy	= hierarchy;
	img_iface_act(IMG_IFACE_ACT_CONTOURS, (void *)&data);

	proc_show_img();
}

static	void	proc_contours_size		(
			std::vector <std::vector <class cv::Point_ <int>>>  *contours,
			double  *area,
			double  *perimeter)
{
	struct Img_Iface_Data_Contours_Size	data;
	data.contours	= contours;
	data.area	= area;
	data.perimeter	= perimeter;
	img_iface_act(IMG_IFACE_ACT_CONTOURS_SIZE, (void *)&data);
}

static	void	proc_fit_ellipse	(
			std::vector <class cv::Point_ <int>>  *contour,
			class cv::RotatedRect  *rect,
			bool show)
{
	struct Img_Iface_Data_MinARect		data;
	data.contour	= contour;
	data.rect	= rect;
	data.show	= show;
	img_iface_act(IMG_IFACE_ACT_FIT_ELLIPSE, (void *)&data);


	if (show) {
		proc_show_img();
	}
}

static	void	proc_min_area_rect	(
			std::vector <class cv::Point_ <int>>  *contour,
			class cv::RotatedRect  *rect,
			bool  show)
{
	struct Img_Iface_Data_MinARect		data;
	data.contour	= contour;
	data.rect	= rect;
	data.show	= show;
	img_iface_act(IMG_IFACE_ACT_MIN_AREA_RECT, (void *)&data);

	if (show) {
		proc_show_img();
	}
}

static	void	proc_OCR		(int lang, int conf)
{
	struct Img_Iface_Data_Read		data;
	data.lang	= lang;	/* eng=0, spa=1, cat=2 */
	data.conf	= conf;	/* none=0, price=1 */
	img_iface_act(IMG_IFACE_ACT_READ, (void *)&data);
}

static	void	proc_zbar		(int type)
{
	struct Img_Iface_Data_Decode		data;
	data.code_type	= (enum zbar::zbar_symbol_type_e)type;
	img_iface_act(IMG_IFACE_ACT_DECODE, (void *)&data);
}

static	void	proc_show_img		(void)
{
	if (proc_debug >= PROC_DBG_DELAY_STEP) {
		img_iface_show_img();

		if (proc_debug >= PROC_DBG_STOP_STEP) {
			getchar();
		}
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
