/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_PROC_COMMON_HPP
	# define	VA_PROC_COMMON_HPP


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* C wrapper ************************************************************
 ******************************************************************************/
extern	"C" {


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
void	proc_apply		(void);
void	proc_save_mem		(int n);
void	proc_load_mem		(int n);
void	proc_save_ref		(void);
void	proc_save_file		(void);
void	proc_save_update	(void);

void	proc_local_max		(void);
void	proc_skeleton		(void);
void	proc_lines_vertical	(void);
void	proc_median_horizontal	(void);
void	proc_median_vertical	(void);

void	proc_pixel_get		(int x, int y, unsigned char *val);
void	proc_pixel_set		(int x, int y, unsigned char val);
void	proc_ROI		(int x, int y, int w, int h);
void	proc_and_2ref		(void);
void	proc_not		(void);
void	proc_or_2ref		(void);
void	proc_cmp		(int cmp);
void	proc_dilate		(int size);
void	proc_erode		(int size);
void	proc_dilate_erode	(int size);
void	proc_erode_dilate	(int size);
void	proc_smooth		(int method, int ksize);
void	proc_border		(int size);
void	proc_rotate		(double center_x, double center_y, double angle);
void	proc_adaptive_threshold	(int method, int type, int ksize);
void	proc_cvt_color		(int method);
void	proc_distance_transform	(void);
void	proc_threshold		(int type, int ksize);
void	proc_contours		(
		std::vector <std::vector <class cv::Point_ <int>>>  *contours,
		class cv::Mat  *hierarchy);
void	proc_contours_size	(
		std::vector <std::vector <class cv::Point_ <int>>>  *contours,
		double  *area,
		double  *perimeter);
void	proc_bounding_rect	(
		std::vector <class cv::Point_ <int>>  *contour,
		class cv::Rect_ <int>  *rect,
		bool  show);
void	proc_fit_ellipse	(
		std::vector <class cv::Point_ <int>>  *contour,
		class cv::RotatedRect  *rect,
		bool show);
void	proc_min_area_rect	(
		std::vector <class cv::Point_ <int>>  *contour,
		class cv::RotatedRect  *rect,
		bool show);

void	proc_OCR		(int lang, int conf);
void	proc_zbar		(int type);

void	proc_show_img		(void);

void	clock_start		(void);
void	clock_stop		(const char  *txt);


/******************************************************************************
 ******* C wrapper ************************************************************
 ******************************************************************************/
}	/* extern "C" */


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* proc_common.hpp */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
