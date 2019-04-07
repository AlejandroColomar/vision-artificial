#! /usr/bin/make -f

################################################################################
# Copyright (C) 2019	Alejandro Colomar Andrés
# SPDX-License-Identifier:	GPL-2.0-only
################################################################################
# *AUTHOR*
# FULL NAME	"Alejandro Colomar Andrés"
# EMAIL		"1903716@gmail.com"
################################################################################

################################################################################
# dependencies

OBJ		=							\
		main.o

MAIN_INC	=							\
		$(LIBALX_INC_DIR)/libalx/base/stdio/wait.hpp		\
		$(LIBALX_INC_DIR)/libalx/extra/ncurses/common.hpp	\
		$(INC_DIR)/vision-artificial/about/about.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/menu/iface.hpp		\
		$(INC_DIR)/vision-artificial/menu/parse.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp		\
		$(INC_DIR)/vision-artificial/save/save.hpp
MAIN_SRC	=							\
		$(SRC_DIR)/main.cpp


# target: dependencies
#	action

PHONY := all
all: $(OBJ) about ctrl image menu proc save user


PHONY += about
about:
	@echo	"	MAKE	tmp/about"
	$(Q)$(MAKE)	-C $(TMP_DIR)/about/

PHONY += ctrl
ctrl:
	@echo	"	MAKE	tmp/ctrl"
	$(Q)$(MAKE)	-C $(TMP_DIR)/ctrl/

PHONY += image
image:
	@echo	"	MAKE	tmp/image"
	$(Q)$(MAKE)	-C $(TMP_DIR)/image/

PHONY += menu
menu:
	@echo	"	MAKE	tmp/menu"
	$(Q)$(MAKE)	-C $(TMP_DIR)/menu/

PHONY += proc
proc:
	@echo	"	MAKE	tmp/proc"
	$(Q)$(MAKE)	-C $(TMP_DIR)/proc/

PHONY += save
save:
	@echo	"	MAKE	tmp/save"
	$(Q)$(MAKE)	-C $(TMP_DIR)/save/

PHONY += user
user:
	@echo	"	MAKE	tmp/user"
	$(Q)$(MAKE)	-C $(TMP_DIR)/user/


main.s: $(MAIN_SRC) $(MAIN_INC)
	@echo	"	CXX	vision-artificial/tmp/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
main.o: main.s
	@echo	"	AS	vision-artificial/tmp/$@"
	$(Q)$(AS) $< -o $@


PHONY += clean
clean:
	@echo	"	RM	*.o *.s"
	$(Q)rm -f	*.o *.s

################################################################################
# Declare the contents of the .PHONY variable as phony.
.PHONY: $(PHONY)




################################################################################
######## End of file ###########################################################
################################################################################
