# Prefix for the arm-eabi-none toolchain.
# I'm using codesourcery g++ lite compilers available here:
# http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/
# Microcontroller properties.

export TARGET=test_at_command
export PROJ_ROOT=../..
include $(PROJ_ROOT)/gcc_i386.mk
SRCS:=util/string/string_util.c util/json/json.c
SRCS+=app/mqtt/mqtt_init.c
SRCS+= component/at_modem/at_modem.c \
	component/sim800a/sim800a.c
	SRCS+=service/network/network.c service/timer/timer.c
SRCS+= test/$(TARGET)/$(TARGET).c 

PAHO_MQTT_ROOT=libs/paho.mqtt.embedded-c
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
LIB_INCLUDES:=libs $(PAHO_MQTT_ROOT)/MQTTClient-C/src
LIB_INCLUDES+=$(PAHO_MQTT_ROOT)/MQTTPacket/src

INCLUDES+=$(LIB_INCLUDES)			
INCLUDES+= app/mqtt
INCLUDES+=	component/at_modem
INCLUDES+=	component/sim800a
INCLUDES+= service/timer service/network
INCLUDES+=	board
INCLUDES+= util/delay util/string util/json
OBJDIR=build

INCLUDES:=$(addprefix -I$(PROJ_ROOT)/,$(INCLUDES))
INCLUDES+=-I.

OBJS:=$(addprefix $(PROJ_ROOT)/$(OBJDIR)/,$(SRCS))
OBJS:= $(patsubst %.c,%.o,$(OBJS))
OBJS:= $(patsubst %.s,%.o,$(OBJS))
DEPS:= $(patsubst %.o,%.d,$(OBJS))
SRCS:=$(addprefix $(PROJ_ROOT)/,$(SRCS))

LDSCRIPT_INC=
DEFS:=

#--------------------------------------------------------

OPTIMIZE=-O0

# Option arguments for C compiler.
CFLAGS+= $(INCLUDES)
CFLAGS+= $(OPTIMIZE)
CFLAGS+=-fmessage-length=0
CFLAGS+=-fsigned-char
CFLAGS+=-Wall -Winline -ggdb -lm -MMD -MP -Wno-unused-function -Wextra -Wstrict-prototypes
CFLAGS+=-std=gnu11
CFLAGS+=-ffunction-sections -fdata-sections

#LFLAGS	+=-Xlinker --gc-sections
LFLAGS  +=-Wl,-Map=$(TARGET).map

#LFLAGS  += -flto -fuse-linker-plugin
# Flags for objcopy
CPFLAGS = -Obinary

# flags for objectdump
ODFLAGS = -S

###################################################

.PHONY:all proj

all: proj $(DEPS)
	$(HEX)   $(TARGET).elf 	$(TARGET).hex
	$(BIN)   $(TARGET).elf  $(TARGET).bin
	$(DUMP) -St $(TARGET).elf >$(TARGET).lst
	$(SIZE)  $(OBJS) $(TARGET).elf $(TARGET).hex

proj: 	$(TARGET).elf

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