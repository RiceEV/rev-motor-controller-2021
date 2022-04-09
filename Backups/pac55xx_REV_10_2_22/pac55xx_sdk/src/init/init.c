#include "pac5xxx.h" 
#include "init.h"

void system_init(void)
{
    // Set Flash Lock to allow write access to MEMCTL register for configuring clocks
    PAC55XX_MEMCTL->FLASHLOCK = FLASH_LOCK_ALLOW_WRITE_MEMCTL;
    
    // Turn on Flache Cache
    PAC55XX_MEMCTL->MEMCTL.CACHEDIS = 0;                        //Enable Flash CACHE

    // Always make sure MCLK is set to use ROSC CLK when making changes to FRCLK, SCLK, or HCLK
    PAC55XX_MEMCTL->MEMCTL.MCLKSEL = MEMCTL_MCLK_ROSCCLK;       // MCLK = ROSCCLK
    
    // Make sure SCLK is set to FRCLK instead of PLLCLK
    PAC55XX_SCC->CCSCTL.SCLKMUXSEL = CCSCTL_SCLK_FRCLK; 
    
    // Select 4 MHz CLKREF for Free Running Clock FRCLK
    PAC55XX_SCC->CCSCTL.FRCLKMUXSEL = CCSCTL_CLKIN_CLKREF;

    pac5xxx_sys_pll_config_enable(4, 300, 0);                   // PLLCLK = 300 MHz = (4/4 * 300) /1

    // Configure SCLK=PLLCLK=300 MHz, HCLK=150 MHz, PCLK=150 MHz, ACLK=50 MHz and WaitStates;  Use default PCLKDIV=1
    PAC55XX_SCC->CCSCTL.HCLKDIV = CCSCTL_HCLKDIV_DIV2;          // HCLK = 150 MHz = SCLK/2; when SCLK = PLLCLK
    PAC55XX_SCC->CCSCTL.ACLKDIV = CCSCTL_ACLKDIV_DIV1;          // ACLK = 300 MHz = SCLK/1; when SCLK = PLLCLK
    PAC55XX_MEMCTL->MEMCTL.WSTATE = 5 + 1;                      // Flash = 150/25 = 6 clocks = 5 WS; So, need 5 + 1 Extra WS
    PAC55XX_SCC->CCSCTL.SCLKMUXSEL = CCSCTL_SCLK_PLLCLK;        // SCLK = PLLCLK

    // Set MCLK for Flash write & erase in addition to read
    PAC55XX_MEMCTL->MEMCTL.MCLKDIV = MEMCTL_MCLK_DIV5;          // MCLK will = HCLK/5 when MCLKSEL = MEMCTL_MCLK_HCLKDIV
    PAC55XX_MEMCTL->MEMCTL.MCLKSEL = MEMCTL_MCLK_HCLKDIV;       // MCLK = HCLK/5 = 30 MHz; allows reading and writing of Flash

    PAC55XX_MEMCTL->FLASHLOCK = 0;                              // Disallow write access to MEMCTL

    // Enable GPIO Input clock synchronization; should always be enabled, especially when using GPIO edge based interrupts
    PAC55XX_GPIOA->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOB->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOC->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOD->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOE->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOF->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOG->CLKSYNC.w = 0xFF;
}

/**

Initializes peripherals for hall sensor positional detection

PD0 - HALL U
PD1 - HALL V
PD2 - HALL W

**/

