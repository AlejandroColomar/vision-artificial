#! /usr/bin/make -f

# dependencies

OBJS		=							\
		$(TMP_DIR)/main.o					\
		$(TMP_DIR)/ctrl/start.o					\
		$(TMP_DIR)/image/calib3d.o				\
		$(TMP_DIR)/image/cv.o					\
		$(TMP_DIR)/image/iface.o				\
		$(TMP_DIR)/image/orb.o					\
		$(TMP_DIR)/image/zbar.o					\
		$(TMP_DIR)/menu/iface.o					\
		$(TMP_DIR)/menu/parse.o					\
		$(TMP_DIR)/menu/tui.o					\
		$(TMP_DIR)/save/save.o					\
		$(TMP_DIR)/share/share.o				\
		$(TMP_DIR)/user/iface.o					\
		$(TMP_DIR)/user/tui.o

ALL	= $(BIN_NAME) size


# target: dependencies
#	action

PHONY := all
all: $(ALL)


$(BIN_NAME): $(OBJS) $(DEPS_LIBS)
	@echo	"	CXX	$@"
	$(Q)$(CXX) $(OBJS) -o $@ $(LIBS)

size: $(BIN_NAME)
	@echo	"	SZ	$(BIN_NAME)"
	$(Q)$(SZ) $(BIN_NAME)


clean:
	@echo	"	RM	$(ALL)"
	$(Q)rm -f	$(ALL)

################################################################################
# Declare the contents of the .PHONY variable as phony.
.PHONY: $(PHONY)

################################################################################
######## End of file ###########################################################
################################################################################
