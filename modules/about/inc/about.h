/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_ABOUT_H
	# define	VA_ABOUT_H


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
		/* FILENAME_MAX */
	#include <stdio.h>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	PROG_NAME	"vision-artificial"
	# define	PROG_YEAR	"2018"


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Share_File {
		SHARE_COPYRIGHT,
		SHARE_DISCLAIMER,
		SHARE_HELP,
		SHARE_LICENSE,
		SHARE_USAGE
	};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
	extern	char	share_path [FILENAME_MAX];


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	void	about_init		(void);
	void	snprint_share_file	(char *dest, int destsize, int share_file);
	void	print_share_file	(int share_file);
	void	print_version		(void);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* about.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/