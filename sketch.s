	.file	"sketch.cpp"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.global __do_copy_data
	.global __do_clear_bss
	.text
	.type	_ZL7FreeRamv, @function
_ZL7FreeRamv:
	push r29
	push r28
	push __tmp_reg__
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	lds r24,__brkval
	lds r25,(__brkval)+1
	sbiw r24,0
	brne .L2
	movw r24,r28
	adiw r24,1
	ldi r18,lo8(__bss_end)
	ldi r19,hi8(__bss_end)
	sub r24,r18
	sbc r25,r19
	std Y+2,r25
	std Y+1,r24
	rjmp .L3
.L2:
	movw r18,r28
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
	lds r24,__brkval
	lds r25,(__brkval)+1
	movw r20,r18
	sub r20,r24
	sbc r21,r25
	movw r24,r20
	std Y+2,r25
	std Y+1,r24
.L3:
	ldd r24,Y+1
	ldd r25,Y+2
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_ZL7FreeRamv, .-_ZL7FreeRamv
.global	_Z8free_ramv
	.type	_Z8free_ramv, @function
_Z8free_ramv:
	push r29
	push r28
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 6 */
	movw r18,r28
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
	std Y+6,r19
	std Y+5,r18
	lds r24,__brkval
	lds r25,(__brkval)+1
	sbiw r24,0
	breq .L6
	lds r24,__brkval
	lds r25,(__brkval)+1
	std Y+4,r25
	std Y+3,r24
	rjmp .L7
.L6:
	ldi r24,lo8(__heap_start)
	ldi r25,hi8(__heap_start)
	std Y+4,r25
	std Y+3,r24
.L7:
	ldd r24,Y+5
	ldd r25,Y+6
	ldd r18,Y+3
	ldd r19,Y+4
	sub r24,r18
	sbc r25,r19
/* epilogue start */
	adiw r28,6
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_Z8free_ramv, .-_Z8free_ramv
	.weak	_ZN5PrintaSERKS_
	.type	_ZN5PrintaSERKS_, @function
_ZN5PrintaSERKS_:
	push r29
	push r28
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
	std Y+2,r25
	std Y+1,r24
	std Y+4,r23
	std Y+3,r22
	ldd r30,Y+3
	ldd r31,Y+4
	ldd r24,Z+2
	ldd r25,Z+3
	ldd r30,Y+1
	ldd r31,Y+2
	std Z+3,r25
	std Z+2,r24
	ldd r24,Y+1
	ldd r25,Y+2
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_ZN5PrintaSERKS_, .-_ZN5PrintaSERKS_
	.weak	_ZN6StreamaSERKS_
	.type	_ZN6StreamaSERKS_, @function
_ZN6StreamaSERKS_:
	push r29
	push r28
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
	std Y+2,r25
	std Y+1,r24
	std Y+4,r23
	std Y+3,r22
	ldd r24,Y+1
	ldd r25,Y+2
	ldd r18,Y+3
	ldd r19,Y+4
	movw r22,r18
	call _ZN5PrintaSERKS_
	ldd r30,Y+3
	ldd r31,Y+4
	ldd r24,Z+4
	ldd r25,Z+5
	ldd r26,Z+6
	ldd r27,Z+7
	ldd r30,Y+1
	ldd r31,Y+2
	std Z+4,r24
	std Z+5,r25
	std Z+6,r26
	std Z+7,r27
	ldd r30,Y+3
	ldd r31,Y+4
	ldd r24,Z+8
	ldd r25,Z+9
	ldd r26,Z+10
	ldd r27,Z+11
	ldd r30,Y+1
	ldd r31,Y+2
	std Z+8,r24
	std Z+9,r25
	std Z+10,r26
	std Z+11,r27
	ldd r24,Y+1
	ldd r25,Y+2
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_ZN6StreamaSERKS_, .-_ZN6StreamaSERKS_
	.weak	_ZN4FileaSERKS_
	.type	_ZN4FileaSERKS_, @function
_ZN4FileaSERKS_:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,9
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 9 */
	std Y+2,r25
	std Y+1,r24
	std Y+4,r23
	std Y+3,r22
	ldd r24,Y+1
	ldd r25,Y+2
	ldd r18,Y+3
	ldd r19,Y+4
	movw r22,r18
	call _ZN6StreamaSERKS_
	ldd r24,Y+1
	ldd r25,Y+2
	ldd r18,Y+3
	ldd r19,Y+4
	movw r20,r24
	subi r20,lo8(-(12))
	sbci r21,hi8(-(12))
	std Y+6,r21
	std Y+5,r20
	movw r24,r18
	adiw r24,12
	std Y+8,r25
	std Y+7,r24
	ldi r25,lo8(13)
	std Y+9,r25
.L14:
	ldd r30,Y+7
	ldd r31,Y+8
	ld r0,Z
	ldd r20,Y+7
	ldd r21,Y+8
	subi r20,lo8(-(1))
	sbci r21,hi8(-(1))
	std Y+8,r21
	std Y+7,r20
	ldd r30,Y+5
	ldd r31,Y+6
	st Z,r0
	ldd r20,Y+5
	ldd r21,Y+6
	subi r20,lo8(-(1))
	sbci r21,hi8(-(1))
	std Y+6,r21
	std Y+5,r20
	ldd r21,Y+9
	subi r21,lo8(-(-1))
	std Y+9,r21
	ldd r24,Y+9
	tst r24
	brne .L14
	ldd r30,Y+3
	ldd r31,Y+4
	ldd r24,Z+25
	ldd r25,Z+26
	ldd r30,Y+1
	ldd r31,Y+2
	std Z+26,r25
	std Z+25,r24
	ldd r24,Y+1
	ldd r25,Y+2
