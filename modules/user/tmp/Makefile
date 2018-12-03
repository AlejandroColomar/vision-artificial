# -*- MakeFile -*-

# MACRO = substitute with this

# directories

LIBALX_INC_DIR	= $(LIBALX_DIR)/inc/
IMG_INC_DIR	= $(IMG_DIR)/inc/

IMG_INC_DIR	= $(IMG_DIR)/inc/
MENU_INC_DIR	= $(MENU_DIR)/inc/
PROC_INC_DIR	= $(PROC_DIR)/inc/

INC_DIR		= $(USR_DIR)/inc/
SRC_DIR		= $(USR_DIR)/src/

# dependencies

_ALL	= user_iface.o user_clui.o user_tui.o
ALL	= $(_ALL) user_mod.o

UI_INC_IMG		= img_iface.h
UI_INC_PROC		= proc_iface.h
UI_INC			= user_iface.h	user_clui.h user_tui.h
UI_DEPS			= $(SRC_DIR)/user_iface.c \
				$(patsubst %,$(INC_DIR)/%,$(UI_INC)) \
				$(patsubst %,$(IMG_INC_DIR)/%,$(UI_INC_IMG)) \
				$(patsubst %,$(PROC_INC_DIR)/%,$(UI_INC_PROC))
UI_INC_DIRS		= -I $(INC_DIR) \
				-I $(IMG_INC_DIR) \
				-I $(PROC_INC_DIR)

UCLUI_INC_LIBALX	= alx_ncur.h
UCLUI_INC_IMG		= img_iface.h
UCLUI_INC		= user_clui.h	user_iface.h
UCLUI_DEPS		= $(SRC_DIR)/user_clui.c \
				$(patsubst %,$(INC_DIR)/%,$(UCLUI_INC)) \
				$(patsubst %,$(IMG_INC_DIR)/%,$(UCLUI_INC_IMG)) \
				$(patsubst %,$(LIBALX_INC_DIR)/%,$(UCLUI_INC_LIBALX))
UCLUI_INC_DIRS		= -I $(INC_DIR) \
				-I $(LIBALX_INC_DIR) \
				-I $(IMG_INC_DIR)

UTUI_INC_LIBALX		= alx_ncur.h
UTUI_INC_IMG		= img_iface.h
UTUI_INC		= user_tui.h	user_iface.h
UTUI_DEPS		= $(SRC_DIR)/user_tui.c \
				$(patsubst %,$(INC_DIR)/%,$(UTUI_INC)) \
				$(patsubst %,$(IMG_INC_DIR)/%,$(UTUI_INC_IMG)) \
				$(patsubst %,$(LIBALX_INC_DIR)/%,$(UTUI_INC_LIBALX))
UTUI_INC_DIRS		= -I $(INC_DIR) \
				-I $(LIBALX_INC_DIR) \
				-I $(IMG_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


user_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"


user_iface.s: $(UI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(UI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
user_iface.o: user_iface.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

user_clui.s: $(UCLUI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(UCLUI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
user_clui.o: user_clui.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

user_tui.s: $(UTUI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(UTUI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
user_tui.o: user_tui.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s
