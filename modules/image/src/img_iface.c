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

/*	*	*	*	*	*	*	*	*	*
 *	*	* Other	*	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
		/* load_image_file() */
	#include "save.h"
		/* user_iface_log */
	#include "user_iface.h"

		/* img_cv_act() */
	#include "img_cv.h"
		/* img_zb_act() */
	#include "img_zbar.h"
		/* OCR */
	#include "img_ocr.h"

	#include "img_iface.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	IMG_MEM_SIZE	(10)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
static	struct _IplImage	*image_copy_old;
static	struct _IplImage	*image_copy_tmp;
static	struct _IplImage	*image_copy_usr;
static	struct _IplImage	*image_mem [10];


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
	/* Actions */
static	void	img_iface_action	(int action);
static	void	img_iface_invert	(void);
static	void	img_iface_rotate	(void);
static	void	img_iface_bgr2gray	(void);
static	void	img_iface_component	(void);
static	void	img_iface_decode	(void);
static	void	img_iface_read		(void);
static	void	img_iface_apply		(void);
static	void	img_iface_discard	(void);
static	void	img_iface_save_mem	(void);
static	void	img_iface_load_mem	(void);
static	void	img_iface_save_file	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void			img_iface_cleanup_main	(void)
{
	int	i;

	for (i = 0; i < IMG_MEM_SIZE; i++) {
		cvReleaseImage(&image_mem[i]);
	}
}

struct _IplImage	*img_iface_load		(void)
{
	load_image_file();

	/* Make a static copy of image */
	cvReleaseImage(&image_copy_old);
	image_copy_old	= cvCloneImage(image);
	cvReleaseImage(&image_copy_tmp);
	image_copy_tmp	= cvCloneImage(image);

	/* Make a copy of the image so that it isn't modified by the user */
	cvReleaseImage(&image_copy_usr);
	image_copy_usr	= cvCloneImage(image_copy_tmp);
	return	image_copy_tmp;
}

void			img_iface_cleanup	(void)
{
	cvReleaseImage(&image_copy_old);
	cvReleaseImage(&image_copy_tmp);
	cvReleaseImage(&image_copy_usr);
}

struct _IplImage	*img_iface_act		(int action)
{
	img_iface_action(action);

	/* Make a static copy of tmp so that it isn't modified by the user */
	cvReleaseImage(&image_copy_usr);
	image_copy_usr	= cvCloneImage(image_copy_tmp);

	return	image_copy_usr;
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
/*	*	*	*	*	*	*	*	*	*
 *	*	* Actions	*	*	*	*	*	*
 *	*	*	*	*	*	*	*	*	*/
static	void	img_iface_action	(int action)
{
	switch (action) {
	/* img_cv */
	case IMG_IFACE_ACT_INVERT:
		img_iface_invert();
		break;
	case IMG_IFACE_ACT_ROTATE:
		img_iface_rotate();
		break;
	case IMG_IFACE_ACT_BGR2GRAY:
		img_iface_bgr2gray();
		break;
	case IMG_IFACE_ACT_COMPONENT:
		img_iface_component();
		break;

	/* img_zbar */
	case IMG_IFACE_ACT_DECODE:
		img_iface_decode();
		break;

	/* img_ocr */
	case IMG_IFACE_ACT_READ:
		img_iface_read();
		break;

	/* img_iface */
	case IMG_IFACE_ACT_APPLY:
		img_iface_apply();
		break;
	case IMG_IFACE_ACT_DISCARD:
		img_iface_discard();
		break;
	case IMG_IFACE_ACT_SAVE_MEM:
		img_iface_save_mem();
		break;
	case IMG_IFACE_ACT_LOAD_MEM:
		img_iface_load_mem();
		break;
	case IMG_IFACE_ACT_SAVE_FILE:
		img_iface_save_file();
		break;

	default:
		/* Invalid action */
		break;
	}
}

static	void	img_iface_invert	(void)
{
	/* Filter: invert color */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_INVERT);
}

static	void	img_iface_rotate	(void)
{
	/* Rotate */
	img_cv_act(&image_copy_tmp, IMG_CV_ACT_ROTATE);
}

static	void	img_iface_bgr2gray	(void)
{
	/* Filter: invert color */
	if (image_copy_tmp->nChannels == 3) {
		img_cv_act(&image_copy_tmp, IMG_CV_ACT_BGR2GRAY);
	}
}

static	void	img_iface_component	(void)
{
	/* Filter: extract component */
	if (image_copy_tmp->nChannels == 3) {
		img_cv_act(&image_copy_tmp, IMG_CV_ACT_COMPONENT);
	}
}

static	void	img_iface_decode	(void)
{
	/* Decode */
//	if (image_copy_tmp->nChannels == 3) {
		img_zb_act(&image_copy_tmp, IMG_ZB_ACT_DECODE);
//	}
}

static	void	img_iface_read		(void)
{
	/* OCR */
//	if (image_copy_tmp->nChannels == 1) {
		img_ocr_act(&image_copy_tmp, IMG_OCR_ACT_READ);
//	}
}

static	void	img_iface_apply		(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Apply changes");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Write tmp into old */
	cvReleaseImage(&image_copy_old);
	image_copy_old	= cvCloneImage(image_copy_tmp);
}

static	void	img_iface_discard	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Discard changes");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Discard tmp image copy */
	cvReleaseImage(&image_copy_tmp);
	image_copy_tmp	= cvCloneImage(image_copy_old);
}

static	void	img_iface_save_mem	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Save to memory");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Apply changes before saving */
	img_iface_apply();

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Save to mem_%i", 0);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write into mem */
	cvReleaseImage(&image_mem[0]);
	image_mem[0]	= cvCloneImage(image_copy_old);
}

static	void	img_iface_load_mem	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Load from memory");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Discard tmp image copy */
	cvReleaseImage(&image_copy_tmp);

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Load from mem_%i", 0);
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Load from mem */
	image_copy_tmp	= cvCloneImage(image_mem[0]);
}

static	void	img_iface_save_file	(void)
{
	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Save to file");
	user_iface_log.lvl[user_iface_log.len]	= 0;
	(user_iface_log.len)++;

	/* Apply changes before saving */
	img_iface_apply();

	/* Write into log */
	snprintf(user_iface_log.line[user_iface_log.len], LOG_LINE_LEN, "Save as...");
	user_iface_log.lvl[user_iface_log.len]	= 1;
	(user_iface_log.len)++;

	/* Write into image struct (save.c) */
	cvReleaseImage(&image);
	image	= cvCloneImage(image_copy_old);

	/* Save into file */
	save_image_file();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