/* epilogue start */
	adiw r28,9
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_ZN4FileaSERKS_, .-_ZN4FileaSERKS_
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
	push r16
	push r17
	push r29
	push r28
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
	std Y+2,r25
	std Y+1,r24
	std Y+4,r23
	std Y+3,r22
	ldd r24,Y+1
	ldd r25,Y+2
	cpi r24,1
	cpc r25,__zero_reg__
	brne .L17
	ldd r24,Y+3
	ldd r25,Y+4
	ldi r18,hi8(-1)
	cpi r24,lo8(-1)
	cpc r25,r18
	brne .L17
	ldi r24,lo8(aMotor)
	ldi r25,hi8(aMotor)
	ldi r22,lo8(15)
	ldi r23,hi8(15)
	ldi r20,lo8(14)
	ldi r21,hi8(14)
	ldi r18,lo8(16)
	ldi r19,hi8(16)
	ldi r16,lo8(0)
	ldi r17,hi8(0)
	call _ZN5MotorC1Eiiii
	ldi r24,lo8(bMotor)
	ldi r25,hi8(bMotor)
	ldi r22,lo8(18)
	ldi r23,hi8(18)
	ldi r20,lo8(17)
	ldi r21,hi8(17)
	ldi r18,lo8(19)
	ldi r19,hi8(19)
	ldi r16,lo8(0)
	ldi r17,hi8(0)
	call _ZN5MotorC1Eiiii
	ldi r24,lo8(ServoR)
	ldi r25,hi8(ServoR)
	call _ZN5ServoC1Ev
	ldi r24,lo8(ServoL)
	ldi r25,hi8(ServoL)
	call _ZN5ServoC1Ev
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4FileC1Ev
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommandC1Ev
	ldi r24,lo8(settings)
	ldi r25,hi8(settings)
	call _ZN8SettingsC1Ev
.L17:
	ldd r24,Y+1
	ldd r25,Y+2
	sbiw r24,0
	brne .L19
	ldd r24,Y+3
	ldd r25,Y+4
	ldi r18,hi8(-1)
	cpi r24,lo8(-1)
	cpc r25,r18
	brne .L19
	ldi r24,lo8(settings)
	ldi r25,hi8(settings)
	call _ZN8SettingsD1Ev
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4FileD1Ev
.L19:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	pop r16
	ret
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__D_aMotor, @function
_GLOBAL__D_aMotor:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(0)
	ldi r25,hi8(0)
	ldi r22,lo8(-1)
	ldi r23,hi8(-1)
	call _Z41__static_initialization_and_destruction_0ii
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	_GLOBAL__D_aMotor, .-_GLOBAL__D_aMotor
	.global __do_global_dtors
	.section .dtors,"a",@progbits
	.word	gs(_GLOBAL__D_aMotor)
	.text
	.type	_GLOBAL__I_aMotor, @function
_GLOBAL__I_aMotor:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(1)
	ldi r25,hi8(1)
	ldi r22,lo8(-1)
	ldi r23,hi8(-1)
	call _Z41__static_initialization_and_destruction_0ii
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	_GLOBAL__I_aMotor, .-_GLOBAL__I_aMotor
	.global __do_global_ctors
	.section .ctors,"a",@progbits
	.word	gs(_GLOBAL__I_aMotor)
	.text
.global	_Z8fireHeadhhhh
	.type	_Z8fireHeadhhhh, @function
_Z8fireHeadhhhh:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 8 */
	std Y+5,r24
	std Y+6,r22
	std Y+7,r20
	std Y+8,r18
	ldd r24,Y+5
	tst r24
	brne .L25
	ldd r24,Y+6
	tst r24
	brne .L25
	ldd r24,Y+7
	tst r24
	brne .L25
	ldd r24,Y+8
	tst r24
	brne .+2
	rjmp .L27
.L25:
	ldi r26,lo8(40)
	ldi r27,hi8(40)
	ldi r30,lo8(40)
	ldi r31,hi8(40)
	ld r25,Z
	ldd r24,Y+8
	or r24,r25
	st X,r24
	ldi r26,lo8(40)
	ldi r27,hi8(40)
	ldi r30,lo8(40)
	ldi r31,hi8(40)
	ld r24,Z
	mov r18,r24
	ldd r24,Y+6
	mov r24,r24
	ldi r25,lo8(0)
	swap r24
	swap r25
	andi r25,0xf0
	eor r25,r24
	andi r24,0xf0
	eor r25,r24
	or r24,r18
	st X,r24
/* #APP */
 ;  514 "src/sketch.ino" 1
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	
 ;  0 "" 2
/* #NOAPP */
	ldi r30,lo8(267)
	ldi r31,hi8(267)
	ldd r24,Y+7
	st Z,r24
	ldi r30,lo8(34)
	ldi r31,hi8(34)
	ldd r24,Y+5
	st Z,r24
	ldi r24,lo8(6)
	ldi r25,hi8(6)
	call delayMicroseconds
/* #APP */
 ;  536 "src/sketch.ino" 1
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	
 ;  0 "" 2
/* #NOAPP */
	ldi r30,lo8(34)
	ldi r31,hi8(34)
	st Z,__zero_reg__
	ldi r30,lo8(267)
	ldi r31,hi8(267)
	st Z,__zero_reg__
/* #APP */
 ;  547 "src/sketch.ino" 1
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	
 ;  0 "" 2
/* #NOAPP */
	ldi r30,lo8(40)
	ldi r31,hi8(40)
	st Z,__zero_reg__
.L27:
/* epilogue start */
	adiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_Z8fireHeadhhhh, .-_Z8fireHeadhhhh
	.data
.LC0:
	.string	"Resuming"
	.text
.global	_Z14resume_commandv
	.type	_Z14resume_commandv, @function
_Z14resume_commandv:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC0)
	ldi r19,hi8(.LC0)
	movw r22,r18
	call _ZN5Print7printlnEPKc
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	_Z14resume_commandv, .-_Z14resume_commandv
	.data
.LC1:
	.string	"Paused - enter R to resume"
	.text
.global	_Z13pause_commandv
	.type	_Z13pause_commandv, @function
_Z13pause_commandv:
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC1)
	ldi r19,hi8(.LC1)
	movw r22,r18
	call _ZN5Print7printlnEPKc
.L32:
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN14HardwareSerial4readEv
	std Y+1,__zero_reg__
	cpi r24,82
	cpc r25,__zero_reg__
	breq .L31
	ldi r24,lo8(1)
	std Y+1,r24
