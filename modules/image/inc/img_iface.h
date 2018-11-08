/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_IMG_IFACE_H
	# define	VA_IMG_IFACE_H


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
/* Constants -----------------------------------------------------------------*/
	# define	CONTOURS_MAX		(65536)
	# define	OCR_TEXT_MAX		(1048576)
	# define	ZB_CODES_MAX		(10)
	# define	ZBAR_LEN_MAX		(1048576)

	# define	IMG_IFACE_THR_OTSU	(-1)

/* Functions -----------------------------------------------------------------*/
	# define	img_iface_act_nodata(x)	img_iface_act(x, NULL);


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Img_Iface_Action {
		IMG_IFACE_ACT_FOO = 0,

		IMG_IFACE_ACT_ALX = 0x0010,
		IMG_IFACE_ACT_LOCAL_MAX,

		IMG_IFACE_ACT_CV = 0x0100,
		IMG_IFACE_ACT_PIXEL_VALUE,
		IMG_IFACE_ACT_SET_ROI,
		IMG_IFACE_ACT_SET_ROI_2RECT,
		IMG_IFACE_ACT_AND_2REF,
		IMG_IFACE_ACT_NOT,
		IMG_IFACE_ACT_OR_2REF,
		IMG_IFACE_ACT_COMPONENT,
		IMG_IFACE_ACT_DILATE,
		IMG_IFACE_ACT_ERODE,
		IMG_IFACE_ACT_DILATE_ERODE,
		IMG_IFACE_ACT_ERODE_DILATE,
		IMG_IFACE_ACT_SMOOTH,
		IMG_IFACE_ACT_SOBEL,
		IMG_IFACE_ACT_ROTATE_ORTO,
		IMG_IFACE_ACT_ROTATE,
		IMG_IFACE_ACT_ROTATE_2RECT,
		IMG_IFACE_ACT_ADAPTIVE_THRESHOLD,
		IMG_IFACE_ACT_CVT_COLOR,
		IMG_IFACE_ACT_DISTANCE_TRANSFORM,
		IMG_IFACE_ACT_THRESHOLD,
		IMG_IFACE_ACT_HISTOGRAM,
		IMG_IFACE_ACT_HISTOGRAM_C3,
		IMG_IFACE_ACT_CONTOURS,
		IMG_IFACE_ACT_CONTOURS_SIZE,
		IMG_IFACE_ACT_BOUNDING_RECT,
		IMG_IFACE_ACT_FIT_ELLIPSE,
		IMG_IFACE_ACT_MIN_AREA_RECT,
		IMG_IFACE_ACT_HOUGH_CIRCLES,

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

	enum	Img_Iface_Cmp_BGR {
		IMG_IFACE_CMP_BLUE = 0,
		IMG_IFACE_CMP_GREEN,
		IMG_IFACE_CMP_RED
	};

	enum	Img_Iface_Cmp_HSV {
		IMG_IFACE_CMP_HUE = 0,
		IMG_IFACE_CMP_SATURATION,
		IMG_IFACE_CMP_VALUE
	};

	enum	ImgI_Smooth {
		IMGI_SMOOTH_MEAN = 0,
		IMGI_SMOOTH_GAUSS,
		IMGI_SMOOTH_MEDIAN
	};

	enum	Img_Iface_OCR_Lang {
		IMG_IFACE_OCR_LANG_ENG = 0,
		IMG_IFACE_OCR_LANG_SPA,
		IMG_IFACE_OCR_LANG_CAT,
		IMG_IFACE_OCR_LANG_DIGITS,
		IMG_IFACE_OCR_LANG_DIGITS_COMMA
	};

	enum	Img_Iface_OCR_Conf {
		IMG_IFACE_OCR_CONF_NONE = 0,
		IMG_IFACE_OCR_CONF_PRICE
	};


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
	struct	Img_Iface_Data_Read {
		int	lang;
		int	conf;
		struct {
			void	*data;
			int	width;
			int	height;
			int	B_per_pix;
			int	B_per_line;
		} img;
	};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
	extern	char				img_ocr_text [OCR_TEXT_MAX];
	extern	struct Img_Iface_ZB_Codes	zb_codes;


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	void	img_iface_cleanup_main	(void);
	void	img_iface_load		(const char *fpath, const char *fname);
	void	img_iface_cleanup	(void);
		/* data should *always* be NULL */ 
	void	img_iface_act		(int action, void *data);
	void	img_iface_show_img	(void);
	void	img_iface_show_hist_c1	(void);
	void	img_iface_show_hist_c3	(void);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* img_iface.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
