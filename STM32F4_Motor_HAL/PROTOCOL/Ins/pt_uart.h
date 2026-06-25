#ifndef _PT_UART_H_
#define _PT_UART_H_

#include "main.h"

typedef struct{
	uint8_t head;					// Ö¡Í·
	uint8_t cmd;					// ÃüÁî
	uint8_t speed;				// ËÙ¶È
	uint8_t frail;				// Ö¡Î²
}ProtocolFrame;

uint8_t PROTOCOL_Pack(uint8_t cmd,uint8_t speed,uint8_t *out_frame);
uint8_t PROTOCOL_Parse(uint8_t *data, uint8_t len, uint8_t *cmd, uint8_t *speed);

#endif /* _PT_UART_H_ */
