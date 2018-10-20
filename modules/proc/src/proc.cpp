/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
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

/* Project -------------------------------------------------------------------*/
		/* img_iface_act() */
	#include "img_iface.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
	#include "proc.hpp"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
	int	proc_debug;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	int	proc_etiqueta		(void);
static	void	result_etiqueta		(int status);

static	void	proc_save_mem		(int n);
static	void	proc_load_mem		(int n);
static	void	proc_cmp		(int cmp);
static	void	proc_smooth		(int method, int mask_size);
static	void	proc_adaptive_threshold	(int method, int type, int size);
static	void	proc_threshold		(int type, int size);
static	void	proc_invert		(void);
static	void	proc_dilate		(int size);
static	void	proc_erode		(int size);
static	void	proc_dilate_erode	(int size);
static	void	proc_contours		(std::vector <
						std::vector <
							class cv::Point_ <int>
						>
					>  *contours,
					class cv::Mat  *hierarchy);
static	void	proc_min_area_rect	(std::vector <
						class cv::Point_ <int>
					>  *contour,
					class cv::RotatedRect  *rect);
static	void	proc_rotate		(class cv::RotatedRect  *rect);
static	void	proc_ROI		(int x, int y, int w, int h);
static	void	proc_OCR		(int lang, int conf);
static	void	proc_zbar		(int type);

static	void	proc_show_img		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	proc_iface		(int proc_mode)
{
	int	error;
	clock_t	time_0;
	clock_t	time_1;
	double	time_tot;

	/* Init timer */
	time_0	= clock();

	/* Process */
	switch (proc_mode) {
	case PROC_MODE_LABEL:
		error	= proc_etiqueta();
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


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	int	proc_etiqueta		(void)
{
	int	status;

	char	price [80];

	std::vector <std::vector <cv::Point_ <int>>>	contours;
	class cv::Mat					hierarchy;
	class cv::RotatedRect				rect;
	int	x;
	int	y;
	int	w;
	int	h;

	proc_save_mem(0);
	/* Find label (position and angle) */
	{
		proc_cmp(IMG_IFACE_CMP_BLUE);
		proc_smooth(IMGI_SMOOTH_MEDIAN, 7);
		#if 0
			proc_adaptive_threshold(CV_ADAPTIVE_THRESH_MEAN_C,
							CV_THRESH_BINARY, 5);
		#else
			proc_invert();
		#endif
		proc_smooth(IMGI_SMOOTH_MEAN, 21);
		proc_threshold(cv::THRESH_BINARY_INV, 2);
		proc_dilate_erode(100);
		proc_contours(&contours, &hierarchy);

		/* If no contour is found, error:  NOK_LABEL */
		if (!contours.size()) {
			status	= LABEL_NOK_LABEL;
			result_etiqueta(status);
			return	status;
		}

		proc_min_area_rect(&(contours[0]), &rect);

		/* If angle is < -45º, it is taking into acount the incorrect side */
		if (rect.angle < -45.0) {
			int	tmp;
			rect.angle		= rect.angle + 90.0;
			tmp			= rect.size.width;
			rect.size.width		= rect.size.height;
			rect.size.height	= tmp;
		}
	}
	/* Align label and extract green component */
	{
		proc_load_mem(0);
		proc_rotate(&rect);
		proc_cmp(IMG_IFACE_CMP_GREEN);
		proc_save_mem(1);
	}
	/* Find "Cerdo" in aligned image */
	{

		x	= rect.center.x - (1.05 * rect.size.width / 2);
		y	= rect.center.y - (1.47 * rect.size.height / 2);
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
			result_etiqueta(status);
			return	status;
		}
	}
	/* Read barcode in original image */
	{
		proc_load_mem(0);
		proc_cmp(IMG_IFACE_CMP_GREEN);
		proc_zbar(zbar::ZBAR_EAN13);

		/* Check that 1 and only 1 bcode is read. */
		if (zb_codes.n != 1) {
			status	= LABEL_NOK_BCODE;
			result_etiqueta(status);
			return	status;
		}
	}
	/* Check product code in barcode */
	{
		bool	prod_nok;
		prod_nok	= strncmp(zb_codes.arr[0].data, "2301703",
							strlen("2301703"));
		if (prod_nok) {
			status	= LABEL_NOK_PRODUCT;
			result_etiqueta(status);
			return	status;
		}
	}
	/* Read price in aligned image (green component) */
	{
		proc_load_mem(1);

		x	= rect.center.x + (0.33 * rect.size.width / 2);
		y	= rect.center.y + (0.64 * rect.size.height / 2);
		w	= rect.size.width * 0.33;
		h	= rect.size.height * 0.15;
		proc_ROI(x, y, w, h);
		proc_smooth(IMGI_SMOOTH_MEAN, 3);
		proc_threshold(cv::THRESH_BINARY, IMG_IFACE_THR_OTSU);
//		proc_threshold(CV_THRESH_BINARY, 100);
//		proc_smooth(CV_BLUR, 3);
//		proc_dilate(1);
		proc_OCR(IMG_IFACE_OCR_LANG_ENG, IMG_IFACE_OCR_CONF_PRICE);
	}
	/* Extract price from barcode */
	{
		if (zb_codes.arr[0].data[8] != '0') {
			snprintf(price, 80, "%c%c.%c%c€",
						zb_codes.arr[0].data[8],
						zb_codes.arr[0].data[9],
						zb_codes.arr[0].data[10],
						zb_codes.arr[0].data[11]);
		} else {
			snprintf(price, 80, "%c.%c%c€",
						zb_codes.arr[0].data[9],
						zb_codes.arr[0].data[10],
						zb_codes.arr[0].data[11]);
		}
	}
	/* Check label price with barcode price */
	{
		bool	price_nok;
		price_nok	= strncmp(img_ocr_text, price, strlen(price));
		if (price_nok) {
			status	= LABEL_NOK_PRICE;
			result_etiqueta(status);
			return	status;
		}
	}

	status	= LABEL_OK;
	result_etiqueta(status);
	return	status;
}

static	void	result_etiqueta		(int status)
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

static	void	proc_save_mem		(int n)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_MEM, (void *)&n);
}

