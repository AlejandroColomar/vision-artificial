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

/* Module --------------------------------------------------------------------*/
		/* data */
	#include "img_iface.hpp"

	#include "img_alx.hpp"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_alx_local_max	(class cv::Mat  *imgptr);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_alx_act	(class cv::Mat  *imgptr, int action, void *data)
{
	switch (action) {
	case IMG_ALX_ACT_LOCAL_MAX:
		img_alx_local_max(imgptr);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_alx_local_max	(class cv::Mat  *imgptr)
{
	int	i;
	int	j;
	int	k;
	int	l;
	bool	local_max;
	int	dist_min;
	int	step_img;
	uint8_t	*img_pix;
	uint8_t	*near_pix;
	uint8_t	*tmp_pix;	
	class cv::Mat	imgtmp;

	imgptr->copyTo(imgtmp);
	step_img	= imgptr->step;
	dist_min	= 16;

	for (i = 1; i < imgptr->rows - 1; i++) {
	for (j = 1; j < imgptr->cols - 1; j++) {
		img_pix		= imgptr->data + i * step_img + j;
		tmp_pix		= imgtmp.data + i * step_img + j;
		local_max	= true;

		for (k = i - dist_min; k < i + dist_min+1; k++) {
		for (l = j - dist_min; l < j + dist_min+1; l++) {
			near_pix	= imgptr->data + k * step_img + l;
			if (j >= 0  &&  j < imgptr->rows) {
			if (l >= 0  &&  l < imgptr->cols) {
				if (*img_pix < *near_pix) {
					local_max	= false;
				}
			}
			}
		}
		}

		if (local_max) {
			*tmp_pix	= *img_pix;
		} else {
			*tmp_pix	= 0;
		}
	}
	}

	/* Cleanup */
	imgtmp.copyTo(*imgptr);
	imgtmp.release();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
