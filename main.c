#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd_lib.h"
#include "USART_Routines.h"

int readADCValue(unsigned char channel);

long ADCValues;
char ADCValuesStr[10];

float temperature = 0.00;

int main(void)
{
	
	DDRB = DDRB | (1 << PB0);
	
	LCDinit();
	LCDclr();
	USART_Init();
	
	ADMUX |= 1 << REFS0;		
	ADCSRA |= 1 << ADPS1;		
	ADCSRA |= (1 << ADEN);		
	
	TX_NEWLINE;	
	transmitString_P(PSTR("ADC initialization done..."));
	TX_NEWLINE;
	
	
	while(1)
	{			
		PORTB = PORTB | (1 << PB0);
		ADCValues = readADCValue(6);	
		itoa(ADCValues, ADCValuesStr, 10);
		transmitString_P(PSTR("ADC Value = "));
		transmitString(ADCValuesStr);	
			
		ADCValues = (ADCValues*5000) / 1023;
		itoa(ADCValues, ADCValuesStr, 10);
		transmitString_P(PSTR("   Vout in millivolt = "));
		transmitString(ADCValuesStr);	
			
		temperature = ADCValues/10;
		transmitString_P(PSTR("   Temperature = "));
		itoa((int)temperature, ADCValuesStr, 10);
		transmitString(ADCValuesStr);
		TX_NEWLINE;TX_NEWLINE;	
		
		LCDWriteStringXY(0, 0,"LM35_thermometer");	
		LCDWriteStringXY(0, 1,"Temp: ");
		LCDWriteIntXY(6, 1, temperature, 2);
		LCDWriteStringXY(8, 1," deg(C)");
		
		_delay_ms(1000);
		PORTB = PORTB & (~(1 << PB0));
		_delay_ms(1000);
	}
}


int readADCValue(unsigned char channel)		
{
	unsigned int ADCH_temp = 0, ADC_temp = 0;
	
	ADMUX = 0b01000000 | channel;			
	
	ADCSRA |= (1 << ADSC);				
	
	while(!(ADCSRA & (1 << ADIF)));		
	
	ADC_temp = ADCL;					
	ADCH_temp = ADCH;					
	ADC_temp = ADC_temp + (ADCH_temp << 8);
	
	return ADC_temp;
}