/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		VA_PROC_HPP
	# define	VA_PROC_HPP


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Proc_Mode {
		PROC_MODE_FOO,

		PROC_MODE = 0x4000,
		PROC_MODE_LABEL
	};

	enum	Proc_DBG {
		PROC_DBG_NO,
		PROC_DBG_STOP_PROD,
		PROC_DBG_DELAY_STEP,
		PROC_DBG_STOP_STEP
	};

	enum	Proc_Label {
		LABEL_OK,
		LABEL_NOK_LABEL,
		LABEL_NOK_CERDO,
		LABEL_NOK_BCODE,
		LABEL_NOK_PRODUCT,
		LABEL_NOK_PRICE
	};


/******************************************************************************
 ******* C wrapper ************************************************************
 ******************************************************************************/
extern	"C" {


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
	extern	int	proc_debug;
	extern	int	proc_mode;


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	int	proc_iface_single	(void);
	void	proc_iface_series	(void);


/******************************************************************************
 ******* C wrapper ************************************************************
 ******************************************************************************/
}	/* extern "C" */


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* proc.hpp */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
