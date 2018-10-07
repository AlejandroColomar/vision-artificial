# -*- MakeFile -*-

# MACRO = substitute with this

# directories

LIBALX_INC_DIR = $(LIBALX_DIR)/inc/

SAVE_INC_DIR = $(SAVE_DIR)/inc/
USR_INC_DIR = $(USR_DIR)/inc/

INC_DIR = $(IMG_DIR)/inc/
SRC_DIR = $(IMG_DIR)/src/

# dependencies

_ALL	= img_cv.o img_zbar.o img_iface.o
ALL	= $(_ALL) img_mod.o

IMGCV_INC_USR	= user_iface.h
IMGCV_INC	= img_cv.h
IMGCV_DEPS	= $(SRC_DIR)/img_cv.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGCV_INC)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGCV_INC_USR))
IMGCV_INC_DIRS	= -I $(INC_DIR) \
			-I $(USR_INC_DIR) \
			-I $(LIBALX_INC_DIR)

IMGZB_INC_USR	= user_iface.h
IMGZB_INC	= img_zbar.h
IMGZB_DEPS	= $(SRC_DIR)/img_zbar.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGZB_INC)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGZB_INC_USR))
IMGZB_INC_DIRS	= -I $(INC_DIR) \
			-I $(USR_INC_DIR) \
			-I $(LIBALX_INC_DIR)

IMGI_INC_SAVE	= save.h
IMGI_INC_USR	= user_iface.h
IMGI_INC	= img_iface.h	img_cv.h img_zbar.h
IMGI_DEPS	= $(SRC_DIR)/img_iface.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGI_INC)) \
			$(patsubst %,$(SAVE_INC_DIR)/%,$(IMGI_INC_SAVE)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(IMGI_INC_USR))
IMGI_INC_DIRS = -I $(INC_DIR) \
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

img_iface.s: $(IMGI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(IMGI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
img_iface.o: img_iface.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s
