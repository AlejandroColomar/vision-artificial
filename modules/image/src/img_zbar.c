/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Standard	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* opencv */
	#include <cv.h>
		/* snprintf() */
	#include <stdio.h>
		/* zbar */
	#include <zbar.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* user_iface_log */
	#include "user_iface.h"

	#include "img_zbar.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
int			img_zb_code_n;
struct Img_ZB_Code	img_zb_code [CODES_MAX];


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_zb_decode	(struct _IplImage  *imgptr);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_zb_act	(struct _IplImage  **imgptr2, int action)
{
	switch (action) {
	case IMG_ZB_ACT_DECODE:
		img_zb_decode(*imgptr2);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_zb_decode	(struct _IplImage  *imgptr)
{

	struct _IplImage		*imgtmp;
	struct zbar_image_scanner_s	*scanner;
	struct zbar_image_s		*image_zb;

	/* Make a copy of imgptr so that it isn't modified by zbar */
	imgtmp	= cvCloneImage(imgptr);

	/* create & configure a reader */
	scanner	= zbar_image_scanner_create();
	zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"Detect codes");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

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
	if (!img_zb_code_n){
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"No barcode detected");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;

	} else {
		/* extract results */
		img_zb_code[0].symbol	= zbar_image_first_symbol(image_zb);
		for (i = 0; i < CODES_MAX && img_zb_code[i].symbol; i++) {
			/* Write results into array */
			img_zb_code[i].type	= zbar_symbol_get_type(img_zb_code[i].symbol);
			img_zb_code[i].sym_name	= zbar_get_symbol_name(img_zb_code[i].type);
			img_zb_code[i].data	= zbar_symbol_get_data(img_zb_code[i].symbol);

			/* Write results into log */
			snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"%s -- '%s'",
							img_zb_code[i].sym_name,
							img_zb_code[i].data);
			user_iface_log.lvl[user_iface_log.len]	= 1;
			(user_iface_log.len)++;

			/* Load next symbol */
			img_zb_code[i+1].symbol	= zbar_symbol_next(img_zb_code[i].symbol);
		}
	}

	// clean up
	zbar_image_destroy(image_zb);
	zbar_image_scanner_destroy(scanner);
	cvReleaseImage(&imgtmp);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
