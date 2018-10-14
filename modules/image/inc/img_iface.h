/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_IMG_IFACE_H
	# define	VA_IMG_IFACE_H


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Packages ------------------------------------------------------------------*/
		/* struct _IplImage */
	#include <cv.h>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	OCR_TEXT_MAX	(1048576)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Img_Iface_Action {
		IMG_IFACE_ACT_FOO = 0,

		IMG_IFACE_ACT_CV = 0x0100,
		IMG_IFACE_ACT_INVERT,
		IMG_IFACE_ACT_BGR2GRAY,
		IMG_IFACE_ACT_COMPONENT,
		IMG_IFACE_ACT_SMOOTH,
		IMG_IFACE_ACT_THRESHOLD,
		IMG_IFACE_ACT_ADAPTIVE_THRESHOLD,
		IMG_IFACE_ACT_DILATE,
		IMG_IFACE_ACT_ERODE,
		IMG_IFACE_ACT_CONTOURS,
		IMG_IFACE_ACT_MIN_AREA_RECT,
		IMG_IFACE_ACT_ROTATE_ORTO,
		IMG_IFACE_ACT_ROTATE,

		IMG_IFACE_ACT_DILATE_ERODE,
		IMG_IFACE_ACT_ERODE_DILATE,

		IMG_IFACE_ACT_ZB = 0x0200,
		IMG_IFACE_ACT_DECODE,

		IMG_IFACE_ACT_OCR = 0x0400,
		IMG_IFACE_ACT_READ,

		IMG_IFACE_ACT_ORB = 0x0800,
		IMG_IFACE_ACT_ALIGN,

		IMG_IFACE_ACT_IMGI = 0x1000,
		IMG_IFACE_ACT_APPLY,
		IMG_IFACE_ACT_DISCARD,
		IMG_IFACE_ACT_SAVE_MEM,
		IMG_IFACE_ACT_LOAD_MEM,
		IMG_IFACE_ACT_SAVE_REF,

		IMG_IFACE_ACT_SAVE = 0x2000,
		IMG_IFACE_ACT_SAVE_FILE
	};


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
/* img_cv --------------------------------------------------------------------*/
	struct	Img_Iface_Data_Component {
		int	cmp;
	};

	struct	Img_Iface_Data_Smooth {
		int	method;
		int	msk_siz;
	};

	struct	Img_Iface_Data_Threshold {
		int	thr_typ;
		int	thr_val;
	};

	struct	Img_Iface_Data_Adaptive_Thr {
		int	method;
		int	thr_typ;
		int	nbh_val;
	};

	struct	Img_Iface_Data_Dilate_Erode {
		int	i;
	};

	struct	Img_Iface_Data_Contours {
		struct CvMemStorage	**storage;
		struct CvSeq		**contours;
	};

	struct	Img_Iface_Data_MinARect {
		struct CvSeq	**contours;
		struct CvBox2D	*rect;
	};

	struct	Img_Iface_Data_Rotate_Orto {
		int	n;
	};

	struct	Img_Iface_Data_Rotate {
		struct CvPoint2D32f	center;
		double			angle;
	};

/* img_zbar -------------------------------------------------------------------*/
	struct	Img_Iface_Data_Decode {
		int	code_type;
	};

/* img_ocr -------------------------------------------------------------------*/
	struct	Img_Iface_Data_Read {
		int	lang;
		int	conf;
	};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
	extern	char	img_ocr_text [OCR_TEXT_MAX];


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	void			img_iface_cleanup_main	(void);
	struct _IplImage	*img_iface_load		(void);
	void			img_iface_cleanup	(void);
	struct _IplImage	*img_iface_act		(int action, void *data);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* img_iface.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
