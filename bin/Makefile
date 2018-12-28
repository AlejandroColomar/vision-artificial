# -*- MakeFile -*-

# directories

LIBALX_LIB_DIR	= $(LIBALX_DIR)/lib/

MODULES_TMP_DIR	= $(MODULES_DIR)/tmp/

TMP_DIR		= $(MAIN_DIR)/tmp/

# dependencies
ALL	= $(BIN_NAME)

MAIN_OBJ_MODULES	= modules.o
MAIN_OBJS		= $(TMP_DIR)/main.o \
				$(patsubst %,$(MODULES_TMP_DIR)/%,$(MAIN_OBJ_MODULES))

MAIN_LIB_LIBALX		= libalx-base.a libalx-io.a libalx-curses.a
MAIN_LIBS		= \
				$(patsubst %,$(LIBALX_LIB_DIR)/%,$(MAIN_LIB_LIBALX))

# static libs
STATIC_LIB_LIBALX	= -L $(LIBALX_LIB_DIR) \
				-l alx-curses -l alx-io -l alx-base

STATIC_LIBS	= $(STATIC_LIB_LIBALX)


# target: dependencies
#	action

all: $(ALL)


$(BIN_NAME): $(MAIN_OBJS) $(MAIN_LIBS)
	$(Q)$(CXX) $(MAIN_OBJS) -o $@ $(STATIC_LIBS) $(LIBS)
	@echo  "	CXX	$@"
	@echo  ""


clean:
	@echo  "	RM	$(ALL)"
	$(Q)rm -f $(ALL)

################################################################################
######## End of file ###########################################################
################################################################################
