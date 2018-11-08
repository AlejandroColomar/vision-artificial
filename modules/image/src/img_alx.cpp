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
	bool		local_max;
	int		dist_min;
	int		val_min;
	class cv::Mat	imgtmp;

		/* pointer to a pixel (in imgptr) */
	uint8_t	*img_pix;
		/* pointer to a pixel near img_pix (in imgptr) */
	uint8_t	*near_pix;
		/* pointer to a pixel (same position as img_pix, but in imgtmp) */
	uint8_t	*tmp_pix;

		/* Tmp image copy */
	imgptr->copyTo(imgtmp);
		/* Minimum distance between local maxima */
	dist_min	= 16;
		/* Minimum value of local maxima */
	val_min		= 16;

	for (i = 1; i < imgptr->rows - 1; i++) {
	for (j = 1; j < imgptr->cols - 1; j++) {
		img_pix		= imgptr->data + i * imgptr->step + j;
		tmp_pix		= imgtmp.data + i * imgptr->step + j;
		local_max	= true;

		if (*img_pix < val_min) {
			local_max	= false;
		}

		for (k = i - dist_min; (k < i + dist_min+1) && local_max; k++) {
		for (l = j - dist_min; (l < j + dist_min+1) && local_max; l++) {
			near_pix	= imgptr->data + k * imgptr->step + l;
			if (k >= 0  &&  k < imgptr->rows) {
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
