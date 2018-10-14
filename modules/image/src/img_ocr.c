/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* INT_MAX */
	#include <limits.h>
		/* snprintf() & FILENAME_MAX */
	#include <stdio.h>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <cv.h>
		/* OCR Tesseract */
	#include <tesseract/capi.h>

/* Project -------------------------------------------------------------------*/
		/* user_iface_log */
	#include "user_iface.h"
		/* user_prog_path */
	#include "save.h"

/* Module --------------------------------------------------------------------*/
		/* data & img_ocr_text & OCR_TEXT_MAX */
	#include "img_iface.h"

	#include "img_ocr.h"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(struct _IplImage  *imgptr, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_ocr_act	(struct _IplImage  **imgptr2, int action, void *data)
{
	switch (action) {
	case IMG_OCR_ACT_READ:
		img_ocr_read(*imgptr2, data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(struct _IplImage  *imgptr, void *data)
{
	struct _IplImage	*imgtmp;
	struct TessBaseAPI	*handle_ocr;

	/* Make a copy of imgptr so that it isn't modified by tesseract */
	imgtmp	= cvCloneImage(imgptr);
	/* Language */
	int	lang;
	char	lang_str [3 + 1];
	lang	= ((struct Img_Iface_Data_Read *)data)->lang;
	switch (lang) {
	case 0:
		sprintf(lang_str, "eng");
		break;
	case 1:
		sprintf(lang_str, "spa");
		break;
	case 2:
		sprintf(lang_str, "cat");
		break;
	}

	/* Config file */
	int	conf;
	char	conf_str [FILENAME_MAX];
	conf	= ((struct Img_Iface_Data_Read *)data)->conf;
	switch (conf) {
	case 1:
		sprintf(conf_str, "%s/%s", user_prog_path, "price");
		break;
	}

	/* init OCR */
	handle_ocr	= TessBaseAPICreate();
	TessBaseAPIInit3(handle_ocr, NULL, lang_str);
	if (conf) {
		/* Configure OCR (whitelist chars) */
		TessBaseAPIReadConfigFile(handle_ocr, conf_str);
	}

	/* scan image for text */
	TessBaseAPISetImage(handle_ocr, imgtmp->imageData,
				imgtmp->width, imgtmp->height,
				imgtmp->depth / 8, imgtmp->widthStep);
	TessBaseAPIRecognize(handle_ocr, NULL);
	char	*txt;
	txt	= TessBaseAPIGetUTF8Text(handle_ocr);

	/* Copy text to global variable */
	snprintf(img_ocr_text, OCR_TEXT_MAX, "%s", txt);

	/* cleanup */
	TessDeleteText(txt);
	TessBaseAPIEnd(handle_ocr);
	TessBaseAPIDelete(handle_ocr);
	cvReleaseImage(&imgtmp);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
