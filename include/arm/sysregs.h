#ifndef _SYSREGS_H
#define _SYSREGS_H

// ***************************************
// SCTLR_EL1, System Control Register (EL1), Page 3080 ARMv8-A
// DDI0487D_a_armv8_arm.pdf file
// ***************************************

/*1.-Ordering and handling of multi load and multistore handled by ARM,
  multi load and store not trapped.
  2.-Value of privileged access never is left unchanged after taking exception,
  the taking of an exception to EL1 is a context synchronizing event.
  EL0 access to SCXTNUM_EL0 is disabled
  An exception return from EL1 is a context synchronizing event
*/

//Reserved values and set 1 and 2
#define SCTLR_RESERVED           (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)

//Endianness exception - set EL1 access to explicit data to little endian
#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)
//Explicit data accesses at EL0 are little-endian.
#define SCTLR_EOE_LITTLE_ENDIAN         (0 << 24)
//All instruction access from EL0 and EL1 are Non-cacheable
#define SCTLR_I_CACHE_DISABLED          (0 << 12)
//All data access to from EL0 and EL1 are Non-cacheable
#define SCTLR_D_CACHE_DISABLED          (0 << 2)
//EL1 and EL0 stage 1 address translation disabled.
#define SCTLR_MMU_DISABLED              (0 << 0)
//EL1 and EL0 stage 1 address translation enabled.
#define SCTLR_MMU_ENABLED               (1 << 0)

#define SCTLR_VALUE_MMU_DISABLED	(SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

// ***************************************
// HCR_EL2, Hypervisor Configuration Register (EL2), Page 2896
// ***************************************

//The Execution state for EL1 is AArch64 instead of 32 which is also possible
//No more configuration as we are not implementing supervisor
#define HCR_RW	    	                  (1 << 31)
#define HCR_VALUE			                  HCR_RW

// ***************************************
// SCR_EL3, Secure Configuration Register (EL3), Page 3072
// ***************************************

//Reserved
#define SCR_RESERVED	    		          (3 << 4)
//The next lower level is AArch64. If EL2 present AArch64 else EL1 AArch64
#define SCR_RW				                  (1 << 10)
/*Indicates that Exception levels lower than EL3 are in Non-secure state, and
so memory accesses from those Exception levels cannot access Secure memory.*/
#define SCR_NS				                  (1 << 0)

#define SCR_VALUE	    	    	          (SCR_RESERVED | SCR_RW | SCR_NS)

// ***************************************
// SPSR_EL3, Saved Program Status Register (EL3), Page 408
// ***************************************

//Mask SError interrupt mask bit, IRQ mask bit, FIQ mask bit, disable interrupts
#define SPSR_MASK_ALL 			             (7 << 6)
//AArch64 Exception level and selected Stack Pointer -EL1h
#define SPSR_EL1h			                   (5 << 0)

#define SPSR_VALUE_EL1	                 (SPSR_MASK_ALL | SPSR_EL1h)

// ***************************************
// SPSR_EL2, Saved Program Status Register (EL2), Page 408
// ***************************************
//AArch64 Exception level and selected Stack Pointer -EL2h
#define SPSR_EL2h			                   (9 << 0)

#define SPSR_VALUE_EL2	                 (SPSR_MASK_ALL | SPSR_EL2h)

#endif

/*Complementary info in
https://developer.arm.com/docs/ddi0595/b/aarch64-system-registers/*/
