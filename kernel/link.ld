ENTRY(begin)
STARTUP(begin.o)
SECTIONS
{
	. = 0x10000;
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
	.ctors :
	{
		___CTOR_LIST__ = .;
		LONG((___CTOR_END__ - ___CTOR_LIST__) / 4 - 2)
		*(.ctors)
		LONG(0)
		___CTOR_END__ = .;

		___INIT_ARRAY_LIST__ = .;
		LONG((___INIT_ARRAY_END__ - ___INIT_ARRAY_LIST__) / 4 - 2)
		*(.init_array)
		LONG(0)
		___INIT_ARRAY_END__ = .;

		/* dtor omitted. */

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
