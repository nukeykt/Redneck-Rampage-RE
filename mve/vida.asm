.386

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'

ASSUME CS:_TEXT


PUBLIC _GenWinFuncPtr

EXTRN _vbe_SetWindowPosition:PROC
EXTRN _vbe_GetWindowPosition:PROC

_GenWinFuncPtr:
	push ebp
	mov ebp, esp
	add esp, 0FFFFFFFCh
	push ecx
	xor ecx, ecx
	mov cl, bl
	or bh, bh
	jnz short L1
	push edx
	push ecx
	call _vbe_SetWindowPosition
	add esp, 8
	jmp short L2
L1:
	cmp bh, 1
	jnz short L3
	lea eax, [ebp-4]
	push eax
	push ecx
	call _vbe_GetWindowPosition
	add esp, 8
	mov edx, [ebp-4]
	jmp short L2
L3:
	mov eax, 0
L2:
	or eax, eax
	jz short L4
	mov eax, 4Fh
	jmp short L5
L4:
	mov eax, 14Fh
L5:
	pop ecx
	leave
	retn
_TEXT ENDS

END
