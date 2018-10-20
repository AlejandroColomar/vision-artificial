/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
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
<<<<<<< HEAD:modules/save/src/save.cpp
	#include <cstdlib>
=======
	#include <stdlib.h>
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/save/src/save.c

/* Linux ---------------------------------------------------------------------*/
		/* mkdir */
	#include <sys/stat.h>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
<<<<<<< HEAD:modules/save/src/save.cpp
	#include <opencv2/opencv.hpp>
=======
	#include <opencv/cv.h>
		/* cvLoadImage & cvSaveImage */
	#include <opencv/highgui.h>
//	#include <opencv2/imgcodecs/imgcodecs_c.h>
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/save/src/save.c

/* Project -------------------------------------------------------------------*/
		/* user_iface_save_name() */
	#include "user_iface.hpp"

/* Module --------------------------------------------------------------------*/
<<<<<<< HEAD:modules/save/src/save.cpp
	#include "save.hpp"
=======
	#include "save.h"
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/save/src/save.c


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
class cv::Mat	image;
char		home_path [FILENAME_MAX];
char		user_prog_path [FILENAME_MAX];
char		saved_path [FILENAME_MAX];
char		saved_name [FILENAME_MAX];


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	save_init	(void)
{
	snprintf(home_path, FILENAME_MAX, "%s/", getenv(ENV_HOME));
	snprintf(user_prog_path, FILENAME_MAX, "%s/%s/", home_path, USER_PROG_DIR);
	snprintf(saved_path, FILENAME_MAX, "%s/%s/", home_path, USER_SAVED_DIR);
	sprintf(saved_name, "");

	int	err;
	err	= mkdir(user_prog_path, 0700);

	if (!err) {
		mkdir(saved_path, 0700);
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
}

void	save_clr	(void)
{
	snprintf(saved_path, FILENAME_MAX, "%s/%s/", home_path, USER_SAVED_DIR);
}

void	load_image_file	(void)
{
	char	file_name [FILENAME_MAX];

	/* Free old image */
	image.release();

	/* File name */
	snprintf(file_name, FILENAME_MAX, "%s/%s", saved_path, saved_name);

	/* Load image */
<<<<<<< HEAD:modules/save/src/save.cpp
	image	= cv::imread(file_name, CV_LOAD_IMAGE_COLOR);
=======
//	image	= cvLoadImage(file_name, CV_LOAD_IMAGE_COLOR);
	image	= cvvLoadImage(file_name);
>>>>>>> 648c9aa9fca19cbf3730029691500fa92c1ad323:modules/save/src/save.c

	/* Manage load error */
	if (image.empty()) {
		printf("Could not load file: %s\n", file_name);
//		exit(0);
	}
}

void	save_cleanup	(void)
{
	/* Free old image */
	image.release();
}

void	save_image_file	(void)
{
	char	file_name [FILENAME_MAX];
	FILE	*fp;

	/* Default path & name */
	save_clr();
	snprintf(saved_name, FILENAME_MAX, "%s", SAVED_NAME_DEFAULT);

	/* Request file name */
	user_iface_save_name(saved_path, saved_name, FILENAME_MAX);

	/* Prepend the path */
	snprintf(file_name, FILENAME_MAX, "%s/%s", saved_path, saved_name);

	fp =	fopen(file_name, "r");
	if (fp) {
		fclose(fp);
		/* Name in use;  ask once more */
		user_iface_save_name(saved_path, saved_name, FILENAME_MAX);
		/* Prepend the path */
		snprintf(file_name, FILENAME_MAX, "%s/%s", saved_path, saved_name);
	}

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "%s", saved_name);
	user_iface_log.lvl[user_iface_log.len]	= 2;
	(user_iface_log.len)++;


	/* Write to a new file */
	cv::imwrite(file_name, image);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
