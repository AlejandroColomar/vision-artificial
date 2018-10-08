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
		/* INT_MAX */
	#include <limits.h>
		/* snprintf() */
	#include <stdio.h>
		/* OCR Tesseract */
	#include <tesseract/capi.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* user_iface_log */
	#include "user_iface.h"

	#include "img_ocr.h"


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
char	img_ocr_text [TEXT_MAX];


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(struct _IplImage  *imgptr);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_ocr_act	(struct _IplImage  **imgptr2, int action)
{
	switch (action) {
	case IMG_OCR_ACT_READ:
		img_ocr_read(*imgptr2);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_ocr_read	(struct _IplImage  *imgptr)
{

	struct _IplImage	*imgtmp;
	struct TessBaseAPI	*handle_ocr;

	/* Make a copy of imgptr so that it isn't modified by zbar */
	imgtmp	= cvCloneImage(imgptr);

	/* Ask user which language to scan */
	char	title [80];
	int	lang;
	char	lang_code [3 + 1];
	snprintf(title, 80, "Language: ENG = 0, SPA = 1, CAT = 2");
	lang	= user_iface_getint(0, 1, 2, title, NULL);
	switch (lang) {
	case 0:
		sprintf(lang_code, "eng");
		break;
	case 1:
		sprintf(lang_code, "spa");
		break;
	case 2:
		sprintf(lang_code, "cat");
		break;
	}

	/* init OCR */
	handle_ocr	= TessBaseAPICreate();
	TessBaseAPIInit3(handle_ocr, NULL, lang_code);

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"OCR (%s)", lang_code);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* scan image for text */
	TessBaseAPISetImage(handle_ocr, imgtmp->imageData,
				imgtmp->width, imgtmp->height,
				imgtmp->depth / 8, imgtmp->widthStep);
	TessBaseAPIRecognize(handle_ocr, NULL);
	char	*txt;
	txt	= TessBaseAPIGetUTF8Text(handle_ocr);

	/* scan the image for text */
	if (!txt){
		snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
							"No text detected");
		user_iface_log.lvl[user_iface_log.len]	= 1;
		(user_iface_log.len)++;
	}

	/* Copy text to global variable */
	snprintf(img_ocr_text, TEXT_MAX, "%s", txt);

	/* cleanup */
	TessDeleteText(txt);
	TessBaseAPIEnd(handle_ocr);
	TessBaseAPIDelete(handle_ocr);
	cvReleaseImage(&imgtmp);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
