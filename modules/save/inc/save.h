/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_SAVE_H
	# define	VA_SAVE_H


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* FILENAME_MAX */
	#include <stdio.h>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	ENV_HOME		"HOME"

	# define	USER_PROG_DIR		"vision-artificial/"
	# define	USER_SAVED_DIR		"vision-artificial/saved/"
	# define	USER_LABELS_DIR		"vision-artificial/labels/"
	# define	USER_LABELS_FAIL_DIR	"vision-artificial/labels/fail"
	# define	USER_OBJECTS_DIR	"vision-artificial/objects/"
	# define	USER_OBJECTS_FAIL_DIR	"vision-artificial/objects/fail"
	# define	USER_COINS_DIR		"vision-artificial/coins/"
	# define	USER_COINS_FAIL_DIR	"vision-artificial/coins/fail"
	# define	USER_RESISTORS_DIR	"vision-artificial/resistors/"
	# define	USER_RESISTORS_FAIL_DIR	"vision-artificial/resistors/fail"
	# define	SAVED_NAME_DEFAULT	"saved"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
	extern	char	home_path [FILENAME_MAX];
	extern	char	user_prog_path [FILENAME_MAX];
	extern	char	saved_path [FILENAME_MAX];
	extern	char	labels_path [FILENAME_MAX];
	extern	char	labels_fail_path [FILENAME_MAX];
	extern	char	objects_path [FILENAME_MAX];
	extern	char	objects_fail_path [FILENAME_MAX];
	extern	char	coins_path [FILENAME_MAX];
	extern	char	coins_fail_path [FILENAME_MAX];
	extern	char	resistors_path [FILENAME_MAX];
	extern	char	resistors_fail_path [FILENAME_MAX];
	extern	char	saved_name [FILENAME_MAX];


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	void	save_init	(void);
	void	save_cleanup	(void);
	void	save_clr	(void);
	void	load_image_file	(const char *fpath, const char *fname);
	void	save_image_file	(const char *fpath, const char *save_as);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif				/* save.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
