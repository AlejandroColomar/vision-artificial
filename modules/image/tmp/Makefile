# -*- MakeFile -*-

# MACRO = substitute with this

# directories

LIBALX_INC_DIR = $(LIBALX_DIR)/inc/

ABOUT_INC_DIR = $(ABOUT_DIR)/inc/
SAVE_INC_DIR = $(SAVE_DIR)/inc/
USR_INC_DIR = $(USR_DIR)/inc/

INC_DIR = $(IMG_DIR)/inc/
SRC_DIR = $(IMG_DIR)/src/

# dependencies

_ALL	= img_cv.o img_zbar.o img_ocr.o img_orb.o img_iface.o
ALL	= $(_ALL) img_mod.o

IMGCV_INC_USR		= user_iface.h
IMGCV_INC		= img_cv.h	img_iface.h
IMGCV_DEPS		= $(SRC_DIR)/img_cv.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGCV_INC)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGCV_INC_USR))
IMGCV_INC_DIRS		= -I $(INC_DIR) \
			-I $(USR_INC_DIR) \
			-I $(LIBALX_INC_DIR)

IMGZB_INC_USR		= user_iface.h
IMGZB_INC		= img_zbar.h	img_iface.h
IMGZB_DEPS		= $(SRC_DIR)/img_zbar.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGZB_INC)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGZB_INC_USR))
IMGZB_INC_DIRS		= -I $(INC_DIR) \
			-I $(USR_INC_DIR) \
			-I $(LIBALX_INC_DIR)

IMGOCR_INC_USR		= user_iface.h
IMGOCR_INC_ABOUT	= about.h
IMGOCR_INC		= img_ocr.h	img_iface.h
IMGOCR_DEPS		= $(SRC_DIR)/img_ocr.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGOCR_INC)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGOCR_INC_USR)) \
			$(patsubst %,$(ABOUT_INC_DIR)/%,$(IMGOCR_INC_ABOUT))
IMGOCR_INC_DIRS		= -I $(INC_DIR) \
			-I $(USR_INC_DIR) \
			-I $(ABOUT_INC_DIR) \
			-I $(LIBALX_INC_DIR)

IMGORB_INC_USR		= user_iface.hpp
IMGORB_INC		= img_orb.hpp	img_iface.h
IMGORB_DEPS		= $(SRC_DIR)/img_orb.cpp \
			$(patsubst %,$(INC_DIR)/%,$(IMGORB_INC)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGORB_INC_USR))
IMGORB_INC_DIRS		= -I $(INC_DIR) \
			-I $(USR_INC_DIR) \
			-I $(LIBALX_INC_DIR)

IMGI_INC_SAVE		= save.h
IMGI_INC_USR		= user_iface.h
IMGI_INC		= img_iface.h	img_cv.h img_zbar.h img_ocr.h img_orb.h
IMGI_DEPS		= $(SRC_DIR)/img_iface.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGI_INC)) \
			$(patsubst %,$(SAVE_INC_DIR)/%,$(IMGI_INC_SAVE)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGI_INC_USR))
IMGI_INC_DIRS		= -I $(INC_DIR) \
			-I $(SAVE_INC_DIR) \
			-I $(USR_INC_DIR)

# target: dependencies
#	action

all: $(ALL)
	

img_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"
	@echo  ""


img_cv.s: $(IMGCV_DEPS)
	$(Q)$(CC) $(CFLAGS) $(IMGCV_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
img_cv.o: img_cv.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

img_zbar.s: $(IMGZB_DEPS)
	$(Q)$(CC) $(CFLAGS) $(IMGZB_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
img_zbar.o: img_zbar.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

img_ocr.s: $(IMGOCR_DEPS)
	$(Q)$(CC) $(CFLAGS) $(IMGOCR_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
img_ocr.o: img_ocr.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

img_orb.s: $(IMGORB_DEPS)
	$(Q)$(CXX) $(CXXFLAGS) $(IMGORB_INC_DIRS) -S $< -o $@
	@echo  "	CXX	$@"
img_orb.o: img_orb.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

img_iface.s: $(IMGI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(IMGI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
img_iface.o: img_iface.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s
