# Requires devkitPro 3DS toolchain and libjpeg-turbo (3DS port).
# Make with: make 3dsx, after installing devkitPro libjpeg package.
.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error DEVKITARM not set; install devkitPro's 3DS toolchain)
endif
include $(DEVKITARM)/3ds_rules
TARGET := TomBenNews3DS
BUILD := build
SOURCES := source
INCLUDES :=
ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS := -g -Wall -O2 -mword-relocations -ffunction-sections $(ARCH) $(INCLUDE) -D__3DS__
CXXFLAGS := $(CFLAGS)
ASFLAGS := -g $(ARCH)
LDFLAGS := -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(TARGET).map
LIBS := -ljpeg -lctru -lm
LIBDIRS := $(CTRULIB) $(PORTLIBS)
ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT := $(CURDIR)/$(TARGET)
export TOPDIR := $(CURDIR)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)
export OFILES := $(patsubst %.c,%.o,$(notdir $(wildcard $(SOURCES)/*.c)))
export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(CTRULIB)/include -I$(PORTLIBS)/include
export LIBPATHS := -L$(CTRULIB)/lib -L$(PORTLIBS)/lib
.PHONY: all clean
all: $(BUILD)
$(BUILD):
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
clean:
	@rm -rf $(BUILD) $(TARGET).3dsx $(TARGET).elf $(TARGET).smdh $(TARGET).map
else
DEPENDS := $(OFILES:.o=.d)
.PHONY: all
all: $(OUTPUT).3dsx
$(OUTPUT).3dsx: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)
-include $(DEPSDIR)/*.d
endif
