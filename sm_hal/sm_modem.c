/*
 * sm_modem.c
 *
 *  Created on: Nov 28, 2025
 *      Author: MYPC
 */

#include "sm_modem.h"
#include <string.h>

void modem_recv_data_callback(uint8_t _data, void *_arg){ // call in interrupt
    if(!_arg) return;
    sm_modem_t *modem = (sm_modem_t*)_arg;
    char* buff = (char*)modem->buff;
    if(modem->buff_index < modem->buff_size){
        buff[modem->buff_index++] = (char)_data;
    }
}

void sm_modem_init(sm_modem_t *modem, sm_hal_io_t *io, sm_hal_uart_t *driver, uint32_t _size){
    if(!modem) return;
    modem->reset_pin = io;
    modem->driver = driver;
    modem->buff_size = _size;
    modem->buff_index = 0;
    if(modem->buff == NULL){
        modem->buff = (char*)malloc(_size);
    }
    sm_hal_uart_set_rx_cb(driver, modem_recv_data_callback, modem);
    modem->lock = 0;
}

void sm_modem_deinit(sm_modem_t *modem){
    if(!modem) return;
    modem->reset_pin = NULL;
    modem->driver = NULL;
}

sm_modem_send_cmd(sm_modem_t *modem, char* cmd, char* res_succ, char* res_fail){
    if(!modem || !cmd || !res_succ || !res_fail){
        return -1;
    }
    while (modem->lock){
        // can mot ham thoi gian de doi
    }
    modem->lock = 1;
    //
    //can 2 ham resetz_timer
    memset(modem->buff, 0, modem->buff_size);
    modem->buff_index = 0;
    sm_hal_uart_write(modem->driver, (uint8_t*)cmd, strlen(cmd));
    int32_t res = 0;
    while(1){
        // can 1 ham elapsed_timer_get_remain ktra thoi gian phan hoi, neu timeout thì res trả ve -1 vi het tgian phan hoi
        if(strstr(modem->buff, res_succ) != NULL){
            res = 0;
            break;
        }
        if(strstr(modem->buff, res_fail) != NULL){
            res = -2;
            break;
        }
    }
    modem->lock = 0;
    return res;
}

// sm_modem_send_recv giong sm_modem_send nhưng khác ở chỗ có res_buf để lưu phản hồi
sm_modem_send_recv(sm_modem_t *modem, char* cmd, char* res_succ, char* res_fail, char* res_buff, uint32_t res_buff_size){
    if(!modem || !cmd){
        return -1;
    }
    while(modem->lock){
        // can ham tgian delay de chờ lock mở
    }
    modem->lock = 1;
    // can 2 ham reset tgian
    memset(modem->buff, 0, modem->buff_size);
    modem->buff_index = 0;
    sm_hal_uart_write(modem->driver, (uint8_t*)cmd, strlen(cmd));
    int32_t res= 0;
    while(1){
        // timeout phan hoi thi tra ve -1 (can ham tgian)
        if(strstr(modem->buff, res_succ) != NULL){
            res = 0;
            break;
        }
        if(strstr(modem->buff, res_fail) != NULL){
                    res = -2;
                    break;
                }
    }
    memcpy(res_buff, modem->buff, res_buff_size);
    modem->lock = 0;
    return res;
}


sm_modem_write(sm_modem_t *modem, char* data, int32_t len){
    if(!modem){
        return -1;
    }
    while(modem->lock){
        // can 1 ham delay de doi het lock
    }
    modem->lock = 1;
    int rc = sm_hal_uart_write(modem->driver, (uint8_t*)data, len);
    modem->lock = 0;
    return rc;
}

sm_modem_hw_reset(sm_modem_t *modem, uint32_t duration_ms, int logic){
    (void)duration_ms;
    if(!modem) return;
    if(logic){
        sm_hal_io_set_value(modem->reset_pin, 0);
        //ham delay thoi gian
        sm_hal_io_set_value(modem->reset_pin, 1);
    }else{
        sm_hal_io_set_value(modem->reset_pin, 1);
        //ham delay thoi gian
        sm_hal_io_set_value(modem->reset_pin, 0);
    }
}

sm_modem_read(sm_modem_t *modem, char* data, uint32_t len){
    if(!modem) return -1;
    while(modem->lock){
        // ham delay doi
    }
    modem->lock = 1;
    uint32_t length = 0;
    if(len <= modem->buff_size && modem->buff_index != 0){
        length = (len < (modem->buff_index + 1)) ? len : (modem->buff_index + 1);
        memcpy(data, modem->buff, length);
        memset(modem->buff, 0, modem->buff_size);
        modem->buff_index = 0;
    }
    modem->lock = 0;
    return length;
}


