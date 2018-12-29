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

PHONY := all
all:
	@echo	'	MAKE	modules:	about'
	$(Q)$(MAKE) -C $(ABOUT_DIR)
	@echo	'	MAKE	modules:	ctrl'
	$(Q)$(MAKE) -C $(CTRL_DIR)
	@echo	'	MAKE	modules:	image'
	$(Q)$(MAKE) -C $(IMG_DIR)
	@echo	'	MAKE	modules:	menu'
	$(Q)$(MAKE) -C $(MENU_DIR)
	@echo	'	MAKE	modules:	proc'
	$(Q)$(MAKE) -C $(PROC_DIR)
	@echo	'	MAKE	modules:	save'
	$(Q)$(MAKE) -C $(SAVE_DIR)
	@echo	'	MAKE	modules:	usr'
	$(Q)$(MAKE) -C $(USR_DIR)
	@echo	'	MAKE	modules.o'
	$(Q)$(MAKE) -C $(TMP_DIR)


PHONY += clean
clean:
	@echo	"	RM	*.o *.s"
	$(Q)find . -type f -name '*.o' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.s' -exec rm '{}' '+'

################################################################################
# Declare the contents of the .PHONY variable as phony.
.PHONY: $(PHONY)




################################################################################
######## End of file ###########################################################
################################################################################
