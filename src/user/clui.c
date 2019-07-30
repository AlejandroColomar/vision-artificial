/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/user/clui.h"

#include <stdio.h>
#include <string.h>

#include "libalx/base/compiler/size.h"
#include "libalx/base/stdio/get.h"
#include "libalx/base/stdio/seekc.h"

#include "vision-artificial/image/iface.h"
#include "vision-artificial/user/iface.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define BUFF_SIZE	(0xFFF)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	log_loop	(void);
static	int	usr_input	(void);
static	void	show_help	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
int	user_clui		(const char *restrict title,
				const char *restrict subtitle)
{

	show_help();
	user_clui_show_log(title, subtitle);
	return	usr_input();
}

void	user_clui_fname		(const char *restrict fpath,
				char *restrict fname)
{

	alx_get_fname(fpath, fname, false, "File name:", "Valid extensions: .bmp .dib .jpeg .png .pbm .pgm .ppm .tiff",
									2);
}

void	user_clui_show_log	(const char *restrict title,
				const char *restrict subtitle)
{

	printf("________________________________________________________________________________\n");
	log_loop();
	printf("%s	-	%s\n", subtitle, title);
	printf("--------------------------------------------------------------------------------\n");

}

void	user_clui_show_ocr	(void)
{

	printf("%s", img_ocr_text);
	alx_wait4enter();
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	log_loop	(void)
{
	const	char	*txt;

	putchar('\n');
	for (;;) {
		txt	= user_iface_log_read();
		if (!txt)
			break;
		for (int i = 0; i < user_iface_log.lvl[user_iface_log.pos]; i++)
			printf("\t");
		printf("%s\n", txt);
	}
	printf("\n");
}

static	int	usr_input	(void)
{
	char	buff[BUFF_SIZE];
	char	ch [10];

	memset(ch, 0, ARRAY_SIZE(ch));
	buff[0]	= '\0';

	if (!fgets(buff, BUFF_SIZE, stdin))
		goto err_fgets;
	if (sscanf(buff, " %5c", ch) <= 0)
		goto err_sscanf;

	switch (ch[0]) {
	case '+':
		return	USER_IFACE_ACT_APPLY;
	case '-':
		return	USER_IFACE_ACT_DISCARD;
	case 'e':
		/* Exercises from class */
		switch (ch[1]) {
		case '1':
			/* Label */
			switch (ch[2]) {
			case '1':
				return	USER_IFACE_ACT_PROC_LABEL_SERIES;
			}
			break;
		case '2':
			/* Objects */
			switch (ch[2]) {
			case '0':
				return	USER_IFACE_ACT_PROC_OBJECTS_CALIB;
			case '1':
				return	USER_IFACE_ACT_PROC_OBJECTS_SERIES;
			}
			break;
		case '3':
			/* Coins */
			switch (ch[2]) {
			case '1':
				return	USER_IFACE_ACT_PROC_COINS_SERIES;
			}
			break;
		case '4':
			/* Resistor */
			switch (ch[2]) {
			case '1':
				return	USER_IFACE_ACT_PROC_RESISTOR_SERIES;
			}
			break;
		case '5':
			/* Lighters */
			switch (ch[2]) {
			case '1':
				return	USER_IFACE_ACT_PROC_LIGHTERS_SERIES;
			}
			break;
		}
		break;

	case 'f':
		/* Use simple funtions */
		switch (ch[1]) {
		case '0':
			/* img_alx */
			switch (ch[2]) {
			case '0':
				/* Distance transform postprocessing */
				switch (ch[3]) {
				case '0':
					return	USER_IFACE_ACT_LOCAL_MAX;
				case '1':
					return	USER_IFACE_ACT_SKELETON;
				}
				break;
			case '1':
				/* Lines */
				switch (ch[3]) {
				case '0':
					return	USER_IFACE_ACT_LINES_HORIZONTAL;
				case '1':
					return	USER_IFACE_ACT_LINES_VERTICAL;
				}
				break;
			case '2':
				/* Smooth */
				switch (ch[3]) {
				case '0':
					return	USER_IFACE_ACT_MEAN_HORIZONTAL;
				case '1':
					return	USER_IFACE_ACT_MEAN_VERTICAL;
				case '2':
					return	USER_IFACE_ACT_MEDIAN_HORIZONTAL;
				case '3':
					return	USER_IFACE_ACT_MEDIAN_VERTICAL;
				}
				break;
			}
			break;
		case '1':
			/* img_cv */
			switch (ch[2]) {
			case '0':
				/* Core: The core functionality */
				switch (ch[3]) {
				case '0':
					/* Pixel */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_PIXEL_GET;
					case '1':
						return	USER_IFACE_ACT_PIXEL_SET;
					}
					break;
				case '1':
					/* ROI */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_SET_ROI;
					case '1':
						return	USER_IFACE_ACT_SET_ROI_2RECT;
					}
					break;
				case '2':
					/* Operations on Arrays */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_AND_2REF;
					case '1':
						return	USER_IFACE_ACT_NOT;
					case '2':
						return	USER_IFACE_ACT_OR_2REF;
					case '3':
						return	USER_IFACE_ACT_COMPONENT;
					}
					break;
				}
				break;
			case '1':
				/* Imgproc: Image processing */
				switch (ch[3]) {
				case '0':
					/* Image filtering */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_DILATE;
					case '1':
						return	USER_IFACE_ACT_ERODE;
					case '2':
						return	USER_IFACE_ACT_DILATE_ERODE;
					case '3':
						return	USER_IFACE_ACT_ERODE_DILATE;
					case '4':
						return	USER_IFACE_ACT_SMOOTH;
					case '5':
						return	USER_IFACE_ACT_SOBEL;
					case '6':
						return	USER_IFACE_ACT_BORDER;
					}
					break;
				case '1':
					/* Geometric image transformations */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_MIRROR;
					case '1':
						return	USER_IFACE_ACT_ROTATE_ORTO;
					case '2':
						return	USER_IFACE_ACT_ROTATE;
					case '3':
						return	USER_IFACE_ACT_ROTATE_2RECT;
					}
					break;
				case '2':
					/* Miscellaneous image transformations */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_ADAPTIVE_THRESHOLD;
					case '1':
						return	USER_IFACE_ACT_CVT_COLOR;
					case '2':
						return	USER_IFACE_ACT_DISTANCE_TRANSFORM;
					case '3':
						return	USER_IFACE_ACT_THRESHOLD;
					}
					break;
				case '3':
					/* Histograms */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_HISTOGRAM;
					case '1':
						return	USER_IFACE_ACT_HISTOGRAM_C3;
					}
					break;
				case '4':
					/* Structural analysis and shape descriptors */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_CONTOURS;
					case '1':
						return	USER_IFACE_ACT_CONTOURS_SIZE;
					case '2':
						return	USER_IFACE_ACT_BOUNDING_RECT;
					case '3':
						return	USER_IFACE_ACT_FIT_ELLIPSE;
					case '4':
						return	USER_IFACE_ACT_MIN_AREA_RECT;
					}
					break;
				case '5':
					/* Feature detection */
					switch (ch[4]) {
					case '0':
						return	USER_IFACE_ACT_HOUGH_CIRCLES;
					}
					break;
				}
				break;
			}
			break;
		case '2':
			/* img_orb */
			switch (ch[2]) {
			case '0':
				return	USER_IFACE_ACT_ALIGN;
			}
			break;
		case '3':
			/* img_calib3d */
			switch (ch[2]) {
			case '0':
				return	USER_IFACE_ACT_CALIBRATE;
			case '1':
				return	USER_IFACE_ACT_UNDISTORT;
			}
			break;
		case '4':
			/* img_zbar */
			switch (ch[2]) {
			case '0':
				return	USER_IFACE_ACT_DECODE;
			}
			break;
		case '5':
			/* img_ocr */
			switch (ch[2]) {
			case '0':
				return	USER_IFACE_ACT_READ;
			}
			break;
		}
		break;
	case 'l':
		return	USER_IFACE_ACT_LOAD_MEM;
	case 'm':
		return	USER_IFACE_ACT_SAVE_MEM;
	case 'q':
		return	USER_IFACE_ACT_QUIT;
	case 'r':
		return	USER_IFACE_ACT_SAVE_REF;
	case 's':
		return	USER_IFACE_ACT_SAVE_FILE;
	case 'u':
		/* User iface actions */
		switch (ch[1]) {
		case '1':
			return	USER_IFACE_ACT_SHOW_OCR;
		}
		break;
	case 'x':
		/* Special sequence "xyzzy" */
		if (ch[1] == 'y') {
			if (ch[2] == 'z') {
			if (ch[3] == 'z') {
			if (ch[4] == 'y') {
				return	USER_IFACE_ACT_FOO;
			}
			}
			}
		}
		break;
	}

