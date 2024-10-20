//
// Created by 81301 on 2024/10/9.
//
#include "dma.h"
#include "gpio.h"
#include "remote_control.h"
#include "stm32f4xx.h"

extern RC rc1;
extern int cnt;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart == &huart1) {
        cnt++;
        rc1.rc_rx_enable();
        rc1.rc_data_process();
    }
}
