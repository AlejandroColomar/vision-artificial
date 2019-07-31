/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/proc/common.hpp"

#include <cstddef>
#include <cstdio>
#include <ctime>

#include <opencv2/core/core.hpp>

#include "libalx/base/stdio/seekc.hpp"

#include "vision-artificial/image/iface.hpp"
#include "vision-artificial/proc/iface.hpp"
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
static	clock_t	clock_0;
static	clock_t	clock_1;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	proc_apply		(void)
{
	img_iface_act(IMG_IFACE_ACT_APPLY);
}

void	proc_save_mem		(ptrdiff_t n)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_MEM/*, (const void *)&n*/);
}

void	proc_load_mem		(ptrdiff_t n)
{

	img_iface_act(IMG_IFACE_ACT_LOAD_MEM/*, (const void *)&n*/);

	proc_show_img();
}

void	proc_save_ref		(void)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_REF);
}

void	proc_save_file		(void)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_FILE);
}

void	proc_save_update	(void)
{
	img_iface_act(IMG_IFACE_ACT_SAVE_UPDT);
}

void	proc_local_max		(void)
{

	img_iface_act(IMG_IFACE_ACT_LOCAL_MAX);
	proc_show_img();
}

void	proc_skeleton		(void)
{

	img_iface_act(IMG_IFACE_ACT_SKELETON);
	proc_show_img();
}

void	proc_lines_vertical	(void)
{

	img_iface_act(IMG_IFACE_ACT_LINES_VERTICAL);
	proc_show_img();
}

void	proc_median_horizontal	(void)
{

	img_iface_act(IMG_IFACE_ACT_MEDIAN_HORIZONTAL);
	proc_show_img();
}

void	proc_median_vertical	(void)
{

	img_iface_act(IMG_IFACE_ACT_MEDIAN_VERTICAL);
	proc_show_img();
}

void	proc_pixel_get		(ptrdiff_t x, ptrdiff_t y, unsigned char *val)
{

	img_iface_act(IMG_IFACE_ACT_PIXEL_GET/*, (const void *)&data*/);
}

void	proc_pixel_set		(ptrdiff_t x, ptrdiff_t y, unsigned char val)
{

	img_iface_act(IMG_IFACE_ACT_PIXEL_SET/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_ROI		(int x, int y, int w, int h)
{

	img_iface_act(IMG_IFACE_ACT_SET_ROI/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_and_2ref		(void)
{

	img_iface_act(USER_IFACE_ACT_AND_2REF);
	proc_show_img();
}

void	proc_not		(void)
{

	img_iface_act(USER_IFACE_ACT_NOT);
	proc_show_img();
}

void	proc_or_2ref		(void)
{

	img_iface_act(USER_IFACE_ACT_OR_2REF);
	proc_show_img();
}

void	proc_cmp		(int cmp)
{

	img_iface_act(IMG_IFACE_ACT_COMPONENT/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_dilate		(int size)
{

	img_iface_act(IMG_IFACE_ACT_DILATE/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_erode		(int size)
{

	img_iface_act(IMG_IFACE_ACT_ERODE/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_dilate_erode	(int size)
{

	img_iface_act(IMG_IFACE_ACT_DILATE_ERODE/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_erode_dilate	(int size)
{

	img_iface_act(IMG_IFACE_ACT_ERODE_DILATE/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_smooth		(int method, int ksize)
{

	img_iface_act(IMG_IFACE_ACT_SMOOTH/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_border		(int size)
{

	img_iface_act(IMG_IFACE_ACT_BORDER/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_rotate		(float center_x, float center_y, double angle)
{

	img_iface_act(IMG_IFACE_ACT_ROTATE/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_adaptive_threshold	(int method, int type, int ksize)
{

	img_iface_act(USER_IFACE_ACT_ADAPTIVE_THRESHOLD/*, (void *)&data*/);
	proc_show_img();
}

void	proc_cvt_color		(int method)
{

	img_iface_act(IMG_IFACE_ACT_CVT_COLOR/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_threshold		(int type, int size)
{

	img_iface_act(IMG_IFACE_ACT_THRESHOLD/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_distance_transform	(void)
{

	img_iface_act(IMG_IFACE_ACT_DISTANCE_TRANSFORM);
	proc_show_img();
}

void	proc_contours(
	class std::vector <class std::vector <class cv::Point_<int>>> *contours,
	class cv::Mat  *hierarchy)
{

	img_iface_act(IMG_IFACE_ACT_CONTOURS/*, (const void *)&data*/);
	proc_show_img();
}

void	proc_contours_size(
	const class std::vector <class std::vector <class cv::Point_ <int>>> *contours,
	double *area,
	double *perimeter)
{

	img_iface_act(IMG_IFACE_ACT_CONTOURS_SIZE/*, (const void *)&data*/);
}

void	proc_bounding_rect(
	const class std::vector <class cv::Point_ <int>>  *contour,
	class cv::Rect_ <int>  *rect,
	bool show)
{

	img_iface_act(IMG_IFACE_ACT_BOUNDING_RECT/*, (const void *)&data*/);
	if (show)
		proc_show_img();
}

void	proc_fit_ellipse	(
	const class std::vector <class cv::Point_ <int>>  *contour,
	class cv::RotatedRect  *rect,
	bool show
)
{

	img_iface_act(IMG_IFACE_ACT_FIT_ELLIPSE/*, (const void *)&data*/);
	if (show)
		proc_show_img();
}

void	proc_min_area_rect	(
	const class std::vector <class cv::Point_ <int>>  *contour,
	class cv::RotatedRect  *rect,
	bool  show
)
{

	img_iface_act(IMG_IFACE_ACT_MIN_AREA_RECT/*, (const void *)&data*/);
	if (show)
		proc_show_img();
}

void	proc_OCR		(int lang, int conf)
{
//	struct Img_Iface_Data_Read		data;

//	data.lang	= lang,	/* eng=0, spa=1, cat=2 */
//	data.conf	= conf,	/* none=0, price=1 */
	img_iface_act(IMG_IFACE_ACT_READ/*, (const void *)&data*/);
}

void	proc_zbar		(int type)
{
	struct Img_Iface_Data_Decode		data;

	data	= (struct Img_Iface_Data_Decode){
		.code_type	= type,
	};
	img_iface_act(IMG_IFACE_ACT_DECODE/*, (const void *)&data*/);
}

void	proc_show_img		(void)
{

	if (proc_debug < PROC_DBG_DELAY_STEP)
		return;
	img_iface_show_img();
	if (proc_debug >= PROC_DBG_STOP_STEP)
		alx_wait4enter();
}

void	clock_start		(void)
{
	clock_0	= clock();
}

void	clock_stop		(const char  *txt)
{
	clock_t	clock_diff;
	double	time_diff;
	char	tmp[LOG_LINE_LEN];

	clock_1		= clock();
	clock_diff	= clock_1 - clock_0;
	time_diff	= (double)clock_diff / (double)CLOCKS_PER_SEC;

	snprintf(tmp, LOG_LINE_LEN, "Time:  %.3lf  (%s)", time_diff, txt);
	user_iface_log_write(0, tmp);
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
