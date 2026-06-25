#ifndef _APP_ENCODER_H_
#define _APP_ENCODER_H_

#include "main.h"

void APP_ENCODER_Init(void);

void APP_ENCODER_Start(void);
int32_t APP_ENCODER_Count(void);
float APP_ENCODER_GetRevolutions(void);
void APP_ENCODER_Clear(void);
#endif /* _APP_ENCODER_H_ */
