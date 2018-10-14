/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* INT_MAX */
	#include <limits.h>
		/* snprintf() */
	#include <stdio.h>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <cv.h>
		/* zbar */
	#include <zbar.h>

/* Project -------------------------------------------------------------------*/
		/* user_iface_log */
	#include "user_iface.h"

/* Module --------------------------------------------------------------------*/
		/* data */
	#include "img_iface.h"

	#include "img_zbar.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
int			img_zb_code_n;
struct Img_ZB_Code	img_zb_code [CODES_MAX];


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_zb_decode	(struct _IplImage  *imgptr, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_zb_act	(struct _IplImage  **imgptr2, int action, void *data)
{
	switch (action) {
	case IMG_ZB_ACT_DECODE:
		img_zb_decode(*imgptr2, data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_zb_decode	(struct _IplImage  *imgptr, void *data)
{

	struct _IplImage		*imgtmp;
	struct zbar_image_scanner_s	*scanner;
	struct zbar_image_s		*image_zb;

	/* Make a copy of imgptr so that it isn't modified by zbar */
	imgtmp	= cvCloneImage(imgptr);

	/* Type of code to scan */
	int	code_type;
	code_type	= ((struct Img_Iface_Data_Decode *)data)->code_type;

	/* create & configure a reader */
	scanner	= zbar_image_scanner_create();
	zbar_image_scanner_set_config(scanner, code_type, ZBAR_CFG_ENABLE, 1);

	/* wrap image data */
	image_zb	= zbar_image_create();
	zbar_image_set_format(image_zb, *(int*)"GREY");
	zbar_image_set_size(image_zb, imgtmp->width, imgtmp->height);
	zbar_image_set_data(image_zb, (void *)(imgtmp->imageData),
					(imgtmp->width * imgtmp->height),
					NULL);

	/* scan the image for barcodes */
	int	i;
	img_zb_code_n	= zbar_scan_image(scanner, image_zb);
	if (img_zb_code_n) {
		/* extract results */
		img_zb_code[0].symbol	= zbar_image_first_symbol(image_zb);
		for (i = 0; i < CODES_MAX && img_zb_code[i].symbol; i++) {
			/* Write results into array */
			img_zb_code[i].type	= zbar_symbol_get_type(img_zb_code[i].symbol);
			img_zb_code[i].sym_name	= zbar_get_symbol_name(img_zb_code[i].type);
			img_zb_code[i].data	= zbar_symbol_get_data(img_zb_code[i].symbol);

			/* Load next symbol */
			img_zb_code[i+1].symbol	= zbar_symbol_next(img_zb_code[i].symbol);
		}
	}

	/* clean up */
	zbar_image_destroy(image_zb);
	zbar_image_scanner_destroy(scanner);
	cvReleaseImage(&imgtmp);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
