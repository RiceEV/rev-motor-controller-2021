//=============================================================================
// Copyright (C) 2018-2019, Qorvo, Inc.
//
// THIS SOFTWARE IS SUBJECT TO A SOURCE CODE LICENSE AGREEMENT WHICH PROVIDES,
// AMONG OTHER THINGS:  (i) THAT IT CAN BE USED ONLY TO ADAPT THE LICENSEE'S
// APPLICATION TO PAC PROCESSORS SUPPLIED BY QORVO, INC.;
// (ii) THAT  IT IS PROVIDED "AS IS" WITHOUT WARRANTY;  (iii) THAT
// QORVO, INC. IS NOT LIABLE FOR ANY INDIRECT DAMAGES OR FOR DIRECT
// DAMAGES EXCEEDING US$1,500;  AND (iv) THAT IT CAN BE DISCLOSED TO AND USED
// ONLY BY CERTAIN AUTHORIZED PERSONS.
//
//=============================================================================

// PAC55xx SDK Usage Notes
//
// - When using the pac55xx_sdk, include the pac5xxx.h file to access PAC55xx register structures
//     and definitions.
// - Add PAC55xx driver header files as needed for access to driver levels functions (see examples below).
// - It's also important to include one of the device header files before accessing the AFE registers
//     that are included in pac5xxx_tile... headers (see example below).  Each analog module in the AFE
//     is considered a "Tile".
// - Including the device header file defines the appropriate PAC55xx device (E.g. PAC5523)
//     and the associated CAFE Architecture (CAFE_ARCH1 or CAFE_ARCH2).

                                  // pac5xxx.h also includes most peripheral header files

//#include "pac5xxx.h"

#include "common.h"
#include "init.h"
//#define MOTORCONTROL            // Uncomment to initialize peripherals for motor control
//#define TEST                    // Uncomment for use when testing

// BLDC DIRECTION 0 LOGIC
//STATE TABLE		PWM-HI			PWM-LO			LO-SIDE		CurrentFlow		TriState	Comparator	Floating Phase	Polarity
//0			PWM4-A3(UH)		PWM0-A0(UL)		A2(WL)		U->W			V		SLCOMP8		V-Falling		1
//1			PWM5-A4(VH)		PWM1-A1((VL)	        A2(WL)		V->W			U		SLCOMP7		U-Rising		0
//2			PWM5-A4(VH)		PWM1-A1((VL)	        A0(UL)		V->U			W		SLCOMP9		W-Falling		1
//3			PWM6-A5(WH)		PWM2-A2(WL)		A0(UL)		W->U			V		SLCOMP8		V-Rising		0
//4			PWM6-A5(WH)		PWM2-A2(WL)		A1(VL)		W->V			U		SLCOMP7		U-Falling		1
//5			PWM4-A3(UH)		PWM0-A0(UL)		A1(VL)		U->V			W		SLCOMP9		W-Rising		0

// BLDC DIRECTION 1 LOGIC
//STATE TABLE		PWM-HI			PWM-LO			LO-SIDE		CurrentFlow		TriState	Comparator	Floating Phase	Polarity
//0			PWM4-A3(UH)		PWM0-A0(UL)		A2(WL)		U->W			V		SLCOMP8		V-Falling		1
//1			PWM4-A3(UH)		PWM0-A0(UL)		A1(VL)		U->V			W		SLCOMP9		W-Rising		0
//2			PWM6-A5(WH)		PWM2-A2(WL)		A1(VL)		W->V			U		SLCOMP7		U-Falling		1
//3			PWM6-A5(WH)		PWM2-A2(WL)		A0(UL)		W->U			V		LCOMP8		V-Rising		0
//4			PWM5-A4(VH)		PWM1-A1((VL)	        A0(UL)		V->U			W		SLCOMP9		W-Falling		1
//5		        PWM5-A4(VH)		PWM1-A1((VL)	        A2(WL)		V->W			U		SLCOMP7		U-Rising		0
		


                                                     //                                                                                                                                    0x0WVU0WVU 
