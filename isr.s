@Based on code from the GBA TONC tutorial

.section .iwram, "ax", %progbits
.align 2
.arm
.global callSysISR
callSysISR:
	@Save spsr and lr.
	mrs r1, spsr
	stmfd sp!, {r1, lr}
	mov r2, #0x6000000
	
	@Switch to system mode.
	mrs r1, cpsr
    bic r1, r1, #0xDF
    orr r1, r1, #0x1F
    msr cpsr, r1
	
	@Call the ISR.
	stmfd sp!, {r0, lr}
	add lr, pc, #0
	bx r0
	ldmfd sp!, {r0, lr}
	
	@Return to IRQ mode.
	mrs r1, cpsr
    bic r1, r1, #0xDF
    orr r1, r1, #0x92
    msr cpsr, r1
	
	@Restore spsr and lr.
	ldmfd sp!, {r1, lr}
	msr spsr, r1
	
	bx lr
	