.L31:
	ldd r24,Y+1
	tst r24
	brne .L32
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC0)
	ldi r19,hi8(.LC0)
	movw r22,r18
	call _ZN5Print7printlnEPKc
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z13pause_commandv, .-_Z13pause_commandv
.global	_Z11serialEventv
	.type	_Z11serialEventv, @function
_Z11serialEventv:
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN14HardwareSerial4readEv
	std Y+1,r24
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldd r22,Y+1
	call _ZN13SerialCommand8add_byteEh
	ldd r24,Y+1
	cpi r24,lo8(13)
	brne .L35
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print7printlnEv
.L35:
	ldd r18,Y+1
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	mov r22,r18
	call _ZN5Print5printEc
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z11serialEventv, .-_Z11serialEventv
	.data
.LC2:
	.string	"File could not be opened: "
	.text
.global	_Z8readFilePc
	.type	_Z8readFilePc, @function
_Z8readFilePc:
	push r16
	push r17
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,49
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 49 */
	std Y+43,r25
	std Y+42,r24
	movw r24,r28
	adiw r24,5
	ldi r18,lo8(SD)
	ldi r19,hi8(SD)
	ldd r20,Y+42
	ldd r21,Y+43
	movw r22,r18
	ldi r18,lo8(1)
	call _ZN7SDClass4openEPKch
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	movw r18,r28
	subi r18,lo8(-(5))
	sbci r19,hi8(-(5))
	movw r22,r18
	call _ZN4FileaSERKS_
	movw r24,r28
	adiw r24,5
	call _ZN4FileD1Ev
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4FilecvbEv
	mov r25,r24
	ldi r24,lo8(1)
	eor r24,r25
	tst r24
	breq .L38
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC2)
	ldi r19,hi8(.LC2)
	movw r22,r18
	call _ZN5Print5printEPKc
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r18,Y+42
	ldd r19,Y+43
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L55
.L38:
	ldi r22,lo8(16711680)
	ldi r23,hi8(16711680)
	ldi r24,hlo8(16711680)
	ldi r25,hhi8(16711680)
	call _Z14setLEDToColourm
	rjmp .L40
.L54:
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4File4readEv
	std Y+32,r24
	ldd r24,Y+32
	mov r18,r24
	ldi r19,lo8(0)
	std Y+48,r19
	std Y+47,r18
	ldd r24,Y+47
	ldd r25,Y+48
	cpi r24,77
	cpc r25,__zero_reg__
	breq .L43
	ldd r18,Y+47
	ldd r19,Y+48
	cpi r18,109
	cpc r19,__zero_reg__
	breq .L43
	ldd r24,Y+47
	ldd r25,Y+48
	cpi r24,1
	cpc r25,__zero_reg__
	breq .+2
	rjmp .L41
.L42:
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
	rjmp .L44
.L45:
	ldd r24,Y+1
	ldd r25,Y+2
	movw r16,r24
	subi r16,lo8(-(1))
	sbci r17,hi8(-(1))
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4File4readEv
	mov r18,r24
	movw r24,r28
	adiw r24,32
	movw r30,r24
	add r30,r16
	adc r31,r17
	st Z,r18
	ldd r24,Y+1
	ldd r25,Y+2
	adiw r24,1
	std Y+2,r25
	std Y+1,r24
.L44:
	ldd r24,Y+1
	ldd r25,Y+2
	cpi r24,7
	cpc r25,__zero_reg__
	brlt .L45
	rjmp .L41
.L43:
	ldi r24,lo8(1)
	ldi r25,hi8(1)
	std Y+4,r25
	std Y+3,r24
	rjmp .L46
.L49:
	ldd r16,Y+3
	ldd r17,Y+4
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4File4readEv
	mov r18,r24
	movw r24,r28
	adiw r24,32
	movw r30,r24
	add r30,r16
	adc r31,r17
	st Z,r18
	ldd r24,Y+3
	ldd r25,Y+4
	adiw r24,1
	std Y+4,r25
	std Y+3,r24
.L46:
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4File4peekEv
	cpi r24,10
	cpc r25,__zero_reg__
	breq .L47
	ldd r24,Y+3
	ldd r25,Y+4
	cpi r24,9
	cpc r25,__zero_reg__
	brge .L47
	ldi r25,lo8(1)
	std Y+46,r25
	rjmp .L48
.L47:
	std Y+46,__zero_reg__
.L48:
	ldd r24,Y+46
	tst r24
	brne .L49
	ldd r18,Y+3
	ldd r19,Y+4
	movw r24,r28
	adiw r24,32
	movw r30,r24
	add r30,r18
	adc r31,r19
	ldi r24,lo8(10)
	st Z,r24
.L41:
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN14HardwareSerial9availableEv
	std Y+45,__zero_reg__
	sbiw r24,0
	breq .L50
	ldi r18,lo8(1)
	std Y+45,r18
.L50:
	ldd r19,Y+45
	tst r19
	breq .L40
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN14HardwareSerial4peekEv
	std Y+44,__zero_reg__
	cpi r24,83
	cpc r25,__zero_reg__
	brne .L51
	ldi r24,lo8(1)
	std Y+44,r24
.L51:
	ldd r25,Y+44
	tst r25
	breq .L52
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4File5closeEv
	lds r24,xMotor
	lds r25,(xMotor)+1
	call _ZN5Motor14reset_positionEv
	lds r24,yMotor
	lds r25,(yMotor)+1
	call _ZN5Motor14reset_positionEv
	rjmp .L55
.L52:
	call _Z11serialEventv
.L40:
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4File9availableEv
	std Y+49,__zero_reg__
	sbiw r24,0
	breq .L53
	ldi r18,lo8(1)
	std Y+49,r18
.L53:
	ldd r19,Y+49
	tst r19
	breq .+2
	rjmp .L54
	ldi r22,lo8(65280)
	ldi r23,hi8(65280)
	ldi r24,hlo8(65280)
	ldi r25,hhi8(65280)
	call _Z14setLEDToColourm
	ldi r24,lo8(myFile)
	ldi r25,hi8(myFile)
	call _ZN4File5closeEv
.L55:
/* epilogue start */
	adiw r28,49
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	pop r17
	pop r16
	ret
	.size	_Z8readFilePc, .-_Z8readFilePc
	.data
.LC3:
	.string	"Printing file."