void hall_sensor_init(void)
{
  
        // Set pins to input mode
        PAC55XX_GPIOD->MODE.P0 = IO_HIGH_IMPEDENCE_INPUT;
	PAC55XX_GPIOD->MODE.P1 = IO_HIGH_IMPEDENCE_INPUT;
	PAC55XX_GPIOD->MODE.P2 = IO_HIGH_IMPEDENCE_INPUT;

        // Mux pins to timer C
	PAC55XX_SCC->PDMUXSEL.P0 = 2;			//DPM PD0 to TIMERC0
	PAC55XX_SCC->PDMUXSEL.P1 = 2;			//DPM PD1 to TIMERC1
	PAC55XX_SCC->PDMUXSEL.P2 = 2;			//DPM PD2 to TIMERC2

        // Pull up pins internally
	PAC55XX_SCC->PDPUEN.w = 0x07;			//PD0/1/2 are pulled up

	//Configure TIMERC to act as the Hall Sensor Input Capture Engine
	pac5xxx_timer_clock_config(TimerC, TXCTL_CS_PCLK, TXCTL_PS_DIV16);      // Configure timer clock input for PCLK /16 = 3.125 MHz
	PAC55XX_TIMERC->CTL.BASEIE = 0;
	PAC55XX_TIMERC->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;
	PAC55XX_TIMERC->CCTL0.CCMODE = TXCCTL_CCMODE_CAPTURE;
	PAC55XX_TIMERC->CCTL0.CCINTEDGE = TXCCTL_CCINT_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL0.CCLATCH = TXCCTL_CCLATCH_CAPTURE_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL0.CCINTEN = 1;
	PAC55XX_TIMERC->CCTL1.CCMODE = TXCCTL_CCMODE_CAPTURE;
	PAC55XX_TIMERC->CCTL1.CCINTEDGE = TXCCTL_CCINT_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL1.CCLATCH = TXCCTL_CCLATCH_CAPTURE_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL1.CCINTEN = 1;
	PAC55XX_TIMERC->CCTL2.CCMODE = TXCCTL_CCMODE_CAPTURE;
	PAC55XX_TIMERC->CCTL2.CCINTEDGE = TXCCTL_CCINT_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL2.CCLATCH = TXCCTL_CCLATCH_CAPTURE_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL2.CCINTEN = 1;
	PAC55XX_TIMERC->INT.w = 0x07;
	pac5xxx_timer_base_config(TimerC, 0x7FFF, 0, TxCTL_MODE_UP, 0);			// Configure Timer
	NVIC_SetPriority(TimerC_IRQn, 1);										// Set Priority; Enablement will occur after Open Loop ends; Disablement will occur when motor is disabled
	NVIC_EnableIRQ(TimerC_IRQn);

        SysTick->LOAD = 150000;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk +  SysTick_CTRL_CLKSOURCE_Msk + SysTick_CTRL_TICKINT_Msk);
	NVIC_SetPriority(SysTick_IRQn, 3);

}

/**

Initializes gate drivers


**/

void gate_drive_init(void) 
{
        pac5xxx_timer_clock_config(TimerA, TXCTL_CS_ACLK, TXCTL_PS_DIV1);      		// Configure timer clock input for ACLK, /1 divider
	volatile int phase_pwm_period = PWM_SWITCH_FREQ;											// Number of KHz
	volatile int pwm_period_ticks = TIMER_X_FREQ_CNV / (phase_pwm_period);						// 50,000 / # KHz
	pac5xxx_timer_base_config(TimerA, pwm_period_ticks, 0, TxCTL_MODE_UP, 0);	// Configure Timer


	//Configure PWM Outputs
	PAC55XX_TIMER_SEL->CCTR4.CTR = pwm_period_ticks >> 1;
	PAC55XX_TIMER_SEL->CCTR5.CTR = pwm_period_ticks >> 1;
	PAC55XX_TIMER_SEL->CCTR6.CTR = pwm_period_ticks >> 1;
        
        //DEAD TIME CONFIGURATION
        
        PAC55XX_GPIOB->OUT.w = 0;			//PORTB OUT GPIO = 0;
	PAC55XX_SCC->PBMUXSEL.w = 0;		//PORTB PSEL is ALL GPIO's
	//Configure PORTB Outputs to Push Pull
	PAC55XX_GPIOB->MODE.w = 0x1515;


	// Configure nIRQ1 interrupt input signals and enable interrupts
	PAC55XX_GPIOA->MODE.P7 = 3;             // Input for over current interrupt
	PAC55XX_GPIOA->INTTYPE.P7 = 0;          // Edge trigger
	PAC55XX_GPIOA->INTCFG.P7 = 0;           // Falling edge
	PAC55XX_GPIOA->INTEDGEBOTH.P7 = 0;      // Only one edge
	PAC55XX_GPIOA->INTCLEAR.P7 = 1;         // Clear any pending PA7 interrupt
	PAC55XX_GPIOA->INTEN.P7 = 1;            // Enable Interrupt
	NVIC_EnableIRQ(GpioA_IRQn);				// Enable interrupts in NVIC

        volatile uint32_t dt_leading_ticks = DT_LED_TICKS;
	volatile uint32_t dt_trailing_ticks = DT_TED_TICKS;
        
        pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL0), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA0
	pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL1), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA1
	pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL2), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA2	

}

