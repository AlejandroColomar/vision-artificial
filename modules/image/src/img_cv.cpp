/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C++ --------------------------------------------------------------*/
	#include <vector>
/* Standard C ----------------------------------------------------------------*/
	#include <cmath>
	#include <cstdbool>
	#include <cstdio>
/* Packages ------------------------------------------------------------------*/
	#include <opencv2/opencv.hpp>
/* Module --------------------------------------------------------------------*/
	#include "img_iface.hpp"

	#include "img_cv.hpp"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Core: The core functionality */
		/* Pixel */
static	void	img_cv_pixel_get	(const class cv::Mat *imgptr, const void *data);
static	void	img_cv_pixel_set	(class cv::Mat *imgptr, const void *data);
		/* ROI */
static	void	img_cv_set_ROI		(class cv::Mat *imgptr, const void *data);
		/* Operations on Arrays */
static	void	img_cv_and_2ref		(class cv::Mat *imgptr, const void *data);
static	void	img_cv_not		(class cv::Mat *imgptr);
static	void	img_cv_or_2ref		(class cv::Mat *imgptr, const void *data);
static	void	img_cv_component	(class cv::Mat *imgptr, const void *data);
	/* Imgproc: Image processing */
		/* Image filtering */
static	void	img_cv_dilate		(class cv::Mat *imgptr, const void *data);
static	void	img_cv_erode		(class cv::Mat *imgptr, const void *data);
static	void	img_cv_smooth		(class cv::Mat *imgptr, const void *data);
static	void	img_cv_sobel		(class cv::Mat *imgptr, const void *data);
static	void	img_cv_border		(class cv::Mat *imgptr, const void *data);
		/* Geometric image transformations */
static	void	img_cv_mirror		(class cv::Mat *imgptr, const void *data);
static	void	img_cv_rotate_orto	(class cv::Mat *imgptr, const void *data);
static	void	img_cv_rotate		(class cv::Mat *imgptr, const void *data);
		/* Miscellaneous image transformations */
static	void	img_cv_adaptive_thr	(class cv::Mat *imgptr, const void *data);
static	void	img_cv_cvt_color	(class cv::Mat *imgptr, const void *data);
static	void	img_cv_distance_transform	(class cv::Mat *imgptr);
static	void	img_cv_threshold	(class cv::Mat *imgptr, const void *data);
		/* Histograms */
static	void	img_cv_histogram	(class cv::Mat *imgptr, const void *data);
static	void	img_cv_histogram_c3	(class cv::Mat *imgptr, const void *data);
		/* Structural analysis and shape descriptors */
static	void	img_cv_contours		(class cv::Mat *imgptr, const void *data);
static	void	img_cv_contours_size	(const void *data);
static	void	img_cv_bounding_rect	(class cv::Mat *imgptr, const void *data);
static	void	img_cv_fit_ellipse	(class cv::Mat *imgptr, const void *data);
static	void	img_cv_min_area_rect	(class cv::Mat *imgptr, const void *data);
		/* Feature detection */
