/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* printf() */
	#include <stdio.h>

/* libalx --------------------------------------------------------------------*/
		/* alx_snprint_file() */
	#include "alx_file.h"

/* Module --------------------------------------------------------------------*/
	#include "about.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	BUFF_SIZE_TEXT	(1048576)

	# define	BEGINNING	"\n┌──────────────────────────────────────────────────────────────────────────────┐\n"
	# define	ENDING		"└──────────────────────────────────────────────────────────────────────────────┘\n\n"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
char	share_path [FILENAME_MAX];


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	about_init		(void)
{
	snprintf(share_path, FILENAME_MAX, "%s/%s/",
						INSTALL_SHARE_DIR, SHARE_DIR);
}

void	snprint_share_file	(char *dest, int destsize, int share_file)
{
	char	file_name [FILENAME_MAX];

	switch (share_file) {
	case SHARE_COPYRIGHT:
		if (snprintf(file_name, FILENAME_MAX, "%s/%s",
						share_path, "COPYRIGHT.txt")) {
			goto err_path;
		}
		break;
	case SHARE_DISCLAIMER:
		if (snprintf(file_name, FILENAME_MAX, "%s/%s",
						share_path, "DISCLAIMER.txt")) {
			goto err_path;
		}
		break;
	case SHARE_HELP:
		if (snprintf(file_name, FILENAME_MAX, "%s/%s",
						share_path, "HELP.txt")) {
			goto err_path;
		}
		break;
	case SHARE_LICENSE:
		if (snprintf(file_name, FILENAME_MAX, "%s/%s",
						share_path, "LICENSE.txt")) {
			goto err_path;
		}
		break;
	case SHARE_USAGE:
		if (snprintf(file_name, FILENAME_MAX, "%s/%s",
						share_path, "USAGE.txt")) {
			goto err_path;
		}
		break;
	}

	alx_snprint_file(dest, destsize, file_name);

	return;


err_path:
	printf("Path is too large and has been truncated\n");
	printf("File could not be shown!\n");
}

void	print_share_file	(int share_file)
{
	char	str [BUFF_SIZE_TEXT];

	snprint_share_file(str, BUFF_SIZE_TEXT, share_file);

	printf(BEGINNING);
	printf("%s", str);
	printf(ENDING);
}

void	print_version		(void)
{
	printf("" PROG_NAME " " PROG_VERSION "\n\n");
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
