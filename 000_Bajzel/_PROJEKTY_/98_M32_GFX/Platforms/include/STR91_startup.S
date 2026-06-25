/* 	Startup code for STR91x ARM-based microcontrollers
	based on AT91SAM7S256 C startup source and some examples from www.stm.com 
	T.W. 2006
   
	This file contains ONLY the very low-level device initialization code such as memory setup
	and it shouldn't be modified (except stack sizes).
*/

.equ    Mode_USR,       0x10
.equ    Mode_FIQ,       0x11
.equ    Mode_IRQ,       0x12
.equ    Mode_SVC,       0x13
.equ    Mode_ABT,       0x17
.equ    Mode_UND,       0x1B
.equ    Mode_SYS,       0x1F

.equ VectorAddress,  0xFFFFF030 // VIC Vector address register address
.equ VectorAddressDaisy,    0xFC000030

.equ	SCRO_AHB_UNB,   0x5C002034

.equ    I_Bit,          0x80    /* when I bit is set, IRQ is disabled */
.equ    F_Bit,          0x40    /* when F bit is set, FIQ is disabled */

.equ    Top_Stack,      0x04017ff0
.equ    UND_Stack_Size, 0x0000100
.equ    SVC_Stack_Size, 0x0000100
.equ    ABT_Stack_Size, 0x0000100
.equ    FIQ_Stack_Size, 0x0000100
.equ    IRQ_Stack_Size, 0x00001000
.equ    USR_Stack_Size, 0x00003000

.equ	VIC_VAR,	0x30
.equ	VIC0_BASE,	0xFFFFF000

.text
.arm
.section .vectrom, "ax"

