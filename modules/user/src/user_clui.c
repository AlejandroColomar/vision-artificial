/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* printf() & fgets() & sscanf() */
	#include <stdio.h>

/* Project -------------------------------------------------------------------*/
		/*img_ocr_text */
	#include "img_iface.h"

/* Module --------------------------------------------------------------------*/
		/* user_iface_log */
	#include "user_iface.h"

	#include "user_clui.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	BUFF_SIZE	(1024)


/******************************************************************************
 ******* static variables *****************************************************
 ******************************************************************************/
static	int	log_pos;


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Log */
static	void	log_loop	(void);
	/* Input */
static	int	usr_input	(void);
	/* Help */
static	void	show_help	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	user_clui_init		(void)
{
	log_pos	= 0;
}

int	user_clui		(const char *title, const char *subtitle)
{
	int	action;

	/* User action */
	show_help();
	user_clui_show_log(title, subtitle);
	action	= usr_input();

	return	action;
}

void	user_clui_fname		(const char *filepath, char *filename)
{
	puts("File name:");
	puts("Valid extensions: .bmp .dib .jpeg .png .pbm .pgm .ppm .tiff");
	fgets(filename, FILENAME_MAX, stdin);
}

void	user_clui_show_log	(const char *title, const char *subtitle)
{
	puts("________________________________________________________________________________");

	/* Board */
	log_loop();

	/* Subtitle & title */
	printf("%s	-	%s\n", subtitle, title);
	puts("--------------------------------------------------------------------------------");

}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/* Log -----------------------------------------------------------------------*/
static	void	log_loop	(void)
{
	int	lvl;

	putchar('\n');
	for (; log_pos < user_iface_log.len; log_pos++) {
		if (user_iface_log.lvl[log_pos] <= user_iface_log.visible) {
			for (lvl = 0; lvl < user_iface_log.lvl[log_pos]; lvl++) {
				printf("\t");
			}
			printf("%s\n", user_iface_log.line[log_pos]);
		}
	}
	putchar('\n');
}

