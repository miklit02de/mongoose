# FreeRTOS/{FreeRTOS-Kernel V10.4.3,FreeRTOS-Plus-TCP V2.3.2}
PROG = firmware
ARCH = stm32f7
ROOT = $(realpath $(CURDIR)/../..)
DOCKER ?= docker run -it --rm -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/armgcc
MONGOOSE_OPTS = -DMG_ARCH=MG_ARCH_FREERTOS_TCP -DMG_ENABLE_FS=0 -DMG_ENABLE_LINES=1
MCU = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard
INCS = -I. -I$(ROOT) -I$(ARCH) -I$(ROOT)/test/freertos-kernel/include -I$(ROOT)/test/freertos-tcp/include -I$(ROOT)/test/freertos-tcp/portable/Compiler/GCC -I$(ROOT)/test/freertos-tcp/portable/NetworkInterface/include -IHAL
NETFLAGS = -DSEMIHOSTING -DSTM32F7xx -DSTM32F746xx -Wno-sign-compare -Wno-unused-function -Wno-cpp
CFLAGS = -W -Wall -Werror -Wno-format -Wno-address-of-packed-member -Os -g $(MCU) -fdata-sections -ffunction-sections $(INCS) $(MONGOOSE_OPTS) $(NETFLAGS) $(EXTRA)
#LFLAGS = $(MCU) --static -Wl,-Map=$(TARGET).map -Wl,--gc-sections -lc -lgcc -T$(ARCH)/link.ld #-nostartfiles -lgcc #-Wl,-Map=obj/$(PROG).map,--cref -Wl,--gc-sections
LFLAGS = $(MCU) --static -Wl,-Map=$(TARGET).map -Wl,--gc-sections -specs rdimon.specs -lrdimon -lc -lgcc -T$(ARCH)/link.ld #-nostartfiles -lgcc #-Wl,-Map=obj/$(PROG).map,--cref -Wl,--gc-sections
SRCS = main.c $(wildcard $(ROOT)/test/freertos-kernel/*.c) $(wildcard $(ROOT)/test/freertos-tcp/*.c) 
SRCS += $(ROOT)/test/freertos-kernel/portable/MemMang/heap_4.c $(ARCH)/port.c
SRCS += $(ROOT)/test/freertos-tcp/portable/BufferManagement/BufferAllocation_2.c
SRCS += $(wildcard $(ROOT)/test/freertos-tcp/portable/NetworkInterface/STM32Fxx/*.c)
SRCS += $(wildcard $(ROOT)/test/freertos-tcp/portable/NetworkInterface/Common/*.c)
SRCS += $(wildcard HAL/*.c)
OBJS = obj/boot.o $(SRCS:%.c=obj/%.o) obj/mongoose.o  # ORDER MATTERS - boot (vector table) first!

all: $(PROG).hex

$(PROG).bin: $(PROG).elf
	$(DOCKER) arm-none-eabi-objcopy -O binary $< $@

$(PROG).hex: $(PROG).bin
	$(DOCKER) arm-none-eabi-objcopy -I binary -O ihex --change-address 0x8000000 $< $@

$(PROG).elf: $(OBJS)
	$(DOCKER) arm-none-eabi-gcc $(OBJS) $(LFLAGS) -o $@

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(DOCKER) arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/boot.o:
	@mkdir -p $(dir $@)
	$(DOCKER) arm-none-eabi-as --warn --fatal-warnings $(MCU) $(ARCH)/boot.s -o $@

obj/mongoose.o: 
	$(DOCKER) arm-none-eabi-gcc $(CFLAGS) -c ../../mongoose.c -o $@

flash: $(PROG).bin
	st-flash --reset write $< 0x8000000

openocd:
	openocd -f openocd.cfg

gdb: $(PROG).elf
	arm-none-eabi-gdb \
		-ex 'set confirm off' \
        -ex 'target extended-remote :3333' \
        -ex 'monitor arm semihosting enable' \
        -ex 'monitor reset halt' \
        -ex 'load' \
        -ex 'monitor reset init' \
        -ex 'r' \
        $<

clean:
	@rm -rf *.{bin,elf,map,lst,tgz,zip,hex} obj
