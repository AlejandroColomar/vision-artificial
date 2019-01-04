/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_PROC_LABEL_H
	# define	VA_PROC_LABEL_H


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Proc_Label {
		LABEL_OK,
		LABEL_NOK_LABEL,
		LABEL_NOK_CERDO,
		LABEL_NOK_BCODE,
		LABEL_NOK_PRODUCT,
		LABEL_NOK_PRICE
	};


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	int	proc_label		(void);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* proc_label.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
