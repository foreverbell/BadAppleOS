ENTRY(begin)
STARTUP(begin.o)
SECTIONS
{
	. = 0x9000;
	.text :
	{
		code = .;
		*(.code)
		. = ALIGN(4);
		*(.text)
		*(.rodata)
		. = ALIGN(4);
	}
	.data :
	{
		data = .;
		*(.data)
		. = ALIGN(4);
	}
	end = .;
}