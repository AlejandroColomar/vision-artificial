/******************************************************************************
 *	Copyright (C) 2017	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
	#include <inttypes.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <stdio.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
	#include "alx_input.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	BUFF_SIZE	(1024)

	# define	MAX_TRIES	(2)

	# define	ERR_OK		(0)
	# define	ERR_RANGE	(1)
	# define	ERR_SSCANF	(2)
	# define	ERR_FPTR	(3)
	# define	ERR_FGETS	(4)

	# define	ERR_RANGE_MSG	"¡ Number is out of range !"
	# define	ERR_SSCANF_MSG	"¡ sscanf() error !"
	# define	ERR_FGETS_MSG	"¡ fgets() error !"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	double	loop_getdbl	(double m, double def, double M);
static	int64_t	loop_getint	(double m, int64_t def, double M);
static	void	manage_error	(int err);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
	/*
	 * Scan a double in the range [m, M].
	 * return:	0	if correct
	 *		non 0	if there is an error
	 */
int	alx_sscan_dbl	(double *dest, double m, double def, double M, const char *str)
{
	int	err;

	if (sscanf(str, " %lf", dest) != 1) {
		err	= ERR_SSCANF;
	} else if ((*dest < m) || (*dest > M)) {
		err	= ERR_RANGE;
	} else {
		err	= ERR_OK;
	}

	if (err) {
		*dest = def;
	}

	return	err;
}

	/*
	 * Scan an int64_t in the range [m, M].
	 * return:	0	if correct
	 *		non 0	if there is an error
	 */
int	alx_sscan_int	(int64_t *dest, double m, int64_t def, double M, const char *str)
{
	int	err;

	if (sscanf(str, " %"SCNi64"", dest) != 1) {
		err	= ERR_SSCANF;
	} else if ((*dest < m) || (*dest > M)) {
		err	= ERR_RANGE;
	} else {
		err	= ERR_OK;
	}

	if (err) {
		*dest = def;
	}

	return	err;
}

	/*
	 * Scan a file name in fpath.
	 * return:	0	if correct
	 *		non 0	if there is an error
	 */
int	alx_sscan_fname	(const char *fpath, char *fname, bool exist, const char *str)
{
	char	buff [FILENAME_MAX];
	char	file_path [FILENAME_MAX];
	int	err;
	FILE	*fp;

	if (sscanf(str, " %s ", buff) != 1) {
		err	= ERR_SSCANF;
	} else {
		snprintf(file_path, FILENAME_MAX, "%s%s", fpath, buff);

		fp	= fopen(file_path, "r");

		if (exist) {
			if (fp == NULL) {
				err	= ERR_FPTR;
			} else {
				err	= ERR_OK;
				fclose(fp);
			}
		} else {
			if (fp == NULL) {
				err	= ERR_OK;
			} else {
				err	= ERR_FPTR;
				fclose(fp);
			}
		}
	}

	if (!err) {
		snprintf(fname, FILENAME_MAX, "%s", buff);
	}

	return	err;
}

	/*
	 * Ask for a double in the range [m, M].
	 *
	 * If the user enters a non valid number, it repeats to ask for
	 * the number two more times. After that, it uses the default
	 * value.
	 */
double	alx_getdbl	(double m, double def, double M,
			const char *formatA, const char *formatB, ...)
{
	va_list	args;
	va_start(args, formatB);

	double	R;

	if (formatA != NULL) {
		puts(formatA);
	}
	if (formatB == NULL) {
		printf("Introduce a real number [%lf U %lf] (default %lf):...\t", m, M, def);
	} else {
		vprintf(formatB, args);
	}

	R = loop_getdbl(m, def, M);

	va_end(args);
	return	R;
}

	/*
	 * Ask for an int64_t in the range [m, M].
	 *
	 * If the user enters a non valid number, it repeats to ask for
	 * the number two more times. After that, it uses the default
	 * value.
	 */
int64_t	alx_getint	(double m, int64_t def, double M,
			const char *formatA, const char *formatB, ...)
{
	va_list	args;
	va_start(args, formatB);

	int64_t	Z;

	if (formatA != NULL) {
		puts(formatA);
	}
	if (formatB == NULL) {
		printf("Introduce an integer number [%lf U %lf] (default %"PRIi64"):...\t", m, M, def);
	} else {
		vprintf(formatB, args);
	}

	Z = loop_getint(m, def, M);

	va_end(args);
	return	Z;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	double	loop_getdbl	(double m, double def, double M)
{
	int	i;
	char	buff [BUFF_SIZE];
	char	*x;
	double	R;
	int	err;

	for (i = 0; i < MAX_TRIES; i++) {
		x	= fgets(buff, BUFF_SIZE, stdin);

		if (x == NULL) {
			err	= ERR_FGETS;
		} else {
			err	= alx_sscan_dbl(&R, m, def, M, buff);
		}

		if (err) {
			manage_error(err);
		} else {
			break;
		}
	}

	return	R;
}

static	int64_t	loop_getint	(double m, int64_t def, double M)
{
	int	i;
	char	buff [BUFF_SIZE];
	char	*x;
	int64_t	Z;
	int	err;

	for (i = 0; i < MAX_TRIES; i++) {
		x	= fgets(buff, BUFF_SIZE, stdin);

		if (x == NULL) {
			err	= ERR_FGETS;
		} else {
			err	= alx_sscan_int(&Z, m, def, M, buff);
		}

		if (err) {
			manage_error(err);
		} else {
			break;
		}
	}

	return	Z;
}

static	void	manage_error	(int err)
{
	switch (err) {
	case ERR_RANGE:
		puts(ERR_RANGE_MSG);
		break;
	case ERR_SSCANF:
		puts(ERR_SSCANF_MSG);
		break;
	case ERR_FGETS:
		puts(ERR_FGETS_MSG);
		break;
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
