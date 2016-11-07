.8087
		PUBLIC	_VL_WaitVBL
		PUBLIC	_VL_SetCRTC
		PUBLIC	_VL_SetScreen
		PUBLIC	_VL_VideoID
		EXTRN	_TimeCount:BYTE
		EXTRN	_linewidth:BYTE
DGROUP		GROUP	_DATA
ID_VL_A_TEXT		SEGMENT	WORD PUBLIC USE16 'CODE'
		ASSUME CS:ID_VL_A_TEXT, DS:DGROUP, SS:DGROUP
_VL_WaitVBL:
	push		bp
	mov		bp,sp
	mov		dx,3daH
	mov		cx,word ptr 6[bp]
L$1:
	in		al,dx
	test		al,8
	jne		L$1
L$2:
	in		al,dx
	test		al,8
	je		L$2
	loop		L$1
	pop		bp
	retf
_VL_SetCRTC:
	push		bp
	mov		bp,sp
	cli
	mov		dx,3daH
L$3:
	in		al,dx
	test		al,1
	jne		L$3
	mov		cx,word ptr 6[bp]
	mov		dx,3d4H
	mov		al,0cH
	out		dx,al
	inc		dx
	mov		al,ch
	out		dx,al
	dec		dx
	mov		al,0dH
	out		dx,al
	mov		al,cl
	inc		dx
	out		dx,al
	sti
	pop		bp
	retf
_VL_SetScreen:
	push		bp
	mov		bp,sp
	mov		cx,word ptr DGROUP:_TimeCount
	add		cx,2
	mov		dx,3daH
L$4:
	in		al,dx
	test		al,1
	jne		L$4
L$5:
	sti
	jmp		L$6
L$6:
	cli
	cmp		word ptr DGROUP:_TimeCount,cx
	jae		L$7
	in		al,dx
	test		al,8
	jne		L$4
	test		al,1
	je		L$5
	in		al,dx
	test		al,8
	jne		L$4
	test		al,1
	je		L$5
	in		al,dx
	test		al,8
	jne		L$4
	test		al,1
	je		L$5
	in		al,dx
	test		al,8
	jne		L$4
	test		al,1
	je		L$5
	in		al,dx
	test		al,8
	jne		L$4
	test		al,1
	je		L$5
L$7:
	mov		cx,word ptr 6[bp]
	mov		dx,3d4H
	mov		al,0cH
	out		dx,al
	inc		dx
	mov		al,ch
	out		dx,al
	dec		dx
	mov		al,0dH
	out		dx,al
	mov		al,cl
	inc		dx
	out		dx,al
	mov		dx,3c0H
	mov		al,33H
	out		dx,al
	jmp		L$8
L$8:
	mov		al,byte ptr 8[bp]
	out		dx,al
	sti
	pop		bp
	retf
L$9:
	add		byte ptr [bx+si],al
	add		byte ptr [bx+si],al
L$10:
	add		al,byte ptr [bp+di]
	add		word ptr [bp+si],ax
	add		ax,word ptr [bx+di]
L$11:
	add		byte ptr [bx+si],al
	add		word ptr [bx+di],ax
	add		al,byte ptr [bp+si]
	add		byte ptr [bx+si],al
	add		ax,word ptr [bp+di]
	add		ax,word ptr [bx+di]
	add		byte ptr [bx+si],al
	add		ax,504H
	add		ax,0
	add		al,3
	add		al,4
	add		al,5
L$12:
    DB	1
    DW	offset ID_VL_A_TEXT+11eH
L$13:
	add		byte ptr 1[bp],ch
L$14:
    DB	0
    DW	offset ID_VL_A_TEXT+19aH
L$15:
    DB	0
    DW	offset L$25
_VL_VideoID:
	push		bp
	mov		bp,sp
	push		ds
	push		si
	push		di
	push		cs
	pop		ds
	mov		di,offset L$9
	mov		word ptr [di],0
	mov		word ptr 2[di],0
	mov		byte ptr L$14,1
	mov		byte ptr L$13,1
	mov		byte ptr L$15,1
	mov		cx,4
	mov		si,offset L$12
