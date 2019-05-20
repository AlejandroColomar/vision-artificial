/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	(GPL-2.0-only  OR  LGPL-3.0-only)     *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/image/alx.hpp"

#include <cstdbool>
#include <cstddef>
#include <cstdlib>
#include <cstdio>

#include <opencv2/core/core.hpp>

#include "libalx/base/math/arithmetic_mean.hpp"
#include "libalx/base/math/median.hpp"

#include "vision-artificial/image/iface.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
enum	Cell {
	NOT_LOCAL_MAX,
	MAYBE_LOCAL_MAX,
	LOCAL_MAX
};


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	img_alx_local_max		(class cv::Mat  *imgptr);
static	void	img_alx_skeleton		(class cv::Mat  *imgptr);
static	void	img_alx_lines_horizontal	(class cv::Mat  *imgptr);
static	void	img_alx_lines_vertical		(class cv::Mat  *imgptr);
static	void	img_alx_mean_horizontal		(class cv::Mat  *imgptr);
static	void	img_alx_mean_vertical		(class cv::Mat  *imgptr);
static	void	img_alx_median_horizontal	(class cv::Mat  *imgptr);
static	void	img_alx_median_vertical		(class cv::Mat  *imgptr);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	img_alx_act	(class cv::Mat  *imgptr, int action, const void *data)
{

	switch (action) {
	case IMG_ALX_ACT_LOCAL_MAX:
		img_alx_local_max(imgptr);
		break;
	case IMG_ALX_ACT_SKELETON:
		img_alx_skeleton(imgptr);
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
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	img_alx_local_max		(class cv::Mat  *imgptr)
{
	const ptrdiff_t	rows = imgptr->rows;
	const ptrdiff_t	cols = imgptr->cols;
	const ptrdiff_t	step = imgptr->step;
	/* Minimum distance between local maxima */
	const int	dist = 2;
	/* Minimum value of local maxima */
	const uint8_t	val_min = 1;
	class cv::Mat	imgtmp;
	int		arr_tmp[rows][cols];
	bool		wh;
	/* pointer to a pixel (in imgptr) */
	const uint8_t	*img_pix;
	/* pointer to a pixel near img_pix (in imgptr) */
	const uint8_t	*near_pix;
	/* pointer to a pixel (same position as img_pix, but in imgtmp) */
	uint8_t		*tmp_pix;

	/* Tmp image copy */
	imgptr->copyTo(imgtmp);

	memset(arr_tmp, 0, sizeof(arr_tmp));

	for (ptrdiff_t i = 0; i < rows; i++) {
	for (ptrdiff_t j = 0; j < cols; j++) {
		img_pix		= imgptr->data + i*step + j;
		if (*img_pix < val_min)
			continue;
		for (ptrdiff_t k = (i + dist); k >= (i - dist); k--) {
		for (ptrdiff_t l = (j + dist); l >= (j - dist); l--) {
			if ((k >= 0) && (k < rows)) {
			if ((l >= 0) && (l < cols)) {
				near_pix	= imgptr->data + k*step + l;
				if (*near_pix > *img_pix)
					goto not_maxima;
				if (*near_pix == *img_pix)
					arr_tmp[i][j]	= MAYBE_LOCAL_MAX;
			}
			}
		}
		}
		if (!arr_tmp[i][j])
			arr_tmp[i][j]	= LOCAL_MAX;
		continue;
not_maxima:
		;
	}
	}

	do {
		wh	= false;
		for (ptrdiff_t i = 0; i < rows; i++) {
		for (ptrdiff_t j = 0; j < cols; j++) {
			img_pix		= imgptr->data + i*step + j;
			if (arr_tmp[i][j] != MAYBE_LOCAL_MAX)
				continue;
			for (ptrdiff_t k = (i+dist); k >= (i-dist); k--) {
			for (ptrdiff_t l = (j+dist); l >= (j-dist); l--) {
				if ((k >= 0) && (k < rows)) {
				if ((l >= 0) && (l < cols)) {
					near_pix = imgptr->data + k*step + l;
					if (*near_pix == *img_pix) {
						if (!arr_tmp[k][l])
							goto not_maxima_2;
					}
				}
				}
			}
			}
			continue;
not_maxima_2:
			wh		= true;
			arr_tmp[i][j]	= 0;
		}
		}
	} while (wh);

	for (ptrdiff_t i = 0; i < rows; i++) {
	for (ptrdiff_t j = 0; j < cols; j++) {
		if (!arr_tmp[i][j]) {
			tmp_pix		= imgtmp.data + i * step + j;
			*tmp_pix	= 0;
		}
	}
	}

	/* Cleanup */
	imgtmp.copyTo(*imgptr);
	imgtmp.release();
}

static	void	img_alx_skeleton		(class cv::Mat  *imgptr)
{
	const ptrdiff_t	rows = imgptr->rows;
	const ptrdiff_t	cols = imgptr->cols;
	const ptrdiff_t	step = imgptr->step;
	/* (Half of the) width of the skeleton */
	const ptrdiff_t	width = 5;
	ptrdiff_t	dist_x;
	ptrdiff_t	dist_y;
	bool		skeleton;
	int_fast16_t	cnt_lo [width];
	int_fast16_t	cnt_hi_or_eq [width];
	class cv::Mat	imgtmp;
	/* pointer to a pixel (in imgptr) */
	const uint8_t	*img_pix;
	/* pointer to a pixel near img_pix (in imgptr) */
	const uint8_t	*near_pix;
	/* pointer to a pixel (same position as img_pix, but in imgtmp) */
	uint8_t		*tmp_pix;

	/* Tmp image copy */
	imgptr->copyTo(imgtmp);

	for (ptrdiff_t i = 0; i < rows; i++) {
	for (ptrdiff_t j = 0; j < cols; j++) {
		img_pix		= imgptr->data + i * step + j;
		tmp_pix		= imgtmp.data + i * step + j;

		if (!(*img_pix)) {
			*tmp_pix	= 0;
			continue;
		}

		for (ptrdiff_t r = 0; r < width; r++) {
			cnt_lo[r]	= 0;
			cnt_hi_or_eq[r]	= 0;
		}

		for (ptrdiff_t k = (i + width); k >= (i - width); k--) {
		for (ptrdiff_t l = (j + width); l >= (j - width); l--) {
			near_pix	= imgptr->data + k * step + l;

			dist_x	= abs(k - i);
			dist_y	= abs(l - j);

			if ((k >= 0)  &&  (k < rows)) {
			if ((l >= 0)  &&  (l < cols)) {
			if (dist_x  ||  dist_y) {
				if (*near_pix < *img_pix)
					cnt_lo[std::max(dist_x, dist_y)]++;
				else
					cnt_hi_or_eq[std::max(dist_x, dist_y)]++;
			}
			}
			}

		}
		}

		skeleton	= false;
		for (ptrdiff_t r = 0; r < width; r++) {
			if (cnt_lo[r] > (cnt_hi_or_eq[r] + (1.6) * (r + 1)))
				skeleton	= true;
		}

		if (skeleton)
			*tmp_pix	= *img_pix;
		else
			*tmp_pix	= 0;
	}
	}

	/* Cleanup */
	imgtmp.copyTo(*imgptr);
	imgtmp.release();
}

static	void	img_alx_lines_horizontal	(class cv::Mat  *imgptr)
{
	const ptrdiff_t	rows = imgptr->rows;
	const ptrdiff_t	cols = imgptr->cols;
	const ptrdiff_t	step = imgptr->step;
	bool		white;
	/* pointer to a pixel (in imgptr) */
	uint8_t		*img_pix;

	for (ptrdiff_t i = 0; i < rows; i++) {
		white	= false;

		for (ptrdiff_t j = 0; j < cols; j++) {
			img_pix		= imgptr->data + i * step + j;
			if (*img_pix) {
				white	= true;
				break;
			}
		}

		if (white) {
			for (ptrdiff_t j = 0; j < cols; j++) {
				img_pix		= imgptr->data + i * step + j;
				*img_pix	= 255;
			}
		}
	}
}

static	void	img_alx_lines_vertical		(class cv::Mat  *imgptr)
{
	const ptrdiff_t	rows = imgptr->rows;
	const ptrdiff_t	cols = imgptr->cols;
	const ptrdiff_t	step = imgptr->step;
	bool		white;
	/* pointer to a pixel (in imgptr) */
	uint8_t		*img_pix;

	for (ptrdiff_t i = 0; i < cols; i++) {
		white	= false;

		for (ptrdiff_t j = 0; j < rows; j++) {
			img_pix		= imgptr->data + j * step + i;
			if (*img_pix) {
				white	= true;
				break;
			}
		}

		if (white) {
			for (ptrdiff_t j = 0; j < rows; j++) {
				img_pix		= imgptr->data + j * step + i;
				*img_pix	= 255;
			}
		}
	}
}

static	void	img_alx_mean_horizontal		(class cv::Mat  *imgptr)
{
	const ptrdiff_t	rows = imgptr->rows;
	const ptrdiff_t	cols = imgptr->cols;
	const ptrdiff_t	step = imgptr->step;
	uint8_t		row [cols];
	uint8_t		mean;
	/* pointer to a pixel (in imgptr) */
	uint8_t		*img_pix;

	for (ptrdiff_t i = 0; i < rows; i++) {
		for (ptrdiff_t j = 0; j < cols; j++) {
			img_pix		= imgptr->data + i * step + j;
			row[j]		= *img_pix;
		}
		mean	= alx_arithmetic_mean_u8(cols, row);

		for (ptrdiff_t j = 0; j < cols; j++) {
			img_pix		= imgptr->data + i * step + j;
			*img_pix	= mean;
		}
	}
}

static	void	img_alx_mean_vertical		(class cv::Mat  *imgptr)
{
	const ptrdiff_t	rows = imgptr->rows;
	const ptrdiff_t	cols = imgptr->cols;
	const ptrdiff_t	step = imgptr->step;
	uint8_t		col [rows];
	uint8_t		mean;
	/* pointer to a pixel (in imgptr) */
	uint8_t		*img_pix;

	for (ptrdiff_t i = 0; i < cols; i++) {
		for (ptrdiff_t j = 0; j < rows; j++) {
			img_pix		= imgptr->data + j * step + i;
			col[j]		= *img_pix;
		}
		mean	= alx_arithmetic_mean_u8(rows, col);

		for (ptrdiff_t j = 0; j < rows; j++) {
			img_pix		= imgptr->data + j * step + i;
			*img_pix	= mean;
		}
	}
}

static	void	img_alx_median_horizontal	(class cv::Mat  *imgptr)
{
	const ptrdiff_t	rows = imgptr->rows;
	const ptrdiff_t	cols = imgptr->cols;
	const ptrdiff_t	step = imgptr->step;
	uint8_t		row [cols];
	uint8_t		median;
	/* pointer to a pixel (in imgptr) */
	uint8_t		*img_pix;

	for (ptrdiff_t i = 0; i < rows; i++) {
		for (ptrdiff_t j = 0; j < cols; j++) {
			img_pix	= imgptr->data + i * step + j;
			row[j]	= *img_pix;
		}
		median	= alx_median_u8(cols, row);

		for (ptrdiff_t j = 0; j < cols; j++) {
			img_pix		= imgptr->data + i * step + j;
			*img_pix	= median;
		}
	}
}

static	void	img_alx_median_vertical		(class cv::Mat  *imgptr)
{
	const ptrdiff_t	rows = imgptr->rows;
	const ptrdiff_t	cols = imgptr->cols;
	const ptrdiff_t	step = imgptr->step;
	uint8_t		col [rows];
	uint8_t		median;
	/* pointer to a pixel (in imgptr) */
	uint8_t		*img_pix;

	for (ptrdiff_t i = 0; i < cols; i++) {
		for (ptrdiff_t j = 0; j < rows; j++) {
			img_pix	= imgptr->data + j * step + i;
			col[j]	= *img_pix;
		}
		median	= alx_median_u8(rows, col);

		for (ptrdiff_t j = 0; j < rows; j++) {
			img_pix		= imgptr->data + j * step + i;
			*img_pix	= median;
		}
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
