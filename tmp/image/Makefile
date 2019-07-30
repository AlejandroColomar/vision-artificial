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
		alx.o							\
		calib3d.o						\
		cv.o							\
		iface.o							\
		ocr.o							\
		orb.o							\
		zbar.o

ALX_INC		=							\
		$(INC_DIR)/vision-artificial/image/alx.hpp		\
		$(LIBALX_INC_DIR)/libalx/extra/gsl/rstat/median.hpp	\
		$(LIBALX_INC_DIR)/libalx/extra/gsl/statistics/mean.hpp	\
		$(INC_DIR)/vision-artificial/image/iface.hpp
ALX_SRC		=							\
		$(SRC_DIR)/image/alx.cpp

CALIB3D_INC	=							\
		$(INC_DIR)/vision-artificial/image/calib3d.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp
CALIB3D_SRC	=							\
		$(SRC_DIR)/image/calib3d.cpp

CV_INC		=							\
		$(INC_DIR)/vision-artificial/image/cv.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp
CV_SRC		=							\
		$(SRC_DIR)/image/cv.cpp

IFACE_INC	=							\
		$(INC_DIR)/vision-artificial/image/iface.hpp		\
		$(INC_DIR)/vision-artificial/image/alx.hpp		\
		$(INC_DIR)/vision-artificial/image/calib3d.hpp		\
		$(INC_DIR)/vision-artificial/image/cv.hpp		\
		$(INC_DIR)/vision-artificial/image/ocr.hpp		\
		$(INC_DIR)/vision-artificial/image/orb.hpp		\
		$(INC_DIR)/vision-artificial/image/zbar.hpp		\
		$(INC_DIR)/vision-artificial/save/save.hpp		\
		$(INC_DIR)/vision-artificial/user/iface.hpp
IFACE_SRC	=							\
		$(SRC_DIR)/image/iface.cpp

OCR_INC		=							\
		$(INC_DIR)/vision-artificial/image/ocr.h		\
		$(LIBALX_INC_DIR)/libalx/base/errno/error.hpp		\
		$(LIBALX_INC_DIR)/libalx/base/compiler/size.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.h		\
		$(INC_DIR)/vision-artificial/share/share.h
OCR_SRC		=							\
		$(SRC_DIR)/image/ocr.c

ORB_INC		=							\
		$(INC_DIR)/vision-artificial/image/orb.hpp
ORB_SRC		=							\
		$(SRC_DIR)/image/orb.cpp

ZBAR_INC	=							\
		$(INC_DIR)/vision-artificial/image/zbar.hpp		\
		$(INC_DIR)/vision-artificial/image/iface.hpp
ZBAR_SRC	=							\
		$(SRC_DIR)/image/zbar.cpp

################################################################################
# target: dependencies
#	action

PHONY := all
all: $(OBJ)


alx.s: $(ALX_SRC) $(ALX_INC)
	@echo	"	CXX	vision-artificial/image/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
alx.o: alx.s
	@echo	"	AS	vision-artificial/image/$@"
	$(Q)$(AS) $< -o $@

calib3d.s: $(CALIB3D_SRC) $(CALIB3D_INC)
	@echo	"	CXX	vision-artificial/image/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
calib3d.o: calib3d.s
	@echo	"	AS	vision-artificial/image/$@"
	$(Q)$(AS) $< -o $@

cv.s: $(CV_SRC) $(CV_INC)
	@echo	"	CXX	vision-artificial/image/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
cv.o: cv.s
	@echo	"	AS	vision-artificial/image/$@"
	$(Q)$(AS) $< -o $@

iface.s: $(IFACE_SRC) $(IFACE_INC)
	@echo	"	CXX	vision-artificial/image/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
iface.o: iface.s
	@echo	"	AS	vision-artificial/image/$@"
	$(Q)$(AS) $< -o $@

ocr.s: $(OCR_SRC) $(OCR_INC)
	@echo	"	CC	vision-artificial/image/$@"
	$(Q)$(CC) $(CFLAGS) -I $(INC_DIR) -S $< -o $@
ocr.o: ocr.s
	@echo	"	AS	vision-artificial/image/$@"
	$(Q)$(AS) $< -o $@

orb.s: $(ORB_SRC) $(ORB_INC)
	@echo	"	CXX	vision-artificial/image/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
orb.o: orb.s
	@echo	"	AS	vision-artificial/image/$@"
	$(Q)$(AS) $< -o $@

zbar.s: $(ZBAR_SRC) $(ZBAR_INC)
	@echo	"	CXX	vision-artificial/image/$@"
	$(Q)$(CXX) $(CXXFLAGS) -I $(INC_DIR) -S $< -o $@
zbar.o: zbar.s
	@echo	"	AS	vision-artificial/image/$@"
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
