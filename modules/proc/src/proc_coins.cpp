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

/* Project -------------------------------------------------------------------*/
		/* constants */
	#include "img_iface.hpp"
		/* user_iface_log */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
	#include "proc_common.hpp"

	#include "proc_coins.hpp"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/

/* Static --------------------------------------------------------------------*/
static	std::vector <std::vector <cv::Point_ <int>>>	contours;
static	class cv::Mat					hierarchy;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	result_coins		(int status);

static	int	coins_find		(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_coins			(void)
{
	int	status;

	proc_save_mem(0);
	/* Find coins */
	{
		/* Measure time */
		clock_start();

		status	= coins_find();
		if (status) {
			result_coins(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find coins");
	}
	/* Calibrate with the biggest coin; every img should have a 2 € coin */
	{
		/* Measure time */
		clock_start();

		status	= coins_find();
		if (status) {
			result_coins(status);
			return	status;
		}

		/* Measure time */
		clock_stop("Find coins");
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
	case COINS_NOK_OVERLAP:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK_OVERLAP");
		break;
	default:
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Coin:  NOK");
		break;
	}
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;
}

static	int	coins_find		(void)
{
	int	status;

	proc_load_mem(0);

	proc_cmp(IMG_IFACE_CMP_BLUE);
//	proc_smooth(IMGI_SMOOTH_MEDIAN, 11);
	proc_threshold(cv::THRESH_BINARY_INV, IMG_IFACE_THR_OTSU);
//	proc_threshold(cv::THRESH_BINARY_INV, 100);
	proc_distance_transform();
	proc_local_max();
	proc_dilate(8);
	proc_save_mem(1);

	proc_contours(&contours, &hierarchy);

	/* If no contour is found, error:  NOK_COINS */
	if (!contours.size()) {
		status	= COINS_NOK_COINS;
		return	status;
	}

	status	= COINS_OK;
	return	status;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
