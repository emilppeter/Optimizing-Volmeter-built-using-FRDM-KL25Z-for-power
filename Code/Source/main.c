/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "gpio_defs.h"
#include "LEDs.h"
#include "timers.h"
#include "delay.h"
#include "ADC.h"
#include "config.h"

void Init_Debug_Signals(void) {
	// Enable clock to ports E
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// Make 3 pins GPIO
	PORTE->PCR[DEBUG_RUNNING_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTE->PCR[DEBUG_RUNNING_POS] |= PORT_PCR_MUX(1);          
	
	// Set ports to outputs
	PTE->PDDR |= MASK(DEBUG_RUNNING_POS);
	
	PTE->PSOR |= MASK(DEBUG_RUNNING_POS);
}	
	

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	Init_Debug_Signals();
	Init_RGB_LEDs();
	Init_ADC();
		
	// Allow LPTMR0 to wake LLWU
	LLWU->ME |= LLWU_ME_WUME0_MASK;
	SMC->PMCTRL = 0x42U;	
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
	
	Init_LPTMR();
	Start_LPTMR();

	// work is performed in interrupt
	while (1) {
		PTE->PCOR |= MASK(DEBUG_RUNNING_POS);
		PTE->PSOR |= MASK(DEBUG_RUNNING_POS);
		__wfi();
	}
}

// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