.LC4:
	.string	"Output.hex"
	.text
.global	_Z13print_commandv
	.type	_Z13print_commandv, @function
_Z13print_commandv:
	push r29
	push r28
	push __tmp_reg__
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC3)
	ldi r19,hi8(.LC3)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
	rjmp .L57
.L58:
	ldi r24,lo8(.LC4)
	ldi r25,hi8(.LC4)
	call _Z8readFilePc
	ldd r24,Y+1
	ldd r25,Y+2
	adiw r24,1
	std Y+2,r25
	std Y+1,r24
.L57:
	ldd r24,Y+1
	ldd r25,Y+2
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	brge .L58
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z13print_commandv, .-_Z13print_commandv
	.data
.LC5:
	.string	"RAM: "
	.text
.global	loop
	.type	loop, @function
loop:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC5)
	ldi r19,hi8(.LC5)
	movw r22,r18
	call _ZN5Print5printEPKc
	call _Z8free_ramv
	movw r18,r24
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	movw r22,r18
	ldi r20,lo8(10)
	ldi r21,hi8(10)
	call _ZN5Print7printlnEii
	ldi r22,lo8(500)
	ldi r23,hi8(500)
	ldi r24,hlo8(500)
	ldi r25,hhi8(500)
	call delay
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	loop, .-loop
	.data
.LC6:
	.string	"Lower/Raise"
.LC7:
	.string	"Raise"
.LC8:
	.string	"Lower"
	.text
.global	_Z13lower_commandv
	.type	_Z13lower_commandv, @function
_Z13lower_commandv:
	push r29
	push r28
	push __tmp_reg__
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC6)
	ldi r19,hi8(.LC6)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	ldi r24,lo8(ServoR)
	ldi r25,hi8(ServoR)
	call _ZN5Servo4readEv
	std Y+2,__zero_reg__
	cpi r24,45
	cpc r25,__zero_reg__
	brne .L63
	ldi r24,lo8(1)
	std Y+2,r24
.L63:
	ldd r24,Y+2
	tst r24
	breq .L64
	ldi r24,lo8(ServoR)
	ldi r25,hi8(ServoR)
	ldi r22,lo8(20)
	ldi r23,hi8(20)
	call _ZN5Servo5writeEi
	ldi r24,lo8(ServoL)
	ldi r25,hi8(ServoL)
	ldi r22,lo8(45)
	ldi r23,hi8(45)
	call _ZN5Servo5writeEi
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC7)
	ldi r19,hi8(.LC7)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L67
.L64:
	ldi r24,lo8(ServoR)
	ldi r25,hi8(ServoR)
	call _ZN5Servo4readEv
	std Y+1,__zero_reg__
	cpi r24,20
	cpc r25,__zero_reg__
	brne .L66
	ldi r24,lo8(1)
	std Y+1,r24
.L66:
	ldd r24,Y+1
	tst r24
	breq .L67
	ldi r24,lo8(ServoR)
	ldi r25,hi8(ServoR)
	ldi r22,lo8(45)
	ldi r23,hi8(45)
	call _ZN5Servo5writeEi
	ldi r24,lo8(ServoL)
	ldi r25,hi8(ServoL)
	ldi r22,lo8(20)
	ldi r23,hi8(20)
	call _ZN5Servo5writeEi
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC8)
	ldi r19,hi8(.LC8)
	movw r22,r18
	call _ZN5Print7printlnEPKc
.L67:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z13lower_commandv, .-_Z13lower_commandv
.global	_Z15motor_from_axish
	.type	_Z15motor_from_axish, @function
_Z15motor_from_axish:
	push r29
	push r28
	rcall .
	push __tmp_reg__
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 5 */
	std Y+1,r24
	ldd r24,Y+1
	mov r24,r24
	ldi r25,lo8(0)
	call toupper
	cpi r24,88
	cpc r25,__zero_reg__
	brne .L69
	lds r24,xMotor
	lds r25,(xMotor)+1
	std Y+3,r25
	std Y+2,r24
	rjmp .L70
.L69:
	ldd r24,Y+1
	mov r24,r24
	ldi r25,lo8(0)
	call toupper
	cpi r24,89
	cpc r25,__zero_reg__
	brne .L71
	lds r24,yMotor
	lds r25,(yMotor)+1
	std Y+3,r25
	std Y+2,r24
	rjmp .L70
.L71:
	rjmp .L68
.L70:
	ldd r24,Y+2
	ldd r25,Y+3
	std Y+5,r25
	std Y+4,r24
.L68:
	ldd r24,Y+4
	ldd r25,Y+5
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z15motor_from_axish, .-_Z15motor_from_axish
	.data
.LC9:
	.string	"Missing axis parameter"
.LC10:
	.string	"Missing power parameter"
.LC11:
	.string	"No axis"
.LC12:
	.string	"Unknown power"
	.text
.global	_Z13power_commandv
	.type	_Z13power_commandv, @function
_Z13power_commandv:
	push r29
	push r28
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 6 */
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+6,r25
	std Y+5,r24
	ldd r24,Y+5
	ldd r25,Y+6
	sbiw r24,0
	brne .L74
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC9)
	ldi r19,hi8(.LC9)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L82
.L74:
	ldd r30,Y+5
	ldd r31,Y+6
	ld r24,Z
	std Y+4,r24
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+6,r25
	std Y+5,r24
	ldd r24,Y+5
	ldd r25,Y+6
	sbiw r24,0
	brne .L76
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC10)
	ldi r19,hi8(.LC10)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L82
.L76:
	ldd r30,Y+5
	ldd r31,Y+6
	ld r24,Z
	std Y+3,r24
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
	ldd r24,Y+4
	clr r25
	sbrc r24,7
	com r25
	call toupper
	cpi r24,88
	cpc r25,__zero_reg__
	brne .L77
	lds r24,xMotor
	lds r25,(xMotor)+1
	std Y+2,r25
	std Y+1,r24
	rjmp .L78
.L77:
	ldd r24,Y+4
	clr r25
	sbrc r24,7
	com r25
	call toupper
	cpi r24,89
	cpc r25,__zero_reg__
	brne .L79
	lds r24,yMotor
	lds r25,(yMotor)+1
	std Y+2,r25
	std Y+1,r24
	rjmp .L78
