/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/about/about.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libalx/base/stddef/size.h"


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
char	share_path [FILENAME_MAX];


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	about_init		(void)
{

	if (snprintf(share_path, sizeof(share_path), "%s/vision-artificial/",
				INSTALL_SHARE_DIR)  >=  SSIZEOF(share_path)) {
		goto err;
	}
	return;
err:
	fprintf(stderr, "Path is too large and has been truncated\n");
	exit(EXIT_FAILURE);
}

void	print_share_file	(int file)
{
	char	fname [FILENAME_MAX];
	char	cmd[_POSIX_ARG_MAX];

	switch (file) {
	case SHARE_COPYRIGHT:
		if (snprintf(fname, sizeof(fname), "%s/%s", share_path,
					"COPYRIGHT.txt")  >=  SSIZEOF(fname)) {
			goto err;
		}
		break;
	case SHARE_DISCLAIMER:
		if (snprintf(fname, sizeof(fname), "%s/%s", share_path,
					"DISCLAIMER.txt")  >=  SSIZEOF(fname)) {
			goto err;
		}
		break;
	case SHARE_HELP:
		if (snprintf(fname, sizeof(fname), "%s/%s", share_path,
					"HELP.txt")  >=  SSIZEOF(fname)) {
			goto err;
		}
		break;
	case SHARE_LICENSE:
		if (snprintf(fname, sizeof(fname), "%s/%s", share_path,
					"LICENSE.txt")  >=  SSIZEOF(fname)) {
			goto err;
		}
		break;
	case SHARE_USAGE:
		if (snprintf(fname, sizeof(fname), "%s/%s", share_path,
					"USAGE.txt")  >=  SSIZEOF(fname)) {
			goto err;
		}
		break;
	}

	if (snprintf(cmd, sizeof(cmd), "less %s", fname)  >=  SSIZEOF(cmd))
		goto err;
	if (system(cmd)) {
		fprintf(stderr, "%s[%i]: %s(): %s", __FILE__, __LINE__,
						__func__, strerror(errno));
	}

	return;

err:
	fprintf(stderr, "Path is too large and has been truncated\n");
	fprintf(stderr, "File could not be shown!\n");
}

void	print_version		(void)
{

	printf(""PROG_NAME" "PROG_VERSION"\n\n");
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
