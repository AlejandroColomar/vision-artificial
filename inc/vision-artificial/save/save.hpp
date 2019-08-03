/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
#pragma once	/* vision-artificial/save/save.hpp */


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include <cstdio>

#include <opencv2/core/core.hpp>

#include "libalx/base/compiler/restrict.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	ENV_HOME		"HOME"

	# define	USER_PROG_DIR		"vision-artificial/"
	# define	USER_SAVED_DIR		"vision-artificial/saved/"
	# define	SAVED_NAME_DEFAULT	"saved"


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
extern	class cv::Mat	image;
extern	char		home_path [FILENAME_MAX];
extern	char		user_prog_path [FILENAME_MAX];
extern	char		saved_path [FILENAME_MAX];
extern	char		saved_name [FILENAME_MAX];


/******************************************************************************
 ******* extern functions *****************************************************
 ******************************************************************************/
extern	"C"
{
void	save_init	(void);
void	save_cleanup	(void);
void	save_reset_fpath(void);
void	load_image_file	(const char *fpath, const char *fname);
void	save_image_file	(const char *fpath, const char *save_as);
}


/******************************************************************************
 ******* static inline functions (prototypes) *********************************
 ******************************************************************************/


/******************************************************************************
 ******* static inline functions (definitions) ********************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
