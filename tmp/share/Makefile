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
		share.o

SHARE_INC	=							\
		$(INC_DIR)/vision-artificial/share/share.h		\
		$(LIBALX_INC_DIR)/libalx/base/stddef/size.h
SHARE_SRC	=							\
		$(SRC_DIR)/share/share.c

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(OBJ)


share.s: $(SHARE_SRC) $(SHARE_INC)
	@echo	"	CC	vision-artificial/share/$@"
	$(Q)$(CC) $(CFLAGS) -S $< -o $@
share.o: share.s
	@echo	"	AS	vision-artificial/share/$@"
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