Vectors:        ldr     pc,Reset_Addr         	// RESET vector
                ldr     pc,Undef_Addr		// Undefined instruction vector
                ldr     pc,SWI_Addr		// Software interrupt vector
                ldr     pc,PAbt_Addr		// Prefetch abort vector
                ldr     pc,DAbt_Addr
                nop                            /* Reserved Vector */
		  ldr   pc,[pc,#-0xFF0]
		ldr	pc,FIQ_Addr

Reset_Addr:     .word   Reset_Handler
Undef_Addr:     .word   Undef_Handler
SWI_Addr:       .word   SWI_Handler
PAbt_Addr:      .word   PAbt_Handler
DAbt_Addr:      .word   DAbt_Handler
                .word   0                      /* Reserved Address */
IRQ_Addr:       .word   IRQ_Handler
FIQ_Addr:       .word   FIQ_Handler


Undef_Handler:  B       Undef_Handler
SWI_Handler:    B       SWI_Handler
PAbt_Handler:   B       PAbt_Handler
DAbt_Handler:   B       DAbt_Handler
IRQ_Handler:    B       IRQ_Handler
FIQ_Handler:    B       FIQ_Handler


// Starupt Code must be linked first at Address at which it expects to run.

.text
.arm
.section .init, "ax"
	
.global _startup
.func   _startup

_startup:

// Reset Handler

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

        ldr  pc, =Reset_Handler
Reset_Handler:

// Initialize flash memory controller



       LDR R6, =0x54000000
       LDR R7, =0x0
       STR R7, [R6]

       LDR R6, =0x54000004
       LDR R7, =0x6
       STR R7, [R6]

       LDR R6, =0x5400000C
       LDR R7, =0x0
       STR R7, [R6]

       LDR R6, =0x54000010
       LDR R7, =0x20000
       STR R7, [R6]

       LDR R6, =0x54000018
       LDR R7, =0x18
       STR R7, [R6]
        
// Enable 96K RAM
	nop
	nop
	nop

        ldr     r0, = SCRO_AHB_UNB
        ldr     r1, = 0x0196
        str     r1, [r0]

	nop
	nop

// Setup stack pointers
        LDR     R0, =Top_Stack

//  Enter Undefined Instruction Mode and set its Stack Pointer
        MSR     CPSR_c, #Mode_UND|I_Bit|F_Bit
        MOV     SP, R0
        SUB     R0, R0, #UND_Stack_Size

//  Enter Abort Mode and set its Stack Pointer
        MSR     CPSR_c, #Mode_ABT|I_Bit|F_Bit
        MOV     SP, R0
        SUB     R0, R0, #ABT_Stack_Size

//  Enter FIQ Mode and set its Stack Pointer
        MSR     CPSR_c, #Mode_FIQ|I_Bit|F_Bit
        MOV     SP, R0
        SUB     R0, R0, #FIQ_Stack_Size

//  Enter IRQ Mode and set its Stack Pointer
        MSR     CPSR_c, #Mode_IRQ|I_Bit|F_Bit
        MOV     SP, R0
        SUB     R0, R0, #IRQ_Stack_Size

//  Enter Supervisor Mode and set its Stack Pointer
        MSR     CPSR_c, #Mode_SVC|I_Bit|F_Bit
        MOV     SP, R0
        SUB     R0, R0, #SVC_Stack_Size



//  Enter User Mode and set its Stack Pointer
        MSR     CPSR_c, #Mode_SYS|I_Bit|F_Bit
        MOV     SP, R0

// Setup a default Stack Limit (when compiled with "-mapcs-stack-check")
//        SUB     SL, SP, #USR_Stack_Size


	MOV     r0, #0x60000
	MCR     p15,0x1,r0,c15,c1,0

// Relocate .data section (Copy from ROM to RAM)

        LDR     R1, =_etext
        LDR     R2, =_data
        LDR     R3, =_edata
LoopRel:
	CMP     R2, R3
        LDRLO   R0, [R1], #4
        STRLO   R0, [R2], #4
        BLO     LoopRel

// Clear .bss section (Zero init)

        MOV     R0, #0
        LDR     R1, =__bss_start__
        LDR     R2, =__bss_end__
LoopZI: 
	CMP     R1, R2
        STRLO   R0, [R1], #4
        BLO     LoopZI


// Enter C code - main()

	mov   r0,#0            // no arguments (argc = 0)
	mov   r1,r0
	mov   r2,r0
	mov   fp,r0            // null frame pointer
	mov   r7,r0            // null frame pointer for thumb
	ldr   r10,=main
	adr   lr, __main_exit
	bx    r10              // enter main()

__main_exit:    B       __main_exit

.size   _startup, . - _startup
.endfunc


.global IRQ_Handler_Entry
.func   IRQ_Handler_Entry

/*
IRQ_Handler_Entry:
        SUB    lr,lr,#4        // Update the link register

        STMFD  sp!,{r0-r12,lr} // Save The workspace plus the current return
                              // address lr_ mode into the stack.
        MRS    r1,spsr        // Save the spsr_mode into r1.
        STMFD  sp!,{r1}       // Save spsr.


        LDR    r0, = VectorAddress
        LDR    r0, [r0]                   // Read the routine address
        LDR    r1, = VectorAddressDaisy
        LDR    r1, [r1]
       // Padding between the acknowledge and re-enable of interrupts
       // For more details, please refer to the following URL
       // http://www.arm.com/support/faqip/3682.html
       NOP
       NOP
       MSR    cpsr_c,#0x1F            // Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                // Save the link register.
       LDR    lr, = IRQ_ReturnAddress // Read the return address.
       BX     r0                      // Branch to the IRQ handler.
IRQ_ReturnAddress:
       LDMFD  sp!,{lr}                // Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    // Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     // Write to the VectorAddress to clear the
       STR    r0, [r0]                // respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy  // Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                   // respective interrupt in the internal interrupt


        LDMFD   sp!,{r1}        // Restore the saved spsr_mode into r1.
        MSR     spsr_cxsf,r1    // Restore spsr_mode.
        LDMFD   sp!,{r0-r12,pc} // Return to the instruction following...

*/

IRQ_Handler_Entry:
        SUB    lr,lr,#4        // Update the link register

        STMFD  sp!,{r0-r12,lr} // Save The workspace plus the current return
        MRS    r1,spsr        // Save the spsr_mode into r1.
        STMFD  sp!,{r1}       // Save spsr.

        LDR    r0, = VectorAddress
        LDR    r0, [r0]                   // Read the routine address
        LDR    r1, = VectorAddressDaisy
        LDR    r1, [r1]
       // Padding between the acknowledge and re-enable of interrupts
       // For more details, please refer to the following URL
       // http://www.arm.com/support/faqip/3682.html
       NOP
       NOP
       MSR    cpsr_c,#0x1F            // Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                // Save the link register.
       LDR    lr, = IRQ_ReturnAddress // Read the return address.
       BX     r0                      // Branch to the IRQ handler.
IRQ_ReturnAddress:
       LDMFD  sp!,{lr}                // Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    // Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     // Write to the VectorAddress to clear the
       STR    r0, [r0]                // respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy  // Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                   // respective interrupt in the internal interrupt



        LDMFD   sp!,{r1}        // Restore the saved spsr_mode into r1.
        MSR     spsr_cxsf,r1    // Restore spsr_mode.
        LDMfd  sp!,{r0-r12,lr} // Return to the instruction following...
	subs pc, lr, #4

.endfunc


.end