.L79:
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC11)
	ldi r19,hi8(.LC11)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L82
.L78:
	ldd r24,Y+3
	cpi r24,lo8(48)
	brne .L80
	ldd r24,Y+1
	ldd r25,Y+2
	ldi r22,lo8(0)
	call _ZN5Motor5powerEb
	rjmp .L82
.L80:
	ldd r24,Y+3
	cpi r24,lo8(49)
	brne .L81
	ldd r24,Y+1
	ldd r25,Y+2
	ldi r22,lo8(1)
	call _ZN5Motor5powerEb
	rjmp .L82
.L81:
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC12)
	ldi r19,hi8(.LC12)
	movw r22,r18
	call _ZN5Print7printlnEPKc
.L82:
/* epilogue start */
	adiw r28,6
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_Z13power_commandv, .-_Z13power_commandv
.global	_Z4movehl
	.type	_Z4movehl, @function
_Z4movehl:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 7 */
	std Y+3,r24
	std Y+4,r20
	std Y+5,r21
	std Y+6,r22
	std Y+7,r23
	ldd r24,Y+3
	call _Z15motor_from_axish
	std Y+2,r25
	std Y+1,r24
	ldd r24,Y+4
	ldd r25,Y+5
	ldd r26,Y+6
	ldd r27,Y+7
	sbiw r24,0
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	brne .L84
	ldd r24,Y+1
	ldd r25,Y+2
	call _ZN5Motor14reset_positionEv
	rjmp .L86
.L84:
	ldd r24,Y+1
	ldd r25,Y+2
	ldd r18,Y+4
	ldd r19,Y+5
	ldd r20,Y+6
	ldd r21,Y+7
	movw r22,r20
	movw r20,r18
	call _ZN5Motor4moveEl
.L86:
/* epilogue start */
	adiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_Z4movehl, .-_Z4movehl
.global	_Z12home_commandv
	.type	_Z12home_commandv, @function
_Z12home_commandv:
	push r29
	push r28
	push __tmp_reg__
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	rjmp .L88
.L91:
	lds r24,xMotor
	lds r25,(xMotor)+1
	ldi r20,lo8(-1)
	ldi r21,hi8(-1)
	ldi r22,hlo8(-1)
	ldi r23,hhi8(-1)
	call _ZN5Motor4moveEl
	lds r24,yMotor
	lds r25,(yMotor)+1
	ldi r20,lo8(-1)
	ldi r21,hi8(-1)
	ldi r22,hlo8(-1)
	ldi r23,hhi8(-1)
	call _ZN5Motor4moveEl
.L88:
	ldi r30,lo8(47)
	ldi r31,hi8(47)
	ld r24,Z
	mov r24,r24
	ldi r25,lo8(0)
	andi r24,lo8(1)
	andi r25,hi8(1)
	mov r25,r24
	ldi r24,lo8(1)
	eor r25,r24
	ldi r24,lo8(1)
	eor r24,r25
	tst r24
	brne .L89
	ldi r30,lo8(256)
	ldi r31,hi8(256)
	ld r24,Z
	mov r24,r24
	ldi r25,lo8(0)
	andi r24,lo8(8)
	andi r25,hi8(8)
	sbiw r24,0
	brne .L89
	ldi r24,lo8(1)
	std Y+2,r24
	rjmp .L90
.L89:
	std Y+2,__zero_reg__
.L90:
	ldd r24,Y+2
	tst r24
	brne .L91
	rjmp .L92
.L93:
	lds r24,xMotor
	lds r25,(xMotor)+1
	ldi r20,lo8(-1)
	ldi r21,hi8(-1)
	ldi r22,hlo8(-1)
	ldi r23,hhi8(-1)
	call _ZN5Motor4moveEl
.L92:
	ldi r30,lo8(47)
	ldi r31,hi8(47)
	ld r24,Z
	mov r24,r24
	ldi r25,lo8(0)
	andi r24,lo8(1)
	andi r25,hi8(1)
	mov r25,r24
	ldi r24,lo8(1)
	eor r24,r25
	tst r24
	brne .L93
	rjmp .L94
.L96:
	lds r24,yMotor
	lds r25,(yMotor)+1
	ldi r20,lo8(-1)
	ldi r21,hi8(-1)
	ldi r22,hlo8(-1)
	ldi r23,hhi8(-1)
	call _ZN5Motor4moveEl
.L94:
	ldi r30,lo8(256)
	ldi r31,hi8(256)
	ld r24,Z
	mov r24,r24
	ldi r25,lo8(0)
	andi r24,lo8(8)
	andi r25,hi8(8)
	std Y+1,__zero_reg__
	sbiw r24,0
	brne .L95
	ldi r24,lo8(1)
	std Y+1,r24
.L95:
	ldd r24,Y+1
	tst r24
	brne .L96
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z12home_commandv, .-_Z12home_commandv
	.data
.LC13:
	.string	"Missing steps parameter"
	.text
.global	_Z12move_commandv
	.type	_Z12move_commandv, @function
_Z12move_commandv:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 7 */
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+7,r25
	std Y+6,r24
	ldd r24,Y+6
	ldd r25,Y+7
	sbiw r24,0
	brne .L99
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC9)
	ldi r19,hi8(.LC9)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L102
.L99:
	ldd r30,Y+6
	ldd r31,Y+7
	ld r24,Z
	std Y+5,r24
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+7,r25
	std Y+6,r24
	ldd r24,Y+6
	ldd r25,Y+7
	sbiw r24,0
	brne .L101
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC13)
	ldi r19,hi8(.LC13)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L102
.L101:
	ldd r24,Y+6
	ldd r25,Y+7
	call atol
	movw r26,r24
	movw r24,r22
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	ldd r24,Y+5
	ldd r18,Y+1
	ldd r19,Y+2
	ldd r20,Y+3
	ldd r21,Y+4
	movw r22,r20
	movw r20,r18
	call _Z4movehl
.L102:
/* epilogue start */
	adiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_Z12move_commandv, .-_Z12move_commandv
	.data
.LC14:
	.string	"Missing speed parameter"
	.text
