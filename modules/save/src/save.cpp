/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* errno */
	#include <cerrno>
		/* fscanf() & fprintf() & FILE & FILENAME_MAX & snprintf() */
	#include <cstdio>
		/* getenv() */
	#include <cstdlib>

/* Linux ---------------------------------------------------------------------*/
		/* mkdir */
	#include <sys/stat.h>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <opencv2/opencv.hpp>

/* Project -------------------------------------------------------------------*/
		/* user_iface_save_name() */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
	#include "save.hpp"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
class cv::Mat	image;
char		home_path [FILENAME_MAX];
char		user_prog_path [FILENAME_MAX];
char		saved_path [FILENAME_MAX];
char		labels_path [FILENAME_MAX];
char		labels_fail_path [FILENAME_MAX];
char		lighters_path [FILENAME_MAX];
char		lighters_fail_path [FILENAME_MAX];
char		objects_path [FILENAME_MAX];
char		objects_fail_path [FILENAME_MAX];
char		coins_path [FILENAME_MAX];
char		coins_fail_path [FILENAME_MAX];
char		resistors_path [FILENAME_MAX];
char		resistors_fail_path [FILENAME_MAX];
char		saved_name [FILENAME_MAX];


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	save_init	(void)
{
	if (snprintf(home_path, FILENAME_MAX, "%s/", getenv(ENV_HOME))) {
		goto err_path;
	}
	if (snprintf(user_prog_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_PROG_DIR)) {
		goto err_path;
	}
	if (snprintf(saved_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_SAVED_DIR)) {
		goto err_path;
	}
	if (snprintf(labels_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_LABELS_DIR)) {
		goto err_path;
	}
	if (snprintf(labels_fail_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_LABELS_FAIL_DIR)) {
		goto err_path;
	}
	if (snprintf(lighters_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_LIGHTERS_DIR)) {
		goto err_path;
	}
	if (snprintf(lighters_fail_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_LIGHTERS_FAIL_DIR)) {
		goto err_path;
	}
	if (snprintf(objects_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_OBJECTS_DIR)) {
		goto err_path;
	}
	if (snprintf(objects_fail_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_OBJECTS_FAIL_DIR)) {
		goto err_path;
	}
	if (snprintf(coins_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_COINS_DIR)) {
		goto err_path;
	}
	if (snprintf(coins_fail_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_COINS_FAIL_DIR)) {
		goto err_path;
	}
	if (snprintf(resistors_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_RESISTORS_DIR)) {
		goto err_path;
	}
	if (snprintf(resistors_fail_path, FILENAME_MAX, "%s/%s/",
					home_path, USER_RESISTORS_FAIL_DIR)) {
		goto err_path;
	}
	saved_name[0]	= '\0';

	int	err;
	err	= mkdir(user_prog_path, 0700);

	if (!err) {
	} else {

		switch (errno) {
		case EACCES:
			printf("err = EACCES");
//			exit(EXIT_FAILURE);
			break;

		case EEXIST:
			/* OK */
			break;

		default:
			printf("WTF?!");
//			exit(EXIT_FAILURE);
		}
	}

	mkdir(saved_path, 0700);
	mkdir(labels_path, 0700);
	mkdir(labels_fail_path, 0700);

	return;


err_path:
	printf("Path is too large and has been truncated\n");
	exit(EXIT_FAILURE);
}

void	save_clr	(void)
{
	if (snprintf(saved_path, FILENAME_MAX, "%s/%s/",
						home_path, USER_SAVED_DIR)) {
		goto err_path;
	}

	return;


err_path:
	printf("Path is too large and has been truncated\n");
	exit(EXIT_FAILURE);
}

void	load_image_file	(const char *fpath, const char *fname)
{
	char	file_path [FILENAME_MAX];
	char	file_name [FILENAME_MAX];

	/* Free old image */
	image.release();

	/* Set file_path */
	if (!fpath) {
		/* Default path */
		save_clr();
		snprintf(file_path, FILENAME_MAX, "%s", saved_path);
	} else {
		snprintf(file_path, FILENAME_MAX, "%s", fpath);
	}

	/* Set file_name */
	if (!fname) {
		/* Request file name */
		user_iface_fname(file_path, saved_name);
	} else {
		snprintf(saved_name, FILENAME_MAX, "%s", fname);
	}

	/* File name */
	if (snprintf(file_name, FILENAME_MAX, "%s/%s", file_path, saved_name)) {
		goto err_path;
	}

	/* Load image */
	image	= cv::imread(file_name, CV_LOAD_IMAGE_COLOR);

	/* Manage load error */
	if (image.empty()) {
		printf("Could not load file: %s\n", file_name);
//		exit(0);
	}

	return;


err_path:
	printf("Path is too large and has been truncated\n");
	exit(EXIT_FAILURE);
}

void	save_cleanup	(void)
{
	/* Free old image */
	image.release();
}

void	save_image_file	(const char *fpath, const char *save_as)
{
	char	file_path [FILENAME_MAX];
	char	file_name [FILENAME_MAX];
	FILE	*fp;

	/* Set file_path */
	if (!fpath) {
		/* Default path */
		save_clr();
		snprintf(file_path, FILENAME_MAX, "%s", saved_path);
	} else {
		snprintf(file_path, FILENAME_MAX, "%s", fpath);
	}

	/* Set file_name */
	if (!save_as) {
		/* Default name */
		snprintf(saved_name, FILENAME_MAX, "%s", SAVED_NAME_DEFAULT);
		/* Request file name */
		user_iface_fname(saved_path, saved_name);
	} else {
		snprintf(saved_name, FILENAME_MAX, "%s", save_as);
	}

	/* Prepend the path */
	if (snprintf(file_name, FILENAME_MAX, "%s/%s", file_path, saved_name)) {
		goto err_path;
	}

	fp =	fopen(file_name, "r");
	if (fp) {
		/* Name in use;  ask once more */
		fclose(fp);
		user_iface_fname(saved_path, saved_name);
		if (snprintf(file_name, FILENAME_MAX, "%s/%s",
						file_path, saved_name)) {
			goto err_path;
		}
	}

	/* Write into log */
	(void)snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"%s", saved_name);
	user_iface_log.lvl[user_iface_log.len]	= 2;
	(user_iface_log.len)++;


	/* Write to a new file */
	cv::imwrite(file_name, image);

	return;


err_path:
	printf("Path is too large and has been truncated\n");
	exit(EXIT_FAILURE);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
