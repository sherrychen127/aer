/*
 * File:   main.c
 * Author: Michael Ding
 *
 * Created on July 18, 2016, 12:11 PM
 *
 * Updated by Tyler Gamvrelis on July 28, 2017, 9:06 PM
 * 
 * Description: Demonstrates the use of the Analog to Digital Converter (ADC)
 *              module in the PIC.
 * 
 * Preconditions:
 *   1. Jumpers JP5 and JP4 are shorted
 *   2. Character LCD is in a PIC socket
 */

/***** Includes *****/
#include <xc.h>
#include "configBits.h"
#include "lcd.h"

/**** Local function prototypes *****/
int counter = 0;
int max = 4;
unsigned short analog_result;

//const char PR_value = "01";

//unsigned char ReadSensor(unsigned char ch){
//    ADCON0 = 0x00; //clear ADCON0
//    ADCON0 = (ch << 2);
//    ADON = 1;
    
//    ADCON0bits.GO = 1; // Initiate sampling
//    while(ADCON0bits.GO_NOT_DONE){  continue;   }
//   // __delay_ms(500);
//    ADON = 0;
//    return ADRESH;
    
//}
unsigned short readADC(char channel);
void motorStart_0();
void motorStop_0();


void main(void) {
    
    // <editor-fold defaultstate="collapsed" desc="Machine Configuration">
    /******************************* OSCILLATOR *******************************/
    OSCCON = 0xF2;  // Use 8 MHz internal oscillator block
    
    /********************************* PIN I/O ********************************/
    /* Write outputs to LATx, read inputs from PORTx. Here, all latches (LATx)
     * are being cleared (set low) to ensure a controlled start-up state. */  
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    /* After the states of LATx are known, the data direction registers, TRISx
     * are configured. 0 --> output; 1 --> input. Default is  1. */
    TRISA = 0xFF; // All inputs (this is the default, but is explicated here for learning purposes)
    TRISB = 0xFF;
    TRISC = 0x00;
    TRISD = 0x00; // All output mode on port D for the LCD
    TRISE = 0x00;

    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;  // Disable ADC
    ADCON1 = 0x0B;  // RA0-3 are analog (pg. 222)
    ADCON2bits.ADFM = 1; // Right justify A/D result
    //</editor-fold>
    
    /* Initialize character LCD. */
    
    
    //ADCON0 =  0b0000;
    initLCD();
    printf("%d",counter);
    __delay_ms(100);
    
    //ADCON1 = 0x10;
    //while(1){
        motorStart_0();
    //}
    
    while (1){
        //motorStart_0();
        if (counter == max){
                motorStop_0();
                break;
        }
        if (readADC(2)<=250 ){   //800
            //motorStart_0(); 
            counter ++;
            initLCD();
            printf("%d", counter);
            __delay_ms(50);
            }
        //__delay_ms(60);           //nuts = 70 ms
       
    
       
    }
        while(1){
            motorStop_0();
            initLCD();
            printf("done");
            __delay_ms(300);
        }
    //motorStop_0();
    //initLCD();
    //printf("done");
    /* Main loop. */
    //while(1) {
    //    __lcd_clear();
    //    __lcd_home();
        
        
        
        
       /* Print results to character LCD. Since ADC results are 10 bits, they
        * are split up into two registers ADRESH and ADRESL. The results of the
        * ADC are displayed in hexadecimal so that the values in each of these
        * registers will be easily visible; ADRESL corresponds to the two least
        * significant digits, while ADRESH corresponds to the most significant 
        * bit. */
       //printf("RA2: %.3x", readADC(2));
       //__lcd_newline();
       //printf("RA3: %.3x", readADC(3));
       
       //__delay_ms(100);
    //}
}

unsigned short readADC(char channel){
    /* Reads the analog input from the specified analog channel.
     *
     * Arguments: channel, the byte corresponding to the channel to read
     *
     * Returns: the 10-bit value corresponding to the voltage read from
     *          the specified channel
     */
    
    ADCON0 = (channel & 0x0F) << 2; // Select ADC channel (i.e. pin)
    ADON = 1; // Enable module
    ADCON0bits.GO = 1; // Initiate sampling
    while(ADCON0bits.GO_NOT_DONE){  continue;   } // Poll for acquisition completion
    analog_result = ADRESL + (ADRESH*256);
    return analog_result;
    
    //return (ADRESH << 8) | ADRESL; // Return result as a 16-bit value
}



void motorStart_0(){
    
        LATCbits.LATC0 = 1;  //RC0 HIGH
        TRISCbits.RC0 = 0;
        //LATAbits.LATA2 = 0;  //RC2 LOW
        __delay_ms(200);//2 seconds delay
    
}

void motorStop_0(){
    LATCbits.LATC0 = 0;  //RC0 LOW
    TRISCbits.RC0 = 0;
    //LATAbits.LATA2 = 0;  //RC2 LOW
    __delay_ms(200);//2 seconds delay
}