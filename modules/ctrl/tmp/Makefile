# -*- MakeFile -*-

# MACRO = substitute with this

# directories

IMG_INC_DIR = $(IMG_DIR)/inc/
USR_INC_DIR = $(USR_DIR)/inc/

INC_DIR = $(CTRL_DIR)/inc/
SRC_DIR = $(CTRL_DIR)/src/

# dependencies

_ALL	= start.o
ALL	= $(_ALL) ctrl_mod.o

STRT_INC_IMG	= img_iface.h
STRT_INC_USR	= user_iface.h
STRT_INC	= start.h
STRT_DEPS	= $(SRC_DIR)/start.c \
			$(patsubst %,$(IMG_INC_DIR)/%,$(STRT_INC_IMG)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(STRT_INC_USR)) \
			$(patsubst %,$(INC_DIR)/%,$(STRT_INC))
STRT_INC_DIRS = -I $(INC_DIR) \
			-I $(IMG_INC_DIR) \
			-I $(USR_INC_DIR)

# target: dependencies
#	action

all: $(ALL)
	

ctrl_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"
	@echo  ""


start.s: $(STRT_DEPS)
	$(Q)$(CC) $(CFLAGS) $(STRT_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
start.o: start.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s
