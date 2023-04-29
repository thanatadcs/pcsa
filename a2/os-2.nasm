bits 16         ; Tell NASM this is 16 bit code
org 0x7c00      ; Tell NASM to start outputting stuff at offset 0x7c00


start:
    mov si, message     ; Point SI register to our message buffer
    mov ah, 0Eh         ; Specify 'int 10h' 'teletype output' function
	                ; [AL = Character, BH = Page Number, BL = Colour (in graphics mode)]
    mov di, name

.printMessage:
    lodsb               ; Load byte at address SI into AL, and increment SI
    cmp al, 0
    je  .getKey
    int 10h             ; Invoke the interupt to print out the character
    jmp .printMessage

.getKey:
    mov ah, 0h  	; service 0h Read key press
    int 16h
    mov ah, 0Eh 	; Print byte in AL
    int 10h
    cmp al, 13
    je .printName		
    stosb		; Store byte from AL to DI and increment DI
    cmp al, 8		; Check for backspace
    je .delete
    jmp .getKey

.delete:
    mov al, 32		; Print space
    int 10h
    mov al, 8		; Print backspace
    int 10h
    jmp .getKey

.printName:
    mov si, name
    mov ah, 0Eh         ; Specify 'int 10h' 'teletype output' function
    mov al, 10		; Print newline character
    int 10h

.repeat:
    lodsb               ; Load byte at address SI into AL, and increment SI
    cmp al, 0
    je .done 
    int 10h             ; Invoke the interupt to print out the character
    jmp .repeat 

.done:
    ;mov al, 10		; Print newline character
    ;int 10h
    hlt                 ; Halt execution

data:
	message db 'What is your name?', 32, 0
	name db ""

; Pad to 510 bytes (boot sector size minus 2) with 0s, and finish with the two-byte standard boot signature
times 510-($-$$) db 0 
dw 0xAA55       ; Magic bytes - marks this 512 byte sector bootable!
