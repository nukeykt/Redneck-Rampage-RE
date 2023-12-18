# watcom 9.5a, masm 6.11

mvelib32.lib : vida.obj vbe.obj dpmi.obj mveliba.obj mvelib32.obj
	wlib -n mvelib32.lib -+vida.obj -+vbe.obj -+dpmi.obj -+mveliba.obj -+mvelib32.obj

.c.obj :
	wcc386p /4r /otexaf+ /i=..\sos $[*

.asm.obj :
	masm /mx $[*
