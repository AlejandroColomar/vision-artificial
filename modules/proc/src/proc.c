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
		/* opencv gui */
	#include <highgui.h>
		/* snprintf() & fflush() */
	#include <stdio.h>
		/* bool */
	#include <stdbool.h>
		/* strcmp() */
	#include <string.h>

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* img_iface_act() */
	#include "img_iface.h"
		/* WIN_NAME */
	#include "user_iface.h"

	#include "proc.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	bool	proc_etiqueta		(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
bool	proc_iface		(int proc_mode)
{
	switch (proc_mode) {
	case PROC_MODE_ETIQUETA:
		proc_etiqueta();
		break;
	}

	return	false;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	bool	proc_etiqueta		(void)
{
	struct _IplImage	*imgptr;

	bool	label;
	bool	product;
	bool	price;

//	getchar();
	/*
	 * A:	Save to mem_0
	 * First, find the label, then return to this image and rotate it to
	 * straight position (to help OCR).
	 */
	int					data_A;
	data_A		= 0;
	imgptr	= img_iface_act(IMG_IFACE_ACT_SAVE_MEM, (void *)&data_A);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

	/*
	 * B:	Extract green cmp
	 * Because of the type of light used, green is the best component.
	 */
	struct Img_Iface_Data_Component		data_B;
	data_B.cmp	= 0;	/* B=0, G=1, R=2 */
	imgptr	= img_iface_act(IMG_IFACE_ACT_COMPONENT, (void *)&data_B);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * D:	Smooth (median 3x3)
	 * Median filter to remove salt noise.
	 */
	struct Img_Iface_Data_Smooth		data_C;
	data_C.method	= CV_MEDIAN;
	data_C.msk_siz	= 7;
	imgptr	= img_iface_act(IMG_IFACE_ACT_SMOOTH, (void *)&data_C);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

	/*
	 * C:	Detect label.
	 * If value is 0, it will detect plain surfaces.
	 * It causes problems when there is much noise.
	 * If set to 1, it will detect white surfaces.
	 */
//	getchar();
#if 0
	struct Img_Iface_Data_Adaptive_Thr	data_D;
	data_D.method	= CV_ADAPTIVE_THRESH_MEAN_C;
	data_D.thr_typ	= CV_THRESH_BINARY;
	data_D.nbh_val	= 5;
	imgptr	= img_iface_act(USER_IFACE_ACT_ADAPTIVE_THRESHOLD, (void *)&data_D);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);
#else
	imgptr	= img_iface_act(USER_IFACE_ACT_INVERT, NULL);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);
#endif

//	getchar();
	/*
	 * D:	Smooth (mean 15x15)
	 * Fast smooth to homogenize the label in black,
	 * and noise in gray.
	 */
	struct Img_Iface_Data_Smooth		data_E;
	data_E.method	= CV_BLUR;
	data_E.msk_siz	= 21;
	imgptr	= img_iface_act(IMG_IFACE_ACT_SMOOTH, (void *)&data_E);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * E:	Threshold (BIN_INV)
	 * Label will be white;  rest in black.
	 */
	struct Img_Iface_Data_Threshold		data_F;
	data_F.thr_typ	= CV_THRESH_BINARY_INV;
	data_F.thr_val	= 2;
	imgptr	= img_iface_act(IMG_IFACE_ACT_THRESHOLD, (void *)&data_F);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * F:	Dilate-erode (100 times)
	 * Join all white blobs into one single blob.
	 */
	struct Img_Iface_Data_Dilate_Erode	data_G;
	data_G.i	= 100;
	imgptr	= img_iface_act(IMG_IFACE_ACT_DILATE_ERODE, (void *)&data_G);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * G:	Find contour
	 * This is neccessary for the next step.
	 */
	struct CvMemStorage			*et_storage;
	struct CvSeq				*et_contours;
	struct Img_Iface_Data_Contours		data_H;
	et_storage	= cvCreateMemStorage(0);
	data_H.storage	= &et_storage;
	data_H.contours	= &et_contours;
	imgptr	= img_iface_act(IMG_IFACE_ACT_CONTOURS, (void *)&data_H);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

	/* If no contour is found, label is NOK */
	if (!et_contours) {
		label	= false;
		return	label;
	}