void configure_timer_b_compare_mode(void)
{
    uint32_t pclk = 300000000/2;        // PCLK assumed to be 150 MHz
    uint16_t period;

    // Configure Timer C Controls
    period = pclk / 10000;                                                      // Timer Period will result in 10 kHz
    pac5xxx_timer_clock_config(TimerB, TXCTL_CS_ACLK, TXCTL_PS_DIV1);           // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerB, period, 0, TxCTL_MODE_DISABLED, 0);     // Configure timer frequency and count mode

    PAC55XX_TIMERB->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;                     // 00b--> copied TAPRD into the shadow registers when TACTR from 1 to 0(or from TAPRD to 0)  
                                                                                // 01b--> copied TAPRD into the shadow registers when TACTR from TAPRD-1 to TAPRD
                                                                                // 10b--> copied TAPRD into the shadow registers as soon as the TAPRD register is written

    PAC55XX_TIMERB->CCTR4.CTR = period >> 3;

    // Enable TACCR0 interrupt at highest priority
    PAC55XX_TIMERB->CCTL4.CCMODE = TXCCTL_CCMODE_COMPARE;                       // Set compare mode

    PAC55XX_TIMERB->CCTL4.CCINTEDGE = TXCCTL_CCINT_FALLING_EDGE;                // 0 -->rising edge interrupt
                                                                                // 1 -->falling edge interrupt
                                                                                // 2 -->rising and falling edge interrupt

    PAC55XX_TIMERB->CCTL4.CCLATCH = TXCCTL_CCLATCH_COMPARE_TXCTR_IM;            // 00b--> copied CTR4 into the shadow registers when TACTR from 1 to 0(or from TAPRD to 0) 
                                                                                // 01b--> copied CTR4 into the shadow registers when TACTR from TAPRD-1 to TAPRD 
                                                                                // 10b--> copied CTR4 into the shadow registers as soon as the TAPRD register is written

    PAC55XX_TIMERB->CTL.MODE = TxCTL_MODE_UPDOWN;

    PAC55XX_TIMERB->CCTL4.CCINTEN = 1;                                          // Enable interrupts on TCCCR0
    PAC55XX_TIMERB->INT.CCR4IF = 1;                                             // Clear PWMC0 interrupt flag 
    PAC55XX_TIMERB->CTL.BASEIE = 1;                                             // Enable base timer
    PAC55XX_TIMERB->INT.BASEIF = 1;                                             // Clear timer base interrupt flag
    //NVIC_EnableIRQ(TimerB_IRQn);                                                // Enable TimerC interrupts
    //NVIC_SetPriority(TimerB_IRQn ,1);                                           // Set TimerC Priority to 1

    PAC55XX_SCC->PCMUXSEL.P4 = 0x1;                                             // PC4 -->PWMB4
    PAC55XX_GPIOC->OUTMASK.P4 = 0;                                              // PC4 -->Output
}
/*void cafe_init(void)
{
        
        volatile uint32_t hp_over_current_limit = HP_OCP_DEF;
	volatile uint32_t lp_over_current_limit = LP_OCP_DEF;
        
        
  
	// Configure SOC Bridge for talking to MC02
	pac5xxx_tile_socbridge_config(1, 0);						// enable, int_enable

	// Write all CAFE registers
	if (pac5xxx_tile_register_read(ADDR_STATUS))				// If any power manager error bits set on startup, clear them
		{
		//Add desired checks here and then clear
		pac5xxx_tile_register_write(ADDR_STATUS, 0xFF);
		}

	//pac5xxx_tile_register_write(ADDR_PWRCTL, 0x28);				//PWR MON = VMS1
	pac5xxx_tile_register_write(ADDR_PWRCTL, 0x38);				//PWR MON = VMS2

	// Set HPROT protection threshold
	pac5xxx_tile_register_write(ADDR_HPDACH, hp_over_current_limit >> 2);
	pac5xxx_tile_register_write(ADDR_HPDACL, hp_over_current_limit & 0x03);

	// Set LPROT protection threshold
	pac5xxx_tile_register_write(ADDR_LPDACH, lp_over_current_limit >> 2);
	pac5xxx_tile_register_write(ADDR_LPDACL, lp_over_current_limit & 0x03);

	// Enable Module - Must be done before configuring Diff Amp and OCP Protection Scheme
	volatile uint32_t module_enable_bits = MODULE_ENABLE_FLAGS_DEF;
	pac5xxx_tile_register_write(ADDR_MISC, module_enable_bits);

	// Configure AIOxx for ADC sampling with OC protection
	pac5xxx_tile_register_write(ADDR_CFGAIO0, 0x40 + (DIFFAMP_GAIN_OPT0 << 3) + LPOPT_SEL);			// AIO10: DiffAmp, 16X gain, LPOPT (4 us)
	pac5xxx_tile_register_write(ADDR_CFGAIO1, 0x38 + HPOPT_SEL);			// AIO10: nHP10PR1M set, NO ENOS10, HPOPT (4us)

	pac5xxx_tile_register_write(ADDR_CFGAIO2, 0x40 + (DIFFAMP_GAIN_OPT0 << 3) + LPOPT_SEL);			// AIO32: DiffAmp, 16X gain, LPOPT (4 us)
	pac5xxx_tile_register_write(ADDR_CFGAIO3, 0x38 + HPOPT_SEL);			// AIO32: nHP32PR1M set, NO ENOS32, HPOPT (4us)

	pac5xxx_tile_register_write(ADDR_CFGAIO4, 0x40 + (DIFFAMP_GAIN_OPT0 << 3) + LPOPT_SEL);			// AIO54: DiffAmp, 16X gain, LPOPT (4 us)
	pac5xxx_tile_register_write(ADDR_CFGAIO5, 0x38 + HPOPT_SEL);			// AIO54: nHP54PR1M set, NO ENOS54, HPOPT (4us)

	// Enable protection interrupt mask
	// PROTINTM: nHP54INTM, nHP32INTM, nHP10INTM
	pac5xxx_tile_register_write(ADDR_PROTINTM, PROTINTM_MASK);

        
        /////////////// EDITED - DISABLE OC PROTECT //////////
        
        
	// Disable both HS and LS drivers on PR event (nHSPRM=1, nLSPRM=1);
	// Prop Delay 0 ns; Enable Break Before Make
	pac5xxx_tile_register_write(ADDR_CFGDRV1, 0x01);    
        // originally           pac5xxx_tile_register_write(ADDR_CFGDRV1, 0x0D);   
        
        
        
	// Cycle By Cycle on Diff Amp AIO10/32/54. Disable only high side
	pac5xxx_tile_register_write(ADDR_CFGDRV2, 0x1D);
	// Cycle By Cycle on Diff Amp AIO10/32/54 compared against LPDAC
	pac5xxx_tile_register_write(ADDR_CFGDRV3, 0x54);


	//Configure Blanking Time Feature
	pac5xxx_tile_register_write(ADDR_BLANKING, BLANKING_CONFIG);

	// Set HP comparator hysteresis (HPROTHYS=1b, LPROTHYS=1b)
	pac5xxx_tile_register_write(ADDR_SIGSET, 0x0C);

	//Configure Sensorless Comparators
	pac5xxx_tile_register_write(ADDR_CFGAIO6, 0x26);

	pac5xxx_tile_register_write(ADDR_CFGAIO7, (0xC0 + COMP_POL));			// MODE7[1:0] = 11b (special mode)
	pac5xxx_tile_register_write(ADDR_CFGAIO8, (0xD0 + COMP_POL));			// MODE8[1:0] = 11b (special mode), OPT8[1:0] = 01b (bypass FF, select MUX out for nIRQ2/POS), POL8 = 0 (act high), MUX[2:0] = n/a
	pac5xxx_tile_register_write(ADDR_CFGAIO9, SLCOMP7);			// MODE9[1:0] = 01b (CT Vfilt), OPT9[1:0] = 0bxx (AIO7), POL9 = 0 (act high), MUX[2:0] = n/a

	// HYSMODE[7] = 1b (0/20/40/80), HYSAIO7[3:0] = 1010b (40/40)
	pac5xxx_tile_register_write(ADDR_SPECCFG0, SPECCFG0_CONFIG);
	// HYSAIO8[7:4] = 1010b (40/40), HYSAIO9[3:0] = 1010b (40/40)
	pac5xxx_tile_register_write(ADDR_SPECCFG1, SPECCFG1_CONFIG);

	// SMUXAIO7[3:0] = 01b (AB1 as COMP-), SMUXAIO8[3:0] = 01b (AB1 as COMP-)
	pac5xxx_tile_register_write(ADDR_SPECCFG2, 0x11);
	// SMUXAIO9[3:0] = 01b (AB1 as COMP-)
	pac5xxx_tile_register_write(ADDR_SPECCFG3, 0x10);

	// EMUXEN=1 to enable EMUX, ENADCBUF=1 for ADC Buffer
	pac5xxx_tile_register_write(ADDR_SHCFG1, 0x00);
	pac5xxx_tile_register_write(ADDR_SHCFG1, 0x18);

	// Enable Drivers
	pac5xxx_tile_register_write(ADDR_ENDRV, 0x00);
	pac5xxx_tile_register_write(ADDR_ENDRV, 0x01);
}*/

