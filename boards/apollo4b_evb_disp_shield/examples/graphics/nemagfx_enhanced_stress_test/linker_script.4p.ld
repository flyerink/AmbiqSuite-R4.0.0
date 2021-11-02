/******************************************************************************
 *
 * linker_script.ld - Linker script for applications using startup_gnu.c
 *
 *****************************************************************************/
ENTRY(Reset_Handler)

MEMORY
{
    MCU_MRAM     (rx)  : ORIGIN = 0x00000000, LENGTH = 1998848
    MCU_TCM      (rwx) : ORIGIN = 0x10000000, LENGTH = 393216
    MCU_SRAM     (rx)  : ORIGIN = 0x00000000, LENGTH = 0
    SHARED_SRAM  (rwx) : ORIGIN = 0x10060000, LENGTH = 1048576
    PSRAM        (rx)  : ORIGIN = 0x14100000, LENGTH = 1048576
}

SECTIONS
{
    .xip_text :
    {
        . = ALIGN(4);
        bin/loops-all-mid-10k-sp.o(.text)
        bin/loops-all-mid-10k-sp.o(.text*)
        bin/loops-all-mid-10k-sp.o(.rodata)
        bin/loops-all-mid-10k-sp.o(.rodata*)
        bin/loops.o(.text)
        bin/loops.o(.text*)
        bin/loops.o(.rodata)
        bin/loops.o(.rodata*)
        bin/1k.o(.text)
        bin/1k.o(.text*)
        bin/1k.o(.rodata)
        bin/1k.o(.rodata*)
        bin/10k.o(.text)
        bin/10k.o(.text*)
        bin/10k.o(.rodata)
        bin/10k.o(.rodata*)
        bin/100k.o(.text)
        bin/100k.o(.text*)
        bin/100k.o(.rodata)
        bin/100k.o(.rodata*)
        bin/1kdot.o(.text)
        bin/1kdot.o(.text*)
        bin/1kdot.o(.rodata)
        bin/1kdot.o(.rodata*)
        bin/10kdot.o(.text)
        bin/10kdot.o(.text*)
        bin/10kdot.o(.rodata)
        bin/10kdot.o(.rodata*)
        bin/100kdot.o(.text)
        bin/100kdot.o(.text*)
        bin/100kdot.o(.rodata)
        bin/100kdot.o(.rodata*)
        bin/100.o(.text)
        bin/100.o(.text*)
        bin/100.o(.rodata)
        bin/100.o(.rodata*)
        bin/32.o(.text)
        bin/32.o(.text*)
        bin/32.o(.rodata)
        bin/32.o(.rodata*)
        . = ALIGN(4);
    } > PSRAM

    .text :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        KEEP(*(.patch))
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > MCU_MRAM

    /* User stack section initialized by startup code. */
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        *(.stack)
        *(.stack*)
        . = ALIGN(8);
    } > MCU_TCM

    .heap : {
        __heap_start__ = .;
        end = __heap_start__;
        _end = end;
        __end = end;
        KEEP(*(.heap))
        __heap_end__ = .;
        __HeapLimit = __heap_end__;
    } > MCU_TCM

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > MCU_TCM AT>MCU_MRAM

    /* used by startup to initialize data */
    _init_data = LOADADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > MCU_TCM

    .mcu_sram :
    {
        . = ALIGN(4);
        *(.mcu_sram)
        . = ALIGN(4);
    } > MCU_SRAM AT>MCU_MRAM

    .shared :
    {
        . = ALIGN(4);
        KEEP(*(.resource_table))
        KEEP(*(.shared))
        . = ALIGN(4);
    } > SHARED_SRAM AT>MCU_MRAM
    .ARM.attributes 0 : { *(.ARM.attributes) }
}