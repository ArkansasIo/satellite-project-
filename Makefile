ASM := nasm
QEMU := qemu-system-i386
AARCH64_AS := aarch64-none-elf-as
AARCH64_LD := aarch64-none-elf-ld
AARCH64_OBJCOPY := aarch64-none-elf-objcopy
BOOT_SRC := voyager1_os.asm
STAGE2_SRC := stage2.asm
PI6_SRC := pi6/kernel8.S
PI6_LD := pi6/linker.ld
BUILD_DIR := build
BOOT_BIN := $(BUILD_DIR)/boot.bin
STAGE2_BIN := $(BUILD_DIR)/stage2.bin
IMG := $(BUILD_DIR)/voyager1_os.img
PI6_DIR := $(BUILD_DIR)/pi6
PI6_OBJ := $(PI6_DIR)/kernel8.o
PI6_ELF := $(PI6_DIR)/kernel8.elf
PI6_IMG := $(PI6_DIR)/kernel8.img

.PHONY: all run pi6 run-pi6 clean

all: $(IMG)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	$(ASM) -f bin $(BOOT_SRC) -o $(BOOT_BIN)

$(STAGE2_BIN): $(STAGE2_SRC) | $(BUILD_DIR)
	$(ASM) -f bin $(STAGE2_SRC) -o $(STAGE2_BIN)

$(IMG): $(BOOT_BIN) $(STAGE2_BIN)
	cat $(BOOT_BIN) $(STAGE2_BIN) > $(IMG)
	truncate -s 8704 $(IMG)

run: $(IMG)
	$(QEMU) -drive format=raw,file=$(IMG)

$(PI6_DIR):
	mkdir -p $(PI6_DIR)

$(PI6_OBJ): $(PI6_SRC) | $(PI6_DIR)
	$(AARCH64_AS) $(PI6_SRC) -o $(PI6_OBJ)

$(PI6_ELF): $(PI6_OBJ) $(PI6_LD)
	$(AARCH64_LD) -T $(PI6_LD) $(PI6_OBJ) -o $(PI6_ELF)

$(PI6_IMG): $(PI6_ELF)
	$(AARCH64_OBJCOPY) -O binary $(PI6_ELF) $(PI6_IMG)
	cp pi6/config.txt $(PI6_DIR)/config.txt
	cp pi6/cmdline.txt $(PI6_DIR)/cmdline.txt

pi6: $(PI6_IMG)

run-pi6: $(PI6_IMG)
	qemu-system-aarch64 -M virt -cpu cortex-a72 -nographic -kernel $(PI6_IMG)

clean:
	rm -rf $(BUILD_DIR)
