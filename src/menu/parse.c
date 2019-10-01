/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/menu/parse.h"

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "vision-artificial/menu/iface.h"
#include "vision-artificial/save/save.h"
#include "vision-artificial/share/share.h"
#include "vision-artificial/user/iface.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define OPT_LIST	"hLuv""f:"


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	parse_file	(char *arg);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	parse	(int argc, char *argv[])
{
	int	opt		= 0;
	int	opt_index	= 0;

	const	struct option	long_options[]	= {
		/* Standard */
		{"help",		no_argument,		0,	'h'},
		{"license",		no_argument,		0,	'L'},
		{"usage",		no_argument,		0,	'u'},
		{"version",		no_argument,		0,	'v'},
		/* Non-standard */
		{"file",		required_argument,	0,	'f'},
		/* Null */
		{0,			0,			0,	0}
	};

	while ((opt = getopt_long(argc, argv, OPT_LIST, long_options,
						&opt_index)) != -1) {

		switch (opt) {
		/* Standard */
		case 'h':
			print_share_file(SHARE_HELP);
			exit(EXIT_SUCCESS);
		case 'L':
			print_share_file(SHARE_LICENSE);
			exit(EXIT_SUCCESS);
		case 'u':
			print_share_file(SHARE_USAGE);
			exit(EXIT_SUCCESS);
		case 'v':
			print_version();
			exit(EXIT_SUCCESS);
		/* Non-standard */
		case 'f':
			parse_file(optarg);
			break;

		case '?':
			/* getopt_long already printed an error message. */
		default:
			print_share_file(SHARE_USAGE);
			exit(EXIT_FAILURE);
		}
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	parse_file	(char *arg)
{
	FILE	*fp;

	/* FIXME */
	fp	= fopen(arg, "r");
	if (!fp)
		goto err;
	fclose(fp);

	saved_path[0]	= '\0';
	snprintf(saved_name, FILENAME_MAX, "%s", arg);

	return;
err:
	printf("--file argument not valid\n");
	printf("It must be a valid file name (relative to saved dir)\n");
	exit(EXIT_FAILURE);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
