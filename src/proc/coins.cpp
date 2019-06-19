/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/proc/coins.hpp"

#include <cstdio>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "libalx/base/stddef/size.hpp"
#include "libalx/base/stdlib/average.hpp"
#include "libalx/base/stdlib/maximum.hpp"

#include "vision-artificial/image/iface.hpp"
#include "vision-artificial/user/iface.hpp"
#include "vision-artificial/proc/common.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define COINS_MAX		(0xFFF)

#define COIN_DIAMETER_2E	(25.75)
#define COIN_DIAMETER_50c	(24.25)
#define COIN_DIAMETER_1E	(23.25)
#define COIN_DIAMETER_20c	(22.25)
#define COIN_DIAMETER_5c	(21.25)
#define COIN_DIAMETER_10c	(19.75)
#define COIN_DIAMETER_2c	(18.75)
#define COIN_DIAMETER_1c	(16.25)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/
struct	Coins_Properties {
	ptrdiff_t	x;
	ptrdiff_t	y;
	uint8_t		diameter_pix;
	double		diameter_mm;
	double		value;
};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	class std::vector <class std::vector <class cv::Point_ <int>>>	contours;
static	class cv::Mat		hierarchy;
static	class cv::Rect_ <int>	rectangle[COINS_MAX];
static	ptrdiff_t		coins_n;
static	struct Coins_Properties	coins[COINS_MAX];
static	double			ratio_mm_per_pix;
static	double			value_total;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	result_coins		(int status);

