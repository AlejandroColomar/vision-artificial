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
		start.o

START_INC	=							\
		$(INC_DIR)/vision-artificial/ctrl/start.h		\
		$(INC_DIR)/vision-artificial/proc/iface.h		\
		$(INC_DIR)/vision-artificial/save/save.h		\
		$(INC_DIR)/vision-artificial/user/iface.h
START_SRC	=							\
		$(SRC_DIR)/ctrl/start.c

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(OBJ)


start.s: $(START_SRC) $(START_INC)
	@echo	"	CC	vision-artificial/ctrl/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
start.o: start.s
	@echo	"	AS	vision-artificial/ctrl/$@"
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
