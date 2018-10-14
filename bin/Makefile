# -*- MakeFile -*-

# directories

LIBALX_TMP_DIR	= $(LIBALX_DIR)/tmp/
MODULES_TMP_DIR	= $(MODULES_DIR)/tmp/

TMP_DIR	= $(MAIN_DIR)/tmp/

# dependencies
_ALL	= vision-artificial

ALL	= $(BIN_NAME)

MAIN_OBJ_LIBALX		= alx_lib.o
MAIN_OBJ_MODULES	= modules.o

MAIN_OBJS	= $(TMP_DIR)/main.o \
			$(patsubst %,$(MODULES_TMP_DIR)/%,$(MAIN_OBJ_MODULES)) \
			$(patsubst %,$(LIBALX_TMP_DIR)/%,$(MAIN_OBJ_LIBALX))


# target: dependencies
#	action

all: $(ALL)


$(BIN_NAME): $(MAIN_OBJS)
	$(Q)$(CXX) $^ -o $@ $(LIBS)
	@echo  "	CXX	$@"
	@echo  ""


clean:
	$(Q)rm -f $(_ALL)
	@echo  "Clean binaries"

################################################################################
######## End of file ###########################################################
################################################################################
