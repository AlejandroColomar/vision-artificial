/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* INT_MAX */
	#include <limits.h>
	#include <stddef.h>
		/* snprintf() & FILENAME_MAX */
	#include <stdio.h>

/* Packages ------------------------------------------------------------------*/
		/* OCR Tesseract */
	#include <tesseract/capi.h>

/* Project -------------------------------------------------------------------*/
		/* share_path */
	#include "about.h"

/* Module --------------------------------------------------------------------*/
		/* data & img_ocr_text & OCR_TEXT_MAX */
	#include "img_iface.h"

	#include "img_ocr.h"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_ocr_act	(int action, void *data)
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
static	void	img_ocr_read	(void *data)
{
	struct TessBaseAPI	*handle_ocr;

	/* Data */
	struct Img_Iface_Data_Read	*data_cast;
	data_cast	= (struct Img_Iface_Data_Read *)data;

	/* Language */
	int	lang;
	char	lang_str [20 + 1];
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
	int	conf;
	char	conf_str [FILENAME_MAX];
	conf	= data_cast->conf;
	switch (conf) {
	case IMG_IFACE_OCR_CONF_PRICE:
		snprintf(conf_str, 6, "%s/%s", share_path, "price");
		break;
	}

	/* init OCR */
	handle_ocr	= TessBaseAPICreate();
	TessBaseAPIInit2(handle_ocr, NULL, lang_str,
#ifdef	OEM_LSTM_ONLY
						OEM_LSTM_ONLY);
#else
						OEM_DEFAULT);
#endif
//						OEM_TESSERACT_LSTM_COMBINED);
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
	char	*txt;
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
