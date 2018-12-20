/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* errno */
	#include <errno.h>
	#include <stdbool.h>
		/* snprintf() & fflush() */
	#include <stdio.h>
		/* clock_t & clock() & CLOCKS_PER_SEC */
	#include <time.h>

/* libalx -------------------------------------------------------------------*/
		/* alx_sscan_fname() */
	#include "alx_input.h"

/* Project -------------------------------------------------------------------*/
		/* img_iface_load() */
	#include "img_iface.h"
		/* user_iface_log */
	#include "user_iface.h"
		/* saved_path */
	#include "save.h"

/* Module --------------------------------------------------------------------*/
	#include "proc_label.h"
	#include "proc_lighter.h"
	#include "proc_objects.h"
	#include "proc_coins.h"
	#include "proc_resistor.h"
	#include "proc_common.h"

	#include "proc_iface.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
	int	proc_debug;
	int	proc_mode;

/* Static --------------------------------------------------------------------*/
static	char	proc_path [FILENAME_MAX];
static	char	proc_fail_path [FILENAME_MAX];


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	proc_iface_single	(int action)
{
	int	error;
	clock_t	time_0;
	clock_t	time_1;
	double	time_tot;

	/* Init timer */
	time_0	= clock();

	/* Process */
	switch (action) {
	case PROC_MODE_LABEL_SERIES:
		error	= proc_label();
		break;
	case PROC_MODE_OBJECTS_CALIB:
		error	= proc_objects_calibrate();
		break;
	case PROC_MODE_OBJECTS_SERIES:
		error	= proc_objects();
		break;
	case PROC_MODE_COINS_SERIES:
		error	= proc_coins();
		break;
	case PROC_MODE_RESISTOR_SERIES:
		error	= proc_resistor();
		break;
	case PROC_MODE_LIGHTER_CALIB:
		error	= proc_lighter_calibrate();
		break;
	case PROC_MODE_LIGHTER_SERIES:
		error	= proc_lighter();
		break;
	default:
		error	= -1;
	}

	/* End timer */
	time_1	= clock();

	/* Calculate time in seconds */
	time_tot	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;

	/* Write time into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Item total time:  %.3lf",
						time_tot);
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	return	error;
}

void	proc_iface_series	(void)
{
	bool	file_error;
	int	num_len;
	char	file_basename [FILENAME_MAX];
	char	file_ext [FILENAME_MAX];
	char	file_name [FILENAME_MAX];
	bool	proc_error;
	char	save_error_as [FILENAME_MAX];
	bool	wh;
		/* if i starts being 0, the camera needs calibration */
	int	i;

	switch (proc_mode) {
	case PROC_MODE_LABEL_SERIES:
		snprintf(proc_path, FILENAME_MAX, "%s", labels_path);
		snprintf(proc_fail_path, FILENAME_MAX, "%s", labels_fail_path);
		snprintf(file_basename, FILENAME_MAX, "b");
		num_len	= 4;
		snprintf(file_ext, FILENAME_MAX, ".BMP");
		i	= 1;
		break;
	case PROC_MODE_OBJECTS_SERIES:
		snprintf(proc_path, FILENAME_MAX, "%s", objects_path);
		snprintf(proc_fail_path, FILENAME_MAX, "%s", objects_fail_path);
		snprintf(file_basename, FILENAME_MAX, "o");
		num_len	= 4;
		snprintf(file_ext, FILENAME_MAX, ".jpeg");
		i	= 0;
		proc_mode++;
		break;
	case PROC_MODE_COINS_SERIES:
		snprintf(proc_path, FILENAME_MAX, "%s", coins_path);
		snprintf(proc_fail_path, FILENAME_MAX, "%s", coins_fail_path);
		snprintf(file_basename, FILENAME_MAX, "c");
		num_len	= 4;
		snprintf(file_ext, FILENAME_MAX, ".jpeg");
		i	= 1;
		break;
	case PROC_MODE_RESISTOR_SERIES:
		snprintf(proc_path, FILENAME_MAX, "%s", resistors_path);
		snprintf(proc_fail_path, FILENAME_MAX, "%s", resistors_fail_path);
		snprintf(file_basename, FILENAME_MAX, "r");
		num_len	= 4;
		snprintf(file_ext, FILENAME_MAX, ".png");
		i	= 1;
		break;
	case PROC_MODE_LIGHTER_SERIES:
		snprintf(proc_path, FILENAME_MAX, "%s", lighters_path);
		snprintf(proc_fail_path, FILENAME_MAX, "%s", lighters_fail_path);
		snprintf(file_basename, FILENAME_MAX, "lighter");
		num_len	= 4;
		snprintf(file_ext, FILENAME_MAX, ".png");
		i	= 0;
		proc_mode++;
		break;
	default:
		return;
	}

	wh	= true;
	for (; wh; i++) {
		snprintf(file_name, FILENAME_MAX, "%s%04i%s",
						file_basename, i, file_ext);

		file_error	= alx_sscan_fname(proc_path, file_name,
						true, file_name);

		if (file_error) {
			wh	= false;
		} else {
			errno	= 0;
			img_iface_load(proc_path, file_name);

			if (!errno) {
				/* Process */
				proc_error	= proc_iface_single(proc_mode);

				if (proc_error) {
					/* Save failed image into file */
					proc_show_img();
					snprintf(save_error_as, FILENAME_MAX,
							"%s%0*i_err%s",
							file_basename,
							num_len, i,
							file_ext);
					save_image_file(proc_fail_path,
							save_error_as);
				}

				/* Show log */
				char	txt_tmp [80];
				snprintf(txt_tmp, FILENAME_MAX, "%04i", i);
				user_iface_show_log(txt_tmp, "Item");

				if (proc_debug >= PROC_DBG_STOP_ITEM) {
					getchar();
				}
			} else {
				printf("errno:%i\n", errno);
			}
		}

		if (!i) {
			proc_mode--;
		}
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
