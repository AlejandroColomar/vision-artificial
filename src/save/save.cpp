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
#include <cstring>

#include <sys/stat.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define ALX_NO_PREFIX
#include <libalx/base/compiler/restrict.hpp>
#include <libalx/base/compiler/unused.hpp>
#include <libalx/base/errno/error.hpp>
#include <libalx/base/stdio/printf/sbprintf.hpp>
#include <libalx/extra/cv/highgui/file.hpp>

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
char		saved_name[FILENAME_MAX];


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	save_init	(void)
{

	if (sbprintf(home_path, NULL, "%s/", getenv(ENV_HOME)))
		goto err;
	if (sbprintf(user_prog_path, NULL, "%s/%s/",
					home_path, USER_PROG_DIR))
		goto err;
	if (sbprintf(saved_path, NULL, "%s/%s/", home_path, USER_SAVED_DIR))
		goto err;
	saved_name[0]	= '\0';

	if (mkdir(user_prog_path, 0700)) {
		if (errno != EEXIST)
			errorx(EXIT_FAILURE, user_prog_path);
	}
	mkdir(saved_path, 0700);

	return;
err:
	errorx(EXIT_FAILURE, "Path is too large and has been truncated\n");
}

void	save_reset_fpath(void)
{

	if (sbprintf(saved_path, NULL, "%s/%s/", home_path, USER_SAVED_DIR))
		goto err;
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
		ALX_UNUSED(sbprintf(file_path, NULL, "%s", saved_path));
	} else {
		ALX_UNUSED(sbprintf(file_path, NULL, "%s", fpath));
	}

	/* Set file_name */
	if (!fname)
		user_iface_fname(file_path, saved_name);
	else
		ALX_UNUSED(sbprintf(saved_name, NULL, "%s", fname));

	/* File name */
	if (sbprintf(file_name, NULL, "%s/%s", file_path, saved_name))
		goto err_path;

	alx::CV::imread(&image, file_name);
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
		ALX_UNUSED(sbprintf(file_path, NULL, "%s", saved_path));
	} else {
		ALX_UNUSED(sbprintf(file_path, NULL, "%s", fpath));
	}

	/* Set file_name */
	if (!save_as) {
		ALX_UNUSED(sbprintf(saved_name,NULL, "%s", SAVED_NAME_DEFAULT));
		user_iface_fname(saved_path, saved_name);
	} else {
		ALX_UNUSED(sbprintf(saved_name, NULL, "%s", save_as));
	}

	/* Prepend the path */
	if (sbprintf(file_name, NULL, "%s/%s", file_path, saved_name))
		goto err_path;

	fp =	fopen(file_name, "r");
	if (fp) {
		/* Name in use;  ask once more */
		fclose(fp);
		user_iface_fname(saved_path, saved_name);
		if (sbprintf(file_name,NULL,"%s/%s", file_path, saved_name))
			goto err_path;
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
