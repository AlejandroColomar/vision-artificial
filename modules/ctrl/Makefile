# -*- MakeFile -*-

# MACRO = substitute with this

# directories

TMP_DIR = $(CTRL_DIR)/tmp/

# target: dependencies
#	action

PHONY := all
all:
	$(Q)$(MAKE) -C $(TMP_DIR)


PHONY += clean
clean:
	$(Q)$(MAKE) clean	-C $(TMP_DIR)

################################################################################
# Declare the contents of the .PHONY variable as phony.
.PHONY: $(PHONY)




################################################################################
######## End of file ###########################################################
################################################################################
