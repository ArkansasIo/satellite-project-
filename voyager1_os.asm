; Voyager 1 Inspired Toy OS - stage 1 bootloader
; Assembled by build.ps1 or Makefile.
;
; This 512-byte boot sector loads stage2.bin from the disk image and jumps to it.
; It is educational x86 BIOS code, not real spacecraft software.

bits 16
org 0x7C00

stage2_segment equ 0x0000
stage2_offset  equ 0x8000
stage2_sectors equ 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov [boot_drive], dl

    mov si, loading_msg
    call print_string

    xor ax, ax
    int 0x13
    jc disk_error

    mov ax, stage2_segment
    mov es, ax
    mov bx, stage2_offset
    mov ah, 0x02
    mov al, stage2_sectors
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    jmp stage2_segment:stage2_offset

disk_error:
    mov si, error_msg
    call print_string
    hlt
    jmp $

print_string:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    mov bh, 0
    mov bl, 0x07
    int 0x10
    jmp print_string
.done:
    ret

boot_drive:
    db 0

loading_msg:
    db 'Voyager toy bootloader: loading stage 2...',13,10,0

error_msg:
    db 'Disk read failed.',13,10,0

times 510 - ($ - $$) db 0
dw 0xAA55
