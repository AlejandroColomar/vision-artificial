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
	$(Q)cd $(ABOUT_DIR) && $(MAKE) && cd ..
	$(Q)cd $(CTRL_DIR) && $(MAKE) && cd ..
	$(Q)cd $(IMG_DIR) && $(MAKE) && cd ..
	$(Q)cd $(MENU_DIR) && $(MAKE) && cd ..
	$(Q)cd $(PROC_DIR) && $(MAKE) && cd ..
	$(Q)cd $(SAVE_DIR) && $(MAKE) && cd ..
	$(Q)cd $(USR_DIR) && $(MAKE) && cd ..
	$(Q)cd $(TMP_DIR) && $(MAKE) && cd ..


clean:
	$(Q)find . -type f -name '*.o' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.s' -exec rm '{}' '+'
	@echo  "Clean modules"

################################################################################
######## End of file ###########################################################
################################################################################
