//
// Created by 81301 on 2024/10/9.
//
#include "stm32f4xx.h"
#include "dma.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // 接收一帧18字节后转发

    }
}