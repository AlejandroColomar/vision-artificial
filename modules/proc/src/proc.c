/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* snprintf() & fflush() */
	#include <stdio.h>
		/* bool */
	#include <stdbool.h>
		/* strcmp() */
	#include <string.h>
		/* clock_t & clock() & CLOCKS_PER_SEC */
	#include <time.h>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <cv.h>
	#include <highgui.h>
		/* ZBAR_EAN13 */
	#include <zbar.h>

/* Project -------------------------------------------------------------------*/
		/* img_iface_act() */
	#include "img_iface.h"
		/* WIN_NAME */
	#include "user_iface.h"

/* Module --------------------------------------------------------------------*/
	#include "proc.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	OK		(0)
	# define	NOK_LABEL	(1)
	# define	NOK_CERDO	(2)
	# define	NOK_BCODE	(3)
	# define	NOK_PRODUCT	(4)
	# define	NOK_PRICE	(5)

	# define	show		(false)
	# define	pause		(false)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
static	struct _IplImage	*imgptr;
static	struct CvMemStorage	*proc_storage;


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
static	void	proc_dilate_erode	(int size);
static	void	proc_contours		(struct CvSeq  **cont, int *n);
static	void	proc_min_area_rect	(struct CvSeq  *cont,
					struct CvBox2D *rect);
