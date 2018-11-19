/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* snprintf() & fflush() */
	#include <cstdio>
		/* clock_t & clock() & CLOCKS_PER_SEC */
	#include <ctime>

/* Packages ------------------------------------------------------------------*/
		/* openCV */
	#include <opencv2/opencv.hpp>
		/* enum zbar::zbar_symbol_type_e */
	#include <zbar.h>

/* Project -------------------------------------------------------------------*/
		/* img_iface_act() */
	#include "img_iface.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
		/* enum Proc_DBG */
	#include "proc_iface.hpp"

	#include "proc_common.hpp"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/

/* Static --------------------------------------------------------------------*/
static	clock_t	clock_0;
static	clock_t	clock_1;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	proc_apply		(void)
{
	img_iface_act(IMG_IFACE_ACT_APPLY, NULL);
}

void	proc_save_mem		(int n)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_MEM, (void *)&n);
}

void	proc_load_mem		(int n)
{
	img_iface_act(IMG_IFACE_ACT_LOAD_MEM, (void *)&n);

	proc_show_img();
}

void	proc_save_ref		(void)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_REF, NULL);
}

void	proc_save_file		(void)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_FILE, NULL);
}

void	proc_local_max		(void)
{
	img_iface_act(IMG_IFACE_ACT_LOCAL_MAX, NULL);

	proc_show_img();
}

void	proc_lines_vertical	(void)
{
	img_iface_act(IMG_IFACE_ACT_LINES_VERTICAL, NULL);

	proc_show_img();
}

void	proc_median_horizontal	(void)
{
	img_iface_act(IMG_IFACE_ACT_MEDIAN_HORIZONTAL, NULL);

	proc_show_img();
}

void	proc_median_vertical	(void)
{
	img_iface_act(IMG_IFACE_ACT_MEDIAN_VERTICAL, NULL);

	proc_show_img();
}

void	proc_pixel_value	(int x, int y, unsigned char *val)
{
	struct Img_Iface_Data_Pixel_Value	data;
	data.x		= x;
	data.y		= y;
	data.val	= val;
	img_iface_act(IMG_IFACE_ACT_PIXEL_VALUE, (void *)&data);
}

void	proc_ROI		(int x, int y, int w, int h)
{
	struct Img_Iface_Data_SetROI		data;
	data.rect.x		= x;
	data.rect.y		= y;
	data.rect.width		= w;
	data.rect.height	= h;
	img_iface_act(IMG_IFACE_ACT_SET_ROI, (void *)&data);

	proc_show_img();
}

void	proc_and_2ref		(void)
{
	img_iface_act(USER_IFACE_ACT_AND_2REF, NULL);

	proc_show_img();
}

void	proc_not		(void)
{
	img_iface_act(USER_IFACE_ACT_NOT, NULL);

	proc_show_img();
}

void	proc_or_2ref		(void)
{
	img_iface_act(USER_IFACE_ACT_OR_2REF, NULL);

	proc_show_img();
}

void	proc_cmp		(int cmp)
{
	struct Img_Iface_Data_Component		data;
	data.cmp	= cmp;
	img_iface_act(IMG_IFACE_ACT_COMPONENT, (void *)&data);

	proc_show_img();
}

void	proc_dilate		(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_DILATE, (void *)&data);

	proc_show_img();
}

void	proc_erode		(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_ERODE, (void *)&data);

	proc_show_img();
}

void	proc_dilate_erode	(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_DILATE_ERODE, (void *)&data);

	proc_show_img();
}

void	proc_erode_dilate	(int size)
{
	struct Img_Iface_Data_Dilate_Erode	data;
	data.i	= size;
	img_iface_act(IMG_IFACE_ACT_ERODE_DILATE, (void *)&data);

	proc_show_img();
}

void	proc_smooth		(int method, int ksize)
{
	struct Img_Iface_Data_Smooth		data;
	data.method	= method;
	data.ksize	= ksize;
	img_iface_act(IMG_IFACE_ACT_SMOOTH, (void *)&data);

	proc_show_img();
}

void	proc_rotate		(class cv::RotatedRect  *rect)
{
	struct Img_Iface_Data_Rotate		data;
	data.center.x	= rect->center.x;
	data.center.y	= rect->center.y;
	data.angle	= rect->angle;
	img_iface_act(IMG_IFACE_ACT_ROTATE, (void *)&data);

	proc_show_img();
}

void	proc_adaptive_threshold	(int method, int type, int ksize)
{
	struct Img_Iface_Data_Adaptive_Thr	data;
	data.method	= method;
	data.thr_typ	= type;
	data.ksize	= ksize;
	img_iface_act(USER_IFACE_ACT_ADAPTIVE_THRESHOLD, (void *)&data);

	proc_show_img();
}

void	proc_cvt_color		(int method)
{
	struct Img_Iface_Data_Cvt_Color		data;
	data.method	= method;
	img_iface_act(IMG_IFACE_ACT_CVT_COLOR, (void *)&data);

	proc_show_img();
}

void	proc_threshold		(int type, int size)
{
	struct Img_Iface_Data_Threshold		data;
	data.thr_typ	= type;
	data.thr_val	= size;
	img_iface_act(IMG_IFACE_ACT_THRESHOLD, (void *)&data);

	proc_show_img();
}

void	proc_distance_transform	(void)
{
	img_iface_act(IMG_IFACE_ACT_DISTANCE_TRANSFORM, NULL);

	proc_show_img();
}

void	proc_contours		(
			std::vector <std::vector <class cv::Point_ <int>>>  *contours,
			class cv::Mat  *hierarchy)
{
	struct Img_Iface_Data_Contours		data;
	data.contours	= contours;
	data.hierarchy	= hierarchy;
	img_iface_act(IMG_IFACE_ACT_CONTOURS, (void *)&data);

	proc_show_img();
}

void	proc_contours_size		(
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

void	proc_bounding_rect	(
			std::vector <class cv::Point_ <int>>  *contour,
			class cv::Rect_ <int>  *rect,
			bool  show)
{
	struct Img_Iface_Data_Bounding_Rect	data;
	data.contour	= contour;
	data.rect	= rect;
	data.show	= show;
	img_iface_act(IMG_IFACE_ACT_BOUNDING_RECT, (void *)&data);

	if (show) {
		proc_show_img();
	}
}

void	proc_fit_ellipse	(
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

void	proc_min_area_rect	(
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

void	proc_OCR		(int lang, int conf)
{
	struct Img_Iface_Data_Read		data;
	data.lang	= lang;	/* eng=0, spa=1, cat=2 */
	data.conf	= conf;	/* none=0, price=1 */
	img_iface_act(IMG_IFACE_ACT_READ, (void *)&data);
}

void	proc_zbar		(int type)
{
	struct Img_Iface_Data_Decode		data;
	data.code_type	= (enum zbar::zbar_symbol_type_e)type;
	img_iface_act(IMG_IFACE_ACT_DECODE, (void *)&data);
}

void	proc_show_img		(void)
{
	if (proc_debug >= PROC_DBG_DELAY_STEP) {
		img_iface_show_img();

		if (proc_debug >= PROC_DBG_STOP_STEP) {
			getchar();
		}
	}
}

void	clock_start		(void)
{
	clock_0	= clock();
}

void	clock_stop		(const char  *txt)
{
	clock_t	clock_diff;
	double	time_diff;

	clock_1		= clock();
	clock_diff	= clock_1 - clock_0;
	time_diff	= (double)clock_diff / (double)CLOCKS_PER_SEC;

	/* Write time_diff into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"%s: Time:  %.3lf",
							txt, time_diff);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
