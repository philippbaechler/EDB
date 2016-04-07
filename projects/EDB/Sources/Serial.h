#ifndef SERIAL_H
#define SERIAL_H

uint16_t SER_GetPeriod(uint8 c);
void SER_SerialProcess();
void SER_SendEvent();
void vSerialTask();

#endif /* SERIAL_H */
