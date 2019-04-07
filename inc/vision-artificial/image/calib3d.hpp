/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
#ifndef VA_IMAGE_CALIB3D_HPP
#define VA_IMAGE_CALIB3D_HPP


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include <opencv2/opencv.hpp>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
enum	Img_Calib3d_Action {
	IMG_CALIB3D_ACT_FOO = 0x000000u,

	IMG_CALIB3D_ACT_CALIB3D = 0x000400u,
	IMG_CALIB3D_ACT_CALIBRATE,
	IMG_CALIB3D_ACT_UNDISTORT
};


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* extern functions *****************************************************
 ******************************************************************************/
void	img_calib3d_act(class cv::Mat *imgptr, int action, const void *data);


/******************************************************************************
 ******* static inline functions (prototypes) *********************************
 ******************************************************************************/


/******************************************************************************
 ******* static inline functions (definitions) ********************************
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
#endif		/* vision-artificial/image/calib3d.hpp */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
