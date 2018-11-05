/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* fabs */
	#include <cmath>
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
static	void	img_cv_not		(class cv::Mat  *imgptr);
static	void	img_cv_or_2ref		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_and_2ref		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_cvt_color	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_component	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_histogram	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_histogram_c3	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_smooth		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_sobel		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_threshold	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_adaptive_thr	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_dilate		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_erode		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_contours		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_contours_size	(void  *data);
static	void	img_cv_min_area_rect	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_fit_ellipse	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_rotate_orto	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_rotate		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_set_ROI		(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_pixel_value	(class cv::Mat  *imgptr,  void  *data);
static	void	img_cv_distance_transform	(class cv::Mat  *imgptr);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_cv_act	(class cv::Mat  *imgptr, int action, void *data)
{
	switch (action) {
	case IMG_CV_ACT_NOT:
		img_cv_not(imgptr);
		break;
	case IMG_CV_ACT_OR_2REF:
		img_cv_or_2ref(imgptr, data);
		break;
	case IMG_CV_ACT_AND_2REF:
		img_cv_and_2ref(imgptr, data);
		break;

	case IMG_CV_ACT_CVT_COLOR:
		img_cv_cvt_color(imgptr, data);
		break;
	case IMG_CV_ACT_COMPONENT:
		img_cv_component(imgptr, data);
		break;

	case IMG_CV_ACT_HISTOGRAM:
		img_cv_histogram(imgptr, data);
		break;
	case IMG_CV_ACT_HISTOGRAM_C3:
		img_cv_histogram_c3(imgptr, data);
		break;
	case IMG_CV_ACT_SMOOTH:
		img_cv_smooth(imgptr, data);
		break;
	case IMG_CV_ACT_SOBEL:
		img_cv_sobel(imgptr, data);
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
	case IMG_CV_ACT_FIT_ELLIPSE:
		img_cv_fit_ellipse(imgptr, data);
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

	case IMG_CV_ACT_PIXEL_VALUE:
		img_cv_pixel_value(imgptr, data);
		break;

	case IMG_CV_ACT_DISTANCE_TRANSFORM:
		img_cv_distance_transform(imgptr);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_cv_not		(class cv::Mat  *imgptr)
{
	cv::bitwise_not(*imgptr, *imgptr);
}

static	void	img_cv_or_2ref		(class cv::Mat  *imgptr, void *data)
{
	class cv::Mat	*img_ref;
	img_ref	= (class cv::Mat *)data;

	cv::bitwise_or(*imgptr, *img_ref, *imgptr);
}

static	void	img_cv_and_2ref		(class cv::Mat  *imgptr, void *data)
{
	class cv::Mat	*img_ref;
	img_ref	= (class cv::Mat *)data;

	cv::bitwise_and(*imgptr, *img_ref, *imgptr);
}

static	void	img_cv_cvt_color	(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Cvt_Color	*data_cast;
	data_cast	= (struct Img_Iface_Data_Cvt_Color *)data;

	/* Conversion method */
	int	method;
	method	= data_cast->method;

	cv::cvtColor(*imgptr, *imgptr, method, 0);
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

static	void	img_cv_histogram	(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Histogram	*data_cast;
	data_cast	= (struct Img_Iface_Data_Histogram *)data;

	/* Contours */
	class cv::Mat	*hist;
	hist		= data_cast->hist_c0;
	class cv::Mat	*hist_img;
	hist_img	= data_cast->hist_img;

	/* Write components into cmp_img[] */
	int		h_size		= 256;
	float		h_range_arr[]	= {0.0, 256.0};
	const float	*h_range	= {h_range_arr};
	cv::calcHist(imgptr, 1, 0, cv::Mat(), *hist, 1, &h_size, &h_range,
								true, false);

	/* Init */
	hist_img->setTo(cv::Scalar(0));

	/* Normalize the result to [0, hist_img->rows - 1] */
	cv::normalize(*hist, *hist, 0, hist_img->rows - 1, cv::NORM_MINMAX, -1,
								cv::Mat());

	/* Draw hist into hist_img */
	int	i;
	for(i = 0; i < 256; i++ ) {
		cv::line(*hist_img, cv::Point(i, hist_img->rows - 0),
				cv::Point(i, hist_img->rows - hist->at<float>(i)),
				cv::Scalar(255, 0, 0), 1, 8, 0);
	}

	/* Show histogram */
	img_iface_show_hist_c1();
}

static	void	img_cv_histogram_c3	(class cv::Mat  *imgptr, void *data)
{
	class cv::Mat	cmp_img[3];

	/* Write components into cmp_img[] */
	cv::split(*imgptr, cmp_img);

	/* Data */
	struct Img_Iface_Data_Histogram	*data_cast;
	data_cast	= (struct Img_Iface_Data_Histogram *)data;

	/* Contours */
	class cv::Mat	*hist_c0;
	hist_c0		= data_cast->hist_c0;
	class cv::Mat	*hist_c1;
	hist_c1		= data_cast->hist_c1;
	class cv::Mat	*hist_c2;
	hist_c2		= data_cast->hist_c2;
	class cv::Mat	*hist_img;
	hist_img	= data_cast->hist_img;

	/* Write components into cmp_img[] */
	int		h_size		= 256;
	float		h_range_arr[]	= {0.0, 256.0};
	const float	*h_range	= {h_range_arr};
	cv::calcHist(&cmp_img[0], 1, 0, cv::Mat(), *hist_c0, 1, &h_size,
							&h_range, true, false);
	cv::calcHist(&cmp_img[1], 1, 0, cv::Mat(), *hist_c1, 1, &h_size,
							&h_range, true, false);
	cv::calcHist(&cmp_img[2], 1, 0, cv::Mat(), *hist_c2, 1, &h_size,
							&h_range, true, false);

	/* Init */
	hist_img->setTo(cv::Scalar(0));

	/* Normalize the result to [0, hist_img->rows - 1] */
	cv::normalize(*hist_c0, *hist_c0, 0, hist_img->rows - 1,
						cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(*hist_c1, *hist_c1, 0, hist_img->rows - 1,
						cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(*hist_c2, *hist_c2, 0, hist_img->rows - 1,
						cv::NORM_MINMAX, -1, cv::Mat());

	/* Draw hist into hist_img */
	int	i;
	int	j;
	for(i = 0; i < 256; i++ ) {
		cv::line(*hist_img, cv::Point(3*i, hist_img->rows - 0),
				cv::Point(3*i,
				hist_img->rows - hist_c0->at<float>(i)),
				cv::Scalar(255, 0, 0), 1, 8, 0);
	}
	for(i = 0; i < 256; i++ ) {
		cv::line(*hist_img, cv::Point(3*i + 1, hist_img->rows - 0),
				cv::Point(3*i+1,
				hist_img->rows - hist_c1->at<float>(i)),
				cv::Scalar(0, 255, 0), 1, 8, 0);
	}
	for(i = 0; i < 256; i++ ) {
		cv::line(*hist_img, cv::Point(3*i + 2, hist_img->rows - 0),
				cv::Point(3*i + 2,
				hist_img->rows - hist_c2->at<float>(i)),
				cv::Scalar(0, 0, 255), 1, 8, 0);
	}

	/* Show histogram */
	img_iface_show_hist_c3();

	/* Cleanup */
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
	/* Kernel size */
	int	ksize;
	ksize	= data_cast->ksize;
	if (!(ksize % 2)) {
		ksize++;
	}

	switch (method) {
	case IMGI_SMOOTH_MEAN:
		cv::blur(*imgptr, *imgptr, cv::Size(ksize, ksize),
					cv::Point(-1,-1), cv::BORDER_DEFAULT);
		break;
	case IMGI_SMOOTH_GAUSS:
		cv::GaussianBlur(*imgptr, *imgptr, cv::Size(ksize, ksize),
					0, 0, cv::BORDER_DEFAULT);
		break;
	case IMGI_SMOOTH_MEDIAN:
		cv::medianBlur(*imgptr, *imgptr, ksize);
		break;
	}
}

static	void	img_cv_sobel		(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Sobel	*data_cast;
	data_cast	= (struct Img_Iface_Data_Sobel *)data;

	/* Order of the derivative x */
	int	dx;
	dx	= data_cast->dx;
	/* Order of the derivative y */
	int	dy;
	dy	= data_cast->dy;
	/* Size of the extended Sobel kernel */
	int	ksize;
	ksize	= data_cast->ksize;
	if (!(ksize % 2)) {
		ksize++;
	}

	cv::Sobel(*imgptr, *imgptr, -1, dx, dy, ksize, 1, 0,
							cv::BORDER_DEFAULT);
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
	int	ksize;
	ksize	= data_cast->ksize;
	if (!(ksize % 2)) {
		ksize++;
	}

	/* Apply adaptive threshold */
	cv::adaptiveThreshold(*imgptr, *imgptr, 255, method, thr_typ, ksize,
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
		data_cast->area[i]	= cv::contourArea(
							(*contours)[i], false);
		data_cast->perimeter[i]	= cv::arcLength(
							(*contours)[i], true);
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
	rect	= data_cast->rect;
	/* Show rectangle ? */
	bool					show;
	show	= data_cast->show;

	/* Get rectangle */
	*rect	= cv::minAreaRect(*contour);

	/* Draw rectangle */
	class cv::Point_<float>	vertices[4];
	if (show) {
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
}

static	void	img_cv_fit_ellipse	(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_MinARect	*data_cast;
	data_cast	= (struct Img_Iface_Data_MinARect *)data;

	/* Contours */
	std::vector <class cv::Point_ <int>>	*contour;
	contour	= data_cast->contour;
	/* Rotated rectangle */
	class cv::RotatedRect			*rect;
	rect	= data_cast->rect;

	/* Get rectangle */
	*rect	= cv::fitEllipse(*contour);

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

	/* Don't rotate if angle is negligible */
	if (fabs(angle) > 1.0) {
		/* Get map_matrix */
		map_matrix	= cv::getRotationMatrix2D(*center, angle, 1);

		/* Rotate */
		cv::warpAffine(*imgptr, *imgptr, map_matrix, imgptr->size(),
					cv::INTER_LINEAR, cv::BORDER_CONSTANT,
					cv::Scalar(0, 0, 0));
	}

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

static	void	img_cv_pixel_value	(class cv::Mat  *imgptr, void *data)
{
	/* Data */
	struct Img_Iface_Data_Pixel_Value	*data_cast;
	data_cast	= (struct Img_Iface_Data_Pixel_Value *)data;

	/* Value */
	unsigned char	*val;
	val		= data_cast->val;
	/* Position */
	int		x;
	x		= data_cast->x;
	int		y;
	y		= data_cast->y;

	/* Get value */
	*val	= imgptr->at<unsigned char>(y, x);
}

static	void	img_cv_distance_transform	(class cv::Mat  *imgptr)
{
	class cv::Mat	imgtmp;

	/* Get transform */
	cv::distanceTransform(*imgptr, imgtmp, CV_DIST_L2, CV_DIST_MASK_PRECISE);

	/* DistanceTransform  gives CV_32F image */
	imgtmp.convertTo(*imgptr, CV_8U);

	/* Cleanup */
	imgtmp.release();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
