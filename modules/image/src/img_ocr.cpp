/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* snprintf() & FILENAME_MAX */
	#include <cstdio>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <opencv2/opencv.hpp>
		/* OCR Tesseract */
	#include <tesseract/baseapi.h>
	#include <leptonica/allheaders.h>

/* Project -------------------------------------------------------------------*/
		/* user_iface_log */
	#include "user_iface.hpp"
		/* share_path */
	#include "about.hpp"

/* Module --------------------------------------------------------------------*/
		/* data & img_ocr_text & OCR_TEXT_MAX */
	#include "img_iface.hpp"

	#include "img_ocr.hpp"


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(class cv::Mat  *imgptr, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_ocr_act	(class cv::Mat  *imgptr, int action, void *data)
{
	switch (action) {
	case IMG_OCR_ACT_READ:
		img_ocr_read(imgptr, data);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(class cv::Mat  *imgptr, void *data)
{
	class tesseract::TessBaseAPI	*handle_ocr;

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
		sprintf(conf_str, "%s/%s", share_path, "price");
		break;
	}

	/* init OCR */
	handle_ocr	= new tesseract::TessBaseApi();
	handle_ocr->Init(NULL, lang_str, tesseract::OEM_TESSERACT_CUBE_COMBINED);
	if (conf) {
		/* Configure OCR (whitelist chars) */
		handle_ocr->ReadConfigFile(conf_str);
	}

	/* scan image for text */
	handle_ocr->SetImage(imgptr->data, imgptr->cols, imgptr->rows,
				imgptr->depth() / 8, imgptr->step);
	char	*txt;
	txt	= handle_ocr->GetUTF8Text();

	/* Copy text to global variable */
	snprintf(img_ocr_text, OCR_TEXT_MAX, "%s", txt);

	/* cleanup */
	delete []	txt;
	handle_ocr->Clear();
	handle_ocr->End();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