err_fgets:
err_sscanf:
	return	USER_IFACE_ACT_FOO;
}

static	void	show_help	(void)
{

	printf("Apply:			%s\n",	"Space");
	printf("Discard:		%s\n",	"Backspace");
	printf("Save to mem:		%c\n",	'm');
	printf("Load from mem:		%c\n",	'l');
	printf("Save to ref:		%c\n",	'r');
	printf("Save to file:		%c\n",	's');
	printf("Functions:\n");
	printf(" - Local maxima:	%s\n",	"f000");
	printf(" - Skeleton:		%s\n",	"f001");
	printf(" - Horizontal lines:	%s\n",	"f010");
	printf(" - Vertical lines:	%s\n",	"f011");
	printf(" - Horizontal mean:	%s\n",	"f020");
	printf(" - Vertical mean:	%s\n",	"f021");
	printf(" - Horizontal median:	%s\n",	"f022");
	printf(" - Vertical median:	%s\n",	"f023");
	printf(" - Pixel get:		%s\n",	"f1000");
	printf(" - Pixel set:		%s\n",	"f1001");
	printf(" - Set ROI:		%s\n",	"f1010");
	printf(" - Set ROI 2rect:	%s\n",	"f1011");
	printf(" - Bitwise AND 2ref:	%s\n",	"f1020");
	printf(" - Bitwise NOT:		%s\n",	"f1021");
	printf(" - Bitwise OR 2ref:	%s\n",	"f1022");
	printf(" - Component:		%s\n",	"f1023");
	printf(" - Dilate:		%s\n",	"f1100");
	printf(" - Erode:		%s\n",	"f1101");
	printf(" - D-E:			%s\n",	"f1102");
	printf(" - E-D:			%s\n",	"f1103");
	printf(" - Smooth:		%s\n",	"f1104");
	printf(" - Sobel:		%s\n",	"f1105");
	printf(" - Border:		%s\n",	"f1106");
	printf(" - Mirror:		%s\n",	"f1110");
	printf(" - Rotate ortogonally:	%s\n",	"f1111");
	printf(" - Rotate:		%s\n",	"f1112");
	printf(" - Rotate 2rect_rot:	%s\n",	"f1113");
	printf(" - Adaptive threshold:	%s\n",	"f1120");
	printf(" - Cvt color:		%s\n",	"f1121");
	printf(" - Distance transform:	%s\n",	"f1122");
	printf(" - Threshold:		%s\n",	"f1123");
	printf(" - Histogram:		%s\n",	"f1130");
	printf(" - Histogram (3 chan):	%s\n",	"f1131");
	printf(" - Contours:		%s\n",	"f1140");
	printf(" - Contours size:	%s\n",	"f1141");
	printf(" - Bounding rectangle:	%s\n",	"f1142");
	printf(" - Fit ellipse:		%s\n",	"f1143");
	printf(" - Min. area rectangle:	%s\n",	"f1144");
	printf(" - Hough circles:	%s\n",	"f1150");
	printf(" - Align 2ref (ORB):	%s\n",	"f20");
	printf(" - Calibrate (Calib3d):	%s\n",	"f30");
	printf(" - Undistort (Calib3d):	%s\n",	"f31");
	printf(" - Scan codes (ZBAR):	%s\n",	"f40");
	printf(" - Scan text (OCR):	%s\n",	"f50");
	printf("Exercises:\n");
	printf(" - Label:		%s\n",	"e11");
	printf(" - Objects (calib):	%s\n",	"e20");
	printf(" - Objects:		%s\n",	"e21");
	printf(" - Coins:		%s\n",	"e31");
	printf(" - Resistor:		%s\n",	"e41");
	printf(" - Lighters:		%s\n",	"e51");
	printf("Other:\n");
	printf(" - Show OCR text:	%s\n",	"u1");
	printf("Quit:		%c\n",	'q');
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
