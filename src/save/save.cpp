/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/save/save.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include <sys/stat.h>

#include <opencv2/opencv.hpp>

#include "libalx/base/errno/errno_str.hpp"
#include "libalx/base/stddef/restrict.hpp"

#include "vision-artificial/user/iface.hpp"


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
	class cv::Mat	image;
	char		home_path[FILENAME_MAX];
	char		user_prog_path[FILENAME_MAX];
	char		saved_path[FILENAME_MAX];
	char		labels_path[FILENAME_MAX];
	char		labels_fail_path[FILENAME_MAX];
	char		lighters_path[FILENAME_MAX];
	char		lighters_fail_path[FILENAME_MAX];
	char		objects_path[FILENAME_MAX];
	char		objects_fail_path[FILENAME_MAX];
	char		coins_path[FILENAME_MAX];
	char		coins_fail_path[FILENAME_MAX];
	char		resistors_path[FILENAME_MAX];
	char		resistors_fail_path[FILENAME_MAX];
	char		saved_name[FILENAME_MAX];


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	save_init	(void)
{

	if (snprintf(home_path, FILENAME_MAX, "%s/",
				getenv(ENV_HOME))  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(user_prog_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_PROG_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(saved_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_SAVED_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(labels_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_LABELS_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(labels_fail_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_LABELS_FAIL_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(lighters_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_LIGHTERS_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(lighters_fail_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_LIGHTERS_FAIL_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(objects_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_OBJECTS_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(objects_fail_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_OBJECTS_FAIL_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(coins_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_COINS_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(coins_fail_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_COINS_FAIL_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(resistors_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_RESISTORS_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	if (snprintf(resistors_fail_path, FILENAME_MAX, "%s/%s/",
				home_path,
				USER_RESISTORS_FAIL_DIR)  >=  FILENAME_MAX) {
		goto err;
	}
	saved_name[0]	= '\0';

	if (mkdir(user_prog_path, 0700)) {
		switch (errno) {
		case EEXIST:
			/* OK */
			break;
		default:
			printf("%s: %s\n", errno_str[errno][0],
							errno_str[errno][1]);
		}
	}

	mkdir(saved_path, 0700);
	mkdir(labels_path, 0700);
	mkdir(labels_fail_path, 0700);

	return;
err:
	printf("Path is too large and has been truncated\n");
}

void	save_reset_fpath(void)
{

	if (snprintf(saved_path, FILENAME_MAX, "%s/%s/",
					home_path,
					USER_SAVED_DIR)  >=  FILENAME_MAX) {
		goto err;
	}

	return;
err:
	printf("Path is too large and has been truncated\n");
}

void	load_image_file	(const char *restrict fpath,
			const char *restrict fname)
{
	char	file_path[FILENAME_MAX];
	char	file_name[FILENAME_MAX];

	image.release();

	/* Set file_path */
	if (!fpath) {
		/* Default path */
		save_reset_fpath();
		snprintf(file_path, FILENAME_MAX, "%s", saved_path);
	} else {
		snprintf(file_path, FILENAME_MAX, "%s", fpath);
	}

	/* Set file_name */
	if (!fname)
		user_iface_fname(file_path, saved_name);
	else
		snprintf(saved_name, FILENAME_MAX, "%s", fname);

	/* File name */
	if (snprintf(file_name, FILENAME_MAX, "%s/%s",
					file_path,
					saved_name)  >=  FILENAME_MAX) {
		goto err_path;
	}

	image	= cv::imread(file_name, CV_LOAD_IMAGE_COLOR);
	if (image.empty())
		goto err_img;

	return;
err_img:
	printf("Could not load file: %s\n", file_name);
	return;
err_path:
	printf("Path is too large and has been truncated\n");
}

void	save_cleanup	(void)
{

	image.release();
}

void	save_image_file	(const char *restrict fpath,
			const char *restrict save_as)
{
	char	file_path[FILENAME_MAX];
	char	file_name[FILENAME_MAX];
	FILE	*fp;

	/* Set file_path */
	if (!fpath) {
		/* Default path */
		save_reset_fpath();
		snprintf(file_path, FILENAME_MAX, "%s", saved_path);
	} else {
		snprintf(file_path, FILENAME_MAX, "%s", fpath);
	}

	/* Set file_name */
	if (!save_as) {
		snprintf(saved_name, FILENAME_MAX, "%s", SAVED_NAME_DEFAULT);
		user_iface_fname(saved_path, saved_name);
	} else {
		snprintf(saved_name, FILENAME_MAX, "%s", save_as);
	}

	/* Prepend the path */
	if (snprintf(file_name, FILENAME_MAX, "%s/%s",
						file_path,
						saved_name)  >=  FILENAME_MAX) {
		goto err_path;
	}

	fp =	fopen(file_name, "r");
	if (fp) {
		/* Name in use;  ask once more */
		fclose(fp);
		user_iface_fname(saved_path, saved_name);
		if (snprintf(file_name, FILENAME_MAX, "%s/%s",
						file_path,
						saved_name)  >=  FILENAME_MAX) {
			goto err_path;
		}
	}

	user_iface_log_write(2, saved_name);

	cv::imwrite(file_name, image);

	return;
err_path:
	printf("Path is too large and has been truncated\n");
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
