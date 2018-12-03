#!/usr/bin/make -f
VERSION		= 2
PATCHLEVEL	= ~b5
SUBLEVEL	= 
EXTRAVERSION	=
NAME		=

export	VERSION
export	PATCHLEVEL
export	SUBLEVEL

################################################################################
# *AUTHOR*

# EMAIL		"1903716@gmail.com"
# FULL NAME	"Alejandro Colomar Andrés"

################################################################################
# *DOCUMENTATION*
# To see a list of typical targets execute "make help"
# More info can be located in ./README.txt
# Comments in this file are targeted only to the developer, do not
# expect to learn how to build mine-sweeper reading this file.

################################################################################
# Beautify output
# ---------------------------------------------------------------------------
# Prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)some command here
#
# If BUILD_VERBOSE equals 0 then the above command will be hidden.
# If BUILD_VERBOSE equals 1 then the above command is displayed.
#
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands

ifeq ("$(origin V)","command line")
  BUILD_VERBOSE = $(V)
endif
ifndef BUILD_VERBOSE
  BUILD_VERBOSE = 0
endif

ifeq ($(BUILD_VERBOSE), 1)
  Q =
else
  Q = @
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands

ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
  Q = @
endif

export	Q
export	BUILD_VERBOSE

################################################################################
# Do not print "Entering directory ...",
# but we want to display it when entering to the output directory
# so that IDEs/editors are able to understand relative filenames.
MAKEFLAGS += --no-print-directory

################################################################################
PROGRAMVERSION	= $(VERSION)$(if $(PATCHLEVEL),$(PATCHLEVEL)$(if $(SUBLEVEL),$(SUBLEVEL)))$(EXTRAVERSION)
export	PROGRAMVERSION

################################################################################
# Make variables (CC, etc...)
  CC		= gcc
  CXX		= g++
  AS		= as
  AR		= ar
  LD		= ld

export	CC
export	CXX
export	AS
export	AR
export	LD

################################################################################
# cflags
CFLAGS_STD	= -std=c11

CFLAGS_OPT	= -O3
CFLAGS_OPT     += -march=native

CFLAGS_W	= -Wall
CFLAGS_W       += -Wextra
CFLAGS_W       += -Wstrict-prototypes
CFLAGS_W       += -Werror
CFLAGS_W       += -Wno-format-truncation
CFLAGS_W       += -Wno-format-zero-length
#CFLAGS_W       += -Wno-unused-function
CFLAGS_W       += -Wno-unused-parameter

CFLAGS_PKG	= `pkg-config --cflags ncurses`
CFLAGS_PKG     += `pkg-config --cflags opencv`
CFLAGS_PKG     += `pkg-config --cflags zbar`
CFLAGS_PKG     += `pkg-config --cflags tesseract`
CFLAGS_PKG     += `pkg-config --cflags lept`

CFLAGS_D	= -D PROG_VERSION=\"$(PROGRAMVERSION)\"
CFLAGS_D       += -D INSTALL_SHARE_DIR=\"$(INSTALL_SHARE_DIR)\"
CFLAGS_D       += -D SHARE_DIR=\"$(SHARE_DIR)\"
CFLAGS_D       += -D INSTALL_VAR_DIR=\"$(INSTALL_VAR_DIR)\"
CFLAGS_D       += -D VAR_DIR=\"$(VAR_DIR)\"

CFLAGS		= $(CFLAGS_STD)
CFLAGS         += $(CFLAGS_OPT)
CFLAGS         += $(CFLAGS_W)
CFLAGS         += $(CFLAGS_PKG)
CFLAGS         += $(CFLAGS_D)

export	CFLAGS

################################################################################
# c++flags
CXXFLAGS_STD	= -std=c++17

CXXFLAGS_OPT	= -O3
CXXFLAGS_OPT   += -march=native

CXXFLAGS_W	= -Wall
CXXFLAGS_W     += -Wextra
CXXFLAGS_W     += -Werror
CXXFLAGS_W     += -Wno-format-truncation
CXXFLAGS_W     += -Wno-format-zero-length
#CXXFLAGS_W     += -Wno-unused-function
CXXFLAGS_W     += -Wno-unused-parameter

CXXFLAGS_PKG	= `pkg-config --cflags ncurses`
CXXFLAGS_PKG   += `pkg-config --cflags opencv`
CXXFLAGS_PKG   += `pkg-config --cflags zbar`
CXXFLAGS_PKG   += `pkg-config --cflags tesseract`
CXXFLAGS_PKG   += `pkg-config --cflags lept`

CXXFLAGS_D	= -D PROG_VERSION=\"$(PROGRAMVERSION)\"
CXXFLAGS_D     += -D INSTALL_SHARE_DIR=\"$(INSTALL_SHARE_DIR)\"
CXXFLAGS_D     += -D SHARE_DIR=\"$(SHARE_DIR)\"
CXXFLAGS_D     += -D INSTALL_VAR_DIR=\"$(INSTALL_VAR_DIR)\"
CXXFLAGS_D     += -D VAR_DIR=\"$(VAR_DIR)\"

