/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_IMAGE_H
	# define	VA_IMAGE_H


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
	/* struct IplImage */
	#include "cv.h"


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Img_Action {
		IMG_ACT_FOO,

		IMG_ACT_INVERT,

		IMG_ACT_APPLY,
		IMG_ACT_SAVE,
		IMG_ACT_DISCARD
	};


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	struct _IplImage	*img_load	(void);
	void			img_cleanup	(void);
	struct _IplImage	*img_act	(int action);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* image.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
