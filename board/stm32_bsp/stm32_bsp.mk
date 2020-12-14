BSP_SRCS=  \
	system_stm32f0xx.c \
	stm32f0xx_it.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_adc.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_can.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_cec.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_comp.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_dma.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_exti.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_gpio.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_i2c.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_misc.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_pwr.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_rcc.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_spi.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_tim.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_usart.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_syscfg.c \
#        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_crc.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_crs.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_wwdg.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_rtc.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_iwdg.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_dac.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_dbgmcu.c \
        sdk/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_flash.c \
 

BSP_SRCS+=$(BOARD_TYPE).c  \
	debug_com_port_hardware/debug_com_port_hardware.c \
	can_hardware/can_hardware.c \
	hc05_hw/hc05_hw.c \
	neo_6m_hw/neo_6m_hw.c \
	sim800a_hw/sim800a_hw.c \
	debug_io_hw/debug_io_hw.c \
	core_hw/core_hw.c \
	timer_hw/timer_hw.c \
	delay_hw/delay_hw.c

# ASM sources
BSP_ASM_SRCS = sdk/CMSIS/Device/ST/STM32F0xx/Source/startup_stm32f072.s
BSP_SRCS+=$(BSP_ASM_SRCS)

# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m0

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
CFLAGS+= $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
ASFLAGS+= $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_STDPERIPH_DRIVER \
-DSTM32F072

# C includes
BSP_INCLUDES = . debug_com_port_hardware \
	can_hardware \
	hc05_hw \
	neo_6m_hw \
	sim800a_hw \
	delay_hw \
	core_hw \
	debug_io_hw \
	timer_hw \
	sdk/CMSIS/Include \
	sdk/STM32F0xx_StdPeriph_Driver/inc \
	sdk/CMSIS/Device/ST/STM32F0xx/Include

# compile gcc flags
ASFLAGS+= $(AS_DEFS) $(AS_INCLUDES)

CFLAGS+= $(C_DEFS) $(C_INCLUDES)

#######################################
# LDFLAGS
#######################################
# link script
LD_FILES:=sdk/CMSIS/Device/ldscripts/stm32f0xx.ld
