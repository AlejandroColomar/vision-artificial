/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_USER_TUI_H
	# define	VA_USER_TUI_H


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
void	user_tui_init		(void);
void	user_tui_cleanup	(void);
int	user_tui		(const char *title, const char *subtitle);
void	user_tui_show_log	(const char *title, const char *subtitle);
void	user_tui_save_name	(const char *filepath, char *filename, int destsize);
double	user_tui_getdbl		(double m, double def, double M,
				const char *title, const char *help);
int64_t	user_tui_getint		(double m, int64_t def, double M,
				const char *title, const char *help);
void	user_tui_show_ocr	(void);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* user_tui.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
