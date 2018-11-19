/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_PROC_RESISTOR_HPP
	# define	VA_PROC_RESISTOR_HPP


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Proc_Resistor {
		RESISTOR_OK,
		RESISTOR_NOK_RESISTOR,
		RESISTOR_NOK_BANDS,
		RESISTOR_NOK_STD_VALUE,
		RESISTOR_NOK_TOLERANCE
	};


/******************************************************************************
 ******* C wrapper ************************************************************
 ******************************************************************************/
extern	"C" {


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	int	proc_resistor		(void);


/******************************************************************************
 ******* C wrapper ************************************************************
 ******************************************************************************/
}	/* extern "C" */


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* proc_resistor.hpp */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
