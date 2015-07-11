ENTRY(start)
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
	.ctor :
	{
		___CTOR_LIST__ = .;
		*(.ctors);
		*(.ctor);
		*(SORT(.ctors.*));
		LONG (0);
		. = ALIGN(4);
	}
	.data :
	{
		data = .;
		*(.data)
		. = ALIGN(4);
	}
	.bss :
	{
		bss = .;
		*(.bss)
		. = ALIGN(4);
	}	
	end = .;
}