static	void	img_cv_hough_circles	(class cv::Mat *imgptr, const void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_cv_act	(class cv::Mat  *imgptr, int action, const void *data)
{
	switch (action) {
	case IMG_CV_ACT_PIXEL_GET:
		img_cv_pixel_get(imgptr, data);
		break;
	case IMG_CV_ACT_PIXEL_SET:
		img_cv_pixel_set(imgptr, data);
		break;

	case IMG_CV_ACT_SET_ROI:
		img_cv_set_ROI(imgptr, data);
		break;

	case IMG_CV_ACT_AND_2REF:
		img_cv_and_2ref(imgptr, data);
		break;
	case IMG_CV_ACT_NOT:
		img_cv_not(imgptr);
		break;
	case IMG_CV_ACT_OR_2REF:
		img_cv_or_2ref(imgptr, data);
		break;
	case IMG_CV_ACT_COMPONENT:
		img_cv_component(imgptr, data);
		break;

	case IMG_CV_ACT_DILATE:
		img_cv_dilate(imgptr, data);
		break;
	case IMG_CV_ACT_ERODE:
		img_cv_erode(imgptr, data);
		break;
	case IMG_CV_ACT_SMOOTH:
		img_cv_smooth(imgptr, data);
		break;
	case IMG_CV_ACT_SOBEL:
		img_cv_sobel(imgptr, data);
		break;
	case IMG_CV_ACT_BORDER:
		img_cv_border(imgptr, data);
		break;

	case IMG_CV_ACT_MIRROR:
		img_cv_mirror(imgptr, data);
		break;
	case IMG_CV_ACT_ROTATE_ORTO:
		img_cv_rotate_orto(imgptr, data);
		break;
	case IMG_CV_ACT_ROTATE:
		img_cv_rotate(imgptr, data);
		break;

	case IMG_CV_ACT_ADAPTIVE_THRESHOLD:
		img_cv_adaptive_thr(imgptr, data);
		break;
	case IMG_CV_ACT_CVT_COLOR:
		img_cv_cvt_color(imgptr, data);
		break;
	case IMG_CV_ACT_DISTANCE_TRANSFORM:
		img_cv_distance_transform(imgptr);
		break;
	case IMG_CV_ACT_THRESHOLD:
		img_cv_threshold(imgptr, data);
		break;

	case IMG_CV_ACT_HISTOGRAM:
		img_cv_histogram(imgptr, data);
		break;
	case IMG_CV_ACT_HISTOGRAM_C3:
		img_cv_histogram_c3(imgptr, data);
		break;

	case IMG_CV_ACT_CONTOURS:
		img_cv_contours(imgptr, data);
		break;
	case IMG_CV_ACT_CONTOURS_SIZE:
		img_cv_contours_size(data);
		break;
	case IMG_CV_ACT_BOUNDING_RECT:
		img_cv_bounding_rect(imgptr, data);
		break;
	case IMG_CV_ACT_FIT_ELLIPSE:
		img_cv_fit_ellipse(imgptr, data);
		break;
	case IMG_CV_ACT_MIN_AREA_RECT:
		img_cv_min_area_rect(imgptr, data);
		break;

	case IMG_CV_ACT_HOUGH_CIRCLES:
		img_cv_hough_circles(imgptr, data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/* Core: The core functionality */
/* ----- Pixel */
static	void	img_cv_pixel_get	(const class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Pixel_Get	*data_cast;
	unsigned char	*val;
	int		x;
	int		y;

	data_cast	= (const struct Img_Iface_Data_Pixel_Get *)data;
	val		= data_cast->val;
	x		= data_cast->x;
	y		= data_cast->y;

	*val	= imgptr->at<unsigned char>(y, x);
}

static	void	img_cv_pixel_set	(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Pixel_Set	*data_cast;
	unsigned char	val;
	int		x;
	int		y;

	data_cast	= (const struct Img_Iface_Data_Pixel_Set *)data;
	val	= data_cast->val;
	x	= data_cast->x;
	y	= data_cast->y;

	imgptr->at<unsigned char>(y, x)	= val;
}

/* ----- ROI */
static	void	img_cv_set_ROI		(class cv::Mat *imgptr, const void *data)
{
	class cv::Mat				imgtmp;
	const	struct Img_Iface_Data_SetROI	*data_cast;
	const	class cv::Rect_ <int>		*rect;

	data_cast	= (const struct Img_Iface_Data_SetROI *)data;
	rect		= &(data_cast->rect);

	(*imgptr)(*rect).copyTo(imgtmp);
	imgtmp.copyTo(*imgptr);

	imgptr->release();
	imgtmp.copyTo(*imgptr);

	imgtmp.release();
}

/* ----- Operations on arrays */
static	void	img_cv_and_2ref		(class cv::Mat *imgptr, const void *data)
{
	const	class cv::Mat	*img_ref;

	img_ref	= (const class cv::Mat *)data;

	cv::bitwise_and(*imgptr, *img_ref, *imgptr);
}

static	void	img_cv_not		(class cv::Mat *imgptr)
{

	cv::bitwise_not(*imgptr, *imgptr);
}

static	void	img_cv_or_2ref		(class cv::Mat *imgptr, const void *data)
{
	const	class cv::Mat	*img_ref;

	img_ref	= (const class cv::Mat *)data;

	cv::bitwise_or(*imgptr, *img_ref, *imgptr);
}

static	void	img_cv_component	(class cv::Mat *imgptr, const void *data)
{
	class cv::Mat				cmp_img[3];
	const	struct Img_Iface_Data_Component	*data_cast;
	int	cmp;

	data_cast	= (const struct Img_Iface_Data_Component *)data;
	cmp	= data_cast->cmp;

	cv::split(*imgptr, cmp_img);

	imgptr->release();
	cmp_img[cmp].copyTo(*imgptr);

	cmp_img[0].release();
	cmp_img[1].release();
	cmp_img[2].release();
}

/* Imgproc: Image processing */
/* ----- Image filtering */
static	void	img_cv_dilate		(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	int	i;

	data_cast	= (const struct Img_Iface_Data_Dilate_Erode *)data;
	i	= data_cast->i;

	cv::dilate(*imgptr, *imgptr, cv::Mat(), cv::Point(-1,-1), i,
					cv::BORDER_CONSTANT, cv::Scalar(0));
}

static	void	img_cv_erode		(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Dilate_Erode	*data_cast;
	int	i;

	data_cast	= (const struct Img_Iface_Data_Dilate_Erode *)data;
	i	= data_cast->i;

	cv::erode(*imgptr, *imgptr, cv::Mat(), cv::Point(-1,-1), i,
					cv::BORDER_CONSTANT, cv::Scalar(0));
}

static	void	img_cv_smooth		(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Smooth	*data_cast;
	int	method;
	int	ksize;

	data_cast	= (const struct Img_Iface_Data_Smooth *)data;
	method	= data_cast->method;
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

static	void	img_cv_sobel		(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Sobel	*data_cast;
	int	dx;
	int	dy;
	int	ksize;

	data_cast	= (const struct Img_Iface_Data_Sobel *)data;
	dx	= data_cast->dx;
	dy	= data_cast->dy;
	ksize	= data_cast->ksize;
	if (!(ksize % 2)) {
		ksize++;
	}

	cv::Sobel(*imgptr, *imgptr, -1, dx, dy, ksize, 1, 0,
							cv::BORDER_DEFAULT);
}

static	void	img_cv_border		(class cv::Mat *imgptr, const void *data)
{
	class cv::Mat				imgtmp;
	const	struct Img_Iface_Data_Border	*data_cast;
	int	sz;

	/* Data */
	data_cast	= (struct Img_Iface_Data_Border *)data;
	sz	= data_cast->size;

	imgtmp = cv::Mat(cv::Size(imgptr->cols + sz, imgptr->rows + sz), CV_8U);

	cv::copyMakeBorder(*imgptr, imgtmp, sz, sz, sz, sz,
					cv::BORDER_CONSTANT, cv::Scalar(0));

	imgtmp.copyTo(*imgptr);

	imgtmp.release();
}

/* ----- Geometric image transformations */
static	void	img_cv_mirror		(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Mirror	*data_cast;
	int	axis;

	data_cast	= (const struct Img_Iface_Data_Mirror *)data;
	axis	= data_cast->axis;

	cv::flip(*imgptr, *imgptr, axis);
}

static	void	img_cv_rotate_orto	(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Rotate_Orto	*data_cast;
	int	n;

	data_cast	= (struct Img_Iface_Data_Rotate_Orto *)data;
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

static	void	img_cv_rotate		(class cv::Mat *imgptr, const void *data)
{
	class cv::Mat				map_matrix;
	const	struct Img_Iface_Data_Rotate	*data_cast;
	const	class cv::Point_ <float>	*center;
	double	angle;

	data_cast	= (const struct Img_Iface_Data_Rotate *)data;
	center		= &(data_cast->center);
	angle		= data_cast->angle;

	map_matrix	= cv::getRotationMatrix2D(*center, angle, 1);

	cv::warpAffine(*imgptr, *imgptr, map_matrix, imgptr->size(),
				cv::INTER_LINEAR, cv::BORDER_CONSTANT,
				cv::Scalar(0, 0, 0));

	map_matrix.release();
}

/* ----- Miscellaneous image transformations */
static	void	img_cv_adaptive_thr	(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Adaptive_Thr	*data_cast;
	int	method;
	int	thr_typ;
	int	ksize;

	data_cast	= (const struct Img_Iface_Data_Adaptive_Thr *)data;
	method	= data_cast->method;
	thr_typ	= data_cast->thr_typ;
	ksize	= data_cast->ksize;
	if (!(ksize % 2)) {
		ksize++;
	}

	cv::adaptiveThreshold(*imgptr, *imgptr, 255, method, thr_typ, ksize, 0);
}

static	void	img_cv_cvt_color	(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Cvt_Color	*data_cast;
	int	method;

	data_cast	= (const struct Img_Iface_Data_Cvt_Color *)data;
	method	= data_cast->method;

	cv::cvtColor(*imgptr, *imgptr, method, 0);
}

static	void	img_cv_distance_transform	(class cv::Mat *imgptr)
{
	class cv::Mat	imgtmp;

	cv::distanceTransform(*imgptr,imgtmp, CV_DIST_L2, CV_DIST_MASK_PRECISE);

	/* DistanceTransform gives CV_32F image */
	imgtmp.convertTo(*imgptr, CV_8U);

	imgtmp.release();
}

static	void	img_cv_threshold	(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Threshold	*data_cast;
	int	thr_typ;
	int	thr_val;

	data_cast	= (const struct Img_Iface_Data_Threshold *)data;
	thr_typ	= data_cast->thr_typ;
	thr_val	= data_cast->thr_val;
	if (thr_val == -1) {
		thr_typ	|= cv::THRESH_OTSU;
	}

	cv::threshold(*imgptr, *imgptr, thr_val, 0xFF, thr_typ);
}

/* ----- Histograms */
static	void	img_cv_histogram	(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Histogram	*data_cast;
	class cv::Mat				*hist;
	class cv::Mat				*hist_img;
	const	int	h_size		= 256;
	const	float	h_range_arr[]	= {0.0, 256.0};
	const	float	*h_range	= {h_range_arr};
	int	i;

	data_cast	= (const struct Img_Iface_Data_Histogram *)data;
	hist		= data_cast->hist_c0;
	hist_img	= data_cast->hist_img;

	/* Write components into cmp_img[] */
	cv::calcHist(imgptr, 1, 0, cv::Mat(), *hist, 1, &h_size, &h_range,
								true, false);

	/* Init */
	hist_img->setTo(cv::Scalar(0));

	/* Normalize the result to [0, hist_img->rows - 1] */
	cv::normalize(*hist, *hist, 0, hist_img->rows - 1, cv::NORM_MINMAX, -1,
								cv::Mat());

	/* Draw hist into hist_img */
	for(i = 0; i < h_size; i++ ) {
		cv::line(*hist_img, cv::Point(i, hist_img->rows - 0),
				cv::Point(i,
					hist_img->rows - hist->at<float>(i)),
				cv::Scalar(255, 0, 0), 1, 8, 0);
	}

	/* Show histogram */
	img_iface_show_hist_c1();
}

static	void	img_cv_histogram_c3	(class cv::Mat *imgptr, const void *data)
{
	class cv::Mat				cmp_img[3];
	const	struct Img_Iface_Data_Histogram	*data_cast;
	class cv::Mat				*hist_c0;
	class cv::Mat				*hist_c1;
	class cv::Mat				*hist_c2;
	class cv::Mat				*hist_img;
	const	int	h_size		= 256;
	const	float	h_range_arr[]	= {0.0, 256.0};
	const	float	*h_range	= {h_range_arr};
	int	i;

	data_cast	= (const struct Img_Iface_Data_Histogram *)data;
	hist_c0		= data_cast->hist_c0;
	hist_c1		= data_cast->hist_c1;
	hist_c2		= data_cast->hist_c2;
	hist_img	= data_cast->hist_img;

	/* Write components into cmp_img[] */
	cv::split(*imgptr, cmp_img);

	/* Write components into cmp_img[] */
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
	for(i = 0; i < 256; i++ ) {
		cv::line(*hist_img, cv::Point(3 * i, hist_img->rows - 0),
				cv::Point(3 * i,
					hist_img->rows - hist_c0->at<float>(i)),
				cv::Scalar(255, 0, 0), 1, 8, 0);
	}
	for(i = 0; i < 256; i++ ) {
		cv::line(*hist_img, cv::Point(3 * i + 1, hist_img->rows - 0),
				cv::Point(3 * i + 1,
					hist_img->rows - hist_c1->at<float>(i)),
				cv::Scalar(0, 255, 0), 1, 8, 0);
	}
	for(i = 0; i < 256; i++ ) {
		cv::line(*hist_img, cv::Point(3 * i + 2, hist_img->rows - 0),
				cv::Point(3 * i + 2,
					hist_img->rows - hist_c2->at<float>(i)),
				cv::Scalar(0, 0, 255), 1, 8, 0);
	}

	/* Show histogram */
	img_iface_show_hist_c3();

	cmp_img[0].release();
	cmp_img[1].release();
	cmp_img[2].release();
}

/* ----- Structural analysis and shape descriptors */
static	void	img_cv_contours		(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Contours	*data_cast;
	class std::vector<class std::vector<class cv::Point_<int>>>  *contours;
	class cv::Mat				*hierarchy;

	/* Data */
	data_cast	= (const struct Img_Iface_Data_Contours *)data;

	/* Contours */
	contours	= data_cast->contours;
	hierarchy	= data_cast->hierarchy;

	/* Get contours */

	cv::findContours(*imgptr, *contours, *hierarchy, CV_RETR_EXTERNAL,
							CV_CHAIN_APPROX_SIMPLE);

	/* Set image to black */
	imgptr->setTo(cv::Scalar(0));

	/* Draw contours in color */
	cv::drawContours(*imgptr, *contours, -1, cv::Scalar(255), 1, 8,
						*hierarchy, 1, cvPoint(0, 0));
}

static	void	img_cv_contours_size	(const void *data)
{
	const	struct Img_Iface_Data_Contours_Size	*data_cast;
	const	class std::vector<class std::vector<class cv::Point_<int>>>  *contours;
	unsigned	i;

	data_cast	= (const struct Img_Iface_Data_Contours_Size *)data;
	contours	= data_cast->contours;

	if (data_cast->area) {
		for (i = 0; i < contours->size(); i++) {
			data_cast->area[i]	= cv::contourArea(
							(*contours)[i], false);
		}
	}
	if (data_cast->perimeter) {
		for (i = 0; i < contours->size(); i++) {
			data_cast->perimeter[i]	= cv::arcLength(
							(*contours)[i], true);
		}
	}
}

static	void	img_cv_bounding_rect	(class cv::Mat *imgptr, const void *data)
{
	class cv::Point_ <float>				vertices[4];
	const	struct Img_Iface_Data_Bounding_Rect		*data_cast;
	const	class std::vector <class cv::Point_ <int>>	*contour;
	class cv::Rect_ <int>					*rect;
	bool	show;

	data_cast	= (const struct Img_Iface_Data_Bounding_Rect *)data;
	contour	= data_cast->contour;
	rect	= data_cast->rect;
	show	= data_cast->show;

	*rect	= cv::boundingRect(*contour);

	/* Draw rectangle */
	if (show) {
		vertices[0].x	= rect->x;
		vertices[0].y	= rect->y;
		vertices[1].x	= rect->x + rect->width;
		vertices[1].y	= rect->y;
		vertices[2].x	= rect->x + rect->width;
		vertices[2].y	= rect->y + rect->height;
		vertices[3].x	= rect->x;
		vertices[3].y	= rect->y + rect->height;
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

static	void	img_cv_fit_ellipse	(class cv::Mat *imgptr, const void *data)
{
	class cv::Point_ <float>				vertices[4];
	const	struct Img_Iface_Data_MinARect			*data_cast;
	const	class std::vector <class cv::Point_ <int>>	*contour;
	class cv::RotatedRect					*rect;
	bool	show;

	data_cast	= (const struct Img_Iface_Data_MinARect *)data;
	contour	= data_cast->contour;
	rect	= data_cast->rect;
	show	= data_cast->show;

	*rect	= cv::fitEllipse(*contour);

	/* Draw rectangle */
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

static	void	img_cv_min_area_rect	(class cv::Mat *imgptr, const void *data)
{
	class cv::Point_ <float>				vertices[4];
	const	struct Img_Iface_Data_MinARect			*data_cast;
	const	class std::vector <class cv::Point_ <int>>	*contour;
	class cv::RotatedRect					*rect;
	bool	show;

	data_cast	= (const struct Img_Iface_Data_MinARect *)data;
	contour	= data_cast->contour;
	rect	= data_cast->rect;
	show	= data_cast->show;

	*rect	= cv::minAreaRect(*contour);

	/* Draw rectangle */
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

/* ----- Feature detection */
static	void	img_cv_hough_circles	(class cv::Mat *imgptr, const void *data)
{
	class cv::Point_ <int>	center;
	int		radius;
	unsigned	i;
	const	struct Img_Iface_Data_Hough_Circles	*data_cast;
	class std::vector <class cv::Vec <float, 3>>	*circles;
	double	dist_min;
	double	param_1;
	double	param_2;
	int	radius_min;
	int	radius_max;

	data_cast	= (const struct Img_Iface_Data_Hough_Circles *)data;
	circles		= data_cast->circles;
	dist_min	= data_cast->dist_min;
	param_1		= data_cast->param_1;
	param_2		= data_cast->param_2;
	radius_min	= data_cast->radius_min;
	radius_max	= data_cast->radius_max;

	/* Get circles */
	cv::HoughCircles(*imgptr, *circles, CV_HOUGH_GRADIENT, 1, dist_min,
				param_1, param_2, radius_min, radius_max);

	/* Set image to black */
	imgptr->setTo(cv::Scalar(0));

	/* Draw circles */
	for (i = 0; i < circles->size(); i++) {
		center.x	= cvRound((*circles)[i][0]);
		center.y	= cvRound((*circles)[i][1]);
		radius		= cvRound((*circles)[i][2]);

		/* Draw the circle center */
/*			cv::circle(*imgptr, center, 3,
				cv::Scalar(0, 255, 0), -1, 8, 0);*/

		/* Draw the circle outline */
		cv::circle(*imgptr, center, radius,
				cv::Scalar(250), 1, 8, 0);
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
