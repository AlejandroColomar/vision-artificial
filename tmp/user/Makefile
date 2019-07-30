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
		clui.o							\
		iface.o							\
		tui.o

CLUI_INC	=							\
		$(INC_DIR)/vision-artificial/user/clui.h		\
		$(LIBALX_INC_DIR)/libalx/base/stdlib/swap.h		\
		$(LIBALX_INC_DIR)/libalx/base/stdio/get.h		\
		$(LIBALX_INC_DIR)/libalx/base/stdio/seekc.h		\
		$(INC_DIR)/vision-artificial/image/iface.h		\
		$(INC_DIR)/vision-artificial/user/iface.h
CLUI_SRC	=							\
		$(SRC_DIR)/user/clui.c

IFACE_INC	=							\
		$(INC_DIR)/vision-artificial/user/iface.h		\
		$(LIBALX_INC_DIR)/libalx/base/stdio/get.h		\
		$(INC_DIR)/vision-artificial/image/iface.h		\
		$(INC_DIR)/vision-artificial/proc/iface.h		\
		$(INC_DIR)/vision-artificial/user/clui.h		\
		$(INC_DIR)/vision-artificial/user/tui.h
IFACE_SRC	=							\
		$(SRC_DIR)/user/iface.c

TUI_INC	=								\
		$(INC_DIR)/vision-artificial/user/tui.h			\
		$(LIBALX_INC_DIR)/libalx/base/stdio/seekc.h		\
		$(LIBALX_INC_DIR)/libalx/extra/ncurses/common.h		\
		$(LIBALX_INC_DIR)/libalx/extra/ncurses/get.h		\
		$(INC_DIR)/vision-artificial/image/iface.h		\
		$(INC_DIR)/vision-artificial/user/iface.h
TUI_SRC	=								\
		$(SRC_DIR)/user/tui.c

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(OBJ)


clui.s: $(CLUI_SRC) $(CLUI_INC)
	@echo	"	CC	vision-artificial/user/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
clui.o: clui.s
	@echo	"	AS	vision-artificial/user/$@"
	$(Q)$(AS) $< -o $@

iface.s: $(IFACE_SRC) $(IFACE_INC)
	@echo	"	CC	vision-artificial/user/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
iface.o: iface.s
	@echo	"	AS	vision-artificial/user/$@"
	$(Q)$(AS) $< -o $@

tui.s: $(TUI_SRC) $(TUI_INC)
	@echo	"	CC	vision-artificial/user/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
tui.o: tui.s
	@echo	"	AS	vision-artificial/user/$@"
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
