export PATH=$PATH:/usr/local/i386elfgcc/bin

set fails=0

echo "Assembling..."
nasm "src/bootloader/boot.asm" -f bin -o "bin/boot.bin" -i src/bootloader
echo "Assembled."

echo "Compiling..."
i386-elf-gcc -ffreestanding -m32 -g -c "src/kernel.cpp" -o "bin/obj/kernel.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/drivers/port_io.cpp" -o "bin/obj/port_io.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/drivers/keyboard.cpp" -o "bin/obj/keyboard.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/cpu/irq.cpp" -o "bin/obj/irq.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/utils/conversions.cpp" -o "bin/obj/conversions.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/cpu/idt.cpp" -o "bin/obj/idt.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/utils/string.cpp" -o "bin/obj/string.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/cpu/isr.cpp" -o "bin/obj/isr.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/mem.cpp" -o "bin/obj/mem.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/cpu/timer.cpp" -o "bin/obj/timer.o" || let "fails=fails+1"
i386-elf-gcc -ffreestanding -m32 -g -c "src/drivers/vga_graphics.cpp" -o "bin/obj/vga_graphics.o" || let "fails=fails+1"
echo "Compiled."

echo "Linking..."
i386-elf-ld -T linker.ld -o "bin/full_kernel.bin" -Ttext 0x1000 \
    "bin/obj/kernel_entry.o" "bin/obj/kernel.o" "bin/obj/port_io.o" "bin/obj/keyboard.o" "bin/obj/irq.o" \
    "bin/obj/conversions.o" "bin/obj/idt.o" "bin/obj/string.o" "bin/obj/isr.o" "bin/obj/mem.o" \
     "bin/obj/timer.o" "bin/obj/vga_graphics.o" \
    --oformat binary || let "fails=fails+1"
echo "Linked."

# "bin/screen.o"

echo "Creating ISO..."
rm bin/out/os.iso
dd if=/dev/zero of=bin/out/os.iso bs=512 count=2880
dd if=./bin/boot.bin of=bin/out/os.iso conv=notrunc bs=512 seek=0 count=1
dd if=./bin/full_kernel.bin of=bin/out/os.iso conv=notrunc bs=512 seek=1 count=128
echo "Created."

echo "Running ISO..."
qemu-system-x86_64 -drive format=raw,file="bin/out/os.iso",index=0, -m 128M #-curses (This is for terminal stuff)