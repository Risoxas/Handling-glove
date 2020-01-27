#include <xc.h>
#include <pic18f25k80.h>
#include "Configuration_Bits.h"

//variables dedos
unsigned char pulgar;
unsigned char indice;
unsigned char mayor;
unsigned char anular;
unsigned char menique;
//variables giros
unsigned char giroX;
unsigned char giroY;
unsigned char giroZ;
//banderas
//contadores
unsigned char j;
unsigned char EstadoMano();
void interrupt alta_prioridad()
{
    if((ADIE)&&(ADIF))
    {
        ADIF=0;  //apaga bandera del ADC
        if(j==1)
        {
            CHS4=0;
            CHS3=0;
            CHS2=0;
            CHS1=0;
            CHS0=0;
            
            pulgar=ADRESH;
        }
        if(j==2)
        {
            CHS4=0;
            CHS3=0;
            CHS2=0;
            CHS1=0;
            CHS0=1;
           
            menique=ADRESH;
        }
        if(j==3)
        {
            CHS4=0;
            CHS3=0;
            CHS2=0;
            CHS1=1;
            CHS0=0;
            
            anular=ADRESH;
        }
        if(j==4)
        {
            CHS4=0;
            CHS3=0;
            CHS2=0;
            CHS1=1;
            CHS0=1;
            
            mayor=ADRESH;
        }
        if(j==5)
        {
            CHS4=0;
            CHS3=0;
            CHS2=1;
            CHS1=0;
            CHS0=0;
            
            indice=ADRESH;
            j=0;
        }/*
        if(j==1)
        {
            CHS4=0;
            CHS3=1;
            CHS2=0;
            CHS1=0;
            CHS0=0;
            
            giroX=ADRESH;
        }
        if(j==2)
        {
            CHS4=0;
            CHS3=1;
            CHS2=0;
            CHS1=0;
            CHS0=1;
            
            giroY=ADRESH;
        }
        if(j==3)
        {
            CHS4=0;
            CHS3=1;
            CHS2=0;
            CHS1=1;
            CHS0=0;
            
            giroZ=ADRESH;
            j=0;
        }*/
        j= j + 1;
    }
    if ((TXIE) && (TXIF))
	{
        TXIF=0;
        if(EstadoMano() == 'A')
        {
            TXREG1=0xF0;
            TXIE=0;
        }
        if(EstadoMano() == 'B')
        {
            TXREG1=0xF1;
            TXIE=0;
        }
        if(EstadoMano() == 'C')
        {
            TXREG1=0xF2;
            TXIE=0;
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
unsigned char EstadoMano()
{
    unsigned char senal;
    if((pulgar<=230)&&(indice<=190)&&(anular<=160)&&(menique<=35))
    {
        senal='A';
    }
    if((pulgar<=255)&&(indice<=255)&&(anular<=200)&&(menique<=52))
    {
        senal='B';
    }
    if((pulgar<=255)&&(indice<=180)&&(anular<=180)&&(menique<=50))
    {
        senal='C';
    }
    else
    {
        senal=0;
    }
    
    return senal;
}

void main(void)
{
    OSCCON=0b01100000;
    T0CON=0b01000010;

    ADCON0=0b00000000;
    ADCON1=0b00100000;
    ADCON2=0b00100000;

    TXSTA1=0b00100000;
    RCSTA1=0b10010000;
    BAUDCON1=0b00000000;
    SPBRG1=51;//baudrate de 9600
        
    ANSEL0=1;
    ANSEL1=1;
    ANSEL2=1;
    ANSEL3=1;
    ANSEL4=1;
    ANSEL8=1;
    ANSEL9=1;
    ANSEL10=1;
    
    TRISC2=0;
    
    IPEN=1;
    GIEH=1;
    
    ADIE=1;
    ADIP=1;
    
    TXIP=1;
   
    j=0;
    
    ADON=1;
    GO_DONE=1;
    
    while(1)
    {
        
        if((EstadoMano()=='A') || (EstadoMano()=='B') || (EstadoMano()=='C') )
        {
            TXIE=1;
            LATC2=0;
            delay_ms(100);
            LATC2=1;
            delay_ms(100);
        }
        GO_DONE=1;
    }
}
