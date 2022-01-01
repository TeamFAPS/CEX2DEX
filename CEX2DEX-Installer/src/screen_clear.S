
	.align 4
	.text
	.cpu cortex-a9
	.arch armv7-a
	.syntax unified
	.thumb
	.thumb_func
	.fpu neon

	.global screen_clear
	.type   screen_clear, %function

@ int screen_clear(void *data, int width, int height, uint32_t color);

screen_clear:

	push {r4, r5, r6, r7, r8, r9, lr}

	mov r4, r1
	mov r5, r2
	mov r6, r1
	mov r7, r0

	mov r0, r3
	mov r1, r3
	mov r2, r3

height_copy:
	cmp r5, #0
	ble height_end			@ if(height <= 0)goto height_end;

	mov r4, r6

width_copy:
	cmp r4, #0
	ble width_end			@ if(width <= 0)goto width_end;

	stm r7!, {r0, r1, r2, r3}

	sub r4, #4
	b width_copy			@ goto width_copy;

width_end:
	sub r5, #1
	b height_copy			@ goto height_copy;

height_end:
	movs r0, #0			@ res = 0

	pop {r4, r5, r6, r7, r8, r9, pc}

	.data
