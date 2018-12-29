# -*- MakeFile -*-

# MACRO = substitute with this

# directories

LIBALX_INC_DIR	= $(LIBALX_DIR)/inc/

ABOUT_DIR	= $(MODULES_DIR)/about/
ABOUT_INC_DIR	= $(ABOUT_DIR)/inc/

CTRL_DIR	= $(MODULES_DIR)/ctrl/
CTRL_INC_DIR	= $(CTRL_DIR)/inc/

IMG_DIR		= $(MODULES_DIR)/image/
IMG_INC_DIR	= $(IMG_DIR)/inc/

MENU_DIR	= $(MODULES_DIR)/menu/
MENU_INC_DIR	= $(MENU_DIR)/inc/

SAVE_DIR	= $(MODULES_DIR)/save/
SAVE_INC_DIR	= $(SAVE_DIR)/inc/

USR_DIR		= $(MODULES_DIR)/user/
USR_INC_DIR	= $(USR_DIR)/inc/

SRC_DIR		= $(MAIN_DIR)/src/

# dependencies

ALL	= main.o

MAIN_INC_LIBALX	= alx_ncur.hpp
MAIN_INC_ABOUT	= about.hpp
MAIN_INC_IMG	= img_iface.hpp
MAIN_INC_MENU	= parser.hpp menu_iface.hpp
MAIN_INC_SAVE	= save.hpp
MAIN_INC_USR	= user_iface.hpp
MAIN_DEPS	= $(SRC_DIR)/main.cpp \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(MAIN_INC_LIBALX)) \
			$(patsubst %,$(ABOUT_INC_DIR)/%,$(MAIN_INC_ABOUT)) \
			$(patsubst %,$(IMG_INC_DIR)/%,$(MAIN_INC_IMG)) \
			$(patsubst %,$(MENU_INC_DIR)/%,$(MAIN_INC_MENU)) \
			$(patsubst %,$(SAVE_INC_DIR)/%,$(MAIN_INC_SAVE)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(MAIN_INC_USR))
MAIN_INC_DIRS	= -I $(LIBALX_INC_DIR) \
			-I $(ABOUT_INC_DIR) \
			-I $(IMG_INC_DIR) \
			-I $(MENU_INC_DIR) \
			-I $(SAVE_INC_DIR) \
			-I $(USR_INC_DIR)


# target: dependencies
#	action

PHONY := all
all: $(ALL)

main.s: $(MAIN_DEPS)
	@echo	"	CXX	$@"
	$(Q)$(CXX) $(CXXFLAGS) $(MAIN_INC_DIRS) -S $< -o $@
main.o: main.s
	@echo	"	AS	$@"
	$(Q)$(AS) $< -o $@


PHONY += clean
clean:
	@echo	"	RM	*.o *.s"
	$(Q)rm -f *.o *.s

################################################################################
# Declare the contents of the .PHONY variable as phony.
.PHONY: $(PHONY)




################################################################################
######## End of file ###########################################################
################################################################################
