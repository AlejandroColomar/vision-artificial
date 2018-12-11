/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* snprintf() & fflush() */
	#include <cstdio>

/* Packages ------------------------------------------------------------------*/
		/* openCV */
	#include <opencv2/opencv.hpp>

/* libalx ------------------------------------------------------------------*/
		/* alx_maximum_u8() */
	#include "alx_math.hpp"

/* Project -------------------------------------------------------------------*/
		/* constants */
	#include "img_iface.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
	#include "proc_common.hpp"

	#include "proc_coins.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	COINS_MAX	1024


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
struct	Coins_Properties {
	/* position */
	int	x;
	int	y;

	/* value */
	uint8_t	diameter_pix;
	double	diameter_mm;
	double	value;
};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/

/* Static --------------------------------------------------------------------*/
static	std::vector <std::vector <cv::Point_ <int>>>	contours;
static	class cv::Mat					hierarchy;
static	class cv::Rect_ <int>				rectangle [COINS_MAX];
static	int						coins_n;
static	struct Coins_Properties				coins [COINS_MAX];
static	double						ratio_mm_per_pix;
static	double						value_total;


/******************************************************************************
 ******* static functions *****************************************************
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
int	proc_coins			(void)
{
	int	status;

	proc_save_mem(0);
	/* Segmentate coins */
	{
		/* Measure time */
		clock_start();

		coins_segmentate();

		/* Measure time */
		clock_stop("Segmentate coins");
	}
	/* Find coins positions */
	{
		/* Measure time */
		clock_start();

		status	= coins_positions();
		if (status) {
			result_coins(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find coins positions");
	}
	/* Get coin diameters in pixels */
	{
		/* Measure time */
		clock_start();

		coins_diameters_pix();

		/* Measure time */
		clock_stop("Coins diameters in pixels");
	}
	/* Calibrate with the biggest coin; every img should have a 2 € coin */
	{
		/* Measure time */
		clock_start();

		calibrate_mm_per_pix();

		/* Measure time */
		clock_stop("Calibrate (mm per pix)");
	}
	/* Get coins diameters in mm */
	{
		/* Measure time */
		clock_start();

		coins_diameters_mm();

		/* Measure time */
		clock_stop("Coins diameters in mm");
	}
	/* Get coins values (by their sizes) */
	{
		/* Measure time */
		clock_start();

		status	= coins_values();
		if (status) {
			result_coins(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Coins values");
	}
	/* Add total value */
	{
		/* Measure time */
		clock_start();

		coins_total_value();

		/* Measure time */
		clock_stop("Total value");
	}

	status	= COINS_OK;
	result_coins(status);
	return	status;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_coins		(int status)
{
	/* Cleanup */

	/* Write result into log */
	switch (status) {
	case COINS_OK:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  OK");
		break;
	case COINS_NOK_COINS:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK_COINS");
		break;
#if 0
	case COINS_NOK_OVERLAP:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK_OVERLAP");
		break;
#endif
	case COINS_NOK_SIZE:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK_SIZE");
		break;
	default:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK");
		break;
	}
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

static	void	coins_segmentate	(void)
{
	proc_load_mem(0);

	proc_cmp(IMG_IFACE_CMP_BLUE);
	proc_threshold(cv::THRESH_BINARY_INV, IMG_IFACE_THR_OTSU);
	proc_distance_transform();
	proc_local_max();
	proc_dilate(6);
	proc_save_mem(1);
}

static	int	coins_positions		(void)
{
	int	status;

	proc_load_mem(1);

	proc_contours(&contours, &hierarchy);
	coins_n	= contours.size();

	/* If no contour is found, error:  NOK_COINS */
	if (!coins_n) {
		status	= COINS_NOK_COINS;
		return	status;
	}

	/* Get position of each contour */
	int	i;
	for (i = 0; i < coins_n; i++) {
		proc_bounding_rect(&(contours[i]), &(rectangle[i]), true);
		coins[i].x	= rectangle[i].x + rectangle[i].width / 2.0;
		coins[i].y	= rectangle[i].y + rectangle[i].height / 2.0;
	}

	status	= COINS_OK;
	return	status;
}

static	void	coins_diameters_pix	(void)
{
	proc_load_mem(1);

	/* Get coins diameters in pixels */
	int	i;
	for (i = 0; i < coins_n; i++) {
		proc_pixel_value(coins[i].x, coins[i].y,
						&(coins[i].diameter_pix));
		coins[i].diameter_pix	*= 2;
	}
}

static	void	calibrate_mm_per_pix	(void)
{
	uint8_t	coins_size_pix [coins_n];

	int	i;
	for (i = 0; i < coins_n; i++) {
		coins_size_pix[i]	= coins[i].diameter_pix;
	}

	int	max_pos;
	int	max_size;
	max_pos			= alx_maximum_u8(coins_n, coins_size_pix);
	max_size		= coins_size_pix[max_pos];
	ratio_mm_per_pix	= 25.75 / max_size;
}

static	void	coins_diameters_mm	(void)
{
	/* Get coins diameters in mm */
	int	i;
	for (i = 0; i < coins_n; i++) {
		coins[i].diameter_mm	= ratio_mm_per_pix *
							coins[i].diameter_pix;

		/* Write diameters into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin[%i]:	%.2lf mm",
						i,
						coins[i].diameter_mm);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}
}

static	int	coins_values		(void)
{
	int	status;

	/* Get coins values in € (EUR) */
	int	i;
	for (i = 0; i < coins_n; i++) {
		coins[i].value	= coin_value(coins[i].diameter_mm);

		/* If a coin is of invalid size, error:  NOK_SIZE */
		if (coins[i].value < 0) {
			status	= COINS_NOK_SIZE;
			return	status;
		}

		/* Write values into log */
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin[%i]:	%.2lf EUR",
						i,
						coins[i].value);
		user_iface_log.lvl[user_iface_log.len]	= 0;
		(user_iface_log.len)++;
	}

	status	= COINS_OK;
	return	status;
}

static	double	coin_value		(double diameter_mm)
{
	double	value;

	/* Get coin value by its diameter in mm */
	if (diameter_mm > 27.00) {
		value	= -1;
	} else if (diameter_mm > 25.00) {
		value	= 2.00;
	} else if (diameter_mm > 23.75) {
		value	= 0.50;
	} else if (diameter_mm > 22.75) {
		value	= 1.00;
	} else if (diameter_mm > 21.75) {
		value	= 0.20;
	} else if (diameter_mm > 20.50) {
		value	= 0.05;
	} else if (diameter_mm > 19.25) {
		value	= 0.10;
	} else if (diameter_mm > 17.50) {
		value	= 0.02;
	} else if (diameter_mm > 15.00) {
		value	= 0.01;
	} else {
		value	= -1;
	}

	return	value;
}

static	void	coins_total_value	(void)
{
	value_total	= 0.00;

	/* Get total value in € (EUR) */
	int	i;
	for (i = 0; i < coins_n; i++) {
		value_total	+= coins[i].value;
	}

	/* Write total value into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
					"Total value:	%.2lf EUR",
					value_total);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