.global	_Z13speed_commandv
	.type	_Z13speed_commandv, @function
_Z13speed_commandv:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,9
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 9 */
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+9,r25
	std Y+8,r24
	ldd r24,Y+8
	ldd r25,Y+9
	sbiw r24,0
	brne .L104
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC9)
	ldi r19,hi8(.LC9)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L108
.L104:
	ldd r30,Y+8
	ldd r31,Y+9
	ld r24,Z
	std Y+7,r24
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+9,r25
	std Y+8,r24
	ldd r24,Y+8
	ldd r25,Y+9
	sbiw r24,0
	brne .L106
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC14)
	ldi r19,hi8(.LC14)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L108
.L106:
	ldd r24,Y+8
	ldd r25,Y+9
	call atol
	movw r26,r24
	movw r24,r22
	std Y+3,r24
	std Y+4,r25
	std Y+5,r26
	std Y+6,r27
	ldd r24,Y+3
	ldd r25,Y+4
	ldd r26,Y+5
	ldd r27,Y+6
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	cpc __zero_reg__,r26
	cpc __zero_reg__,r27
	brlt .L107
	ldi r24,lo8(1)
	ldi r25,hi8(1)
	ldi r26,hlo8(1)
	ldi r27,hhi8(1)
	std Y+3,r24
	std Y+4,r25
	std Y+5,r26
	std Y+6,r27
.L107:
	ldd r24,Y+7
	call _Z15motor_from_axish
	std Y+2,r25
	std Y+1,r24
	ldd r18,Y+3
	ldd r19,Y+4
	ldd r24,Y+1
	ldd r25,Y+2
	movw r22,r18
	call _ZN5Motor9set_speedEi
.L108:
/* epilogue start */
	adiw r28,9
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_Z13speed_commandv, .-_Z13speed_commandv
	.data
.LC15:
	.string	"Writing setting."
.LC16:
	.string	"Missing address parameter"
.LC17:
	.string	"Missing value parameter"
.LC18:
	.string	" = 0x"
	.text
.global	_Z21write_setting_commandv
	.type	_Z21write_setting_commandv, @function
_Z21write_setting_commandv:
	push r29
	push r28
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC15)
	ldi r19,hi8(.LC15)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+4,r25
	std Y+3,r24
	ldd r24,Y+3
	ldd r25,Y+4
	sbiw r24,0
	brne .L110
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC16)
	ldi r19,hi8(.LC16)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L113
.L110:
	ldd r24,Y+3
	ldd r25,Y+4
	call atoi
	std Y+2,r24
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+4,r25
	std Y+3,r24
	ldd r24,Y+3
	ldd r25,Y+4
	sbiw r24,0
	brne .L112
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC17)
	ldi r19,hi8(.LC17)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L113
.L112:
	ldd r24,Y+3
	ldd r25,Y+4
	call atoi
	std Y+1,r24
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r22,Y+2
	ldi r20,lo8(10)
	ldi r21,hi8(10)
	call _ZN5Print5printEhi
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC18)
	ldi r19,hi8(.LC18)
	movw r22,r18
	call _ZN5Print5printEPKc
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r22,Y+1
	ldi r20,lo8(16)
	ldi r21,hi8(16)
	call _ZN5Print7printlnEhi
	ldi r24,lo8(settings)
	ldi r25,hi8(settings)
	ldd r22,Y+2
	ldd r20,Y+1
	call _ZN8Settings10write_byteEhh
.L113:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z21write_setting_commandv, .-_Z21write_setting_commandv
.global	_Z20read_setting_commandv
	.type	_Z20read_setting_commandv, @function
_Z20read_setting_commandv:
	push r29
	push r28
	rcall .
	push __tmp_reg__
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 5 */
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+5,r25
	std Y+4,r24
	ldd r24,Y+4
	ldd r25,Y+5
	sbiw r24,0
	brne .L115
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC16)
	ldi r19,hi8(.LC16)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L117
.L115:
	ldd r24,Y+4
	ldd r25,Y+5
	call atoi
	std Y+3,r25
	std Y+2,r24
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r18,Y+2
	ldd r19,Y+3
	movw r22,r18
	ldi r20,lo8(10)
	ldi r21,hi8(10)
	call _ZN5Print5printEii
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC18)
	ldi r19,hi8(.LC18)
	movw r22,r18
	call _ZN5Print5printEPKc
	ldd r18,Y+2
	ldi r24,lo8(settings)
	ldi r25,hi8(settings)
	mov r22,r18
	call _ZN8Settings9read_byteEh
	std Y+1,r24
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r22,Y+1
	ldi r20,lo8(16)
	ldi r21,hi8(16)
	call _ZN5Print7printlnEhi
.L117:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z20read_setting_commandv, .-_Z20read_setting_commandv
	.data
.LC19:
	.string	" = "
	.text
.global	_Z17read_long_commandv
	.type	_Z17read_long_commandv, @function
_Z17read_long_commandv:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 8 */
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+8,r25
	std Y+7,r24
	ldd r24,Y+7
	ldd r25,Y+8
	sbiw r24,0
	brne .L119
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC16)
	ldi r19,hi8(.LC16)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L121
.L119:
	ldd r24,Y+7
	ldd r25,Y+8
	call atoi
	std Y+6,r25
	std Y+5,r24
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r18,Y+5
	ldd r19,Y+6
	movw r22,r18
	ldi r20,lo8(10)
	ldi r21,hi8(10)
	call _ZN5Print5printEii
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC19)
	ldi r19,hi8(.LC19)
	movw r22,r18
	call _ZN5Print5printEPKc
	ldd r18,Y+5
	ldi r24,lo8(settings)
	ldi r25,hi8(settings)
	mov r22,r18
	call _ZN8Settings9read_longEh
	movw r26,r24
	movw r24,r22
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r18,Y+1
	ldd r19,Y+2
	ldd r20,Y+3
	ldd r21,Y+4
	movw r22,r20
	movw r20,r18
	ldi r18,lo8(10)
	ldi r19,hi8(10)
	call _ZN5Print7printlnEli
.L121:
/* epilogue start */
	adiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_Z17read_long_commandv, .-_Z17read_long_commandv
	.data
