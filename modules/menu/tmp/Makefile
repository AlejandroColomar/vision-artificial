# -*- MakeFile -*-

# MACRO = substitute with this

# directories

LIBALX_INC_DIR	= $(LIBALX_DIR)/inc/

ABOUT_INC_DIR	= $(ABOUT_DIR)/inc/
CTRL_INC_DIR	= $(CTRL_DIR)/inc/
PROC_INC_DIR	= $(PROC_DIR)/inc/
SAVE_INC_DIR	= $(SAVE_DIR)/inc/
USR_INC_DIR	= $(USR_DIR)/inc/

INC_DIR		= $(MENU_DIR)/inc/
SRC_DIR		= $(MENU_DIR)/src/

# dependencies

_ALL	= parser.o menu_iface.o menu_clui.o menu_tui.o
ALL	= $(_ALL) menu_mod.o

PARS_INC_ABOUT	= about.h
PARS_INC_SAVE	= save.h
PARS_INC_USR	= user_iface.h
PARS_INC	= parser.h	menu_iface.h
PARS_DEPS	= $(SRC_DIR)/parser.c \
			$(patsubst %,$(INC_DIR)/%,$(PARS_INC)) \
			$(patsubst %,$(ABOUT_INC_DIR)/%,$(PARS_INC_ABOUT)) \
			$(patsubst %,$(SAVE_INC_DIR)/%,$(PARS_INC_SAVE)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(PARS_INC_USR))
PARS_INC_DIRS	= -I $(INC_DIR) \
			-I $(ABOUT_INC_DIR) \
			-I $(SAVE_INC_DIR) \
			-I $(USR_INC_DIR)

MENUI_INC	= menu_iface.h	menu_clui.h menu_tui.h
MENUI_DEPS	= $(SRC_DIR)/menu_iface.c \
			$(patsubst %,$(INC_DIR)/%,$(MENUI_INC))
MENUI_INC_DIRS	= -I $(INC_DIR)

MENUCLUI_INC_LIBALX	= libalx/io/alx_input.h
MENUCLUI_INC_ABOUT	= about.h
MENUCLUI_INC_CTRL	= start.h
MENUCLUI_INC_SAVE	= save.h
MENUCLUI_INC		= menu_clui.h
MENUCLUI_DEPS	= $(SRC_DIR)/menu_clui.c \
			$(patsubst %,$(INC_DIR)/%,$(MENUCLUI_INC)) \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(MENUCLUI_INC_LIBALX)) \
			$(patsubst %,$(ABOUT_INC_DIR)/%,$(MENUCLUI_INC_ABOUT)) \
			$(patsubst %,$(CTRL_INC_DIR)/%,$(MENUCLUI_INC_CTRL)) \
			$(patsubst %,$(SAVE_INC_DIR)/%,$(MENUCLUI_INC_SAVE))
MENUCLUI_INC_DIRS	= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(ABOUT_INC_DIR) \
			-I $(CTRL_INC_DIR) \
			-I $(SAVE_INC_DIR)

MENUTUI_INC_LIBALX	= libalx/curses/alx_ncur.h
MENUTUI_INC_ABOUT	= about.h
MENUTUI_INC_CTRL	= start.h
MENUTUI_INC_PROC	= proc_iface.h
MENUTUI_INC_SAVE	= save.h
MENUTUI_INC_USR		= user_iface.h
MENUTUI_INC		= menu_tui.h
MENUTUI_DEPS	= $(SRC_DIR)/menu_tui.c \
			$(patsubst %,$(INC_DIR)/%,$(MENUTUI_INC)) \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(MENUTUI_INC_LIBALX)) \
			$(patsubst %,$(ABOUT_INC_DIR)/%,$(MENUTUI_INC_ABOUT)) \
			$(patsubst %,$(CTRL_INC_DIR)/%,$(MENUTUI_INC_CTRL)) \
			$(patsubst %,$(PROC_INC_DIR)/%,$(MENUTUI_INC_PROC)) \
			$(patsubst %,$(SAVE_INC_DIR)/%,$(MENUTUI_INC_SAVE)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(MENUTUI_INC_USR))
MENUTUI_INC_DIRS	= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(ABOUT_INC_DIR) \
			-I $(CTRL_INC_DIR) \
			-I $(PROC_INC_DIR) \
			-I $(SAVE_INC_DIR) \
			-I $(USR_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


menu_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"


parser.s: $(PARS_DEPS)
	$(Q)$(CC) $(CFLAGS) $(PARS_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
parser.o: parser.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

menu_iface.s: $(MENUI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(MENUI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
menu_iface.o: menu_iface.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

menu_clui.s: $(MENUCLUI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(MENUCLUI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
menu_clui.o: menu_clui.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

menu_tui.s: $(MENUTUI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(MENUTUI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
menu_tui.o: menu_tui.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s
