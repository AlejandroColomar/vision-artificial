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
		about.o

ABOUT_INC	=							\
		$(INC_DIR)/vision-artificial/about/about.h
ABOUT_SRC	=							\
		$(SRC_DIR)/about/about.c

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(OBJ)


about.s: $(ABOUT_SRC) $(ABOUT_INC)
	@echo	"	CC	vision-artificial/about/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
about.o: about.s
	@echo	"	AS	vision-artificial/about/$@"
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
