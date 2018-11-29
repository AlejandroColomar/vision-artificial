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

/* libalx --------------------------------------------------------------------*/
		/* alx_median_u8() */
	#include "alx_math.hpp"

/* Module --------------------------------------------------------------------*/
		/* data */
	#include "img_iface.hpp"

	#include "img_alx.hpp"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_alx_local_max		(class cv::Mat  *imgptr);
static	void	img_alx_lines_horizontal	(class cv::Mat  *imgptr);
static	void	img_alx_lines_vertical		(class cv::Mat  *imgptr);
static	void	img_alx_mean_horizontal		(class cv::Mat  *imgptr);
static	void	img_alx_mean_vertical		(class cv::Mat  *imgptr);
static	void	img_alx_median_horizontal	(class cv::Mat  *imgptr);
static	void	img_alx_median_vertical		(class cv::Mat  *imgptr);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_alx_act	(class cv::Mat  *imgptr, int action, void *data)
{
	switch (action) {
	case IMG_ALX_ACT_LOCAL_MAX:
		img_alx_local_max(imgptr);
		break;

	case IMG_ALX_ACT_LINES_HORIZONTAL:
		img_alx_lines_horizontal(imgptr);
		break;
	case IMG_ALX_ACT_LINES_VERTICAL:
		img_alx_lines_vertical(imgptr);
		break;

	case IMG_ALX_ACT_MEAN_HORIZONTAL:
		img_alx_mean_horizontal(imgptr);
		break;
	case IMG_ALX_ACT_MEAN_VERTICAL:
		img_alx_mean_vertical(imgptr);
		break;
	case IMG_ALX_ACT_MEDIAN_HORIZONTAL:
		img_alx_median_horizontal(imgptr);
		break;
	case IMG_ALX_ACT_MEDIAN_VERTICAL:
		img_alx_median_vertical(imgptr);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_alx_local_max		(class cv::Mat  *imgptr)
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

	for (i = 0; i < imgptr->rows; i++) {
	for (j = 0; j < imgptr->cols; j++) {
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

static	void	img_alx_lines_horizontal	(class cv::Mat  *imgptr)
{
	int	i;
	int	j;
	bool	white;

	/* pointer to a pixel (in imgptr) */
	uint8_t	*img_pix;

	for (i = 0; i < imgptr->rows; i++) {
		white	= false;

		for (j = 0; j < imgptr->cols; j++) {
			img_pix		= imgptr->data + i * imgptr->step + j;
			if (*img_pix) {
				white	= true;
				break;
			}
		}

		if (white) {
			for (j = 0; j < imgptr->cols; j++) {
				img_pix		= imgptr->data + i * imgptr->step + j;
				*img_pix	= 255;
			}
		}
	}
}

static	void	img_alx_lines_vertical		(class cv::Mat  *imgptr)
{
	int	i;
	int	j;
	bool	white;

	/* pointer to a pixel (in imgptr) */
	uint8_t	*img_pix;

	for (i = 0; i < imgptr->cols; i++) {
		white	= false;

		for (j = 0; j < imgptr->rows; j++) {
			img_pix		= imgptr->data + j * imgptr->step + i;
			if (*img_pix) {
				white	= true;
				break;
			}
		}

		if (white) {
			for (j = 0; j < imgptr->rows; j++) {
				img_pix		= imgptr->data + j * imgptr->step + i;
				*img_pix	= 255;
			}
		}
	}
}

static	void	img_alx_mean_horizontal		(class cv::Mat  *imgptr)
{
	int		i;
	int		j;
	uint32_t	tmp;
	uint8_t		mean;

	/* pointer to a pixel (in imgptr) */
	uint8_t	*img_pix;

	for (i = 0; i < imgptr->rows; i++) {
		tmp	= 0;
		for (j = 0; j < imgptr->cols; j++) {
			img_pix		= imgptr->data + i * imgptr->step + j;
			tmp		+= *img_pix;
		}
		mean	= tmp / imgptr->cols;

		for (j = 0; j < imgptr->cols; j++) {
			img_pix		= imgptr->data + i * imgptr->step + j;
			*img_pix	= mean;
		}
	}
}

static	void	img_alx_mean_vertical		(class cv::Mat  *imgptr)
{
	int		i;
	int		j;
	uint32_t	tmp;
	uint8_t		mean;

	/* pointer to a pixel (in imgptr) */
	uint8_t	*img_pix;

	for (i = 0; i < imgptr->cols; i++) {
		tmp	= 0;
		for (j = 0; j < imgptr->rows; j++) {
			img_pix		= imgptr->data + j * imgptr->step + i;
			tmp		+= *img_pix;
		}
		mean	= tmp / imgptr->rows;

		for (j = 0; j < imgptr->rows; j++) {
			img_pix		= imgptr->data + j * imgptr->step + i;
			*img_pix	= mean;
		}
	}
}

static	void	img_alx_median_horizontal	(class cv::Mat  *imgptr)
{
	int		i;
	int		j;
	uint8_t		row [imgptr->cols];
	uint8_t		median;

	/* pointer to a pixel (in imgptr) */
	uint8_t	*img_pix;

	for (i = 0; i < imgptr->rows; i++) {
		for (j = 0; j < imgptr->cols; j++) {
			img_pix	= imgptr->data + i * imgptr->step + j;
			row[j]	= *img_pix;
		}
		median	= alx_median_u8(imgptr->cols, row);

		for (j = 0; j < imgptr->cols; j++) {
			img_pix		= imgptr->data + i * imgptr->step + j;
			*img_pix	= median;
		}
	}
}

static	void	img_alx_median_vertical		(class cv::Mat  *imgptr)
{
	int		i;
	int		j;
	uint8_t		col [imgptr->rows];
	uint8_t		median;

	/* pointer to a pixel (in imgptr) */
	uint8_t	*img_pix;

	for (i = 0; i < imgptr->cols; i++) {
		for (j = 0; j < imgptr->rows; j++) {
			img_pix	= imgptr->data + j * imgptr->step + i;
			col[j]	= *img_pix;
		}
		median	= alx_median_u8(imgptr->rows, col);

		for (j = 0; j < imgptr->rows; j++) {
			img_pix		= imgptr->data + j * imgptr->step + i;
			*img_pix	= median;
		}
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
