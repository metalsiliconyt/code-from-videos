arm-none-eabi-gcc -nostartfiles -T generic.ld examples.c -Wl,-Map,output.map -o output.elf
arm-none-eabi-nm -S --size-sort output.elf
arm-none-eabi-objdump -h output.elf