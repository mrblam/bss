BSP_SRCS=  \
	   Core/Src/core.c \
	Core/Src/sysmem.c \
	Core/Src/syscalls.c \
	Core/Src/system_stm32f1xx.c \
	Core/Src/stm32f1xx_it.c \
	Core/Src/stm32f1xx_hal_msp.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_can.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cec.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_crc.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_hcd.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2s.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_irda.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_mmc.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_msp_template.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nand.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nor.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pccard.c\
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sd.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_smartcard.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_timebase_rtc_alarm_template.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_timebase_tim_template.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_wwdg.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c  

#BSP_SRCS+=sdk/CMSIS/CM3/CoreSupport/core_cm3.c

BSP_SRCS+=$(BOARD_TYPE).c  \
	debug_com_port_hardware/debug_com_port_hardware.c \
	can_hardware/can_hardware.c \
	adc_hw/adc_hw.c \
	debug_io_hw/debug_io_hw.c \
	ioe_hw/ioe_hw.c \
	delay_hw/delay_hw.c

# ASM sources
BSP_ASM_SRCS = Core/Startup/startup_stm32f103rbtx.s
BSP_SRCS+=$(BSP_ASM_SRCS)

# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi
FLOAT-ABI=-mfloat-abi=soft

# mcu
CFLAGS+= $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
ASFLAGS+= $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =-DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB

# C includes
BSP_INCLUDES = . debug_com_port_hardware \
	can_hardware \
	delay_hw \
	core_hw \
	adc_hw \
	ioe_hw \
	debug_io_hw \
	Core/Inc \
	Drivers/STM32F1xx_HAL_Driver/Inc  \
	Drivers/STM32F1xx_HAL_Driver/Inc/Legacy  \
	Drivers/CMSIS/Device/ST/STM32F1xx/Include \
	Drivers/CMSIS/Include 

# compile gcc flags
ASFLAGS+= $(AS_DEFS) $(AS_INCLUDES)

CFLAGS+= $(C_DEFS) $(C_INCLUDES)

#######################################
# LDFLAGS
#######################################
# link script
LD_FILES:=STM32F103RBTX_FLASH.ld
