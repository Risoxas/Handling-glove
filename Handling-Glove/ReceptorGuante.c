#include <xc.h>
#include <pic18f25k80.h>
#include "Configuration_Bits.h"
#define Vent  LATB0
#define Cafe  LATB1
#define Lamp  LATB2

unsigned char recibi;
void interrupt alta_prioridad()
{
    if((RCIE) && (RCIF))
    {
        RCIF=0;
        if((RCREG1==0xF0)||(RCREG1==0xF1)||(RCREG1==0xF2))
        {
            recibi=RCREG1;
        }
    }
}

void delay_ms(unsigned int tiempo_ms)
{
    unsigned int i;
	TMR0ON=1;
	for (i=0;i<tiempo_ms;i++)
	{
		TMR0L=6;		//250 cuentas para overflow
		do{}while (TMR0IF==0);
		TMR0IF=0;
	}
	TMR0ON=0;
}
void main(void)
{
    OSCCON=0b01100000;
    T0CON=0b01000010;

    TXSTA1=0b00100000;
    RCSTA1=0b10010000;
    BAUDCON1=0b00000000;
    SPBRG1=51;//baudrate de 9600
    
    TRISB0=0;
    TRISB1=0;
    TRISB2=0;
    
    IPEN=1;
    GIEH=1;
    
    RCIP=1;
    RCIE=1;
    unsigned char flag1;
    unsigned char flag2;
    unsigned char flag3;
    
    while(1)
    {
        if(recibi == 0xF0)
        {
            if(flag1&1)
            {
                Vent = 0;
                flag1=0;
            }else
            {
                Vent = 1;
            }
            flag1=1;
        }
        if(recibi ==0xF1)
        {
            if(flag2&1)
            {
                Cafe = 0;
                flag2=0;
            }else
            {
                Cafe = 1;
            }
            flag2=1;
        }
        if(recibi==0xF2)
        {
            if(flag3&1)
            {
                Lamp = 0;
                flag3=0;
            }else
            {
                Lamp = 1;
            }
            flag3=1;
        }
        else
        {
            Lamp=1;
            Cafe=1;
            Vent=1;
        }
    }
}


