# -*- MakeFile -*-

# directories

LIBALX_LIB_DIR	= $(LIBALX_DIR)/lib/

MODULES_TMP_DIR	= $(MODULES_DIR)/tmp/

TMP_DIR	= $(MAIN_DIR)/tmp/

# dependencies
ALL	= $(BIN_NAME)

MAIN_OBJ_MODULES	= modules.o

MAIN_OBJS	= $(TMP_DIR)/main.o \
			$(patsubst %,$(MODULES_TMP_DIR)/%,$(MAIN_OBJ_MODULES))

# static libs
MAIN_LIB_LIBALX		= -L $(LIBALX_LIB_DIR) -l alx

STATIC_LIBS	= $(MAIN_LIB_LIBALX)


# target: dependencies
#	action

all: $(ALL)


$(BIN_NAME): $(MAIN_OBJS)
	$(Q)$(CXX) $^ -o $@ $(STATIC_LIBS) $(LIBS)
	@echo  "	CXX	$@"
	@echo  ""


clean:
	$(Q)rm -f $(ALL)
	@echo  "Clean binaries"

################################################################################
######## End of file ###########################################################
################################################################################