.LC20:
	.string	"Axis Settings:"
	.text
.global	_Z14print_settingsP12AxisSettings
	.type	_Z14print_settingsP12AxisSettings, @function
_Z14print_settingsP12AxisSettings:
	push r29
	push r28
	push __tmp_reg__
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	std Y+2,r25
	std Y+1,r24
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC20)
	ldi r19,hi8(.LC20)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	ldd r30,Y+1
	ldd r31,Y+2
	ld r18,Z
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	mov r22,r18
	ldi r20,lo8(10)
	ldi r21,hi8(10)
	call _ZN5Print7printlnEhi
	ldd r30,Y+1
	ldd r31,Y+2
	ldd r24,Z+1
	mov r18,r24
	ldi r19,lo8(0)
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	movw r22,r18
	ldi r20,lo8(10)
	ldi r21,hi8(10)
	call _ZN5Print7printlnEii
	ldd r30,Y+1
	ldd r31,Y+2
	ldd r18,Z+2
	ldd r19,Z+3
	ldd r20,Z+4
	ldd r21,Z+5
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	movw r22,r20
	movw r20,r18
	ldi r18,lo8(10)
	ldi r19,hi8(10)
	call _ZN5Print7printlnEli
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_Z14print_settingsP12AxisSettings, .-_Z14print_settingsP12AxisSettings
	.data
.LC21:
	.string	"Writing long."
	.text
.global	_Z18write_long_commandv
	.type	_Z18write_long_commandv, @function
_Z18write_long_commandv:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 7 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC21)
	ldi r19,hi8(.LC21)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+7,r25
	std Y+6,r24
	ldd r24,Y+6
	ldd r25,Y+7
	sbiw r24,0
	brne .L125
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC16)
	ldi r19,hi8(.LC16)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L128
.L125:
	ldd r24,Y+6
	ldd r25,Y+7
	call atoi
	std Y+5,r24
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	call _ZN13SerialCommand4nextEv
	std Y+7,r25
	std Y+6,r24
	ldd r24,Y+6
	ldd r25,Y+7
	sbiw r24,0
	brne .L127
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC17)
	ldi r19,hi8(.LC17)
	movw r22,r18
	call _ZN5Print7printlnEPKc
	rjmp .L128
.L127:
	ldd r24,Y+6
	ldd r25,Y+7
	call atol
	movw r26,r24
	movw r24,r22
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r22,Y+5
	ldi r20,lo8(10)
	ldi r21,hi8(10)
	call _ZN5Print5printEhi
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC19)
	ldi r19,hi8(.LC19)
	movw r22,r18
	call _ZN5Print5printEPKc
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r18,Y+1
	ldd r19,Y+2
	ldd r20,Y+3
	ldd r21,Y+4
	movw r22,r20
	movw r20,r18
	ldi r18,lo8(10)
	ldi r19,hi8(10)
	call _ZN5Print7printlnEli
	ldi r24,lo8(settings)
	ldi r25,hi8(settings)
	ldd r18,Y+1
	ldd r19,Y+2
	ldd r20,Y+3
	ldd r21,Y+4
	ldd r22,Y+5
	call _ZN8Settings10write_longEhl
.L128:
/* epilogue start */
	adiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_Z18write_long_commandv, .-_Z18write_long_commandv
	.data
.LC22:
	.string	"c"
.LC23:
	.string	"calibrate"
.LC24:
	.string	"m"
.LC25:
	.string	"M"
.LC26:
	.string	"h"
.LC27:
	.string	"x"
.LC28:
	.string	"s"
.LC29:
	.string	"l"
.LC30:
	.string	"p"
.LC31:
	.string	"P"
.LC32:
	.string	"R"
.LC33:
	.string	"?"
.LC34:
	.string	"!"
.LC35:
	.string	"?L"
.LC36:
	.string	"!L"
.LC37:
	.string	"SD card could not be accessed.."
.LC38:
	.ascii	"Press p to print"
	.string	" Output.hex, S to stop, P to pause, R to resume, c to calibrate."
	.text
.global	setup
	.type	setup, @function
