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
		$(LIBALX_INC_DIR)/libalx/base/stdio/seekc.hpp		\
		$(LIBALX_INC_DIR)/libalx/extra/ncurses/common.hpp	\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/menu/iface.hpp		\
		$(INC_DIR)/vision-artificial/menu/parse.hpp		\
		$(INC_DIR)/vision-artificial/save/save.hpp		\
		$(INC_DIR)/vision-artificial/share/share.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
MAIN_SRC	=							\
		$(SRC_DIR)/main.cpp


# target: dependencies
#	action

PHONY := all
all: $(OBJ) share ctrl image menu proc save user


PHONY += share
share:
	@echo	"	MAKE	tmp/$@"
	$(Q)$(MAKE)	-C $(TMP_DIR)/$@/

PHONY += ctrl
ctrl:
	@echo	"	MAKE	tmp/$@"
	$(Q)$(MAKE)	-C $(TMP_DIR)/$@/

PHONY += image
image:
	@echo	"	MAKE	tmp/$@"
	$(Q)$(MAKE)	-C $(TMP_DIR)/$@/

PHONY += menu
menu:
	@echo	"	MAKE	tmp/$@"
	$(Q)$(MAKE)	-C $(TMP_DIR)/$@/

PHONY += proc
proc:
	@echo	"	MAKE	tmp/$@"
	$(Q)$(MAKE)	-C $(TMP_DIR)/$@/

PHONY += save
save:
	@echo	"	MAKE	tmp/$@"
	$(Q)$(MAKE)	-C $(TMP_DIR)/$@/

PHONY += user
user:
	@echo	"	MAKE	tmp/$@"
	$(Q)$(MAKE)	-C $(TMP_DIR)/$@/


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
