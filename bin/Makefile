#! /usr/bin/make -f

# dependencies

OBJS		=							\
		$(TMP_DIR)/main.o					\
		$(TMP_DIR)/about/about.o				\
		$(TMP_DIR)/ctrl/start.o					\
		$(TMP_DIR)/image/alx.o					\
		$(TMP_DIR)/image/calib3d.o				\
		$(TMP_DIR)/image/cv.o					\
		$(TMP_DIR)/image/iface.o				\
		$(TMP_DIR)/image/ocr.o					\
		$(TMP_DIR)/image/orb.o					\
		$(TMP_DIR)/image/zbar.o					\
		$(TMP_DIR)/menu/clui.o					\
		$(TMP_DIR)/menu/iface.o					\
		$(TMP_DIR)/menu/parse.o					\
		$(TMP_DIR)/menu/tui.o					\
		$(TMP_DIR)/proc/coins.o					\
		$(TMP_DIR)/proc/common.o				\
		$(TMP_DIR)/proc/iface.o					\
		$(TMP_DIR)/proc/label.o					\
		$(TMP_DIR)/proc/lighters.o				\
		$(TMP_DIR)/proc/objects.o				\
		$(TMP_DIR)/proc/resistor.o				\
		$(TMP_DIR)/save/save.o					\
		$(TMP_DIR)/user/clui.o					\
		$(TMP_DIR)/user/iface.o					\
		$(TMP_DIR)/user/tui.o
DEPS_LIBS	= 							\
		$(LIBALX_LIB_DIR)/libalx-ncurses.a			\
		$(LIBALX_LIB_DIR)/libalx-stdio.a			\
		$(LIBALX_LIB_DIR)/libalx-errno.a			\
		$(LIBALX_LIB_DIR)/libalx-math.a				\
		$(LIBALX_LIB_DIR)/libalx-stdlib.a			\
		$(LIBALX_LIB_DIR)/libalx-string.a

ALL	= $(BIN_NAME) size

# static libs

STATIC_LIBS	= -L $(LIBALX_LIB_DIR)					\
		-l alx-ncurses						\
		-l alx-stdio						\
		-l alx-string						\
		-l alx-errno						\
		-l alx-math						\
		-l alx-stdlib


# target: dependencies
#	action

PHONY := all
all: $(ALL)


$(BIN_NAME): $(OBJS) $(DEPS_LIBS)
	@echo	"	CXX	$@"
	$(Q)$(CXX) $(OBJS) -o $@ $(STATIC_LIBS) $(LIBS)

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
