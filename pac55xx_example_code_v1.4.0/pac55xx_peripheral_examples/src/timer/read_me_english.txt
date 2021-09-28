Read me:
1. Test Timer overflow interrupt and PWM along interrupt:
    Configure the timer clock source for the ACLK, ACLK frequency configured to 300Mhz, the configuration Timerb frequency of 15000hz,
    pwmb0 duty ratio of 50%, is capable of timerb overflow interrupt, enable PWMB0 falling Edge interrupt,
    Flipping the PE0 level when generating a PWM overflow interrupt, flipping the PE1 level when the PWMB0 falling Edge is interrupted

2. Test timer A/B/C/D Sync mode:
    Configure timer a main timer, timer B/C/D are synchronized with timer a, PC7, PE0, PF2 output PWM to test 
    (pc7-->pwmb7 pe0-->pwmc4 pf2-->pwmd2), see PC7, PE0, PF2 edge to align;

3. Test the PWM output:
    Configuration PWMC4 from PE0 output, frequency 10KHz, duty ratio 50%, Compare register values can be updated when the timer count 
    value to the maximum value, also can be counted to the timer count to the 0 o'clock update, also can be updated after the comparison
    register is written, Can be configured to trigger an interrupt on the PWM rising edge, a falling edge trigger interrupt, or a rising 
    edge along the falling edge to trigger an interrupt;

4. Test Capture input
    With the chip PE0 foot output a pwm, configure the PE1 as the capture pin, and PE1 corresponding to the PWMD1, the PE0 and PE1 together,
    You can capture high-level time and cycle time, do not need to use the capture interrupt to get the duty ratio, Capture mode, You can 
    configure the timer count value to be stored only on the rising edge to the ctrx, or only on the falling edge of the timer count value 
    Ctr to the ctrx, or on the rising edge and the falling edge both the timer count value is stored in the ctrx, you can also configure the 
    PWM directly
    In the presence of CCTRX.CAP1 in high power, the PWM cycle time is cctrx.cap2, the capture mode can be configured to trigger the interrupt 
    on the rising edge, the falling edge trigger interrupt, or the rising edge of the falling edge will trigger the interrupt;

5. Test the symmetric mode PWM drive
    Configure the Timera to 3 pairs of complementary dead-zone PWM output mode, symmetrical mode, only the upper arm PWMA4/5/6 duty ratio 
    assignment, the lower arm automatically for its upper arm complementary output, so the pwma0/1/2 of the lower arm can be reserved for its 
    use, to have a driver output, but also to be able to simulate the driver output of the core;

6. Test Asymmetric Mode PWM drive
    Asymmetric mode is only applicable to the UpDown count mode, the configuration Timera is 3 pairs of complementary dead-zone PWM output mode,
    in asymmetric mode, The PWMA4/5/6 is the comparison value of the rise of the counting mode, pwma0/1/2 The comparison value of the descending
    part of the counting mode, thus obtaining the asymmetric upper arm drive,
    The lower arm is automatically the complementary output of its upper arm, and the driver output of the core can be simulated by driving output;

7. Test Phase Shift Mode PWM driver
   Phase shift mode is only applicable to up counting mode, configuration Timera is 3 pairs of complementary dead-zone PWM output mode, in 
   Phase-shift mode, You can configure Ctrx.delay to set phase shift time, and then only the upper arm PWMA4/5/6 duty ratio assignment, the lower 
   arm automatically for its upper arm complementary output, therefore, the pwma0/1/2 of the lower arm can be reserved for its use, and the output 
   of the driver can be simulated by a driver.

8. Test QEP
   Using PE0, PE1, PC7 respectively analog QEP signal qepphasea, qepphaseb, qepindex Output signal, configuration PC5 capture Tcqeppha signal, PC6 
   capture TCQEPPHB signal, PC4 capture Tcqepidx signal; The QEP mode can be configured to count the rising edge of the phasea, or the rising and 
   falling edges, and can also be configured to count the rising edge of Phasea and phaseb, or the rising and falling edges of Phasea and phaseb;