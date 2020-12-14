# Prefix for the arm-eabi-none toolchain.
# I'm using codesourcery g++ lite compilers available here:
# http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/
# Microcontroller properties.

export TARGET=s2_cbox
export PROJ_ROOT=.
export BOARD_TYPE=stm32_bsp
include board/$(BOARD_TYPE)/$(BOARD_TYPE).mk
include gcc.mk

SRCS:=board/board.c
SRCS+=util/string/string_util.c util/json/json.c
	
BSP_SRCS:=$(addprefix board/$(BOARD_TYPE)/,$(BSP_SRCS))
BSP_INCLUDES:=$(addprefix board/$(BOARD_TYPE)/,$(BSP_INCLUDES))

INCLUDES:=. app_config board service component util app
INCLUDES+= app/mqtt app/canopen
INCLUDES+=	component/sim800a component/at_modem
INCLUDES+= service/timer service/network

INCLUDES+= util/delay util/string util/json

USER_LIB_INCLUDES=$(PROJ_ROOT)/libs/selex-libc/canopen_clib 
USER_LIBS=CANopen

PAHO_MQTT_ROOT=libs/paho.mqtt.embedded-c

LIB_INCLUDES:=libs $(PAHO_MQTT_ROOT)/MQTTClient-C/src
LIB_INCLUDES+=$(PAHO_MQTT_ROOT)/MQTTPacket/src
OBJDIR=build

INCLUDES+=$(LIB_INCLUDES)			
INCLUDES+=$(BSP_INCLUDES)
INCLUDES:=$(addprefix -I$(PROJ_ROOT)/,$(INCLUDES))
INCLUDES+=$(addprefix -I,$(USER_LIB_INCLUDES))

SRCS+=$(BSP_SRCS)
SRCS+= app/mqtt/mqtt_init.c app/canopen/canopen_init.c
SRCS+= component/sim800a/sim800a.c component/at_modem/at_modem.c
SRCS+= service/timer/timer.c service/network/network.c


#SRCS+=libs/MQTT-C/src/mqtt_pal.c libs/MQTT-C/src/mqtt.c
SRCS+=$(PAHO_MQTT_ROOT)/MQTTClient-C/src/MQTTClient.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTConnectClient.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTConnectServer.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTDeserializePublish.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTFormat.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTPacket.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTSerializePublish.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTSubscribeClient.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTSubscribeServer.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTUnsubscribeClient.c \
	$(PAHO_MQTT_ROOT)/MQTTPacket/src/MQTTUnsubscribeServer.c

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
CFLAGS+=--specs=nosys.specs
CFLAGS+=-ffunction-sections -fdata-sections

LFLAGS:=-T$(PROJ_ROOT)/board/$(BOARD_TYPE)/$(LD_FILES)
LFLAGS	+=-nostartfiles -Xlinker --gc-sections
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
