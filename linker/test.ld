ENTRY(_start)

SECTIONS
{
    . = 0x400000;

    .text : ALIGN(16)
    {
        *(.text .text.*)
    }

    .rodata : ALIGN(16)
    {
        *(.rodata .rodata.*)
    }

    /*
     * Test registration table.
     *
     * Every TEST() puts a struct test_desc into
     * .libc_test.
     */
    .libc_test : ALIGN(16)
    {
        __libc_test_start = .;

        KEEP(*(.libc_test))

        __libc_test_end = .;
    }

    .data : ALIGN(16)
    {
        *(.data .data.*)
    }

    .bss : ALIGN(16)
    {
        *(COMMON)
        *(.bss .bss.*)
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame*)
    }
}
