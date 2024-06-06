
/*
 * EEPROM_driver.h
 *
 * Created: 04/12/2023 10:15:30 PM
 *  Author: Ahmed Sherif
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"
#include "timer.h"
#include "seven segment.h"
#include "LCD.h"
#include "std_macros.h"
#define NOTPRESSED	0XFF
#define F_CPU 8000000UL
#define EEPROM_SATRTING_ADDRESS 0x0000
#define SET_TIME	'1'
#define ADD_ALARM	'2'
#define REMOVE_ALARM	'3'
#define	SHOW_ALARMS		'4'

#include <util/delay.h>
#include "EEPROM_driver.h"
volatile unsigned char seconds_counter=0;
volatile unsigned char reset_flag='0';
unsigned char minutes_counter=0,hours_counter=0;
unsigned char value,first_digit,second_digit;
volatile unsigned char x=0;
signed short availableLocation();

int main(void)
{
	unsigned char Alarm_timeout;
	unsigned char delete_index=-1;
	signed short availableSlot;
	DIO_set_port_direction('D',1);
	DIO_vsetPINDir('A',3,1);
	UART_vInit(9600UL);
	LCD_vInit();
	LCD2_vInit();
	seven_seg_vinit('B');
	SET_BIT(DDRC,0);
    SET_BIT(DDRC,1);
	SET_BIT(DDRC,2);
	SET_BIT(DDRC,3);
	SET_BIT(DDRC,4);
	SET_BIT(DDRC,5);
	LCD_vSend_string("1: set clock");
	LCD_movecursor(2,1);
	LCD_vSend_string("2: add alarm");
	LCD2_vSend_string("3: remove alarm");
	LCD2_movecursor(2,1);
	LCD2_vSend_string("4: show alarms");
	timer2_overflow_init_interrupt();
	UART_vInterruptEnable();
	while(1)
	{		availableSlot=availableLocation();
			DIO_write('A',3,0);
			if (reset_flag==SET_TIME)
			{	reset_flag='0';
				UART_vInterruptDisable();
				LCD_clearscreen();
				LCD2_clearscreen();
				LCD_vSend_string("hours=--");
				LCD_movecursor(1,7);
				_delay_ms(500);
				do
				{
					first_digit=UART_u8ReceiveData();
					
				} while (first_digit==NOTPRESSED);
				LCD_vSend_char(first_digit);
				_delay_ms(300);
				do
				{
					second_digit=UART_u8ReceiveData();
					
				} while (second_digit==NOTPRESSED);
				LCD_vSend_char(second_digit);
				_delay_ms(300);
				hours_counter=(second_digit-48)+10*(first_digit-48);
				LCD_clearscreen();
				
				LCD_vSend_string("minutes=--");
				LCD_movecursor(1,9);
				do
				{
					first_digit=UART_u8ReceiveData();
					
				} while (first_digit==NOTPRESSED);
				LCD_vSend_char(first_digit);
				_delay_ms(300);
				do
				{
					second_digit=UART_u8ReceiveData();
					
				} while (second_digit==NOTPRESSED);
				LCD_vSend_char(second_digit);
				_delay_ms(300);
				minutes_counter=(second_digit-48)+10*(first_digit-48);
				LCD_clearscreen();
				
				LCD_vSend_string("seconds=--");
				LCD_movecursor(1,9);
				
				do 
				{
					first_digit=UART_u8ReceiveData();
					
				} while (first_digit==NOTPRESSED);
				LCD_vSend_char(first_digit);
				_delay_ms(300);
				do
				{
					second_digit=UART_u8ReceiveData();
					
				} while (second_digit==NOTPRESSED);
				LCD_vSend_char(second_digit);
				_delay_ms(300);
				seconds_counter=(second_digit-48)+10*(first_digit-48);
				LCD_clearscreen();
				LCD_vSend_string("1 to set clock");
				LCD_movecursor(2,1);
				LCD_vSend_string("2 to add alarm");
				LCD2_vSend_string("3: remove alarm");
				LCD2_movecursor(2,1);
				LCD2_vSend_string("4: show alarms");
			}
			else if(reset_flag==ADD_ALARM)
			{
				reset_flag='0';
				UART_vInterruptDisable();
				if(availableSlot==-1)
				{
					LCD_clearscreen();
					LCD2_clearscreen();
					LCD_movecursor(1,3);
					LCD_vSend_string("Sorry!");
					LCD2_movecursor(1,3);
					LCD2_vSend_string("Maximum Number");
					LCD2_movecursor(2,5);
					LCD2_vSend_string("Reached");
					_delay_ms(1500);
					LCD_clearscreen();
					LCD_vSend_string("1 to set clock");
					LCD_movecursor(2,1);
					LCD_vSend_string("2 to add alarm");
					LCD2_clearscreen();
					LCD2_vSend_string("3: remove alarm");
					LCD2_movecursor(2,1);
					LCD2_vSend_string("4: show alarms");
					second_digit+=2;
					UART_vSendData('0');
					continue;
					
				}
				UART_vSendData('1');
				LCD2_clearscreen();
				LCD2_movecursor(1,3);
				LCD2_vSend_string("Adding new");
				LCD2_movecursor(2,6);
				LCD2_vSend_string("ALarm");
				LCD_clearscreen();
				LCD_vSend_string("hours=--");
				LCD_movecursor(1,7);
				_delay_ms(500);
				do
				{
					first_digit=UART_u8ReceiveData();
					
				} while (first_digit==NOTPRESSED);
				LCD_vSend_char(first_digit);
				_delay_ms(300);
				do
				{
					second_digit=UART_u8ReceiveData();
					
				} while (second_digit==NOTPRESSED);
				LCD_vSend_char(second_digit);
				_delay_ms(300);
				EEPROM_write(availableSlot,(second_digit-48)+10*(first_digit-48));
				LCD_clearscreen();
				
				LCD_vSend_string("minutes=--");
				LCD_movecursor(1,9);
				do
				{
					first_digit=UART_u8ReceiveData();
					
				} while (first_digit==NOTPRESSED);
				LCD_vSend_char(first_digit);
				_delay_ms(300);
				do
				{
					second_digit=UART_u8ReceiveData();
					
				} while (second_digit==NOTPRESSED);
				LCD_vSend_char(second_digit);
				_delay_ms(300);
				EEPROM_write(availableSlot+1,(second_digit-48)+10*(first_digit-48));
				LCD_clearscreen();
				LCD_vSend_string("1 to set clock");
				LCD_movecursor(2,1);
				LCD_vSend_string("2 to add alarm");
				LCD2_clearscreen();
				LCD2_vSend_string("3: remove alarm");
				LCD2_movecursor(2,1);
				LCD2_vSend_string("4: show alarms");
			}
			else if(reset_flag==SHOW_ALARMS)
			{
				reset_flag='0';
				LCD_clearscreen();
				LCD2_clearscreen();
				LCD2_movecursor(1,2);
				LCD2_vSend_string("Clock is still");
				LCD2_movecursor(2,5);
				LCD2_vSend_string("working");
				for(int i=0,j=0;i<8;i+=2,j++)
				{
					if((EEPROM_read(EEPROM_SATRTING_ADDRESS+i)!=NOTPRESSED))
					{	LCD_vSend_char(i+'1'-j);
						LCD_vSend_char('-');
						LCD_vSend_char((EEPROM_read(EEPROM_SATRTING_ADDRESS+i)/10)+'0');
						LCD_vSend_char((EEPROM_read(EEPROM_SATRTING_ADDRESS+i)%10)+'0');
						LCD_vSend_char(':');
						LCD_vSend_char((EEPROM_read(EEPROM_SATRTING_ADDRESS+i+1)/10)+'0');
						LCD_vSend_char((EEPROM_read(EEPROM_SATRTING_ADDRESS+i+1)%10)+'0');
						UART_vSendData((EEPROM_read(EEPROM_SATRTING_ADDRESS+i)/10)+'0');
						UART_vSendData((EEPROM_read(EEPROM_SATRTING_ADDRESS+i)%10)+'0');
						UART_vSendData((EEPROM_read(EEPROM_SATRTING_ADDRESS+i+1)/10)+'0');
						UART_vSendData((EEPROM_read(EEPROM_SATRTING_ADDRESS+i+1)%10)+'0');
					}
					else
					{
						UART_vSendData('F');
						UART_vSendData('F');
						UART_vSendData('F');
						UART_vSendData('F');
					}
					if(i==2)
					LCD_movecursor(2,1);
					else
					LCD_vSend_string("  ");
				}
				_delay_ms(4000);
				second_digit+=4500;
				LCD_clearscreen();
				LCD_vSend_string("1 to set clock");
				LCD_movecursor(2,1);
				LCD_vSend_string("2 to add alarm");
				LCD2_clearscreen();
				LCD2_vSend_string("3: remove alarm");
				LCD2_movecursor(2,1);
				LCD2_vSend_string("4: show alarms");
				
			}
			else if(reset_flag==REMOVE_ALARM)
			{
				UART_vInterruptDisable();
				reset_flag='0';
				LCD_clearscreen();
				LCD2_clearscreen();
				LCD2_movecursor(1,2);
				LCD2_vSend_string("Clock is still");
				LCD2_movecursor(2,5);
				LCD2_vSend_string("working");
				LCD_movecursor(1,5);
				LCD_vSend_string("Look at");
				LCD_movecursor(2,7);
				LCD_vSend_string("GUI");
				for(int i=0,j=0;i<8;i+=2,j++)
				{	if((EEPROM_read(EEPROM_SATRTING_ADDRESS+i)!=NOTPRESSED))
					{
						UART_vSendData((EEPROM_read(EEPROM_SATRTING_ADDRESS+i)/10)+'0');
						UART_vSendData((EEPROM_read(EEPROM_SATRTING_ADDRESS+i)%10)+'0');
						UART_vSendData((EEPROM_read(EEPROM_SATRTING_ADDRESS+i+1)/10)+'0');
						UART_vSendData((EEPROM_read(EEPROM_SATRTING_ADDRESS+i+1)%10)+'0');
					}
					else
					{
						UART_vSendData('F');
						UART_vSendData('F');
						UART_vSendData('F');
						UART_vSendData('F');
					}
				}
				_delay_ms(500);
				LCD_clearscreen();
				LCD_vSend_string("Delete Alarm No:");
				LCD_movecursor(2,4);
				LCD_vSend_string("0 to back");
				LCD_movecursor(1,17);
				delete_index=UART_u8ReceiveData();
				if(delete_index!=0)
				{	
					switch(delete_index)
					{
						case '1':EEPROM_write(EEPROM_SATRTING_ADDRESS,0XFF);
								EEPROM_write(EEPROM_SATRTING_ADDRESS+1,0XFF);
								break;
						case '2':EEPROM_write(EEPROM_SATRTING_ADDRESS+2,0XFF);
						EEPROM_write(EEPROM_SATRTING_ADDRESS+3,0XFF);
						break;
						case '3':EEPROM_write(EEPROM_SATRTING_ADDRESS+4,0XFF);
						EEPROM_write(EEPROM_SATRTING_ADDRESS+5,0XFF);
						break;
						case '4':EEPROM_write(EEPROM_SATRTING_ADDRESS+6,0XFF);
						EEPROM_write(EEPROM_SATRTING_ADDRESS+7,0XFF);
						break;
						default:break;
					}
					
				}				
				second_digit+=600;
				LCD_vSend_string("1 to set clock");
				LCD_movecursor(2,1);
				LCD_vSend_string("2 to add alarm");
				LCD2_clearscreen();
				LCD2_vSend_string("3: remove alarm");
				LCD2_movecursor(2,1);
				LCD2_vSend_string("4: show alarms");
				
			}
			else if(reset_flag!=ADD_ALARM && reset_flag!=SET_TIME && reset_flag!='0' && reset_flag!=REMOVE_ALARM &&reset_flag!=SHOW_ALARMS )
			{
				LCD_clearscreen();
				LCD_vSend_string("wrong choice");
				_delay_ms(1000);
				LCD_clearscreen();
				LCD_vSend_string("press 1 to");
				LCD_movecursor(2,1);
				LCD_vSend_string("set clock");
				reset_flag='0';
			}
		for(int i=0;i<=8;i+=2)
		{
			if(EEPROM_read(EEPROM_SATRTING_ADDRESS+i)==hours_counter)
				if(EEPROM_read(EEPROM_SATRTING_ADDRESS+1+i)==minutes_counter)
				{
					DIO_write('A',3,1);
					Alarm_timeout=minutes_counter;
				}					
		}
		if(DIO_u8read('A',3))
		{
			if(minutes_counter>Alarm_timeout)
				DIO_write('A',3,0);
		}

		
		CLR_BIT(PORTC,1);
		CLR_BIT(PORTC,2);
		CLR_BIT(PORTC,3);
		CLR_BIT(PORTC,4);
		CLR_BIT(PORTC,5);
		SET_BIT(PORTC,0);
		seven_seg_write('B',seconds_counter%10);
		_delay_ms(5);
		
		CLR_BIT(PORTC,0);
		CLR_BIT(PORTC,2);
		CLR_BIT(PORTC,3);
		CLR_BIT(PORTC,4);
		CLR_BIT(PORTC,5);
		SET_BIT(PORTC,1);
		seven_seg_write('B',seconds_counter/10);
		_delay_ms(5);
		
		CLR_BIT(PORTC,0);
		CLR_BIT(PORTC,1);
		CLR_BIT(PORTC,3);
		CLR_BIT(PORTC,4);
		CLR_BIT(PORTC,5);
		SET_BIT(PORTC,2);
		seven_seg_write('B',minutes_counter%10);
		_delay_ms(5);
		
		CLR_BIT(PORTC,0);
		CLR_BIT(PORTC,1);
		CLR_BIT(PORTC,2);
		CLR_BIT(PORTC,4);
		CLR_BIT(PORTC,5);
		SET_BIT(PORTC,3);
		seven_seg_write('B',minutes_counter/10);
		_delay_ms(5);
		
		CLR_BIT(PORTC,0);
		CLR_BIT(PORTC,1);
		CLR_BIT(PORTC,2);
		CLR_BIT(PORTC,3);
		CLR_BIT(PORTC,5);
		SET_BIT(PORTC,4);
		seven_seg_write('B',hours_counter%10);
		_delay_ms(5);
		
		CLR_BIT(PORTC,0);
		CLR_BIT(PORTC,1);
		CLR_BIT(PORTC,2);
		CLR_BIT(PORTC,3);
		CLR_BIT(PORTC,4);
		SET_BIT(PORTC,5);
		seven_seg_write('B',hours_counter/10);
		_delay_ms(5);
		if (seconds_counter==60)
		{
			seconds_counter=0;
			minutes_counter++;
		}
		if (minutes_counter==60)
		{
			minutes_counter=0;
			hours_counter++;
		}
		if (hours_counter==24)
		{
			hours_counter=0;
		}
		
		UART_vInterruptEnable();
	}
	
}

ISR(TIMER2_OVF_vect)
{
	x++;
	if(x>=30)
	{
		x=0;
		seconds_counter++;
	}		
}
ISR(USART_RXC_vect){
	reset_flag=UDR;
}
unsigned char numberOfAlarms(){
	char i=0;
	while(1)
	{
		if(EEPROM_read(EEPROM_SATRTING_ADDRESS+i)!=NOTPRESSED)
			i++;
		else
			break;
	}
	return i/2;
}
 signed short availableLocation()
{
	signed short avbLoc=-1;
	for(int i=0;i<8;i++)
	{
		if(EEPROM_read(EEPROM_SATRTING_ADDRESS+i)==NOTPRESSED)
		{
			avbLoc=EEPROM_SATRTING_ADDRESS+i;
			break;
		}
	}
	return avbLoc;
}
