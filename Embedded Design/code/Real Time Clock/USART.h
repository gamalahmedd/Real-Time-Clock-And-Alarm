/*
 * EEPROM_driver.h
 *
 * Created: 3/9/2021 4:5:49 PM
 *  Author: Ahmed Sherif
 */ 


#ifndef USART_H_
#define USART_H_
void UART_vInit(unsigned long baud);
void UART_vSendData(char data);
char UART_u8ReceiveData(void);
void UART_vSendstring( char *ptr);
void UART_vInterruptEnable();
void UART_vInterruptDisable();
#endif /* USART_H_ */
