# watcom 9.5a, masm 6.1a

mvelib32.lib : vida.obj vbe.obj dpmi.obj mveliba.obj mvelib32.obj
	wlib -n mvelib32.lib -+mvelib32.obj -+mveliba.obj -+dpmi.obj -+vbe.obj -+vida.obj

.c.obj :
	wcc386p /4r /otexaf+ /i=..\sos $[*

.asm.obj :
	masm /mx $[*
