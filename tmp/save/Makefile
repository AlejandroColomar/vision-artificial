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
		save.o

SAVE_INC	=							\
		$(INC_DIR)/vision-artificial/save/save.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
SAVE_SRC	=							\
		$(SRC_DIR)/save/save.cpp

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(OBJ)


save.s: $(SAVE_SRC) $(SAVE_INC)
	@echo	"	CC	vision-artificial/save/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
save.o: save.s
	@echo	"	AS	vision-artificial/save/$@"
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