//	getchar();
	/*
	 * H:	Find minimum area rectangle for the contours found
	 * This rectangle will contain the label (the white part of it).
	 */
	struct CvBox2D				et_rect_minA;
	struct Img_Iface_Data_MinARect		data_I;
	data_I.contours	= &et_contours;
	data_I.rect	= &et_rect_minA;
	imgptr	= img_iface_act(IMG_IFACE_ACT_MIN_AREA_RECT, (void *)&data_I);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

	/* If angle is < -45º, it is taking into acount the incorrect side */
	if (et_rect_minA.angle < -45.0) {
		int	tmp;
		et_rect_minA.angle		= et_rect_minA.angle + 90.0;
		tmp				= et_rect_minA.size.width;
		et_rect_minA.size.width		= et_rect_minA.size.height;
		et_rect_minA.size.height	= tmp;
	}

//	getchar();
	/*
	 * I:	Load image from mem_0
	 * Recover the image stored previously, which will be rotated with
	 * the information gained with the rectangle (position, size & angle).
	 */
	int					data_J;
	data_J		= 0;
	imgptr	= img_iface_act(IMG_IFACE_ACT_LOAD_MEM, (void *)&data_J);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

	/*
	 * K:	Extract green cmp
	 * Because of the type of light used, green is the best component.
	 */
	struct Img_Iface_Data_Component		data_K;
	data_K.cmp	= 1;	/* B=0, G=1, R=2 */
	imgptr	= img_iface_act(IMG_IFACE_ACT_COMPONENT, (void *)&data_K);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * J:	Rotate
	 * Align the label.
	 */
	struct Img_Iface_Data_Rotate		data_L;
	data_L.center.x	= et_rect_minA.center.x;
	data_L.center.y	= et_rect_minA.center.y;
	data_L.angle	= et_rect_minA.angle;
	imgptr	= img_iface_act(IMG_IFACE_ACT_ROTATE, (void *)&data_L);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * K:	Save to mem_1
	 * Save the aligned image, from which several ROIs will be extracted.
	 */
	int					data_M;
	data_M		= 1;
	imgptr	= img_iface_act(IMG_IFACE_ACT_SAVE_MEM, (void *)&data_M);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * L:	Set ROI: Cerdo
	 * Set a ROI that contains "Cerdo".
	 */
	struct Img_Iface_Data_SetROI		data_N;
	data_N.rect.x		= et_rect_minA.center.x -
					(1.05 * et_rect_minA.size.width / 2);
	data_N.rect.y		= et_rect_minA.center.y -
					(1.47 * et_rect_minA.size.height / 2);
	data_N.rect.width	= et_rect_minA.size.width / 2;
	data_N.rect.height	= et_rect_minA.size.height * 0.20;
	imgptr	= img_iface_act(IMG_IFACE_ACT_SET_ROI, (void *)&data_N);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * M:	Crop to ROI Cerdo
	 */
	imgptr	= img_iface_act(IMG_IFACE_ACT_CROP, NULL);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * N:	Threshold (BIN)
	 * "Cerdo" will be white;  rest in black.
	 */
	struct Img_Iface_Data_Threshold		data_O;
	data_O.thr_typ	= CV_THRESH_BINARY;
	data_O.thr_val	= -1;
	imgptr	= img_iface_act(IMG_IFACE_ACT_THRESHOLD, (void *)&data_O);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);

//	getchar();
	/*
	 * O:	OCR
	 * OCR text should be:  "Cerdo"
	 */
	struct Img_Iface_Data_Read		data_P;
	data_P.lang	= 1;	/* eng=0, spa=1, cat=2 */
	data_P.conf	= 0;	/* none=0, price=1 */
	imgptr	= img_iface_act(IMG_IFACE_ACT_READ, (void *)&data_P);
		/* Display image and do NOT wait for any key to continue */
		cvShowImage(WIN_NAME, imgptr);
		cvWaitKey(WIN_TIMEOUT);
	label	= strncmp(img_ocr_text, "Cerdo", 5);
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN,
						"Label:	%i", label);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

//	getchar();
	/* Cleanup */
	cvReleaseMemStorage(&et_storage);


//	getchar();
	return	false;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
