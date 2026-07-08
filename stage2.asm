; Voyager 1 Inspired Toy OS - stage 2 monitor
;
; Loaded by voyager1_os.asm at 0000:8000.
; Educational demo only. Not for real hardware control.

bits 16
org 0x8000

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    sti

    mov ax, 0x0003
    int 0x10

    mov si, banner
    call print_string

    mov si, boot_line
    call print_string

    mov si, telemetry_header
    call print_string

main_loop:
    mov si, status_line
    call print_string
    call delay

    mov si, science_line
    call print_string
    call delay

    mov si, comms_line
    call print_string
    call delay

    mov si, nav_line
    call print_string
    call delay

    mov si, command_line
    call print_string
    call delay

    mov si, idle_line
    call print_string
    call delay

    jmp main_loop

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

delay:
    push cx
    push dx
    mov cx, 0x0018
.outer:
    mov dx, 0xFFFF
.inner:
    dec dx
    jne .inner
    loop .outer
    pop dx
    pop cx
    ret

banner:
    db 13,10
    db 'VOYAGER-1 EDUCATIONAL FLIGHT OS',13,10
    db '--------------------------------',13,10
    db 'Second-stage monitor loaded at 0000:8000',13,10
    db 13,10,0

boot_line:
    db '[BOOT] Memory map nominal. Console online.',13,10
    db '[BOOT] Deep-space monitor loop active.',13,10
    db '[BOOT] This is a simulation, not spacecraft code.',13,10
    db 13,10,0

telemetry_header:
    db 'TELEMETRY STREAM',13,10
    db '----------------',13,10,0

status_line:
    db '[BUS]  power: stable   thermal: stable   fault: none',13,10,0

science_line:
    db '[SCI]  cosmic-ray counter: sampling      plasma: standby',13,10,0

comms_line:
    db '[COMM] high-gain antenna: earth lock     carrier: simulated',13,10,0

nav_line:
    db '[NAV]  attitude: fixed inertial          star tracker: simulated',13,10,0

command_line:
    db '[CMD]  uplink window: closed             queue: empty',13,10,0

idle_line:
    db '[IDLE] waiting for next command window...',13,10,13,10,0