L$16:
	lodsb
	test		al,al
	lodsw
	je		L$17
	push		si
	push		cx
	call		ax
	pop		cx
	pop		si
L$17:
	loop		L$16
	call		near ptr L$33
	mov		al,byte ptr L$9
	mov		ah,0
	pop		di
	pop		si
	pop		ds
	mov		sp,bp
	pop		bp
	retf
	mov		ax,1a00H
	int		10H
	cmp		al,1aH
	jne		L$20
	mov		cx,bx
	xor		bh,bh
	or		ch,ch
	je		L$18
	mov		bl,ch
	add		bx,bx
	mov		ax,word ptr L$11[bx]
	mov		word ptr 2[di],ax
	mov		bl,cl
	xor		bh,bh
L$18:
	add		bx,bx
	mov		ax,word ptr L$11[bx]
	mov		word ptr [di],ax
	mov		byte ptr L$14,0
	mov		byte ptr L$13,0
	mov		byte ptr L$15,0
	lea		bx,[di]
	cmp		byte ptr [bx],1
	je		L$19
	lea		bx,2[di]
	cmp		byte ptr [bx],1
	jne		L$20
L$19:
	mov		word ptr [bx],0
	mov		byte ptr L$15,1
L$20:
	ret
L$21:
	mov		bl,10H
	mov		ah,12H
	int		10H
	cmp		bl,10H
	je		L$23
	mov		al,cl
	shr		al,1
	mov		bx,offset L$10
	xlatb
	mov		ah,al
	mov		al,3
	call		near ptr L$37
	cmp		ah,1
	je		L$22
	mov		byte ptr L$14,0
	jmp		L$23
L$22:
	mov		byte ptr L$15,0
L$23:
	ret
	mov		dx,3d4H
	call		near ptr L$30
	jb		L$24
	mov		al,2
	mov		ah,2
	call		near ptr L$37
L$24:
	ret
L$25:
	mov		dx,3b4H
	call		near ptr L$30
	jb		L$29
	mov		dl,0baH
	in		al,dx
	and		al,80H
	mov		ah,al
	mov		cx,8000H
L$26:
	in		al,dx
	and		al,80H
	cmp		ah,al
	loopz		L$26
	jne		L$27
	mov		al,1
	mov		ah,1
	call		near ptr L$37
	jmp		L$29
L$27:
	in		al,dx
	mov		dl,al
	and		dl,70H
	mov		ah,1
	mov		al,81H
	cmp		dl,10H
	je		L$28
	mov		al,80H
	cmp		dl,50H
	jne		L$28
	mov		al,82H
	mov		ah,3
L$28:
	call		near ptr L$37
L$29:
	ret
L$30:
	mov		al,0fH
	out		dx,al
	inc		dx
	in		al,dx
	mov		ah,al
	mov		al,66H
	out		dx,al
	mov		cx,100H
L$31:
	loop		L$31
	in		al,dx
	xchg		al,ah
	out		dx,al
	cmp		ah,66H
	je		L$32
	stc
L$32:
	ret
L$33:
	cmp		word ptr 2[di],0
	je		L$36
	cmp		byte ptr [di],4
	jge		L$36
	cmp		byte ptr 2[di],4
	jge		L$36
	mov		ah,0fH
	int		10H
	and		al,7
	cmp		al,7
	je		L$34
	cmp		byte ptr 1[di],1
	jne		L$36
	jmp		L$35
L$34:
	cmp		byte ptr 1[di],1
	je		L$36
L$35:
	mov		ax,word ptr [di]
	xchg		word ptr 2[di],ax
	mov		word ptr [di],ax
L$36:
	ret
L$37:
	lea		bx,[di]
	cmp		byte ptr [bx],0
	je		L$38
	lea		bx,2[di]
L$38:
	mov		word ptr [bx],ax
	ret
ID_VL_A_TEXT		ENDS
_DATA		SEGMENT	WORD PUBLIC USE16 'DATA'
    DB	0, 0

_DATA		ENDS
		END
