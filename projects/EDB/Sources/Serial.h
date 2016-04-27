#ifndef SERIAL_H
#define SERIAL_H

uint16_t SER_GetPeriod(uint8 c);
uint8_t SER_SerialProcess();
void SER_SendEvent();
void SER_SendString(unsigned char *msg);
void SER_SendUint16(uint16_t i);
void SER_SendNewLine();
void vSerialTask();
void SER_Init();

#endif /* SERIAL_H */
