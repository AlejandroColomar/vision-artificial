# -*- MakeFile -*-

# MACRO = substitute with this

# directories

ABOUT_DIR = $(MODULES_DIR)/about/
CTRL_DIR = $(MODULES_DIR)/ctrl/
IMG_DIR = $(MODULES_DIR)/image/
MENU_DIR = $(MODULES_DIR)/menu/
PROC_DIR = $(MODULES_DIR)/proc/
SAVE_DIR = $(MODULES_DIR)/save/
USR_DIR = $(MODULES_DIR)/user/

TMP_DIR = $(MODULES_DIR)/tmp/

export	ABOUT_DIR
export	CTRL_DIR
export	IMG_DIR
export	MENU_DIR
export	PROC_DIR
export	SAVE_DIR
export	USR_DIR

# dependencies


# target: dependencies
#	action

all:
	@echo	'	MAKE	modules: about'
	$(Q)make -C $(ABOUT_DIR)
	@echo	'	MAKE	modules: ctrl'
	$(Q)make -C $(CTRL_DIR)
	@echo	'	MAKE	modules: image'
	$(Q)make -C $(IMG_DIR)
	@echo	'	MAKE	modules: menu'
	$(Q)make -C $(MENU_DIR)
	@echo	'	MAKE	modules: proc'
	$(Q)make -C $(PROC_DIR)
	@echo	'	MAKE	modules: save'
	$(Q)make -C $(SAVE_DIR)
	@echo	'	MAKE	modules: usr'
	$(Q)make -C $(USR_DIR)
	@echo	'	MAKE	modules.o'
	$(Q)make -C $(TMP_DIR)
	@echo  ""


clean:
	@echo  "	RM	*.o *.s"
	$(Q)find . -type f -name '*.o' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.s' -exec rm '{}' '+'

################################################################################
######## End of file ###########################################################
################################################################################
