#########################################################################
#
# U-boot build supports producing a object files to the separate external
# directory. Two use cases are supported:
#
# 1) Add O= to the make command line
# 'make O=/tmp/build all'
#
# 2) Set environement variable BUILD_DIR to point to the desired location
# 'export BUILD_DIR=/tmp/build'
# 'make'
#
# The second approach can also be used with a MAKEALL script
# 'export BUILD_DIR=/tmp/build'
# './MAKEALL'
#
# Command line 'O=' setting overrides BUILD_DIR environent variable.
#
# When none of the above methods is used the local build is performed and
# the object files are placed in the source directory.
#
ifdef O
ifeq ("$(origin O)", "command line")
BUILD_DIR := $(O)
endif
endif
ifneq ($(BUILD_DIR),)
saved-output := $(BUILD_DIR)
# Attempt to create a output directory.
$(shell [ -d ${BUILD_DIR} ] || mkdir -p ${BUILD_DIR})
# Verify if it was successful.
BUILD_DIR := $(shell cd $(BUILD_DIR) && /bin/pwd)
$(if $(BUILD_DIR),,$(error output directory "$(saved-output)" does not exist))
endif # ifneq ($(BUILD_DIR),)

OBJTREE        := $(if $(BUILD_DIR),$(BUILD_DIR),$(CURDIR)/bin)
SRCTREE        := $(CURDIR)
TOPDIR        := $(SRCTREE)
LNDIR        := $(OBJTREE)

export    TOPDIR SRCTREE OBJTREE

MKCONFIG    := $(SRCTREE)/mkconfig
export MKCONFIG

ifneq ($(OBJTREE),$(SRCTREE))
REMOTE_BUILD    := 1
export REMOTE_BUILD
endif

# $(obj) and (src) are defined in config.mk but here in main Makefile
# we also need them before config.mk is included which is the case for
# some targets like unconfig, clean, clobber, distclean, etc.
ifneq ($(OBJTREE),$(SRCTREE))
obj := $(OBJTREE)/
src := $(SRCTREE)/
else
obj :=
src :=
endif 
export obj src

all: terminal

#logview newreg terminal telnetd readmachcode

#readmachcode newreg terminal service shellpath update

register ctrl client logview shellpath telnetd terminal zhangbo single mediac update reg getregkey registmachine service easy51pro newreg readmachcode:
	(cd $@; $(MAKE) all)

clean:
	(cd terminal; $(MAKE) clean)

.PHONY : register client ctrl logview telnetd all clean readmachcode terminal single mediac zhangbo update reg getregkey registmachine service shellpath easy51pro newreg

# towel.blinkenlights.nl# bbs.pku.edu.cn
# bbs.tsinghua.edu.cn