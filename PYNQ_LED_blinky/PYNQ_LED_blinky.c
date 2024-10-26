/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

/* Include Files */
#include "platform.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "sleep.h"



//Interfaces of GPIO 0:
#define LED_CHANNEL 1	// GPIO port/interface for LEDs

//Instance of the GPIO Device Driver.

XGpio Gpio0;

void ToggleLeds(int ledData)
{
	XGpio_DiscreteWrite(&Gpio0, LED_CHANNEL, ledData);
	usleep(500*1000);
	return;
}

/* Main function. */
int main(void){

	int led= 0b00001111;


	// Variable to store the address of Gpio(s) predefined configuration
	XGpio_Config *cfg_ptr0;


	// Initialize xilinx platform
	xil_printf("initialization of the platform\n\r");
	init_platform();

	//----------------------------------------------------
	// INITIALIZE and CONFIGURE (or SETUP) the GPIO(s):
	//----------------------------------------------------
	//1. INTIALIZATION STEP:
		//1.1 Look up for the predefined configuration of the GPIO 0:
	cfg_ptr0 = XGpio_LookupConfig(XPAR_AXI_GPIO_0_DEVICE_ID); // GPIO 0
	// cfg_ptr1 = XGpio_LookupConfig(XPAR_AXI_GPIO_1_DEVICE_ID); // GPIO 1

		//1.2 Initialize GPIO 0:
	XGpio_CfgInitialize(&Gpio0, cfg_ptr0, cfg_ptr0->BaseAddress); // GPIO 0

	//2. CONFIGURE STEP
	//   Configure the interfaces of GPIO 0 in software
	//	 to be compatible with the hardware design of those Gpio(s):
	//On GPIO 0:
	XGpio_SetDataDirection(&Gpio0, LED_CHANNEL, 0x00); // channel 1 (leds), output mode


	//----------------------------------------------------
	// USING THE GPIO(s).
	//----------------------------------------------------

	while(1){
		ToggleLeds(led);
		ToggleLeds(~led);
	}
	// Cleanup the xilinx platform
	xil_printf("cleanup\n\r");
	cleanup_platform();

	return 0;
}