setup:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 7 */
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r20,lo8(9600)
	ldi r21,hi8(9600)
	ldi r22,hlo8(9600)
	ldi r23,hhi8(9600)
	call _ZN14HardwareSerial5beginEm
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN14HardwareSerial5flushEv
	ldi r30,lo8(39)
	ldi r31,hi8(39)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r30,lo8(266)
	ldi r31,hi8(266)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r30,lo8(33)
	ldi r31,hi8(33)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r24,lo8(66)
	ldi r22,lo8(0)
	call pinMode
	ldi r24,lo8(67)
	ldi r22,lo8(0)
	call pinMode
	ldi r24,lo8(68)
	ldi r22,lo8(0)
	call pinMode
	ldi r24,lo8(69)
	ldi r22,lo8(0)
	call pinMode
	ldi r24,lo8(5)
	ldi r22,lo8(0)
	call pinMode
	ldi r24,lo8(54)
	ldi r22,lo8(0)
	call pinMode
	ldi r24,lo8(55)
	ldi r22,lo8(0)
	call pinMode
	ldi r24,lo8(6)
	ldi r22,lo8(0)
	call pinMode
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC22)
	ldi r19,hi8(.LC22)
	ldi r20,lo8(gs(_Z11calibrationv))
	ldi r21,hi8(gs(_Z11calibrationv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC23)
	ldi r19,hi8(.LC23)
	ldi r20,lo8(gs(_Z11calibrationv))
	ldi r21,hi8(gs(_Z11calibrationv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC24)
	ldi r19,hi8(.LC24)
	ldi r20,lo8(gs(_Z12move_commandv))
	ldi r21,hi8(gs(_Z12move_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC25)
	ldi r19,hi8(.LC25)
	ldi r20,lo8(gs(_Z12move_commandv))
	ldi r21,hi8(gs(_Z12move_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC26)
	ldi r19,hi8(.LC26)
	ldi r20,lo8(gs(_Z12home_commandv))
	ldi r21,hi8(gs(_Z12home_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC27)
	ldi r19,hi8(.LC27)
	ldi r20,lo8(gs(_Z13power_commandv))
	ldi r21,hi8(gs(_Z13power_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC28)
	ldi r19,hi8(.LC28)
	ldi r20,lo8(gs(_Z13speed_commandv))
	ldi r21,hi8(gs(_Z13speed_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC29)
	ldi r19,hi8(.LC29)
	ldi r20,lo8(gs(_Z13lower_commandv))
	ldi r21,hi8(gs(_Z13lower_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC30)
	ldi r19,hi8(.LC30)
	ldi r20,lo8(gs(_Z13print_commandv))
	ldi r21,hi8(gs(_Z13print_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC31)
	ldi r19,hi8(.LC31)
	ldi r20,lo8(gs(_Z13pause_commandv))
	ldi r21,hi8(gs(_Z13pause_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC32)
	ldi r19,hi8(.LC32)
	ldi r20,lo8(gs(_Z14resume_commandv))
	ldi r21,hi8(gs(_Z14resume_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC33)
	ldi r19,hi8(.LC33)
	ldi r20,lo8(gs(_Z20read_setting_commandv))
	ldi r21,hi8(gs(_Z20read_setting_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC34)
	ldi r19,hi8(.LC34)
	ldi r20,lo8(gs(_Z21write_setting_commandv))
	ldi r21,hi8(gs(_Z21write_setting_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC35)
	ldi r19,hi8(.LC35)
	ldi r20,lo8(gs(_Z17read_long_commandv))
	ldi r21,hi8(gs(_Z17read_long_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	ldi r24,lo8(serial_command)
	ldi r25,hi8(serial_command)
	ldi r18,lo8(.LC36)
	ldi r19,hi8(.LC36)
	ldi r20,lo8(gs(_Z18write_long_commandv))
	ldi r21,hi8(gs(_Z18write_long_commandv))
	movw r22,r18
	call _ZN13SerialCommand10addCommandEPKcPFvvE
	call _Z7initLEDv
	ldi r24,lo8(settings)
	ldi r25,hi8(settings)
	ldi r22,lo8(88)
	movw r18,r28
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
	movw r20,r18
	call _ZN8Settings18read_axis_settingsEhP12AxisSettings
	movw r24,r28
	adiw r24,1
	call _Z14print_settingsP12AxisSettings
	ldi r22,lo8(100)
	ldi r23,hi8(100)
	ldi r24,hlo8(100)
	ldi r25,hhi8(100)
	call delay
	ldi r22,lo8(255)
	ldi r23,hi8(255)
	ldi r24,hlo8(255)
	ldi r25,hhi8(255)
	call _Z14setLEDToColourm
	ldi r24,lo8(SD)
	ldi r25,hi8(SD)
	ldi r22,lo8(53)
	call _ZN7SDClass5beginEh
	std Y+7,__zero_reg__
	tst r24
	brne .L130
	ldi r24,lo8(1)
	std Y+7,r24
.L130:
	ldd r24,Y+7
	tst r24
	breq .L131
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC37)
	ldi r19,hi8(.LC37)
	movw r22,r18
	call _ZN5Print7printlnEPKc
.L131:
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldi r18,lo8(.LC38)
	ldi r19,hi8(.LC38)
	movw r22,r18
	call _ZN5Print7printlnEPKc
/* epilogue start */
	adiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	setup, .-setup
	.type	_ZL13SerialPrint_PPKc, @function
_ZL13SerialPrint_PPKc:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 7 */
	std Y+6,r25
	std Y+5,r24
	rjmp .L134
.L136:
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	ldd r22,Y+4
	call _ZN14HardwareSerial5writeEh
	ldd r24,Y+5
	ldd r25,Y+6
	adiw r24,1
	std Y+6,r25
	std Y+5,r24
.L134:
	ldd r24,Y+5
	ldd r25,Y+6
	std Y+3,r25
	std Y+2,r24
	ldd r30,Y+2
	ldd r31,Y+3
/* #APP */
 ;  59 "/Applications/Arduino.app/Contents/Resources/Java/libraries/SD/utility/SdFatUtil.h" 1
	lpm r24, Z
	
 ;  0 "" 2
/* #NOAPP */
	std Y+1,r24
	ldd r24,Y+1
	std Y+4,r24
	ldd r24,Y+4
	std Y+7,r24
	ldd r24,Y+7
	tst r24
	breq .L135
	ldi r24,lo8(1)
	std Y+7,r24
.L135:
	ldd r24,Y+7
	tst r24
	brne .L136
/* epilogue start */
	adiw r28,7
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	_ZL13SerialPrint_PPKc, .-_ZL13SerialPrint_PPKc
	.type	_ZL15SerialPrintln_PPKc, @function
_ZL15SerialPrintln_PPKc:
	push r29
	push r28
	push __tmp_reg__
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	std Y+2,r25
	std Y+1,r24
	ldd r24,Y+1
	ldd r25,Y+2
	call _ZL13SerialPrint_PPKc
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print7printlnEv
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	_ZL15SerialPrintln_PPKc, .-_ZL15SerialPrintln_PPKc
.global	aMotor
.global	aMotor
	.section .bss
	.type	aMotor, @object
	.size	aMotor, 21
aMotor:
	.skip 21,0
.global	bMotor
.global	bMotor
	.type	bMotor, @object
	.size	bMotor, 21
bMotor:
	.skip 21,0
.global	xMotor
	.data
	.type	xMotor, @object
	.size	xMotor, 2
xMotor:
	.word	bMotor
.global	yMotor
	.type	yMotor, @object
	.size	yMotor, 2
yMotor:
	.word	aMotor
.global	ServoR
.global	ServoR
	.section .bss
	.type	ServoR, @object
	.size	ServoR, 3
ServoR:
	.skip 3,0
.global	ServoL
.global	ServoL
	.type	ServoL, @object
	.size	ServoL, 3
ServoL:
	.skip 3,0
.global	myFile
.global	myFile
	.type	myFile, @object
	.size	myFile, 27
myFile:
	.skip 27,0
.global	serial_command
.global	serial_command
	.type	serial_command, @object
	.size	serial_command, 44
serial_command:
	.skip 44,0
.global	settings
.global	settings
	.type	settings, @object
	.size	settings, 1
settings:
	.skip 1,0