void cafe_init(void)
{
        volatile uint32_t hp_over_current_limit = HP_OCP_DEF;
	volatile uint32_t lp_over_current_limit = LP_OCP_DEF;
        
        volatile uint32_t module_enable_bits = MODULE_ENABLE_FLAGS_DEF;
  
  
	// Configure SOC Bridge for talking to MC02
	pac5xxx_tile_socbridge_config(1, 0);						// enable, int_enable

	// Write all CAFE registers
	if (pac5xxx_tile_register_read(ADDR_STATUS))				// If any power manager error bits set on startup, clear them
		{
		//Add desired checks here and then clear
		pac5xxx_tile_register_write(ADDR_STATUS, 0xFF);
		}

	//pac5xxx_tile_register_write(ADDR_PWRCTL, 0x28);				//PWR MON = VMS1
	pac5xxx_tile_register_write(ADDR_PWRCTL, 0x38);				//PWR MON = VMS2

	// Set HPROT protection threshold
	pac5xxx_tile_register_write(ADDR_HPDACH, hp_over_current_limit >> 2);
	pac5xxx_tile_register_write(ADDR_HPDACL, hp_over_current_limit & 0x03);

	// Set LPROT protection threshold
	pac5xxx_tile_register_write(ADDR_LPDACH, lp_over_current_limit >> 2);
	pac5xxx_tile_register_write(ADDR_LPDACL, lp_over_current_limit & 0x03);

	// Enable Module - Must be done before configuring Diff Amp and OCP Protection Scheme
	module_enable_bits = MODULE_ENABLE_FLAGS_DEF;
	pac5xxx_tile_register_write(ADDR_MISC, module_enable_bits);

#define ENOS_SEL 0x08
	// Configure AIOxx for ADC sampling with OC protection
	pac5xxx_tile_register_write(ADDR_CFGAIO0, 0x40 + (DIFFAMP_GAIN_OPT0 << 3) + LPOPT_SEL);			// AIO10: DiffAmp, 16X gain, LPOPT (4 us)
	pac5xxx_tile_register_write(ADDR_CFGAIO1, 0x30 + ENOS_SEL + HPOPT_SEL);			// AIO10: nHP10PR1M set, NO ENOS10, HPOPT (4us)

	pac5xxx_tile_register_write(ADDR_CFGAIO2, 0x40 + (DIFFAMP_GAIN_OPT0 << 3) + LPOPT_SEL);			// AIO32: DiffAmp, 16X gain, LPOPT (4 us)
	pac5xxx_tile_register_write(ADDR_CFGAIO3, 0x30 + ENOS_SEL + HPOPT_SEL);			// AIO32: nHP32PR1M set, NO ENOS32, HPOPT (4us)

	pac5xxx_tile_register_write(ADDR_CFGAIO4, 0x40 + (DIFFAMP_GAIN_OPT0 << 3) + LPOPT_SEL);			// AIO54: DiffAmp, 16X gain, LPOPT (4 us)
	pac5xxx_tile_register_write(ADDR_CFGAIO5, 0x30 + ENOS_SEL + HPOPT_SEL);			// AIO54: nHP54PR1M set, NO ENOS54, HPOPT (4us)


	// Enable protection interrupt mask
	// PROTINTM: nHP54INTM, nHP32INTM, nHP10INTM
	pac5xxx_tile_register_write(ADDR_PROTINTM, PROTINTM_MASK);


	// Disable both HS and LS drivers on PR event (nHSPRM=1, nLSPRM=1);
	// Prop Delay 0 ns; Enable Break Before Make
	pac5xxx_tile_register_write(ADDR_CFGDRV1, 0x0D);
	// Cycle By Cycle on Diff Amp AIO10/32/54. Disable only high side
	pac5xxx_tile_register_write(ADDR_CFGDRV2, 0x1D);
	// Cycle By Cycle on Diff Amp AIO10/32/54 compared against LPDAC
	pac5xxx_tile_register_write(ADDR_CFGDRV3, 0x54);

        
	//Configure Blanking Time Feature
	pac5xxx_tile_register_write(ADDR_BLANKING, BLANKING_CONFIG);

	// Set HP comparator hysteresis (HPROTHYS=1b, LPROTHYS=1b)
	pac5xxx_tile_register_write(ADDR_SIGSET, 0x0C);

	//Configure Sensorless Comparators
	pac5xxx_tile_register_write(ADDR_CFGAIO6, 0x26);

	pac5xxx_tile_register_write(ADDR_CFGAIO7, (0xC0 + COMP_POL));			// MODE7[1:0] = 11b (special mode)
	pac5xxx_tile_register_write(ADDR_CFGAIO8, (0xD0 + COMP_POL));			// MODE8[1:0] = 11b (special mode), OPT8[1:0] = 01b (bypass FF, select MUX out for nIRQ2/POS), POL8 = 0 (act high), MUX[2:0] = n/a
	pac5xxx_tile_register_write(ADDR_CFGAIO9, SLCOMP7);			// MODE9[1:0] = 01b (CT Vfilt), OPT9[1:0] = 0bxx (AIO7), POL9 = 0 (act high), MUX[2:0] = n/a

	// HYSMODE[7] = 1b (0/20/40/80), HYSAIO7[3:0] = 1010b (40/40)
	pac5xxx_tile_register_write(ADDR_SPECCFG0, SPECCFG0_CONFIG);
	// HYSAIO8[7:4] = 1010b (40/40), HYSAIO9[3:0] = 1010b (40/40)
	pac5xxx_tile_register_write(ADDR_SPECCFG1, SPECCFG1_CONFIG);

	// SMUXAIO7[3:0] = 01b (AB1 as COMP-), SMUXAIO8[3:0] = 01b (AB1 as COMP-)
	pac5xxx_tile_register_write(ADDR_SPECCFG2, 0x11);
	// SMUXAIO9[3:0] = 01b (AB1 as COMP-)
	pac5xxx_tile_register_write(ADDR_SPECCFG3, 0x10);

	// EMUXEN=1 to enable EMUX, ENADCBUF=1 for ADC Buffer
	pac5xxx_tile_register_write(ADDR_SHCFG1, 0x00);
	pac5xxx_tile_register_write(ADDR_SHCFG1, 0x18);

	// Enable Drivers
	pac5xxx_tile_register_write(ADDR_ENDRV, 0x00);
	pac5xxx_tile_register_write(ADDR_ENDRV, 0x01);
}



void current_sense_init(void) 
{
  
}


  




