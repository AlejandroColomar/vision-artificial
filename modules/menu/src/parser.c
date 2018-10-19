/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
	#include <getopt.h>
		/* FILE & fopen() & snprintf() & FILENAME_MAX */
	#include <stdio.h>
		/* exit() */
	#include <stdlib.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* print_share_...() */
	#include "about.h"
		/* user_iface_mode */
	#include "user_iface.h"
		/* saved_name */
	#include "save.h"

		/* menu_iface_mode */
	#include "menu_iface.h"

	#include "parser.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	OPT_LIST	"hLuv""f:i:"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	parse_file		(char* argument);
static	void	parse_iface		(char* argument);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	parser	(int argc, char *argv[])
{
	int	opt		= 0;
	int	opt_index	= 0;

	struct option	long_options[]	= {
		/* Standard */
		{"help",		no_argument,		0,	'h'},
		{"license",		no_argument,		0,	'L'},
		{"usage",		no_argument,		0,	'u'},
		{"version",		no_argument,		0,	'v'},
		/* Non-standard */
		{"file",		required_argument,	0,	'f'},
		{"iface",		required_argument,	0,	'i'},
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

		case 'i':
			parse_iface(optarg);
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
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	parse_file		(char* argument)
{
	// FIXME
	FILE	*fp;
	fp	= fopen(argument, "r");
	if (!fp) {
		printf("--file argument not valid\n");
		printf("It must be a valid file name (relative to saved dir)\n");
		exit(EXIT_FAILURE);
	} else {
		fclose(fp);

		sprintf(saved_path, "");
		snprintf(saved_name, FILENAME_MAX, "%s", argument);
	}
}

static	void	parse_iface		(char* argument)
{
	menu_iface_mode		= atoi(argument);
	user_iface_mode		= menu_iface_mode;
	if (menu_iface_mode < MENU_IFACE_CLUI || menu_iface_mode > MENU_IFACE_TUI) {
		printf("--iface argument not valid\n");
		printf("It must be an integer [%i U %i]\n", MENU_IFACE_CLUI, MENU_IFACE_TUI);
		exit(EXIT_FAILURE);
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
