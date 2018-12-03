# -*- MakeFile -*-

# MACRO = substitute with this

# directories

LIBALX_INC_DIR	= $(LIBALX_DIR)/inc/

IMG_INC_DIR	= $(IMG_DIR)/inc/
SAVE_INC_DIR	= $(SAVE_DIR)/inc/
USR_INC_DIR	= $(USR_DIR)/inc/

INC_DIR		= $(PROC_DIR)/inc/
SRC_DIR		= $(PROC_DIR)/src/

# dependencies

_ALL	= proc_label.o proc_coins.o proc_resistor.o proc_common.o proc_iface.o
ALL	= $(_ALL) proc_mod.o

PROC_LAB_INC_IMG	= img_iface.hpp
PROC_LAB_INC_USR	= user_iface.hpp
PROC_LAB_INC		= proc_label.hpp	proc_common.hpp
PROC_LAB_DEPS		= $(SRC_DIR)/proc_label.cpp \
				$(patsubst %,$(INC_DIR)/%,$(PROC_LAB_INC)) \
				$(patsubst %,$(IMG_INC_DIR)/%,$(PROC_LAB_INC_IMG)) \
				$(patsubst %,$(USR_INC_DIR)/%,$(PROC_LAB_INC_USR))
PROC_LAB_INC_DIRS	= -I $(INC_DIR) \
				-I $(IMG_INC_DIR) \
				-I $(USR_INC_DIR)

PROC_COIN_INC_LIBALX	= alx_math.hpp
PROC_COIN_INC_IMG	= img_iface.hpp
PROC_COIN_INC_USR	= user_iface.hpp
PROC_COIN_INC		= proc_coins.hpp	proc_common.hpp
PROC_COIN_DEPS		= $(SRC_DIR)/proc_coins.cpp \
				$(patsubst %,$(INC_DIR)/%,$(PROC_COIN_INC)) \
				$(patsubst %,$(IMG_INC_DIR)/%,$(PROC_COIN_INC_IMG)) \
				$(patsubst %,$(USR_INC_DIR)/%,$(PROC_COIN_INC_USR)) \
				$(patsubst %,$(LIBALX_INC_DIR)/%,$(PROC_COIN_INC_LIBALX))
PROC_COIN_INC_DIRS	= -I $(INC_DIR) \
				-I $(IMG_INC_DIR) \
				-I $(USR_INC_DIR) \
				-I $(LIBALX_INC_DIR)

PROC_RES_INC_IMG	= img_iface.hpp
PROC_RES_INC_USR	= user_iface.hpp
PROC_RES_INC		= proc_resistor.hpp	proc_common.hpp
PROC_RES_DEPS		= $(SRC_DIR)/proc_resistor.cpp \
				$(patsubst %,$(INC_DIR)/%,$(PROC_RES_INC)) \
				$(patsubst %,$(IMG_INC_DIR)/%,$(PROC_RES_INC_IMG)) \
				$(patsubst %,$(USR_INC_DIR)/%,$(PROC_RES_INC_USR))
PROC_RES_INC_DIRS	= -I $(INC_DIR) \
				-I $(IMG_INC_DIR) \
				-I $(USR_INC_DIR)

PROC_CMN_INC_IMG	= img_iface.hpp
PROC_CMN_INC_USR	= user_iface.hpp
PROC_CMN_INC		= proc_common.hpp
PROC_CMN_DEPS		= $(SRC_DIR)/proc_common.cpp \
				$(patsubst %,$(INC_DIR)/%,$(PROC_CMN_INC)) \
				$(patsubst %,$(IMG_INC_DIR)/%,$(PROC_CMN_INC_IMG)) \
				$(patsubst %,$(USR_INC_DIR)/%,$(PROC_CMN_INC_USR))
PROC_CMN_INC_DIRS	= -I $(INC_DIR) \
				-I $(IMG_INC_DIR) \
				-I $(USR_INC_DIR)

PROC_IFACE_INC_LIBALX	= alx_input.h
PROC_IFACE_INC_IMG	= img_iface.h
PROC_IFACE_INC_SAVE	= save.h
PROC_IFACE_INC_USR	= user_iface.h
PROC_IFACE_INC		= proc_iface.h	proc_label.h proc_coins.h proc_resistor.h
PROC_IFACE_DEPS		= $(SRC_DIR)/proc_iface.c \
				$(patsubst %,$(INC_DIR)/%,$(PROC_IFACE_INC)) \
				$(patsubst %,$(IMG_INC_DIR)/%,$(PROC_IFACE_INC_IMG)) \
				$(patsubst %,$(SAVE_INC_DIR)/%,$(PROC_IFACE_INC_SAVE)) \
				$(patsubst %,$(USR_INC_DIR)/%,$(PROC_IFACE_INC_USR)) \
				$(patsubst %,$(LIBALX_INC_DIR)/%,$(PROC_IFACE_INC_LIBALX))
PROC_IFACE_INC_DIRS	= -I $(INC_DIR) \
				-I $(IMG_INC_DIR) \
				-I $(SAVE_INC_DIR) \
				-I $(USR_INC_DIR) \
				-I $(LIBALX_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


proc_mod.o: $(_ALL)
	$(Q)$(LD) -r $^ -o $@
	@echo  "	LD	$@"


proc_label.s: $(PROC_LAB_DEPS)
	$(Q)$(CXX) $(CXXFLAGS) $(PROC_LAB_INC_DIRS) -S $< -o $@
	@echo  "	CXX	$@"
proc_label.o: proc_label.s $(PROC_LAB_DEPS)
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

proc_coins.s: $(PROC_COIN_DEPS)
	$(Q)$(CXX) $(CXXFLAGS) $(PROC_COIN_INC_DIRS) -S $< -o $@
	@echo  "	CXX	$@"
proc_coins.o: proc_coins.s $(PROC_COIN_DEPS)
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

proc_resistor.s: $(PROC_RES_DEPS)
	$(Q)$(CXX) $(CXXFLAGS) $(PROC_RES_INC_DIRS) -S $< -o $@
	@echo  "	CXX	$@"
proc_resistor.o: proc_resistor.s $(PROC_RES_DEPS)
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

proc_common.s: $(PROC_CMN_DEPS)
	$(Q)$(CXX) $(CXXFLAGS) $(PROC_CMN_INC_DIRS) -S $< -o $@
	@echo  "	CXX	$@"
proc_common.o: proc_common.s $(PROC_CMN_DEPS)
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

proc_iface.s: $(PROC_IFACE_DEPS)
	$(Q)$(CC) $(CFLAGS) $(PROC_IFACE_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
proc_iface.o: proc_iface.s $(PROC_IFACE_DEPS)
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s
