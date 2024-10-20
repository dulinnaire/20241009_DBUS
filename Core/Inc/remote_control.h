//
// Created by 81301 on 2024/10/9.
//

#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "gpio.h"
#include "stm32f4xx.h"
#include "usart.h"

#define RC_FRAME_SIZE ((uint8_t)18)
#define RC_CH_VAL_MIN ((uint16_t)364)
#define RC_CH_VAL_MAX ((uint16_t)1684)
#define RC_CH_VAL_MID ((uint16_t)1024)

enum RCSwitchStatus { UP = 1, DOWN = 2, MID = 3 };

class RC {
public:
    RC();

    struct RCChannel {
        float r_row;
        float r_col;
        float l_row;
        float l_col;
    } channel_;

    struct RCSwitch {
        RCSwitchStatus l_switch;
        RCSwitchStatus r_switch;
    } switch_;

    struct RCMouse {
        float mouse_x;
        float mouse_y;
        float mouse_z;
    } mouse_;

    struct RCMouseClick {
        uint8_t l_click;
        uint8_t r_click;
    } click_;

    enum Key { W = 0, S = 1, A = 2, D = 4, Q = 8, E = 16, SHIFT = 32, CTRL = 64 } key;

    void rc_rx_enable();

    void rc_data_process();

    void rc_transfer();

private:
    uint8_t rc_buff[RC_FRAME_SIZE];
    uint8_t rc_data[RC_FRAME_SIZE];
};

#endif //REMOTE_CONTROL_H
