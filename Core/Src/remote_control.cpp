#include "remote_control.h"

int cnt = 0;
static float linear_mapping(const float input, float i_min, float i_max, float o_min, float o_max) {
    return (input - i_min) * (o_max - o_min) * 1.0 / (i_max - i_min) + o_min;
}

static float channel_mapping(const float input) {
    return linear_mapping(input, RC_CH_VAL_MIN, RC_CH_VAL_MAX, -1, 1);
}

static float mouse_mapping(const int16_t input) {
    return linear_mapping(input, -32768, 32767, -1, 1);
}

RC::RC() {
    for (int i = 0; i < 18; i++) {
        this->rc_buff[i] = 0;
        this->rc_data[i] = 0;
    }
    channel_.l_col = 0;
    channel_.l_row = 0;
    channel_.r_col = 0;
    channel_.r_row = 0;

    switch_.l_switch = DOWN;
    switch_.r_switch = DOWN;

    mouse_.mouse_x = 0;
    mouse_.mouse_y = 0;
    mouse_.mouse_z = 0;

    click_.l_click = 0;
    click_.r_click = 0;

    key = A;
}

void RC::rc_rx_enable() {
    HAL_UART_Receive_DMA(&huart1, this->rc_buff, 18);
    cnt++;
}

void RC::rc_transfer() {
    for (int i = 0; i < 18; i++) {
        rc_data[i] = rc_buff[i];
        cnt++;
    }
}

void RC::rc_data_process() {
    this->rc_transfer();
    // 11bits channel
    channel_.r_row = channel_mapping((rc_data[0] | rc_data[1] << 8) & 0x07FF);
    channel_.r_col = channel_mapping((rc_data[1] >> 3 | rc_data[2] << 5) & 0x07FF);
    channel_.l_row =
        channel_mapping((rc_data[2] >> 6 | rc_data[3] << 2 | rc_data[4] << 10) & 0x7FF);
    channel_.l_col = channel_mapping((rc_data[4] >> 1 | rc_data[5] << 7) & 0x7FF);

    // 2bits switch
    switch_.l_switch = (RCSwitchStatus)(rc_data[5] >> 4 & 0x03);
    switch_.r_switch = (RCSwitchStatus)(rc_data[5] >> 6 & 0x03);

    // 16bits mouse channel
    mouse_.mouse_x = mouse_mapping((int16_t)(rc_data[6] | rc_data[7] << 8));
    mouse_.mouse_y = mouse_mapping((int16_t)(rc_data[8] | rc_data[9] << 8));
    mouse_.mouse_z = mouse_mapping((int16_t)(rc_data[10] | rc_data[11] << 8));

    // 8bits mouse click
    click_.l_click = rc_data[12] & 0x1;
    click_.r_click = rc_data[13] & 0x1;

    // 16bits key
    key = (Key)((rc_data[14] | rc_data[15] << 8) & 0xFF);
}
