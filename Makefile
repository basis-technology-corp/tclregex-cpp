# This data and information is proprietary to, and a valuable trade secret of,
# Basis Technology Corp.  It is given in confidence by Basis Technology and may
# only be used as permitted under the license agreement under which it has been
# distributed, and in no other way.
#
# Copyright © 2000-2006 Basis Technology Corporation All rights reserved.
#
# The technical data and information provided herein are provided with
# `limited rights', and the computer software provided herein is provided
# with `restricted rights' as those terms are defined in DAR and ASPR 7-104.9(a).

# we do this so we can define BT_LIB_PATH

TOPDIR = .

BT_DEPOT_DIR := $(TOPDIR)/../..
BT_BUILD_ROOT := $(BT_DEPOT_DIR)/build_system
include $(BT_BUILD_ROOT)/common-defs.mk

BT_RLP_DIR := $(BT_DEPOT_DIR)/rlp
BT_THIRD_PARTY_DIR := $(BT_DEPOT_DIR)/third-party-tools

SOURCE = regcomp.c \
	     regexec.c \
	     regerror.c \
	     regfree.c

OBJS = $(addprefix $(OBJDIR)/, $(SOURCE:.c=$(OBJ_SUFFIX)))

INCLUDES += -I$(BT_RLP_DIR)/utilities/include 

TCLGREP := $(BINDIR)/tclgrep$(EXE_SUFFIX)

all: mkdirs $(LIBDIR)/$(LIB_PREFIX)tclregex.$(LIB_SUFFIX) $(TCLGREP)

$(LIBDIR)/$(LIB_PREFIX)tclregex.a: $(OBJS)
	$(RM) $(LIBDIR)/$(LIB_PREFIX)tclregex.a
	$(CXX_LINK_STATIC_LIB) $(LIBDIR)/libtclregex.a $(OBJS)

$(LIBDIR)/$(LIB_PREFIX)tclregex.lib: $(OBJS)
	$(RM) $(LIBDIR)/$(LIB_PREFIX)tclregex.lib
	$(CXX_LINK_STATIC_LIB) $(OBJS)

$(BINDIR)/tclgrep.exe: $(OBJDIR)/tclgrep.obj $(LIBDIR)/$(LIB_PREFIX)tclregex.lib
	$(RM) $(BINDIR)/tclgrep.exe
	$(CXX_LINK_EXE) $^ $(UTILITIES_LIBLIST)

$(BINDIR)/tclgrep: $(OBJDIR)/tclgrep.o $(LIBDIR)/$(LIB_PREFIX)tclregex.a
	$(RM) $(BINDIR)/tclgrep
	$(CXX_LINK_EXE) -o $@ $^ $(UTILITIES_LIBLIST) $(BT_LDLIBS)
	$(BT_BIN_POST_LINK_COMMAND)

.PHONY: clean check tclgrep

clean:
	-$(RM) $(OBJDIR)
	-$(RM) $(LIBDIR)/$(LIB_PREFIX)tclregex.$(LIB_SUFFIX)
	-$(RM) $(BINDIR)/tclgrep$(EXE_SUFFIX)

check:
	echo 'fred' | $(BT_LIB_PATH) TCLGREP_VERBOSE=1 $(TCLGREP) '(re)(d)' -

include $(BT_BUILD_ROOT)/common-rules.mk

