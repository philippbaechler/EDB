#ifndef SERVOS_H
#define SERVOS_H

void SRV_MoveServo(uint8_t degree, uint8_t servo);
void SRV_retract();
void SRV_release();
void SRV_putBack();
void SRV_loadOn();
void SRV_grab();
void SRV_extend();
void SRV_pickUp();
void SRV_Init();

#endif /* SERVOS_H */
