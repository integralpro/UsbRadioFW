	.syntax unified
	.cpu cortex-m3
	.thumb

	.equ APB1PERIPH_BASE_H, 0x4000 @x0000
	.equ TIM_ADDR, 0x800

	.section .text.delay,"ax"

	.global delay_ms
@	.global delay_us

@	.type	delay_us, %function
@delay_us:
@	movw r1, #72-1
@	b delay_ms_cont
@
	.type	delay_ms, %function
delay_ms:
	movw r1, #0xffff
delay_ms_cont:
	mov.w r3, TIM_ADDR
	movt r3, APB1PERIPH_BASE_H

	str r1, [r3, #0x28] @PSC
	str r0, [r3, #0x2C] @ARR

	movs r1, #0x0000
	str r1, [r3, #0x24] @CNT

	movs r1, #0x0001
	str r1, [r3, #0x14] @EGR

	ldr r1, [r3, #0x10] @SR
	and r1, r1, #~0x0001
	str r1, [r3, #0x10] @SR

	movs r1, #0x09
	str r1, [r3, #0x00] @CR1

loop:
	ldr r1, [r3, #0x10] @SR
	tst r1, #0x0001
	wfi
	beq loop

	and r1, r1, #~0x0001
	str r1, [r3, #0x10] @SR

	bx lr