static	void	coins_segmentate	(void);
static	int	coins_positions		(void);
static	void	coins_diameters_pix	(void);
static	void	calibrate_mm_per_pix	(void);
static	void	coins_diameters_mm	(void);
static	int	coins_values		(void);
static	double	coin_value		(double diameter_mm);
static	void	coins_total_value	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_coins	(void)
{
	int	status;

	proc_save_mem(0);

	/* Segmentate coins */
	clock_start();
	coins_segmentate();
	clock_stop("Segmentate coins");

	/* Find coins positions */
	clock_start();
	status	= coins_positions();
	if (status) {
		result_coins(status);
		return	status;
	}
	clock_stop("Find coins positions");

	/* Get coin diameters in pixels */
	clock_start();
	coins_diameters_pix();
	clock_stop("Coins diameters in pixels");

	/* Calibrate with the biggest coin; every img should have a 2 € coin */
	clock_start();
	calibrate_mm_per_pix();
	clock_stop("Calibrate (mm per pix)");

	/* Get coins diameters in mm */
	clock_start();
	coins_diameters_mm();
	clock_stop("Coins diameters in mm");

	/* Get coins values (by their sizes) */
	clock_start();
	status	= coins_values();
	if (status) {
		result_coins(status);
		return	status;
	}
	clock_stop("Coins values");

	/* Add total value */
	clock_start();
	coins_total_value();
	clock_stop("Total value");

	result_coins(COINS_OK);
	return	0;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	result_coins		(int status)
{

	switch (status) {
	case COINS_OK:
		user_iface_log_write(0, "Coin:  OK");
		break;
	case COINS_NOK_COINS:
		user_iface_log_write(0, "Coin:  NOK_COINS");
		break;
#if 0
	case COINS_NOK_OVERLAP:
		user_iface_log_write(0, "Coin:  NOK_OVERLAP");
		break;
#endif
	case COINS_NOK_SIZE:
		user_iface_log_write(0, "Coin:  NOK_SIZE");
		break;
	default:
		user_iface_log_write(0, "Coin:  NOK");
		break;
	}
}

static	void	coins_segmentate	(void)
{
	proc_load_mem(0);

	proc_cmp(IMG_IFACE_CMP_BLUE);
	proc_threshold(cv::THRESH_BINARY_INV, IMG_IFACE_THR_OTSU);
	proc_distance_transform();
	proc_local_max();
	proc_dilate(1);
	proc_save_mem(1);
}

static	int	coins_positions		(void)
{

	proc_load_mem(1);

	proc_contours(&contours, &hierarchy);
	coins_n	= contours.size();
	if (!coins_n)
		return	COINS_NOK_COINS;
	for (ptrdiff_t i = 0; i < coins_n; i++) {
		proc_bounding_rect(&contours[i], &rectangle[i], true);
		coins[i].x	= rectangle[i].x + rectangle[i].width / 2.0;
		coins[i].y	= rectangle[i].y + rectangle[i].height / 2.0;
	}

	return	0;
}

static	void	coins_diameters_pix	(void)
{

	proc_load_mem(1);
	for (ptrdiff_t i = 0; i < coins_n; i++) {
		proc_pixel_get(coins[i].x, coins[i].y, &coins[i].diameter_pix);
		coins[i].diameter_pix	*= 2;
	}
}

static	void	calibrate_mm_per_pix	(void)
{
	uint8_t	coins_size_pix[coins_n];
	int	max_pos;
	int	max_size;

	for (ptrdiff_t i = 0; i < coins_n; i++)
		coins_size_pix[i]	= coins[i].diameter_pix;

	max_pos = alx_maximum_u8(ARRAY_SIZE(coins_size_pix), coins_size_pix);
	max_size		= coins_size_pix[max_pos];
	ratio_mm_per_pix	= 25.75 / max_size;
}

static	void	coins_diameters_mm	(void)
{
	char	txt[LOG_LINE_LEN];

	for (ptrdiff_t i = 0; i < coins_n; i++) {
		coins[i].diameter_mm = ratio_mm_per_pix * coins[i].diameter_pix;
		snprintf(txt, LOG_LINE_LEN, "Coin[%ti]:	%.2lf mm",
						i, coins[i].diameter_mm);
		user_iface_log_write(0, txt);
	}
}

static	int	coins_values		(void)
{
	char	txt[LOG_LINE_LEN];

	for (ptrdiff_t i = 0; i < coins_n; i++) {
		coins[i].value	= coin_value(coins[i].diameter_mm);
		if (coins[i].value < 0)
			return	COINS_NOK_SIZE;
		snprintf(txt, LOG_LINE_LEN, "Coin[%ti]:	%.2lf EUR",
						i, coins[i].value);
		user_iface_log_write(0, txt);
	}

	return	0;
}

static	double	coin_value		(double diameter_mm)
{

	if (diameter_mm > (COIN_DIAMETER_2E + 1.5))
		return	-1;
	if (diameter_mm > AVGfast(COIN_DIAMETER_2E, COIN_DIAMETER_50c))
		return	2.00;
	if (diameter_mm > AVGfast(COIN_DIAMETER_50c, COIN_DIAMETER_1E))
		return	0.50;
	if (diameter_mm > AVGfast(COIN_DIAMETER_1E, COIN_DIAMETER_20c))
		return	1.00;
	if (diameter_mm > AVGfast(COIN_DIAMETER_20c, COIN_DIAMETER_5c))
		return	0.20;
	if (diameter_mm > AVGfast(COIN_DIAMETER_5c, COIN_DIAMETER_10c))
		return	0.05;
	if (diameter_mm > AVGfast(COIN_DIAMETER_10c, COIN_DIAMETER_2c))
		return	0.10;
	if (diameter_mm > AVGfast(COIN_DIAMETER_2c, COIN_DIAMETER_1c))
		return	0.02;
	if (diameter_mm > (COIN_DIAMETER_1c - 1.5))
		return	0.01;
	return	-1;
}

static	void	coins_total_value	(void)
{
	char	txt[LOG_LINE_LEN];

	value_total	= 0.00;
	for (ptrdiff_t i = 0; i < coins_n; i++)
		value_total += coins[i].value;

	snprintf(txt, LOG_LINE_LEN, "Total value:	%.2lf EUR",value_total);
	user_iface_log_write(0, txt);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
