/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_USER_IFACE_H
	# define	VA_USER_IFACE_H


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
	/* struct IplImage */
	#include "cv.h"


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Player_Iface_Mode {
		USER_IFACE_FOO,
		USER_IFACE_CLUI,
		USER_IFACE_TUI
	};

	enum	Player_Iface_Action {
		USER_IFACE_ACT_FOO,

		USER_IFACE_ACT_INVERT,

		USER_IFACE_ACT_APPLY,
		USER_IFACE_ACT_SAVE,
		USER_IFACE_ACT_DISCARD,

		USER_IFACE_ACT_QUIT
	};


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
	struct	User_Iface_Log {
		int	len;
		char	line [35] [20];
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
void	user_iface		(struct _IplImage  *imgptr);
void	user_iface_save_name	(const char *filepath, char *filename, int destsize);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* user_iface.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
