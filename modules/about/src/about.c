/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* printf() */
	#include <stdio.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* alx_prn_file() */
	#include "alx_file.h"

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
	snprintf(share_path, FILENAME_MAX, "%s/%s/", INSTALL_SHARE_DIR, SHARE_DIR);
}

void	snprint_share_file	(char *dest, int destsize, int share_file)
{
	char	file_name [FILENAME_MAX];

	switch (share_file) {
	case SHARE_COPYRIGHT:
		snprintf(file_name, FILENAME_MAX, "%s/%s", share_path, "COPYRIGHT.txt");
		break;
	case SHARE_DISCLAIMER:
		snprintf(file_name, FILENAME_MAX, "%s/%s", share_path, "DISCLAIMER.txt");
		break;
	case SHARE_HELP:
		snprintf(file_name, FILENAME_MAX, "%s/%s", share_path, "HELP.txt");
		break;
	case SHARE_LICENSE:
		snprintf(file_name, FILENAME_MAX, "%s/%s", share_path, "LICENSE.txt");
		break;
	case SHARE_USAGE:
		snprintf(file_name, FILENAME_MAX, "%s/%s", share_path, "USAGE.txt");
		break;
	}

	alx_snprint_file(dest, destsize, file_name);
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