static	void	proc_rotate		(struct CvBox2D *rect);
static	void	proc_ROI		(int x, int y, int w, int h);
static	void	proc_crop		(void);
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
	case PROC_MODE_ETIQUETA:
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
	user_iface_log.lvl[user_iface_log.len]	= 2;
	(user_iface_log.len)++;

	return	error;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	int	proc_etiqueta		(void)
{
	int	status;

	char	price [6];

	struct CvSeq		*et_contours;
	int			et_contours_n;
	struct CvBox2D		et_rect;
	int	x;
	int	y;
	int	w;
	int	h;

	proc_storage	= cvCreateMemStorage(0);

	proc_save_mem(0);
	/* Find label (position and angle) */
	{
		proc_cmp(IMG_IFACE_CMP_BLUE);
		proc_smooth(CV_MEDIAN, 7);
		#if 0
			proc_adaptive_threshold(CV_ADAPTIVE_THRESH_MEAN_C,
							CV_THRESH_BINARY, 5);
		#else
			proc_invert();
		#endif
		proc_smooth(CV_BLUR, 21);
		proc_threshold(CV_THRESH_BINARY_INV, 2);
		proc_dilate_erode(100);
		proc_contours(&et_contours, &et_contours_n);

		/* If no contour is found, error:  NOK_LABEL */
		if (!et_contours) {
			status	= NOK_LABEL;
			result_etiqueta(status);
			return	status;
		}

		proc_min_area_rect(et_contours, &et_rect);

		/* If angle is < -45º, it is taking into acount the incorrect side */
		if (et_rect.angle < -45.0) {
			int	tmp;
			et_rect.angle		= et_rect.angle + 90.0;
			tmp			= et_rect.size.width;
			et_rect.size.width	= et_rect.size.height;
			et_rect.size.height	= tmp;
		}
	}
	/* Align label and extract green component */
	{
		proc_load_mem(0);
		proc_rotate(&et_rect);
		proc_cmp(IMG_IFACE_CMP_GREEN);
		proc_save_mem(1);
	}
	/* Find "Cerdo" in aligned image */
	{

		x	= et_rect.center.x - (1.05 * et_rect.size.width / 2);
		y	= et_rect.center.y - (1.47 * et_rect.size.height / 2);
		w	= et_rect.size.width / 2;
		h	= et_rect.size.height * 0.20;
		proc_ROI(x, y, w, h);
		proc_crop();
		proc_threshold(CV_THRESH_BINARY, IMG_IFACE_THR_OTSU);
		proc_OCR(IMG_IFACE_OCR_LANG_SPA, IMG_IFACE_OCR_CONF_NONE);

		/* Compare Label text to "Cerdo". */
		bool	cerdo_nok;
		cerdo_nok	= strncmp(img_ocr_text, "Cerdo",
							strlen("Cerdo"));
		if (cerdo_nok) {
			status	= NOK_CERDO;
			result_etiqueta(status);
			return	status;
		}
	}
	/* Read barcode in original image */
	{
		proc_load_mem(0);
		proc_cmp(IMG_IFACE_CMP_GREEN);
		proc_zbar(ZBAR_EAN13);

		/* Check that 1 and only 1 bcode is read. */
		if (zb_codes.n != 1) {
			status	= NOK_BCODE;
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
			status	= NOK_PRODUCT;
			result_etiqueta(status);
			return	status;
		}
	}
	/* Read price in aligned image (green component) */
	{
		proc_load_mem(1);

		x	= et_rect.center.x + (0.33 * et_rect.size.width / 2);
		y	= et_rect.center.y + (0.64 * et_rect.size.height / 2);
		w	= et_rect.size.width * 0.33;
		h	= et_rect.size.height * 0.15;
		proc_ROI(x, y, w, h);
		proc_crop();
		proc_smooth(CV_BLUR, 3);
		proc_threshold(CV_THRESH_BINARY, IMG_IFACE_THR_OTSU);
//		proc_threshold(CV_THRESH_BINARY, 100);
//		proc_smooth(CV_BLUR, 3);
		proc_OCR(IMG_IFACE_OCR_LANG_ENG, IMG_IFACE_OCR_CONF_PRICE);
	}
	/* Extract price from barcode */
	{
		if (zb_codes.arr[0].data[8] != '0') {
			snprintf(price, 6, "%c%c.%c%c€",
						zb_codes.arr[0].data[8],
						zb_codes.arr[0].data[9],
						zb_codes.arr[0].data[10],
						zb_codes.arr[0].data[11]);
		} else {
			snprintf(price, 6, "%c.%c%c€",
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
			status	= NOK_PRICE;
			result_etiqueta(status);
			return	status;
		}
	}

	status	= OK;
	result_etiqueta(status);
	return	status;
}

static	void	result_etiqueta		(int status)
{
	/* Cleanup */
	cvReleaseMemStorage(&proc_storage);

	/* Write result into log */
	char	result [LOG_LINE_LEN];
	switch (status) {
	case OK:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Label:  OK");
		break;
	case NOK_LABEL:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Label:  NOK_LABEL");
		break;
	case NOK_CERDO:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Label:  NOK_CERDO");
		break;
	case NOK_BCODE:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Label:  NOK_BCODE");
		break;
	case NOK_PRODUCT:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Label:  NOK_PRODUCT");
		break;
	case NOK_PRICE:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Label:  NOK_PRICE");
		break;
	default:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Label:  NOK");
		break;
	}
	user_iface_log.lvl[user_iface_log.len]	= 1;
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

static	void	proc_dilate_erode	(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_DILATE_ERODE, (void *)&data);

	proc_show_img();
}

static	void	proc_contours		(struct CvSeq  **cont, int *n)
{
	struct Img_Iface_Data_Contours		data;
	data.storage	= &proc_storage;
	data.contours	= cont;
	data.n		= n;
	img_iface_act(IMG_IFACE_ACT_CONTOURS, (void *)&data);

	proc_show_img();
}

static	void	proc_min_area_rect	(struct CvSeq  *cont,
					struct CvBox2D *rect)
{
	struct Img_Iface_Data_MinARect		data;
	data.contours	= cont;
	data.rect	= rect;
	img_iface_act(IMG_IFACE_ACT_MIN_AREA_RECT, (void *)&data);

	proc_show_img();
}

static	void	proc_rotate		(struct CvBox2D *rect)
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

static	void	proc_crop		(void)
{
	img_iface_act(IMG_IFACE_ACT_CROP, NULL);

	proc_show_img();
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
	data.code_type	= type;
	img_iface_act(IMG_IFACE_ACT_DECODE, (void *)&data);
}

static	void	proc_show_img		(void)
{
	if (show) {
		imgptr	= img_iface_show();
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);
	}
	if (pause) {
		getchar();
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
