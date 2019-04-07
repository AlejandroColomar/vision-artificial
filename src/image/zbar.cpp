/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/image/zbar.hpp"

#include <climits>
#include <cstddef>
#include <cstdio>

#include <opencv2/opencv.hpp>
#include <zbar.h>

#include "vision-artificial/image/iface.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


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
static	void	img_zb_decode	(class cv::Mat  *imgptr, const void *data);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	img_zb_act	(class cv::Mat  *imgptr, int action, const void *data)
{

	switch (action) {
	case IMG_ZB_ACT_DECODE:
		img_zb_decode(imgptr, data);
		break;
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	img_zb_decode	(class cv::Mat  *imgptr, const void *data)
{
	const struct zbar::zbar_symbol_s	*symbol;
	struct zbar::zbar_image_scanner_s	*scanner;
	struct zbar::zbar_image_s		*image_zb;
	enum zbar::zbar_symbol_type_e		code_type;
	void					*imgdata;
	ptrdiff_t				rows;
	ptrdiff_t				cols;

	imgdata	= (void *)imgptr->data;
	rows	= imgptr->rows;
	cols	= imgptr->cols;

	/* Type of code to scan */
	code_type = (enum zbar::zbar_symbol_type_e)
			((const struct Img_Iface_Data_Decode *)data)->code_type;

	/* create & configure a reader */
	scanner = zbar::zbar_image_scanner_create();
	zbar::zbar_image_scanner_set_config(scanner, code_type,
						zbar::ZBAR_CFG_ENABLE, 1);

	/* wrap image data */
	image_zb = zbar::zbar_image_create();
	zbar::zbar_image_set_format(image_zb, *(int*)"GREY");
	zbar::zbar_image_set_size(image_zb, cols, rows);
	zbar::zbar_image_set_data(image_zb, imgdata, cols * rows, NULL);

	/* scan the image for barcodes */
	zb_codes.n = zbar::zbar_scan_image(scanner, image_zb);
	if (!zb_codes.n)
		goto not_found;

	/* extract results */
	symbol = zbar::zbar_image_first_symbol(image_zb);
	for (ptrdiff_t i = 0; i < ZB_CODES_MAX && symbol; i++) {
		zb_codes.arr[i].type = zbar::zbar_symbol_get_type(symbol);
		code_type = (enum zbar::zbar_symbol_type_e)zb_codes.arr[i].type;
		snprintf(zb_codes.arr[i].sym_name, 80, "%s",
					zbar::zbar_get_symbol_name(code_type));
		snprintf(zb_codes.arr[i].data, ZBAR_LEN_MAX, "%s",
					zbar::zbar_symbol_get_data(symbol));

		/* Load next symbol */
		symbol = zbar::zbar_symbol_next(symbol);
	}

not_found:
	/* clean up */
	zbar::zbar_image_destroy(image_zb);
	zbar::zbar_image_scanner_destroy(scanner);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
