# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := base
DEFS_Default :=

# Flags passed to all source files.
CFLAGS_Default := \
	-fasm-blocks \
	-mpascal-strings \
	-Os \
	-gdwarf-2 \
	-arch i386

# Flags passed to only C files.
CFLAGS_C_Default :=

# Flags passed to only C++ files.
CFLAGS_CC_Default :=

# Flags passed to only ObjC files.
CFLAGS_OBJC_Default :=

# Flags passed to only ObjC++ files.
CFLAGS_OBJCC_Default :=

INCS_Default :=

OBJS := \
	$(obj).target/$(TARGET)/../base/asyncall.o \
	$(obj).target/$(TARGET)/../base/atomicops-internals-x86.o \
	$(obj).target/$(TARGET)/../base/baseasync.o \
	$(obj).target/$(TARGET)/../base/common.o \
	$(obj).target/$(TARGET)/../base/doobs_hash.o \
	$(obj).target/$(TARGET)/../base/jdbcurl.o \
	$(obj).target/$(TARGET)/../base/logging.o \
	$(obj).target/$(TARGET)/../base/md5.o \
	$(obj).target/$(TARGET)/../base/sha2.o \
	$(obj).target/$(TARGET)/../base/messagequeue.o \
	$(obj).target/$(TARGET)/../base/mkdirs.o \
	$(obj).target/$(TARGET)/../base/network.o \
	$(obj).target/$(TARGET)/../base/no_exception.o \
	$(obj).target/$(TARGET)/../base/once.o \
	$(obj).target/$(TARGET)/../base/pathops.o \
	$(obj).target/$(TARGET)/../base/pcount.o \
	$(obj).target/$(TARGET)/../base/signals.o \
	$(obj).target/$(TARGET)/../base/startuplist.o \
	$(obj).target/$(TARGET)/../base/stringdigest.o \
	$(obj).target/$(TARGET)/../base/stringencode.o \
	$(obj).target/$(TARGET)/../base/thread.o \
	$(obj).target/$(TARGET)/../base/threadpool.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))
$(OBJS): GYP_OBJCFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE)) $(CFLAGS_OBJC_$(BUILDTYPE))
$(OBJS): GYP_OBJCXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE)) $(CFLAGS_OBJCC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Default := \
	-arch i386 \
	-L$(builddir)

LIBTOOLFLAGS_Default :=

LIBS :=

$(builddir)/libbase.a: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(builddir)/libbase.a: LIBS := $(LIBS)
$(builddir)/libbase.a: GYP_LIBTOOLFLAGS := $(LIBTOOLFLAGS_$(BUILDTYPE))
$(builddir)/libbase.a: TOOLSET := $(TOOLSET)
$(builddir)/libbase.a: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,alink)

all_deps += $(builddir)/libbase.a
# Add target alias
.PHONY: base
base: $(builddir)/libbase.a

# Add target alias to "all" target.
.PHONY: all
all: base
