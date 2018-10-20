# -*- MakeFile -*-

# MACRO = substitute with this

# directories

IMG_INC_DIR	= $(IMG_DIR)/inc/
USR_INC_DIR	= $(USR_DIR)/inc/

INC_DIR	= $(PROC_DIR)/inc/
SRC_DIR	= $(PROC_DIR)/src/

# dependencies

_ALL	= proc.o
ALL	= $(_ALL) proc_mod.o

PROC_INC_IMG	= img_iface.hpp
PROC_INC_USER	= user_iface.hpp
PROC_INC	= proc.hpp
PROC_DEPS	= $(SRC_DIR)/proc.cpp \
			$(patsubst %,$(INC_DIR)/%,$(PROC_INC)) \
			$(patsubst %,$(IMG_INC_DIR)/%,$(PROC_INC_IMG)) \
			$(patsubst %,$(USR_INC_DIR)/%,$(PROC_INC_USR))
PROC_INC_DIRS	= -I $(INC_DIR) \
			-I $(IMG_INC_DIR) \
			-I $(USR_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


proc_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"
	@echo  ""


proc.s: $(PROC_DEPS)
	$(Q)$(CXX) $(CXXFLAGS) $(PROC_INC_DIRS) -S $< -o $@
	@echo  "	CXX	$@"
proc.o: proc.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s
