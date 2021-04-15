# Prefix for the arm-eabi-none toolchain.
# I'm using codesourcery g++ lite compilers available here:
# http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/
# Microcontroller properties.

export TARGET=bss_cabinet_control
export PROJ_ROOT=.
export BOARD_TYPE=stm32_hallib_bsp
include board/$(BOARD_TYPE)/$(BOARD_TYPE).mk
include gcc.mk

SRCS:=board/board.c
SRCS+=util/string/string_util.c
	
BSP_SRCS:=$(addprefix board/$(BOARD_TYPE)/,$(BSP_SRCS))
BSP_INCLUDES:=$(addprefix board/$(BOARD_TYPE)/,$(BSP_INCLUDES))

INCLUDES:=. app_config board service component util app
INCLUDES+=component/adc_sensor \
			component/ntc \
			component/switch \
INCLUDES+= util/delay util/string 

USER_LIB_INCLUDES=$(PROJ_ROOT)/libs/selex-libc/canopen_clib 
USER_LIBS=CANopen

LIB_INCLUDES:=libs
LIB_INCLUDES+=
OBJDIR=build

INCLUDES+=$(LIB_INCLUDES)			
INCLUDES+=$(BSP_INCLUDES)
INCLUDES:=$(addprefix -I$(PROJ_ROOT)/,$(INCLUDES))
INCLUDES+=$(addprefix -I,$(USER_LIB_INCLUDES))

SRCS+=$(BSP_SRCS)
SRCS+=component/adc_sensor/adc_sensor.c \
			component/ntc/ntc.c \
			component/switch/switch.c \

SRCS+= main.c 
OBJS:=$(addprefix $(PROJ_ROOT)/$(OBJDIR)/,$(SRCS))
OBJS:= $(patsubst %.c,%.o,$(OBJS))
OBJS:= $(patsubst %.s,%.o,$(OBJS))
DEPS:= $(patsubst %.o,%.d,$(OBJS))
SRCS:=$(addprefix $(PROJ_ROOT)/,$(SRCS))

LDSCRIPT_INC=
DEFS:=

#--------------------------------------------------------

OPTIMIZE=-O0
#OPTIMIZE=-O3

# Option arguments for C compiler.
CFLAGS+= $(INCLUDES)
CFLAGS+= $(OPTIMIZE)
CFLAGS+=-fmessage-length=0
CFLAGS+=-fsigned-char
CFLAGS+=-Wall -Winline -ggdb -lm -MMD -MP -Wno-unused-function -Wextra -Wstrict-prototypes
CFLAGS+=-std=gnu11
CFLAGS+=--specs=nano.specs
CFLAGS+=-ffunction-sections -fdata-sections

LFLAGS:=-T$(PROJ_ROOT)/board/$(BOARD_TYPE)/$(LD_FILES)
#LFLAGS	+=-nostartfiles -Xlinker --gc-sections
LFLAGS	+=-Xlinker --gc-sections
LFLAGS  +=-Wl,-Map=$(TARGET).map

LINK_LIBS:= $(addprefix -L,$(USER_LIB_INCLUDES))
LINK_LIBS+= $(addprefix -l,$(USER_LIBS))

#LFLAGS  += -flto -fuse-linker-plugin
# Flags for objcopy
CPFLAGS = -Obinary

# flags for objectdump
ODFLAGS = -S

# Uploader tool path.
FLASHER=JFlashLite
JLINKGDB=JLinkGDBServer
# Flags for the uploader program.
FLASHER_FLAGS=

###################################################

.PHONY:all proj debug libs test_suit test_suit_clean flash

all: proj
	$(HEX)   $(TARGET).elf 	$(TARGET).hex
	$(BIN)   $(TARGET).elf  $(TARGET).bin
	$(DUMP) -St $(TARGET).elf >$(TARGET).lst
	$(SIZE)  $(OBJS) $(TARGET).elf $(TARGET).hex

proj: 	$(TARGET).elf

-include $(DEPS)
$(PROJ_ROOT)/$(OBJDIR)/%.o: $(PROJ_ROOT)/%.c
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) ${<} -o ${@}

$(PROJ_ROOT)/$(OBJDIR)/%.o: $(PROJ_ROOT)/%.s
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	$(AS) -c $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJS)
	@echo Linking...
	$(CC) $(CFLAGS) $(LFLAGS) -o ${TARGET}.elf $(OBJS) $(LINK_LIBS)

clean:
	find ./ -name '*~' | xargs rm -f	
	rm -f *.o
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/*.d
	rm -f $(TARGET).elf
	rm -f $(TARGET).hex
	rm -f $(TARGET).bin
	rm -f $(TARGET).map
	rm -f $(TARGET).lst

libs:
	make -C libs/selex-libc/canopen_clib all
test_suit:
	make -C libs/selex-libc/test-suit/ -f test_suit.mk all
test_suit_clean:
	make -C libs/selex-libc/test-suit/ -f test_suit.mk clean
	
flash:
	./flasher.sh