/* Input ---------------------------------------------------------------------*/
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
	case '+':
		action	= USER_IFACE_ACT_APPLY;
		break;

	case '-':
		action	= USER_IFACE_ACT_DISCARD;
		break;

	case 'e':
		/* Exercises from class */
		switch (ch[1]) {
		case '1':
			switch (ch[2]) {
			case '1':
				action	= USER_IFACE_ACT_PROC_LABEL;
				break;
			case '2':
				action	= USER_IFACE_ACT_FOO;
				break;
			case '3':
				action	= USER_IFACE_ACT_PROC_RESISTOR;
				break;
			}
			break;
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
		break;

	case 'f':
		/* Use simple funtions */
		switch (ch[1]) {
		case '1':
			/* img_cv */
			switch (ch[2]) {
			case '0':
				/* bitwise manipulation */
				switch (ch[3]) {
				case '0':
					action	= USER_IFACE_ACT_NOT;
					break;
				case '1':
					action	= USER_IFACE_ACT_OR_2REF;
					break;
				case '2':
					action	= USER_IFACE_ACT_AND_2REF;
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			case '1':
				/* color manipulation */
				switch (ch[3]) {
				case '0':
					action	= USER_IFACE_ACT_CVT_COLOR;
					break;
				case '1':
					action	= USER_IFACE_ACT_COMPONENT;
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			case '2':
				/* grayscale filters */
				switch (ch[3]) {
				case '0':
					action	= USER_IFACE_ACT_HISTOGRAM;
					break;
				case '1':
					action	= USER_IFACE_ACT_HISTOGRAM_C3;
					break;
				case '2':
					action	= USER_IFACE_ACT_SMOOTH;
					break;
				case '3':
					action	= USER_IFACE_ACT_SOBEL;
					break;
				case '4':
					action	= USER_IFACE_ACT_THRESHOLD;
					break;
				case '5':
					action	= USER_IFACE_ACT_ADAPTIVE_THRESHOLD;
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			case '3':
				/* black & white filters */
				switch (ch[3]) {
				case '0':
					action	= USER_IFACE_ACT_DILATE;
					break;
				case '1':
					action	= USER_IFACE_ACT_ERODE;
					break;
				case '2':
					action	= USER_IFACE_ACT_DILATE_ERODE;
					break;
				case '3':
					action	= USER_IFACE_ACT_ERODE_DILATE;
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			case '4':
				/* contour */
				switch (ch[3]) {
				case '0':
					action	= USER_IFACE_ACT_CONTOURS;
					break;
				case '1':
					action	= USER_IFACE_ACT_CONTOURS_SIZE;
					break;
				case '2':
					action	= USER_IFACE_ACT_MIN_AREA_RECT;
					break;
				case '3':
					action	= USER_IFACE_ACT_FIT_ELLIPSE;
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			case '5':
				/* contour */
				switch (ch[3]) {
				case '0':
					action	= USER_IFACE_ACT_ROTATE_ORTO;
					break;
				case '1':
					action	= USER_IFACE_ACT_ROTATE;
					break;
				case '2':
					action	= USER_IFACE_ACT_ROTATE_2RECT;
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			case '6':
				/* ROI */
				switch (ch[3]) {
				case '0':
					action	= USER_IFACE_ACT_SET_ROI;
					break;
				default:
					action	= USER_IFACE_ACT_FOO;
					break;
				}
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		case '2':
			/* img_zbar */
			switch (ch[2]) {
			case '0':
				action	= USER_IFACE_ACT_DECODE;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		case '3':
			/* img_ocr */
			switch (ch[2]) {
			case '0':
				action	= USER_IFACE_ACT_READ;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		case '4':
			/* img_ocr */
			switch (ch[2]) {
			case '0':
				action	= USER_IFACE_ACT_ALIGN;
				break;
			default:
				action	= USER_IFACE_ACT_FOO;
				break;
			}
			break;
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
		break;

	case 'l':
		action	= USER_IFACE_ACT_LOAD_MEM;
		break;

	case 'm':
		action	= USER_IFACE_ACT_SAVE_MEM;
		break;

	case 'q':
		action	= USER_IFACE_ACT_QUIT;
		break;

	case 'r':
		action	= USER_IFACE_ACT_SAVE_REF;
		break;

	case 's':
		action	= USER_IFACE_ACT_SAVE_FILE;
		break;

	case 'u':
		/* User iface actions */
		switch (ch[1]) {
		case '1':
			action	= USER_IFACE_ACT_SHOW_OCR;
			break;
		default:
			action	= USER_IFACE_ACT_FOO;
			break;
		}
		break;

	case 'x':
		/* Special sequence "xyzzy" */
		if (ch[1] == 'y') {
			if (ch[2] == 'z') {
			if (ch[3] == 'z') {
			if (ch[4] == 'y') {
				action	= USER_IFACE_ACT_FOO;
			}
			}
			}
		}
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
	printf("Apply:		%s\n",	"Space");
	printf("Discard:	%s\n",	"Backspace");
	printf("Save to mem:	%c\n",	'm');
	printf("Load from mem:	%c\n",	'l');
	printf("Save to ref:	%c\n",	'r');
	printf("Save to file:	%c\n",	's');
	printf("Functions:\n");
	printf(" - Bitwise NOT:	%s\n",	"f100");
	printf(" - BW. OR 2ref:	%s\n",	"f101");
	printf(" - BW. AND 2ref:%s\n",	"f102");
	printf(" - Cvt color:	%s\n",	"f110");
	printf(" - Component:	%s\n",	"f111");
	printf(" - Histogram:	%s\n",	"f120");
	printf(" - Histogram c3:%s\n",	"f121");
	printf(" - Smooth:	%s\n",	"f122");
	printf(" - Sobel:	%s\n",	"f123");
	printf(" - Threshold:	%s\n",	"f124");
	printf(" - Adaptive Thr:%s\n",	"f125");
	printf(" - Dilate:	%s\n",	"f130");
	printf(" - Erode:	%s\n",	"f131");
	printf(" - D-E:		%s\n",	"f132");
	printf(" - E-D:		%s\n",	"f133");
	printf(" - Contours:	%s\n",	"f140");
	printf(" - Contours siz:%s\n",	"f141");
	printf(" - Min. A rect.:%s\n",	"f142");
	printf(" - Fit ellipse:	%s\n",	"f143");
	printf(" - Rotate orto.:%s\n",	"f150");
	printf(" - Rotate:	%s\n",	"f151");
	printf(" - Rotate 2rect:%s\n",	"f152");
	printf(" - Set ROI:	%s\n",	"f160");
	printf(" - Scan codes:	%s\n",	"f20");
	printf(" - Scan text:	%s\n",	"f30");
	printf(" - Align:	%s\n",	"f40");
	printf("Exercises:\n");
	printf(" - Label:	%s\n",	"e11");
	printf(" - Resistor:	%s\n",	"e13");
	printf("Other:\n");
	printf(" - Show OCR:	%s\n",	"u1");
	printf("Quit:		%c\n",	'q');
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
