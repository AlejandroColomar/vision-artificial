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
		iface.o							\
		parse.o							\
		tui.o

IFACE_INC	=							\
		$(INC_DIR)/vision-artificial/menu/iface.h		\
		$(INC_DIR)/vision-artificial/menu/tui.h
IFACE_SRC	=							\
		$(SRC_DIR)/menu/iface.c

PARSE_INC	=							\
		$(INC_DIR)/vision-artificial/menu/parse.h		\
		$(LIBALX_INC_DIR)/libalx/base/stdio/sscan.h		\
		$(INC_DIR)/vision-artificial/menu/iface.h		\
		$(INC_DIR)/vision-artificial/save/save.h		\
		$(INC_DIR)/vision-artificial/share/share.h		\
		$(INC_DIR)/vision-artificial/user/iface.h
PARSE_SRC	=							\
		$(SRC_DIR)/menu/parse.c

TUI_INC	=								\
		$(INC_DIR)/vision-artificial/menu/tui.h			\
		$(LIBALX_INC_DIR)/libalx/base/compiler/size.h		\
		$(LIBALX_INC_DIR)/libalx/base/stdio/seekc.h		\
		$(LIBALX_INC_DIR)/libalx/extra/ncurses/common.h		\
		$(LIBALX_INC_DIR)/libalx/extra/ncurses/get.h		\
		$(LIBALX_INC_DIR)/libalx/extra/ncurses/menu.h		\
		$(INC_DIR)/vision-artificial/ctrl/start.h		\
		$(INC_DIR)/vision-artificial/proc/iface.h		\
		$(INC_DIR)/vision-artificial/save/save.h		\
		$(INC_DIR)/vision-artificial/share/share.h		\
		$(INC_DIR)/vision-artificial/user/iface.h
TUI_SRC	=								\
		$(SRC_DIR)/menu/tui.c

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(OBJ)


iface.s: $(IFACE_SRC) $(IFACE_INC)
	@echo	"	CXX	vision-artificial/menu/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
iface.o: iface.s
	@echo	"	AS	vision-artificial/menu/$@"
	$(Q)$(AS) $< -o $@

parse.s: $(PARSE_SRC) $(PARSE_INC)
	@echo	"	CC	vision-artificial/menu/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
parse.o: parse.s
	@echo	"	AS	vision-artificial/menu/$@"
	$(Q)$(AS) $< -o $@

tui.s: $(TUI_SRC) $(TUI_INC)
	@echo	"	CC	vision-artificial/menu/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
tui.o: tui.s
	@echo	"	AS	vision-artificial/menu/$@"
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
