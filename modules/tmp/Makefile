# -*- MakeFile -*-

# MACRO = substitute with this

# directories

ABOUT_TMP_DIR	= $(ABOUT_DIR)/tmp/
CTRL_TMP_DIR	= $(CTRL_DIR)/tmp/
IMG_TMP_DIR	= $(IMG_DIR)/tmp/
MENU_TMP_DIR	= $(MENU_DIR)/tmp/
PROC_TMP_DIR	= $(PROC_DIR)/tmp/
SAVE_TMP_DIR	= $(SAVE_DIR)/tmp/
USR_TMP_DIR	= $(USR_DIR)/tmp/

# dependencies

ALL = modules.o

MODS_OBJ_ABOUT	= about_mod.o
MODS_OBJ_CTRL	= ctrl_mod.o
MODS_OBJ_IMG	= img_mod.o
MODS_OBJ_MENU	= menu_mod.o
MODS_OBJ_PROC	= proc_mod.o
MODS_OBJ_SAVE	= save_mod.o
MODS_OBJ_USR	= user_mod.o
MODS_OBJS	= $(patsubst %,$(ABOUT_TMP_DIR)/%,$(MODS_OBJ_ABOUT)) \
			$(patsubst %,$(CTRL_TMP_DIR)/%,$(MODS_OBJ_CTRL)) \
			$(patsubst %,$(IMG_TMP_DIR)/%,$(MODS_OBJ_IMG)) \
			$(patsubst %,$(MENU_TMP_DIR)/%,$(MODS_OBJ_MENU)) \
			$(patsubst %,$(PROC_TMP_DIR)/%,$(MODS_OBJ_PROC)) \
			$(patsubst %,$(SAVE_TMP_DIR)/%,$(MODS_OBJ_SAVE)) \
			$(patsubst %,$(USR_TMP_DIR)/%,$(MODS_OBJ_USR))

# target: dependencies
#	action

all: $(ALL)


modules.o: $(MODS_OBJS)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"
	@echo  ""


clean:
	$(Q)rm -f *.o
