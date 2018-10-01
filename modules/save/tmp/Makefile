# -*- MakeFile -*-

# MACRO = substitute with this

# directories

USR_INC_DIR	= $(USR_DIR)/inc/

INC_DIR	= $(SAVE_DIR)/inc/
SRC_DIR	= $(SAVE_DIR)/src/

# dependencies

_ALL	= save.o
ALL	= $(_ALL) save_mod.o

SAVE_INC_VIEW	= user_iface.h
SAVE_INC	= save.h
SAVE_DEPS	= $(SRC_DIR)/save.c \
			$(patsubst %,$(INC_DIR)/%,$(SAVE_INC)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(SAVE_INC_USR))
SAVE_INC_DIRS	= -I $(INC_DIR) \
			-I $(USR_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


save_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"
	@echo  ""
	

save.s: $(SAVE_DEPS)
	$(Q)$(CC) $(CFLAGS) $(SAVE_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
save.o: save.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s
