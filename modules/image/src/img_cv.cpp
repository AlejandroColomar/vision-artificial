/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* true & false */
	#include <cstdbool>
		/* snprintf() */
	#include <cstdio>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <opencv2/opencv.hpp>

/* Project -------------------------------------------------------------------*/
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
		/* data */
	#include "img_iface.hpp"

	#include "img_cv.hpp"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Filters */
static	void	img_cv_invert		(class cv::Mat  *imgptr);
static	void	img_cv_bgr2gray		(class cv::Mat  *imgptr);
static	void	img_cv_component	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_smooth		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_threshold	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_adaptive_thr	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_dilate		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_erode		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_contours		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_contours_size	(void  *data);
static	void	img_cv_min_area_rect	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_rotate_orto	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_rotate		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_set_ROI		(class cv::Mat  *imgptr,  void  *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_cv_act	(class cv::Mat  *imgptr, int action, void *data)
{
	switch (action) {
	case IMG_CV_ACT_INVERT:
		img_cv_invert(imgptr);
		break;

	case IMG_CV_ACT_BGR2GRAY:
		img_cv_bgr2gray(imgptr);
		break;
	case IMG_CV_ACT_COMPONENT:
		img_cv_component(imgptr, data);
		break;

	case IMG_CV_ACT_SMOOTH:
		img_cv_smooth(imgptr, data);
		break;

	case IMG_CV_ACT_THRESHOLD:
		img_cv_threshold(imgptr, data);
		break;
	case IMG_CV_ACT_ADAPTIVE_THRESHOLD:
		img_cv_adaptive_thr(imgptr, data);
		break;

	case IMG_CV_ACT_DILATE:
		img_cv_dilate(imgptr, data);
		break;
	case IMG_CV_ACT_ERODE:
		img_cv_erode(imgptr, data);
		break;

	case IMG_CV_ACT_CONTOURS:
		img_cv_contours(imgptr, data);
		break;
	case IMG_CV_ACT_CONTOURS_SIZE:
		img_cv_contours_size(data);
		break;
	case IMG_CV_ACT_MIN_AREA_RECT:
		img_cv_min_area_rect(imgptr, data);
		break;

	case IMG_CV_ACT_ROTATE_ORTO:
		img_cv_rotate_orto(imgptr, data);
		break;
	case IMG_CV_ACT_ROTATE:
		img_cv_rotate(imgptr, data);
		break;

	case IMG_CV_ACT_SET_ROI:
		img_cv_set_ROI(imgptr, data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Filters	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	img_cv_invert		(class cv::Mat  *imgptr)
{
	cv::bitwise_not(*imgptr, *imgptr);
}

static	void	img_cv_bgr2gray		(class cv::Mat  *imgptr)
{
	cv::cvtColor(*imgptr, *imgptr, cv::COLOR_BGR2GRAY, 0);
}

static	void	img_cv_component	(class cv::Mat  *imgptr, void *data)
{
	class cv::Mat	cmp_img[3];

	/* Data */
	struct Img_Iface_Data_Component	*data_cast;
	data_cast	= (struct Img_Iface_Data_Component *)data;

	/* Color component */
	int	cmp;
	cmp	= data_cast->cmp;

	/* Write components into cmp_img[] */
	cv::split(*imgptr, cmp_img);

	/* Write tmp into imgptr */
	imgptr->release();
	cmp_img[cmp].copyTo(*imgptr);

	/* clean up */
	cmp_img[0].release();
	cmp_img[1].release();
	cmp_img[2].release();
}

static	void	img_cv_smooth		(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Smooth	*data_cast;
	data_cast	= (struct Img_Iface_Data_Smooth *)data;

	/* Smoothing method */
	int	method;
	method	= data_cast->method;
	/* Mask size */
	int	msk_siz;
	msk_siz	= data_cast->msk_siz;
	if (!(msk_siz % 2)) {
		msk_siz++;
	}

	switch (method) {
	case IMGI_SMOOTH_MEAN:
		cv::blur(*imgptr, *imgptr, cv::Size(msk_siz, msk_siz),
					cv::Point(-1,-1), cv::BORDER_DEFAULT);
		break;
	case IMGI_SMOOTH_GAUSS:
		cv::GaussianBlur(*imgptr, *imgptr, cv::Size(msk_siz, msk_siz),
					0, 0, cv::BORDER_DEFAULT);
		break;
	case IMGI_SMOOTH_MEDIAN:
		cv::medianBlur(*imgptr, *imgptr, msk_siz);
		break;
	}
}

static	void	img_cv_threshold	(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Threshold	*data_cast;
	data_cast	= (struct Img_Iface_Data_Threshold *)data;

	/* Threshold type */
	int	thr_typ;
	thr_typ	= data_cast->thr_typ;
	/* Threshold value */
	int	thr_val;
	thr_val	= data_cast->thr_val;
	if (thr_val == -1) {
		thr_typ	|= cv::THRESH_OTSU;
	}

	/* Threshold */
	cv::threshold(*imgptr, *imgptr, thr_val, 0xFF, thr_typ);
}

static	void	img_cv_adaptive_thr	(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Adaptive_Thr	*data_cast;
	data_cast	= (struct Img_Iface_Data_Adaptive_Thr *)data;

	/* Threshold method */
	int	method;
	method	= data_cast->method;
	/* Threshold type */
	int	thr_typ;
	thr_typ	= data_cast->thr_typ;
	/* Neighbourhood size */
	int	nbh_val;
	nbh_val	= data_cast->nbh_val;
	if (!(nbh_val % 2)) {
		nbh_val++;
	}

	/* Apply adaptive threshold */
	cv::adaptiveThreshold(*imgptr, *imgptr, 255, method, thr_typ, nbh_val,
									0);
}

static	void	img_cv_dilate		(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Dilate_Erode *)data;

	/* Iterations */
	int	i;
	i	= data_cast->i;

	/* Dilate */
	cv::dilate(*imgptr, *imgptr, cv::Mat(), cv::Point(-1,-1), i,
							cv::BORDER_REPLICATE);
}

static	void	img_cv_erode		(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	data_cast	= (struct Img_Iface_Data_Dilate_Erode *)data;

	/* Iterations */
	int	i;
	i	= data_cast->i;

	/* Erode */
	cv::erode(*imgptr, *imgptr, cv::Mat(), cv::Point(-1,-1), i,
							cv::BORDER_REPLICATE);
}

static	void	img_cv_contours		(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Contours	*data_cast;
	data_cast	= (struct Img_Iface_Data_Contours *)data;

	/* Contours */
	std::vector <std::vector <class cv::Point_ <int>>>	*contours;
	contours	= data_cast->contours;
	class cv::Mat						*hierarchy;
	hierarchy	= data_cast->hierarchy;

	/* Get contours */
	cv::findContours(*imgptr, *contours, *hierarchy, CV_RETR_EXTERNAL,
							CV_CHAIN_APPROX_SIMPLE);

	/* Draw contours in color */
	cv::drawContours(*imgptr, *contours, -1, cv::Scalar(255, 0, 0), 1, 8,
						*hierarchy, 1, cvPoint(0, 0));
}

static	void	img_cv_contours_size	(void *data)
{
	/* Data */
	struct Img_Iface_Data_Contours_Size	*data_cast;
	data_cast	= (struct Img_Iface_Data_Contours_Size *)data;

	/* Contours */
	std::vector <std::vector <class cv::Point_ <int>>>	*contours;
	contours	= data_cast->contours;

	/* Get area and perimeter */
	int	i;
	for (i = 0; i < contours->size(); i++) {
		data_cast->area[i]	= cv::contourArea((*contours)[i], false);
		data_cast->perimeter[i]	= cv::arcLength((*contours)[i], true);
	}
}

static	void	img_cv_min_area_rect	(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_MinARect	*data_cast;
	data_cast	= (struct Img_Iface_Data_MinARect *)data;

	/* Contours */
	std::vector <class cv::Point_ <int>>	*contour;
	contour	= data_cast->contour;
	/* Rotated rectangle */
	class cv::RotatedRect			*rect;
	rect		= data_cast->rect;

	/* Get rectangle */
	*rect	= cv::minAreaRect(*contour);

	/* Draw rectangle */
	class cv::Point_<float>	vertices[4];
	rect->points(vertices);
	cv::line(*imgptr, cv::Point(vertices[0].x, vertices[0].y),
				cv::Point(vertices[1].x, vertices[1].y),
				CV_RGB(0, 0, 255), 1, 8, 0);
	cv::line(*imgptr, cv::Point(vertices[1].x, vertices[1].y),
				cv::Point(vertices[2].x, vertices[2].y),
				CV_RGB(0, 0, 255), 1, 8, 0);
	cv::line(*imgptr, cv::Point(vertices[2].x, vertices[2].y),
				cv::Point(vertices[3].x, vertices[3].y),
				CV_RGB(0, 0, 255), 1, 8, 0);
	cv::line(*imgptr, cv::Point(vertices[3].x, vertices[3].y),
				cv::Point(vertices[0].x, vertices[0].y),
				CV_RGB(0, 0, 255), 1, 8, 0);
}

static	void	img_cv_rotate_orto	(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Rotate_Orto	*data_cast;
	data_cast	= (struct Img_Iface_Data_Rotate_Orto *)data;

	/* Rotate n * pi/2 rad */
	int	n;
	n	= data_cast->n;

	switch (n) {
	case 1:
		/* Rotate: transpose and flip around horizontal axis: flip_mode=0 */
		cv::transpose(*imgptr, *imgptr);
		cv::flip(*imgptr, *imgptr, 0);
		break;

	case 2:
		/* Rotate: Flip both axises: flip_mode=-1 */
		cv::flip(*imgptr, *imgptr, -1);
		break;

	case 3:
		/* Rotate: transpose and flip around vertical axis: flip_mode=1 */
		cv::transpose(*imgptr, *imgptr);
		cv::flip(*imgptr, *imgptr, 1);
		break;
	}
}

static	void	img_cv_rotate		(class cv::Mat  *imgptr, void *data)
{
	class cv::Mat	map_matrix;

	/* Data */
	struct Img_Iface_Data_Rotate	*data_cast;
	data_cast	= (struct Img_Iface_Data_Rotate *)data;

	/* Angle of rotation */
	class cv::Point_ <float>	*center;
	center		= &(data_cast->center);
	double				angle;
	angle		= data_cast->angle;

	/* Get map_matrix */
	map_matrix	= cv::getRotationMatrix2D(*center, angle, 1);

	/* Rotate */
	cv::warpAffine(*imgptr, *imgptr, map_matrix, imgptr->size(),
				cv::INTER_LINEAR, cv::BORDER_CONSTANT,
				cv::Scalar(0, 0, 0));

	/* clean up */
	map_matrix.release();
}

static	void	img_cv_set_ROI		(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_SetROI	*data_cast;
	data_cast	= (struct Img_Iface_Data_SetROI *)data;

	/* Rectangle */
	class cv::Rect_ <int>	*rect;
	rect		= &(data_cast->rect);

	/* Set ROI */
	*imgptr	= (*imgptr)(*rect);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
