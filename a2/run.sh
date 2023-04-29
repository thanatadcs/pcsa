nasm -f bin os-2.nasm -o test.bin && qemu-system-x86_64 -drive format=raw,file=test.bin -display curses
