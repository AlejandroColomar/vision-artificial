/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* opencv */
	#include <cv.h>
		/* cvLoadImage & cvSaveImage */
	#include <highgui.h>
		/* errno */
	#include <errno.h>
		/* fscanf() & fprintf() & FILE & FILENAME_MAX & snprintf() */
	#include <stdio.h>
		/* getenv() */
	#include <stdlib.h>
		/* mkdir */
	#include <sys/stat.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* user_iface_save_name() */
	#include "user_iface.h"

	#include "save.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
struct _IplImage	*image;
char			home_path [FILENAME_MAX];
char			user_prog_path [FILENAME_MAX];
char			saved_path [FILENAME_MAX];
char			saved_name [FILENAME_MAX];


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
			exit(EXIT_FAILURE);
			break;

		case EEXIST:
			/* OK */
			break;

		default:
			printf("WTF?!");
			exit(EXIT_FAILURE);
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
	cvReleaseImage(&image);

	/* File name */
	snprintf(file_name, FILENAME_MAX, "%s/%s", saved_path, saved_name);

	/* Load image */
	image	= cvLoadImage(file_name, CV_LOAD_IMAGE_COLOR);

	/* Manage load error */
	if (!image) {
		printf("Could not load file: %s\n", file_name);
		exit(0);
	}
}

void	save_cleanup	(void)
{
	/* Free old image */
	cvReleaseImage(&image);
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

	/* Write to a new file */
	cvSaveImage(file_name, image, NULL);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
