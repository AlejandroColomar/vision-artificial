# -*- MakeFile -*-

# MACRO = substitute with this

# directories

LIBALX_INC_DIR = $(LIBALX_DIR)/inc/

SAVE_INC_DIR = $(SAVE_DIR)/inc/
USR_INC_DIR = $(USR_DIR)/inc/

INC_DIR = $(IMG_DIR)/inc/
SRC_DIR = $(IMG_DIR)/src/

# dependencies

_ALL	= image.o img_iface.o
ALL	= $(_ALL) img_mod.o

IMG_INC_SAVE	= save.h
IMG_INC		= image.h
IMG_DEPS	= $(SRC_DIR)/image.c \
			$(patsubst %,$(INC_DIR)/%,$(IMG_INC)) \
			$(patsubst %,$(SAVE_INC_DIR)/%,$(IMG_INC_SAVE))
IMG_INC_DIRS	= -I $(INC_DIR) \
			-I $(SAVE_INC_DIR) \
			-I $(LIBALX_INC_DIR)

IMGI_INC	= img_iface.h	image.h
IMGI_DEPS	= $(SRC_DIR)/img_iface.c \
			$(patsubst %,$(INC_DIR)/%,$(IMGI_INC))
IMGI_INC_DIRS = -I $(INC_DIR)

# target: dependencies
#	action

all: $(ALL)
	

img_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"
	@echo  ""


image.s: $(IMG_DEPS)
	$(Q)$(CC) $(CFLAGS) $(IMG_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
image.o: image.s
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
