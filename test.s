.thumb
.syntax unified

.thumb_func
.global _start
.align 4
_start:
    movs r7, #0x04  @ write
	movs r0, #1     @ stdout
	ldr r1, =msg
	movs r2, #13
	svc #0

	movs r7, #1	   @ exit
	movs r0, #0
	svc #0

.align 4
msg:	.ascii "Hello World\n"
