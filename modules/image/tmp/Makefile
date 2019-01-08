# -*- MakeFile -*-

# MACRO = substitute with this

# directories

LIBALX_INC_DIR	= $(LIBALX_DIR)/inc/

ABOUT_INC_DIR	= $(ABOUT_DIR)/inc/
SAVE_INC_DIR	= $(SAVE_DIR)/inc/
USR_INC_DIR	= $(USR_DIR)/inc/

INC_DIR		= $(IMG_DIR)/inc/
SRC_DIR		= $(IMG_DIR)/src/

# dependencies

_ALL			= img_alx.o img_cv.o img_orb.o img_calib3d.o \
			img_zbar.o img_ocr.o img_iface.o
ALL			= $(_ALL) img_mod.o

IMGALX_INC_LIBALX	= libalx/alx_math.hpp
IMGALX_INC		= img_alx.hpp	img_iface.hpp
IMGALX_DEPS		= $(SRC_DIR)/img_alx.cpp \
			$(patsubst %,$(INC_DIR)/%,$(IMGALX_INC)) \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(IMGALX_INC_LIBALX))
IMGALX_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR)

IMGCV_INC		= img_cv.hpp	img_iface.hpp
IMGCV_DEPS		= $(SRC_DIR)/img_cv.cpp \
			$(patsubst %,$(INC_DIR)/%,$(IMGCV_INC))
IMGCV_INC_DIRS		= -I $(INC_DIR)

IMGORB_INC		= img_orb.hpp	img_iface.hpp
IMGORB_DEPS		= $(SRC_DIR)/img_orb.cpp \
			$(patsubst %,$(INC_DIR)/%,$(IMGORB_INC))
IMGORB_INC_DIRS		= -I $(INC_DIR)

IMGC3D_INC		= img_calib3d.hpp	img_iface.hpp
IMGC3D_DEPS		= $(SRC_DIR)/img_calib3d.cpp \
			$(patsubst %,$(INC_DIR)/%,$(IMGC3D_INC))
IMGC3D_INC_DIRS		= -I $(INC_DIR)

IMGZB_INC		= img_zbar.hpp	img_iface.hpp
IMGZB_DEPS		= $(SRC_DIR)/img_zbar.cpp \
			$(patsubst %,$(INC_DIR)/%,$(IMGZB_INC))
IMGZB_INC_DIRS		= -I $(INC_DIR)

IMGOCR_INC_ABOUT	= about.h
IMGOCR_INC		= img_ocr.h	img_iface.h
IMGOCR_DEPS		= $(SRC_DIR)/img_ocr.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGOCR_INC)) \
			$(patsubst %,$(ABOUT_INC_DIR)/%,$(IMGOCR_INC_ABOUT))
IMGOCR_INC_DIRS		= -I $(INC_DIR) \
			-I $(ABOUT_INC_DIR)

IMGI_INC_SAVE		= save.hpp
IMGI_INC_USR		= user_iface.hpp
IMGI_INC		= img_iface.hpp	img_alx.hpp img_calib3d.hpp img_cv.hpp \
					img_zbar.hpp img_ocr.hpp img_orb.hpp
IMGI_DEPS		= $(SRC_DIR)/img_iface.cpp \
			$(patsubst %,$(INC_DIR)/%,$(IMGI_INC)) \
			$(patsubst %,$(SAVE_INC_DIR)/%,$(IMGI_INC_SAVE)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGI_INC_USR))
IMGI_INC_DIRS		= -I $(INC_DIR) \
			-I $(SAVE_INC_DIR) \
			-I $(USR_INC_DIR)

# target: dependencies
#	action

PHONY := all
all: $(ALL)


img_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"


img_alx.s: $(IMGALX_DEPS)
	@echo  "	CXX	$@"
	$(Q)$(CXX) $(CXXFLAGS) $(IMGALX_INC_DIRS) -S $< -o $@
img_alx.o: img_alx.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@

img_cv.s: $(IMGCV_DEPS)
	@echo  "	CXX	$@"
	$(Q)$(CXX) $(CXXFLAGS) $(IMGCV_INC_DIRS) -S $< -o $@
img_cv.o: img_cv.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@

img_orb.s: $(IMGORB_DEPS)
	@echo  "	CXX	$@"
	$(Q)$(CXX) $(CXXFLAGS) $(IMGORB_INC_DIRS) -S $< -o $@
img_orb.o: img_orb.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@

img_calib3d.s: $(IMGC3D_DEPS)
	@echo  "	CXX	$@"
	$(Q)$(CXX) $(CXXFLAGS) $(IMGC3D_INC_DIRS) -S $< -o $@
img_calib3d.o: img_calib3d.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@

img_zbar.s: $(IMGZB_DEPS)
	@echo  "	CXX	$@"
	$(Q)$(CXX) $(CXXFLAGS) $(IMGZB_INC_DIRS) -S $< -o $@
img_zbar.o: img_zbar.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@

img_ocr.s: $(IMGOCR_DEPS)
	@echo  "	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(IMGOCR_INC_DIRS) -S $< -o $@
img_ocr.o: img_ocr.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@

img_iface.s: $(IMGI_DEPS)
	@echo  "	CXX	$@"
	$(Q)$(CXX) $(CXXFLAGS) $(IMGI_INC_DIRS) -S $< -o $@
img_iface.o: img_iface.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@


PHONY += clean
clean:
	$(Q)rm -f *.o *.s

################################################################################
# Declare the contents of the .PHONY variable as phony.
.PHONY: $(PHONY)




################################################################################
######## End of file ###########################################################
################################################################################

################################################################################
######## End of file ###########################################################
################################################################################
