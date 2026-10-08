TARGET = btt4
BUILD = build

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g3 -Wall -Wextra \
         -ffreestanding -fdata-sections -ffunction-sections \
         -I. -IFreeRTOS-Kernel/include \
         -IFreeRTOS-Kernel/portable/GCC/ARM_CM3

LDFLAGS = -mcpu=cortex-m3 -mthumb -Tstm32f103.ld \
          -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map \
          --specs=nosys.specs

FREERTOS_C = FreeRTOS-Kernel/tasks.c FreeRTOS-Kernel/list.c
PORT_C = FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.c
HEAP_C = FreeRTOS-Kernel/portable/MemMang/heap_4.c

C_SOURCES = main.c $(FREERTOS_C) $(PORT_C) $(HEAP_C)
C_OBJECTS = $(patsubst %.c,$(BUILD)/%.o,$(C_SOURCES))
	
all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin
	$(SIZE) $(BUILD)/$(TARGET).elf

$(BUILD)/$(TARGET).elf: $(C_OBJECTS) $(BUILD)/startup_stm32f103.o
	@mkdir -p $(dir $@)
	$(LD) $(C_OBJECTS) $(BUILD)/startup_stm32f103.o $(LDFLAGS) -o $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/startup_stm32f103.o: startup_stm32f103.s
	@mkdir -p $(dir $@)
	$(AS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

flash: $(BUILD)/$(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $(BUILD)/$(TARGET).bin 0x08000000 verify reset exit"
.PHONY: all clean flash
