/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <limits.h>
	#include <stddef.h>
	#include <stdio.h>
/* Packages ------------------------------------------------------------------*/
	#include <tesseract/capi.h>
/* Project -------------------------------------------------------------------*/
		/* share_path */
	#include "about.h"
/* Module --------------------------------------------------------------------*/
	#include "img_iface.h"

	#include "img_ocr.h"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(const void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_ocr_act	(int action, const void *data)
{
	switch (action) {
	case IMG_OCR_ACT_READ:
		img_ocr_read(data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(const void *data)
{
	const	struct Img_Iface_Data_Read	*data_cast;
	struct TessBaseAPI			*handle_ocr;
	int	lang;
	char	lang_str [20 + 1];
	int	conf;
	char	conf_str [FILENAME_MAX];
	char	*txt;

	/* Data */
	data_cast	= (const struct Img_Iface_Data_Read *)data;

	/* Language */
	lang	= data_cast->lang;
	switch (lang) {
	case IMG_IFACE_OCR_LANG_ENG:
		sprintf(lang_str, "eng");
		break;
	case IMG_IFACE_OCR_LANG_SPA:
		sprintf(lang_str, "spa");
		break;
	case IMG_IFACE_OCR_LANG_CAT:
		sprintf(lang_str, "cat");
		break;
	case IMG_IFACE_OCR_LANG_DIGITS:
		sprintf(lang_str, "digits");
		break;
	case IMG_IFACE_OCR_LANG_DIGITS_COMMA:
		sprintf(lang_str, "digits_comma");
		break;
	}

	/* Config file */
	conf	= data_cast->conf;
	switch (conf) {
	case IMG_IFACE_OCR_CONF_PRICE:
		if (snprintf(conf_str, FILENAME_MAX, "%s/%s",
						share_path,
						"price")  >=  FILENAME_MAX) {
			printf("Path is too large and has been truncated\n");
			printf("Price configuration was not possible!\n");
			conf	= IMG_IFACE_OCR_CONF_NONE;
		}
		break;
	}

	/* init OCR */
	handle_ocr	= TessBaseAPICreate();
#ifdef	OEM_LSTM_ONLY
	TessBaseAPIInit2(handle_ocr, NULL, lang_str, OEM_LSTM_ONLY);
#else
	TessBaseAPIInit2(handle_ocr, NULL, lang_str, OEM_DEFAULT);
#endif
/*	TessBaseAPIInit2(handle_ocr, NULL, lang_str, OEM_TESSERACT_LSTM_COMBINED);*/
	if (conf) {
		/* Configure OCR (whitelist chars) */
		TessBaseAPIReadConfigFile(handle_ocr, conf_str);
	}

	/* scan image for text */
	TessBaseAPISetImage(handle_ocr, data_cast->img.data,
				data_cast->img.width, data_cast->img.height,
				data_cast->img.B_per_pix,
				data_cast->img.B_per_line);
	TessBaseAPIRecognize(handle_ocr, NULL);
	txt	= TessBaseAPIGetUTF8Text(handle_ocr);

	/* Copy text to global variable */
	snprintf(img_ocr_text, OCR_TEXT_MAX, "%s", txt);

	/* cleanup */
	TessDeleteText(txt);
	TessBaseAPIEnd(handle_ocr);
	TessBaseAPIDelete(handle_ocr);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
