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
		coins.o							\
		common.o						\
		iface.o							\
		label.o							\
		lighters.o						\
		objects.o						\
		resistor.o

COINS_INC	=							\
		$(INC_DIR)/vision-artificial/proc/coins.hpp		\
		$(LIBALX_INC_DIR)/libalx/base/math/arithmetic_mean.hpp	\
		$(LIBALX_INC_DIR)/libalx/base/stddef/size.hpp		\
		$(LIBALX_INC_DIR)/libalx/base/stdlib/maximum.hpp	\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/proc/common.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
COINS_SRC	=							\
		$(SRC_DIR)/proc/coins.cpp

COMMON_INC	=							\
		$(INC_DIR)/vision-artificial/proc/common.hpp		\
		$(LIBALX_INC_DIR)/libalx/base/stdio/wait.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/proc/iface.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
COMMON_SRC	=							\
		$(SRC_DIR)/proc/common.cpp

IFACE_INC	=							\
		$(INC_DIR)/vision-artificial/proc/iface.h		\
		$(LIBALX_INC_DIR)/libalx/base/stddef/size.h		\
		$(LIBALX_INC_DIR)/libalx/base/stdio/sscan.h		\
		$(LIBALX_INC_DIR)/libalx/base/stdio/wait.h		\
		$(INC_DIR)/vision-artificial/image/iface.h		\
		$(INC_DIR)/vision-artificial/proc/coins.h		\
		$(INC_DIR)/vision-artificial/proc/common.h		\
		$(INC_DIR)/vision-artificial/proc/label.h		\
		$(INC_DIR)/vision-artificial/proc/lighters.h		\
		$(INC_DIR)/vision-artificial/proc/objects.h		\
		$(INC_DIR)/vision-artificial/proc/resistor.h		\
		$(INC_DIR)/vision-artificial/save/save.h		\
		$(INC_DIR)/vision-artificial/user/iface.h
IFACE_SRC	=							\
		$(SRC_DIR)/proc/iface.c

LABEL_INC	=							\
		$(INC_DIR)/vision-artificial/proc/label.hpp		\
		$(LIBALX_INC_DIR)/libalx/base/stdlib/swap.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/proc/common.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
LABEL_SRC	=							\
		$(SRC_DIR)/proc/label.cpp

LIGHTERS_INC	=							\
		$(INC_DIR)/vision-artificial/proc/lighters.hpp		\
		$(LIBALX_INC_DIR)/libalx/base/stdlib/swap.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/proc/common.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
LIGHTERS_SRC	=							\
		$(SRC_DIR)/proc/lighters.cpp

OBJECTS_INC	=							\
		$(INC_DIR)/vision-artificial/proc/objects.hpp		\
		$(LIBALX_INC_DIR)/libalx/base/math/median.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/proc/common.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
OBJECTS_SRC	=							\
		$(SRC_DIR)/proc/objects.cpp

RESISTOR_INC	=							\
		$(INC_DIR)/vision-artificial/proc/resistor.hpp		\
		$(LIBALX_INC_DIR)/libalx/base/stddef/size.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/proc/common.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
RESISTOR_SRC	=							\
		$(SRC_DIR)/proc/resistor.cpp

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(OBJ)


coins.s: $(COINS_SRC) $(COINS_INC)
	@echo	"	CXX	vision-artificial/proc/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
coins.o: coins.s
	@echo	"	AS	vision-artificial/proc/$@"
	$(Q)$(AS) $< -o $@

common.s: $(COMMON_SRC) $(COMMON_INC)
	@echo	"	CXX	vision-artificial/proc/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
common.o: common.s
	@echo	"	AS	vision-artificial/proc/$@"
	$(Q)$(AS) $< -o $@

iface.s: $(IFACE_SRC) $(IFACE_INC)
	@echo	"	CC	vision-artificial/proc/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
iface.o: iface.s
	@echo	"	AS	vision-artificial/proc/$@"
	$(Q)$(AS) $< -o $@

label.s: $(LABEL_SRC) $(LABEL_INC)
	@echo	"	CXX	vision-artificial/proc/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
label.o: label.s
	@echo	"	AS	vision-artificial/proc/$@"
	$(Q)$(AS) $< -o $@

lighters.s: $(LIGHTERS_SRC) $(LIGHTERS_INC)
	@echo	"	CXX	vision-artificial/proc/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
lighters.o: lighters.s
	@echo	"	AS	vision-artificial/proc/$@"
	$(Q)$(AS) $< -o $@

objects.s: $(OBJECTS_SRC) $(OBJECTS_INC)
	@echo	"	CXX	vision-artificial/proc/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
objects.o: objects.s
	@echo	"	AS	vision-artificial/proc/$@"
	$(Q)$(AS) $< -o $@

resistor.s: $(RESISTOR_SRC) $(RESISTOR_INC)
	@echo	"	CXX	vision-artificial/proc/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
resistor.o: resistor.s
	@echo	"	AS	vision-artificial/proc/$@"
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
