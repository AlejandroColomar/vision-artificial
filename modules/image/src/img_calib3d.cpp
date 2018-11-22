/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C++ --------------------------------------------------------------*/
		/* std::vector */
	#include <vector>

/* Standard C ----------------------------------------------------------------*/
		/* snprintf() */
	#include <cstdio>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <opencv2/opencv.hpp>
	#include <opencv2/calib3d/calib3d.hpp>

/* Module -------------------------------------------------------------------*/
	#include "img_calib3d.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_calib3d_calibrate	(class cv::Mat  *imgptr, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_calib3d_act	(class cv::Mat  *imgptr, int action, void *data)
{
	switch (action) {
	case IMG_CALIB3D_ACT_CALIBRATE:
		img_calib3d_calibrate(imgptr, data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_calib3d_calibrate	(class cv::Mat  *imgptr, void *data)
{
	const	class cv::Size	pattern_size(8, 6);

	class cv::Mat	corners;
	bool		found;

	found	= cv::findChessboardCorners(*imgptr, pattern_size, corners);

	int	i;
	int	j;

	// Object points
	std::vector <std::vector <Vec3f>>	object_points;
	for (i = 0; i < pattern_size.height; i++) {
		for (j = 0; j < pattern_size.width; j++) {
			object_points.at(0).push_back(Vec3f(i, j, 0));
		}
	}

	// Image points
	std::vector <std::vector <Vec3f>>	image_points;
	int	num;
	for (i = 0; i < pattern_size.height; i++) {
		for (j = 0; j < pattern_size.width; j++) {
			image_points.at(0).push_back(corners.at<Vec3f>(num, 0));
		}
	}

	class cv::Mat	camera_matrix;
	class cv::Mat	dist_matrix;
	class cv::Mat	rvecs;
	class cv::Mat	tvecs;

	cv::calibrateCamera(object_points, image_points,
				cv::Size(imgptr->size().width,
						imgptr->size().height)
				camera_matrix, dist_matrix, rvecs, tvecs);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
