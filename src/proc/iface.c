/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/proc/iface.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libalx/base/stddef/size.h"
#include "libalx/base/stdio/sscan.h"
#include "libalx/base/stdio/wait.h"

#include "vision-artificial/image/iface.h"
#include "vision-artificial/proc/coins.h"
#include "vision-artificial/proc/common.h"
#include "vision-artificial/proc/label.h"
#include "vision-artificial/proc/lighters.h"
#include "vision-artificial/proc/objects.h"
#include "vision-artificial/proc/resistor.h"
#include "vision-artificial/save/save.h"
#include "vision-artificial/user/iface.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


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
 ******* static functions (prototypes) ****************************************
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
	char	txt[LOG_LINE_LEN];

	time_0	= clock();

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
	case PROC_MODE_LIGHTERS_SERIES:
		error	= proc_lighter();
		break;
	default:
		error	= -1;
	}

	time_1		= clock();
	time_tot	= ((double) time_1 - time_0) / CLOCKS_PER_SEC;
	snprintf(txt, LOG_LINE_LEN, "Item total time:  %.3lf", time_tot);
	user_iface_log_write(0, txt);

	return	error;
}

void	proc_iface_series	(void)
{
	int	num_len;
	char	file_basename[FILENAME_MAX];
	char	file_ext[FILENAME_MAX];
	char	file_name[FILENAME_MAX];
	char	save_error_as[FILENAME_MAX];
	/* if i starts being 0, the camera needs calibration */
	int	i;
	char	txt[LOG_LINE_LEN];
	char	tmp[FILENAME_MAX];

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
		snprintf(proc_fail_path, FILENAME_MAX,"%s",resistors_fail_path);
		snprintf(file_basename, FILENAME_MAX, "r");
		num_len	= 4;
		snprintf(file_ext, FILENAME_MAX, ".png");
		i	= 1;
		break;
	case PROC_MODE_LIGHTERS_SERIES:
		snprintf(proc_path, FILENAME_MAX, "%s", lighters_path);
		snprintf(proc_fail_path, FILENAME_MAX, "%s",lighters_fail_path);
		snprintf(file_basename, FILENAME_MAX, "lighters");
		num_len	= 4;
		snprintf(file_ext, FILENAME_MAX, ".png");
		i	= 1;
		break;
	default:
		return;
	}

	for (;; i++) {
		if (snprintf(file_name, FILENAME_MAX, "%s%04i%s",
					file_basename,
					i, file_ext)  >=  FILENAME_MAX) {
			goto err_path;
		}

		memcpy(tmp, file_name, ARRAY_SIZE(file_name));
		if (alx_sscan_fname(proc_path, file_name, true, tmp))
			break;

		errno	= 0;
		img_iface_load(proc_path, file_name);

		if (errno) {
			printf("errno:%i\n", errno);
			goto err_load;
		}

		/* Process */
		if (proc_iface_single(proc_mode)) {
			/* Save failed image into file */
			proc_show_img();
			if (snprintf(save_error_as, FILENAME_MAX,"%s%0*i_err%s",
						file_basename, num_len, i,
						file_ext)  >=  FILENAME_MAX) {
				goto err_path;
			}
			save_image_file(proc_fail_path, save_error_as);
		}

		/* Show log */
		snprintf(txt, LOG_LINE_LEN, "%04i", i);
		user_iface_show_log(txt, "Item");

		if (proc_debug >= PROC_DBG_STOP_ITEM)
			alx_wait4enter();
err_load:
		if (!i)
			proc_mode--;
	}

	return;

err_path:
	printf("Path is too large and has been truncated\n");
	exit(EXIT_FAILURE);
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
