#include "pt_uart.h"

uint8_t PROTOCOL_Pack(uint8_t cmd,uint8_t speed,uint8_t *out_frame)
{
	
	out_frame[0] = UART_HAND;
	out_frame[1] = cmd;
	out_frame[2] = speed;
	out_frame[3] = UART_TRAIL;
	
	return 4;
}

uint8_t PROTOCOL_Parse(uint8_t *data, uint8_t len, uint8_t *cmd, uint8_t *speed)
{
    if (len < 4) return 0;
    
    for (int i = 0; i <= len - 4; i++) {
        if (data[i] == UART_HAND && data[i + 3] == UART_TRAIL) {
            *cmd = data[i + 1];
            *speed = data[i + 2];
            return 1;
        }
    }
    return 0;
}
