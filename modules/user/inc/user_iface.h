/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_USER_IFACE_H
	# define	VA_USER_IFACE_H


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	LOG_LEN		(1048576)
	# define	LOG_LINE_LEN	(35)


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
		/* intX_t */
	#include <stdint.h>


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Player_Iface_Mode {
		USER_IFACE_FOO,
		USER_IFACE_CLUI,
		USER_IFACE_TUI
	};

	enum	Player_Iface_Action {
		USER_IFACE_ACT_FOO = 0,

		USER_IFACE_ACT_CV = 0x0100,
		USER_IFACE_ACT_PIXEL_VALUE,
		USER_IFACE_ACT_SET_ROI,
		USER_IFACE_ACT_SET_ROI_2RECT,
		USER_IFACE_ACT_AND_2REF,
		USER_IFACE_ACT_NOT,
		USER_IFACE_ACT_OR_2REF,
		USER_IFACE_ACT_COMPONENT,
		USER_IFACE_ACT_DILATE,
		USER_IFACE_ACT_ERODE,
		USER_IFACE_ACT_DILATE_ERODE,
		USER_IFACE_ACT_ERODE_DILATE,
		USER_IFACE_ACT_SMOOTH,
		USER_IFACE_ACT_SOBEL,
		USER_IFACE_ACT_ROTATE_ORTO,
		USER_IFACE_ACT_ROTATE,
		USER_IFACE_ACT_ROTATE_2RECT,
		USER_IFACE_ACT_ADAPTIVE_THRESHOLD,
		USER_IFACE_ACT_THRESHOLD,
		USER_IFACE_ACT_CVT_COLOR,
		USER_IFACE_ACT_DISTANCE_TRANSFORM,
		USER_IFACE_ACT_HISTOGRAM,
		USER_IFACE_ACT_HISTOGRAM_C3,
		USER_IFACE_ACT_CONTOURS,
		USER_IFACE_ACT_CONTOURS_SIZE,
		USER_IFACE_ACT_BOUNDING_RECT,
		USER_IFACE_ACT_FIT_ELLIPSE,
		USER_IFACE_ACT_MIN_AREA_RECT,

		USER_IFACE_ACT_ZB = 0x0200,
		USER_IFACE_ACT_DECODE,

		USER_IFACE_ACT_OCR = 0x0400,
		USER_IFACE_ACT_READ,

		USER_IFACE_ACT_ORB = 0x0800,
		USER_IFACE_ACT_ALIGN,

		USER_IFACE_ACT_IMGI = 0x1000,
		USER_IFACE_ACT_APPLY,
		USER_IFACE_ACT_DISCARD,
		USER_IFACE_ACT_SAVE_MEM,
		USER_IFACE_ACT_LOAD_MEM,
		USER_IFACE_ACT_SAVE_REF,

		USER_IFACE_ACT_SAVE = 0x2000,
		USER_IFACE_ACT_SAVE_FILE,

		USER_IFACE_ACT_PROC = 0x4000,
		USER_IFACE_ACT_PROC_LABEL,
		USER_IFACE_ACT_PROC_RESISTOR,

		USER_IFACE_ACT_USRI = 0x8000,
		USER_IFACE_ACT_SHOW_OCR,
		USER_IFACE_ACT_QUIT
	};


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
	struct	User_Iface_Log {
		int	len;
		char	line [LOG_LEN] [LOG_LINE_LEN];
		int	lvl [LOG_LEN];
		int	visible;
	};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
	extern	int			user_iface_mode;
	extern	struct User_Iface_Log	user_iface_log;


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	void	user_iface_init		(void);
	void	user_iface_cleanup	(void);
	void	user_iface		(void);
	void	user_iface_show_log	(const char *title, const char *subtitle);
	void	user_iface_fname	(const char *filepath, char *filename);
	double	user_iface_getdbl	(double m, double def, double M,
					const char *title, const char *help);
	int64_t	user_iface_getint	(double m, int64_t def, double M,
					const char *title, const char *help);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* user_iface.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