//OG: 
EXTERN const uint32_t psel_mask[2][6] = 			{{0x00010001, 0x00100010, 0x00100010, 0x01000100, 0x01000100, 0x00010001},{0x00010001, 0x00010001, 0x01000100, 0x01000100, 0x00100010, 0x00100010}};
//EXTERN const uint32_t psel_mask[2][6] = 			{{0x00010100, 0x00100100, 0x00100001, 0x01000001, 0x01000010, 0x00010010},{0x00010100, 0x00010010, 0x01000010, 0x01000001, 0x00100001, 0x00100100}};

//EXTERN const uint8_t c_pwm_io_state[2][6] = 		{{0x14, 0x24, 0x21, 0x41, 0x42, 0x12},{0x14, 0x12, 0x42, 0x41, 0x21, 0x24}};
//EXTERN const uint8_t c_pwm_io_state[2][6] = {{0b00010100, 0b00100100, 0b00100001, 0b01000001, 0b01000010, 0b00010010},{0b00010100, 0b00010010, 0b01000010, 0b01000001, 0b00100001, 0b00100100}};


//OG: 
EXTERN const uint8_t c_pwm_io_state[2][6] = 		{{0x04, 0x04, 0x01, 0x01, 0x02, 0x02},{0x04, 0x02, 0x02, 0x01, 0x01, 0x04}};
//EXTERN const uint8_t c_pwm_hiz_state[2][6] = 		{{0x02, 0x01, 0x04, 0x02, 0x01, 0x04},{0x02, 0x04, 0x01, 0x02, 0x04, 0x01}};
//EXTERN const uint8_t slcomp_mux[2][6] = 			{{SLCOMP8, SLCOMP7, SLCOMP9, SLCOMP8, SLCOMP7, SLCOMP9},{SLCOMP8, SLCOMP9, SLCOMP7, SLCOMP8, SLCOMP9, SLCOMP7}};
//EXTERN const uint32_t slcomp_cross_polarity[2][6] = {{0x01, 0, 0x01, 0, 0x01, 0},{0, 0x01, 0, 0x01, 0, 0x01}};

//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0x03,0x02,0xFF,0x01,0x04,0xFF,0x05,0x00},{0x00,0x01,0xFF,0x02,0x05,0xFF,0x04,0x03}};  	//Motor Code ASVX
EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x03,0x05,0x04,0x01,0x02,0x00,0xFF},{0xFF,0x00,0x04,0x05,0x02,0x01,0x03,0xFF}};		//Motor Code BLY17





int main(void)
{
  
    for (volatile int i = 0; i < 1000000; i++);
    
    __disable_irq();
    system_init();
    //uart_init(UARTB, 115200);
    hall_sensor_init();
    gate_drive_init();
    //configure_timer_b_compare_mode();
    rpm_measure_init();
    cafe_init();
    can_init();
    motor_pwm_disable();
    __enable_irq();
    

   
    // Init complete, convey to aux board
    tx_data[0] = MOTOR_ID;
    can_transmit(1, INIT_COMPLETE_NOTIFY, tx_data); 
    
    

    while(1)
    {
      while(!millisecond);
        millisecond = 0;
        state_machine();
        tx_data[0] = 0xAA;
        //can_transmit(1, 0x0, tx_data);
        //tx_data[0] = 0xF0;
        //tx_data[1] = (uint8_t)(avg_speed >> 8) & 0xFF;
      //tx_data[2] = (uint8_t)(avg_speed & 0xFF);
      //can_transmit(3, SEND_MOTOR_SPEED, tx_data);
        //rx_data[0] = PAC55XX_TIMER_SEL->CTR.COUNTER & 0xFF;
        //rx_data[1] = (PAC55XX_TIMER_SEL->CTR.COUNTER >> 8) && 0xFF;
        //can_transmit(2, SEND_M
        
        //tx_data[0] = 0xF0;
        //tx_data[1] = (uint8_t)(motorspeed >> 8) & 0xFF;
      //tx_data[2] = (uint8_t)(motorspeed & 0xFF);
      //can_transmit(3, SEND_MOTOR_SPEED, tx_data);
  

    }
}

