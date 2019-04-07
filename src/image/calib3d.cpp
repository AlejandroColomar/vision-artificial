/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/image/calib3d.hpp"

#include <cstddef>
#include <cstdio>

#include <vector>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "vision-artificial/image/iface.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define CORNERS_HOR	(16)
#define CORNERS_VER	(13)


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_calib3d_calibrate(class cv::Mat  *imgptr, const void *data);
static	void	img_calib3d_undistort(class cv::Mat  *imgptr, const void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_calib3d_act(class cv::Mat  *imgptr, int action, const void *data)
{
	switch (action) {
	case IMG_CALIB3D_ACT_CALIBRATE:
		img_calib3d_calibrate(imgptr, data);
		break;

	case IMG_CALIB3D_ACT_UNDISTORT:
		img_calib3d_undistort(imgptr, data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_calib3d_calibrate(class cv::Mat *imgptr, const void *data)
{
	const	struct Img_Iface_Data_Calibrate	*data_cast;
	class cv::Mat				*intrinsic_mat;
	class cv::Mat				*dist_coefs;
	class std::vector <class cv::Mat>	*rvecs;
	class std::vector <class cv::Mat>	*tvecs;

	class cv::Size_ <int>				pattern_size;
	class std::vector <class std::vector <class cv::Point3_ <float>>>  object_points;
	class std::vector <class std::vector <class cv::Point_ <float>>>  image_points;
	class std::vector <class cv::Point_ <float>>	corners;
	class std::vector <class cv::Point3_ <float>>	obj;
	bool	found;

	data_cast	= (const struct Img_Iface_Data_Calibrate *)data;
	intrinsic_mat	= data_cast->intrinsic_mat;
	dist_coefs	= data_cast->dist_coefs;
	rvecs		= data_cast->rvecs;
	tvecs		= data_cast->tvecs;

	pattern_size	= cv::Size(CORNERS_HOR, CORNERS_VER);
	*intrinsic_mat	= cv::Mat(3, 3, CV_32FC1);
	(*intrinsic_mat).ptr<float>(0)[0]	= 1;
	(*intrinsic_mat).ptr<float>(1)[1]	= 1;

	for (ptrdiff_t i = 0; i < CORNERS_HOR; i++) {
		for (ptrdiff_t j = 0; j < CORNERS_VER; j++)
			obj.push_back(cv::Point3f(i, j * 10.0, 0.0));
	}

	found	= cv::findChessboardCorners(*imgptr, pattern_size, corners,
					CV_CALIB_CB_ADAPTIVE_THRESH |
					CV_CALIB_CB_FILTER_QUADS);

	cv::cornerSubPix(*imgptr, corners, cv::Size(11, 11),
			cv::Size(-1, -1),
			cv::TermCriteria(CV_TERMCRIT_EPS |
					CV_TERMCRIT_ITER, 30, 0.1));
	cv::drawChessboardCorners(*imgptr, pattern_size, corners, found);

	image_points.push_back(corners);
	object_points.push_back(obj);

	cv::calibrateCamera(object_points, image_points, imgptr->size(),
				*intrinsic_mat, *dist_coefs, *rvecs, *tvecs);
}

static	void	img_calib3d_undistort(class cv::Mat *imgptr, const void *data)
{
	class cv::Mat				imgtmp;
	const	struct Img_Iface_Data_Undistort	*data_cast;
	const	class cv::Mat			*intrinsic_mat;
	const	class cv::Mat			*dist_coefs;

	data_cast	= (const struct Img_Iface_Data_Undistort *)data;
	intrinsic_mat	= data_cast->intrinsic_mat;
	dist_coefs	= data_cast->dist_coefs;

	cv::undistort(*imgptr, imgtmp, *intrinsic_mat, *dist_coefs);

	imgptr->release();
	imgtmp.copyTo(*imgptr);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
