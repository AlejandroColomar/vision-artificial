/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
#ifndef VA_IMAGE_ZBAR_HPP
#define VA_IMAGE_ZBAR_HPP


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include <opencv2/core/core.hpp>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
enum	Img_ZB_Action {
	IMG_ZB_ACT_FOO = 0x000000u,

	IMG_ZB_ACT_ZB = 0x000800u,
	IMG_ZB_ACT_DECODE
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
void	img_zb_act	(class cv::Mat  *imgptr, int action, const void *data);


/******************************************************************************
 ******* static inline functions (prototypes) *********************************
 ******************************************************************************/


/******************************************************************************
 ******* static inline functions (definitions) ********************************
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
#endif		/* vision-artificial/image/zbar.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/