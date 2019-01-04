/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* INT_MAX */
	#include <climits>
	#include <cstddef>
		/* snprintf() */
	#include <cstdio>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <opencv2/opencv.hpp>
		/* zbar */
	#include <zbar.h>

/* Module --------------------------------------------------------------------*/
		/* data & zb_codes */
	#include "img_iface.hpp"

	#include "img_zbar.hpp"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_zb_decode	(class cv::Mat  *imgptr, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_zb_act	(class cv::Mat  *imgptr, int action, void *data)
{
	switch (action) {
	case IMG_ZB_ACT_DECODE:
		img_zb_decode(imgptr, data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_zb_decode	(class cv::Mat  *imgptr, void *data)
{
	struct zbar::zbar_image_scanner_s	*scanner;
	struct zbar::zbar_image_s		*image_zb;
	const struct zbar::zbar_symbol_s	*symbol;

	/* Type of code to scan */
	enum zbar::zbar_symbol_type_e	code_type;
	code_type	= ((struct Img_Iface_Data_Decode *)data)->code_type;

	/* create & configure a reader */
	scanner	= zbar::zbar_image_scanner_create();
	zbar::zbar_image_scanner_set_config(scanner, code_type,
						zbar::ZBAR_CFG_ENABLE, 1);

	/* wrap image data */
	image_zb	= zbar::zbar_image_create();
	zbar::zbar_image_set_format(image_zb, *(int*)"GREY");
	zbar::zbar_image_set_size(image_zb, imgptr->cols, imgptr->rows);
	zbar::zbar_image_set_data(image_zb, (void *)(imgptr->data),
					(imgptr->cols * imgptr->rows),
					NULL);

	/* scan the image for barcodes */
	int	i;
	zb_codes.n	= zbar::zbar_scan_image(scanner, image_zb);
	if (zb_codes.n) {
		/* extract results */
		symbol	= zbar::zbar_image_first_symbol(image_zb);
		for (i = 0; i < ZB_CODES_MAX && symbol; i++) {
			/* Write results into array */
			zb_codes.arr[i].type	= zbar::zbar_symbol_get_type(symbol);
			snprintf(zb_codes.arr[i].sym_name, 80, "%s",
					zbar::zbar_get_symbol_name(
							zb_codes.arr[i].type));
			snprintf(zb_codes.arr[i].data, ZBAR_LEN_MAX, "%s",
					zbar::zbar_symbol_get_data(symbol));

			/* Load next symbol */
			symbol	= zbar::zbar_symbol_next(symbol);
		}
	}

	/* clean up */
	zbar::zbar_image_destroy(image_zb);
	zbar::zbar_image_scanner_destroy(scanner);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
