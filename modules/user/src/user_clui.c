/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* printf() & fgets() & sscanf() */
	#include <stdio.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* user_iface_log */
	#include "user_iface.h"

	#include "user_clui.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	BUFF_SIZE	(1024)


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Play */
static	void	show_log	(const char *title, const char *subtitle);
static	void	log_loop	(void);
	/* Input */
static	int	usr_input	(void);
	/* Help */
static	void	show_help	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	user_clui		(const char *title, const char *subtitle)
{
	int	action;

	/* User action */
	show_help();
	show_log(title, subtitle);
	action	= usr_input();

	return	action;
}

void	user_clui_save_name	(const char *filepath, char *filename, int destsize)
{
	puts("File name:");
	puts("Valid extensions: .bmp .dib .jpeg .png .pbm .pgm .ppm .tiff");
	fgets(filename, destsize, stdin);
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Log	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	show_log	(const char *title, const char *subtitle)
{
	puts("________________________________________________________________________________");

	/* Board */
	log_loop();

	/* Subtitle & title */
	printf("%s	-	%s\n", subtitle, title);
	puts("--------------------------------------------------------------------------------");

}

static	void	log_loop	(void)
{
	int	i;

	putchar('\n');
	for (i = 0; i < user_iface_log.len; i++) {
		printf("%s\n", user_iface_log.line[i]);
	}
	putchar('\n');
}

/*	*	*	*	*	*	*	*	*	*
 *	*	* Input	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	int	usr_input	(void)
{
	int	action;

	/* Wait for input */
	char	buff [BUFF_SIZE];
	char	ch [5];
	buff[0]	= '\0';
	ch[0]	= '\0';
	ch[1]	= '\0';
	ch[2]	= '\0';
	ch[3]	= '\0';
	ch[4]	= '\0';
	fgets(buff, BUFF_SIZE, stdin);

	/* Interpret input */
	sscanf(buff, " %c%c%c%c%c", &ch[0], &ch[1], &ch[2], &ch[3], &ch[4]);
	switch (ch[0]) {
	case ' ':
		action	= USER_IFACE_ACT_APPLY;
		break;

	case 's':
		action	= USER_IFACE_ACT_SAVE;
		break;

		/* ASCII 0x08 is BS */
	case 0x7F:
	case 0x08:
		action	= USER_IFACE_ACT_DISCARD;
		break;

	case 'x':
		/* Special sequence "xyzzy" */
		if (ch[1] == 'y') {
			if (ch[2] == 'z') {
			if (ch[3] == 'z') {
			if (ch[4] == 'y') {
				action =	USER_IFACE_ACT_FOO;
			}
			}
			}
		}
		break;

	case 'f':
		/* Use simple funtions */
		switch (ch[1]) {
		case '0':
			action	= USER_IFACE_ACT_INVERT;
			break;
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
		break;

	case 'e':
		/* Exercises from class */
		switch (ch[1]) {
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
		break;

	case 'q':
		action	= USER_IFACE_ACT_QUIT;
		break;

	default:
		action	= USER_IFACE_ACT_FOO;
		break;
	}

	return	action;
}

/*	*	*	*	*	*	*	*	*	*
 *	*	* Help	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	show_help	(void)
{
	// FIXME
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
