BSP_SRCS=  \
	sdk/STM8S_StdPeriph_Driver/src/stm8s_adc1.c \


#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c
#	sdk/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c

        
#BSP_SRCS+=sdk/CMSIS/CM3/CoreSupport/core_cm3.c

BSP_SRCS+=$(BOARD_TYPE).c  

# ASM sources
#BSP_ASM_SRCS = sdk/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md_vl.s
#BSP_SRCS+=$(BSP_ASM_SRCS)

# CFLAGS
#######################################
# cpu
#CPU = -mcpu=cortex-m3

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
-DUSE_STDPERIPH_DRIVER 
#-DSTM32F10X_MD_VL

# C includes
BSP_INCLUDES = . sdk/STM8S_StdPeriph_Driver/inc \
	can_hardware \
	core_hw \
	delay_hw \
	ioe_hw \
	uart_hw \
	cabinet_hw \
	gpio_hw \
	adc_hw \
	timer_hw

# compile gcc flags
ASFLAGS+= $(AS_DEFS) $(AS_INCLUDES)

CFLAGS+= $(C_DEFS) $(C_INCLUDES)



#######################################
# LDFLAGS
#######################################
# link script
#LD_FILES:=sdk/CMSIS/CM3/DeviceSupport/ST/STM32F10x/STM32F103RBT6.ld