CXXFLAGS	= $(CXXFLAGS_STD)
CXXFLAGS       += $(CXXFLAGS_OPT)
CXXFLAGS       += $(CXXFLAGS_W)
CXXFLAGS       += $(CXXFLAGS_PKG)
CXXFLAGS       += $(CXXFLAGS_D)

export	CXXFLAGS

################################################################################
# libs
LIBS_PKG	= `pkg-config --libs ncurses`
LIBS_PKG       += `pkg-config --libs opencv`
LIBS_PKG       += `pkg-config --libs zbar`
LIBS_PKG       += `pkg-config --libs tesseract`
LIBS_PKG       += `pkg-config --libs lept`

LIBS		= $(LIBS_PKG)

export	LIBS

################################################################################
# directories

MAIN_DIR	= $(CURDIR)

LIBALX_DIR	= $(CURDIR)/libalx/
MODULES_DIR	= $(CURDIR)/modules/
TMP_DIR		= $(CURDIR)/tmp/
BIN_DIR		= $(CURDIR)/bin/

export	MAIN_DIR
export	LIBALX_DIR
export	MODULES_DIR

# FIXME: Set local or not local when building a package
INSTALL_BIN_DIR		= /usr/local/bin/
#INSTALL_BIN_DIR	= /usr/bin/
INSTALL_SHARE_DIR	= /usr/local/share/
#INSTALL_SHARE_DIR	= /usr/share/
SHARE_DIR		= vision-artificial/
INSTALL_VAR_DIR		= /var/local/
#INSTALL_VAR_DIR	= /var/lib/
VAR_DIR			= vision-artificial/

export	INSTALL_DIR
export	INSTALL_SHARE_DIR
export	SHARE_DIR

################################################################################
# executables

BIN_NAME	= vision-artificial2

export	BIN_NAME

################################################################################
# target: dependencies
#	action

# That's the default target when none is given on the command line
PHONY := all
all: binary


PHONY += libalx
libalx:
	@echo	'	MAKE	libalx-base'
	$(Q)make base	-C $(LIBALX_DIR)
	@echo	'	MAKE	libalx-curses'
	$(Q)make curses	-C $(LIBALX_DIR)

PHONY += modules
modules: libalx
	@echo	'	MAKE	modules'
	$(Q)make -C $(MODULES_DIR)

PHONY += object
object: modules libalx
	@echo	'	MAKE	objects'
	$(Q)make -C $(TMP_DIR)

PHONY += binary
binary: object
	@echo	'	MAKE	binary'
	$(Q)make -C $(BIN_DIR)

PHONY += install
install: uninstall
	@echo  "	MKDIR -p	$(INSTALL_BIN_DIR)/"
	$(Q)mkdir -p		$(DESTDIR)/$(INSTALL_BIN_DIR)/
	@echo "Copy $(BIN_NAME)"
	$(Q)cp			$(BIN_DIR)/$(BIN_NAME)	$(DESTDIR)/$(INSTALL_BIN_DIR)/
	@echo  ""
	
	@echo  "	MKDIR -p	$(INSTALL_SHARE_DIR)/$(SHARE_DIR)/"
	$(Q)mkdir -p		$(DESTDIR)/$(INSTALL_SHARE_DIR)/$(SHARE_DIR)/
	@echo	"	CP -r		share/*"
	$(Q)cp -r		./share/*		$(DESTDIR)/$(INSTALL_SHARE_DIR)/$(SHARE_DIR)/
	@echo  ""
	
	@echo  "Done"
	@echo  ""

PHONY += uninstall
uninstall:
	@echo  "Clean old installations"
	$(Q)rm -f	$(DESTDIR)/$(INSTALL_BIN_DIR)/$(BIN_NAME)
	$(Q)rm -f -r	$(DESTDIR)/$(INSTALL_SHARE_DIR)/$(SHARE_DIR)/
	@echo  ""

PHONY += clean
clean:
	@echo	'	CLEAN	libalx'
	$(Q)make clean	-C $(LIBALX_DIR)
	@echo	'	CLEAN	modules'
	$(Q)make clean	-C $(MODULES_DIR)
	@echo	'	CLEAN	tmp'
	$(Q)make clean	-C $(TMP_DIR)
	@echo	'	CLEAN	bin'
	$(Q)make clean	-C $(BIN_DIR)

PHONY += help
help:
	@echo  'Cleaning targets:'
	@echo  '  clean		  - Remove all generated files'
	@echo  ''
	@echo  'Other generic targets:'
	@echo  '  all		  - Build all targets marked with [*]'
	@echo  '* libalx	  - Build the libalx library'
	@echo  '* modules	  - Build all modules'
	@echo  '* object	  - Build the main object'
	@echo  '* binary	  - Build the binary'
	@echo  '  install	  - Install the program into the filesystem'
	@echo  '  uninstall	  - Uninstall the program off the filesystem'
	@echo  ''
	@echo  '  make V=0|1 [targets] 0 => quiet build (default), 1 => verbose build'
	@echo  ''
	@echo  'Execute "make" or "make all" to build all targets marked with [*] '
	@echo  'For further info see the ./README file'

################################################################################
# Declare the contents of the .PHONY variable as phony.
.PHONY: $(PHONY)




################################################################################
######## End of file ###########################################################
################################################################################