static	void	proc_load_mem		(int n)
{
	img_iface_act(IMG_IFACE_ACT_LOAD_MEM, (void *)&n);

	proc_show_img();
}

static	void	proc_cmp		(int cmp)
{
	struct Img_Iface_Data_Component		data;
	data.cmp	= cmp;
	img_iface_act(IMG_IFACE_ACT_COMPONENT, (void *)&data);

	proc_show_img();
}

static	void	proc_smooth		(int method, int mask_size)
{
	struct Img_Iface_Data_Smooth		data;
	data.method	= method;
	data.msk_siz	= mask_size;
	img_iface_act(IMG_IFACE_ACT_SMOOTH, (void *)&data);

	proc_show_img();
}

static	void	proc_adaptive_threshold	(int method, int type, int size)
{
	struct Img_Iface_Data_Adaptive_Thr	data;
	data.method	= method;
	data.thr_typ	= type;
	data.nbh_val	= size;
	img_iface_act(USER_IFACE_ACT_ADAPTIVE_THRESHOLD, (void *)&data);

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

static	void	proc_invert		(void)
{
	img_iface_act(USER_IFACE_ACT_INVERT, NULL);

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

static	void	proc_contours		(std::vector <
						std::vector <
							class cv::Point_ <int>
						>
					>  *contours,
					class cv::Mat  *hierarchy)
{
	struct Img_Iface_Data_Contours		data;
	data.contours	= contours;
	data.hierarchy	= hierarchy;
	img_iface_act(IMG_IFACE_ACT_CONTOURS, (void *)&data);

	proc_show_img();
}

static	void	proc_min_area_rect	(std::vector <
						class cv::Point_ <int>
					>  *contour,
					class cv::RotatedRect  *rect)
{
	struct Img_Iface_Data_MinARect		data;
	data.contour	= contour;
	data.rect	= rect;
	img_iface_act(IMG_IFACE_ACT_MIN_AREA_RECT, (void *)&data);

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

static	void	proc_ROI		(int x, int y, int w, int h)
{
	struct Img_Iface_Data_SetROI		data;
	data.rect.x		= x;
	data.rect.y		= y;
	data.rect.width		= w;
	data.rect.height	= h;
	img_iface_act(IMG_IFACE_ACT_SET_ROI, (void *)&data);
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
		img_iface_show();

		if (proc_debug >= PROC_DBG_STOP_STEP) {
			getchar();
		}
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
