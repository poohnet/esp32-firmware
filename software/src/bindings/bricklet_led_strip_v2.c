/* ***********************************************************
 * This file was automatically generated on 2022-05-02.      *
 *                                                           *
 * C/C++ for Microcontrollers Bindings Version 2.0.0         *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generators git repository on tinkerforge.com       *
 *************************************************************/


#include "bricklet_led_strip_v2.h"
#include "base58.h"
#include "endian_convert.h"
#include "errors.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


#if TF_IMPLEMENT_CALLBACKS != 0
static bool tf_led_strip_v2_callback_handler(void *device, uint8_t fid, TF_PacketBuffer *payload) {
    TF_LEDStripV2 *led_strip_v2 = (TF_LEDStripV2 *)device;
    TF_HALCommon *hal_common = tf_hal_get_common(led_strip_v2->tfp->spitfp->hal);
    (void)payload;

    switch (fid) {
        case TF_LED_STRIP_V2_CALLBACK_FRAME_STARTED: {
            TF_LEDStripV2_FrameStartedHandler fn = led_strip_v2->frame_started_handler;
            void *user_data = led_strip_v2->frame_started_user_data;
            if (fn == NULL) {
                return false;
            }

            uint16_t length = tf_packet_buffer_read_uint16_t(payload);
            hal_common->locked = true;
            fn(led_strip_v2, length, user_data);
            hal_common->locked = false;
            break;
        }

        default:
            return false;
    }

    return true;
}
#else
static bool tf_led_strip_v2_callback_handler(void *device, uint8_t fid, TF_PacketBuffer *payload) {
    return false;
}
#endif
int tf_led_strip_v2_create(TF_LEDStripV2 *led_strip_v2, const char *uid_or_port_name, TF_HAL *hal) {
    if (led_strip_v2 == NULL || hal == NULL) {
        return TF_E_NULL;
    }

    memset(led_strip_v2, 0, sizeof(TF_LEDStripV2));

    TF_TFP *tfp;
    int rc = tf_hal_get_attachable_tfp(hal, &tfp, uid_or_port_name, TF_LED_STRIP_V2_DEVICE_IDENTIFIER);

    if (rc != TF_E_OK) {
        return rc;
    }

    led_strip_v2->tfp = tfp;
    led_strip_v2->tfp->device = led_strip_v2;
    led_strip_v2->tfp->cb_handler = tf_led_strip_v2_callback_handler;
    led_strip_v2->magic = 0x5446;
    led_strip_v2->response_expected[0] = 0x21;
    led_strip_v2->response_expected[1] = 0x00;
    return TF_E_OK;
}

int tf_led_strip_v2_destroy(TF_LEDStripV2 *led_strip_v2) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }
    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    led_strip_v2->tfp->cb_handler = NULL;
    led_strip_v2->tfp->device = NULL;
    led_strip_v2->tfp = NULL;
    led_strip_v2->magic = 0;

    return TF_E_OK;
}

int tf_led_strip_v2_get_response_expected(TF_LEDStripV2 *led_strip_v2, uint8_t function_id, bool *ret_response_expected) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    switch (function_id) {
        case TF_LED_STRIP_V2_FUNCTION_SET_LED_VALUES_LOW_LEVEL:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[0] & (1 << 0)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_FRAME_DURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[0] & (1 << 1)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_CLOCK_FREQUENCY:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[0] & (1 << 2)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_CHIP_TYPE:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[0] & (1 << 3)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_CHANNEL_MAPPING:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[0] & (1 << 4)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_FRAME_STARTED_CALLBACK_CONFIGURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[0] & (1 << 5)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[0] & (1 << 6)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_STATUS_LED_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[0] & (1 << 7)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_RESET:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[1] & (1 << 0)) != 0;
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_WRITE_UID:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (led_strip_v2->response_expected[1] & (1 << 1)) != 0;
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_led_strip_v2_set_response_expected(TF_LEDStripV2 *led_strip_v2, uint8_t function_id, bool response_expected) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    switch (function_id) {
        case TF_LED_STRIP_V2_FUNCTION_SET_LED_VALUES_LOW_LEVEL:
            if (response_expected) {
                led_strip_v2->response_expected[0] |= (1 << 0);
            } else {
                led_strip_v2->response_expected[0] &= ~(1 << 0);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_FRAME_DURATION:
            if (response_expected) {
                led_strip_v2->response_expected[0] |= (1 << 1);
            } else {
                led_strip_v2->response_expected[0] &= ~(1 << 1);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_CLOCK_FREQUENCY:
            if (response_expected) {
                led_strip_v2->response_expected[0] |= (1 << 2);
            } else {
                led_strip_v2->response_expected[0] &= ~(1 << 2);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_CHIP_TYPE:
            if (response_expected) {
                led_strip_v2->response_expected[0] |= (1 << 3);
            } else {
                led_strip_v2->response_expected[0] &= ~(1 << 3);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_CHANNEL_MAPPING:
            if (response_expected) {
                led_strip_v2->response_expected[0] |= (1 << 4);
            } else {
                led_strip_v2->response_expected[0] &= ~(1 << 4);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_FRAME_STARTED_CALLBACK_CONFIGURATION:
            if (response_expected) {
                led_strip_v2->response_expected[0] |= (1 << 5);
            } else {
                led_strip_v2->response_expected[0] &= ~(1 << 5);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (response_expected) {
                led_strip_v2->response_expected[0] |= (1 << 6);
            } else {
                led_strip_v2->response_expected[0] &= ~(1 << 6);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_SET_STATUS_LED_CONFIG:
            if (response_expected) {
                led_strip_v2->response_expected[0] |= (1 << 7);
            } else {
                led_strip_v2->response_expected[0] &= ~(1 << 7);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_RESET:
            if (response_expected) {
                led_strip_v2->response_expected[1] |= (1 << 0);
            } else {
                led_strip_v2->response_expected[1] &= ~(1 << 0);
            }
            break;
        case TF_LED_STRIP_V2_FUNCTION_WRITE_UID:
            if (response_expected) {
                led_strip_v2->response_expected[1] |= (1 << 1);
            } else {
                led_strip_v2->response_expected[1] &= ~(1 << 1);
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_led_strip_v2_set_response_expected_all(TF_LEDStripV2 *led_strip_v2, bool response_expected) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    memset(led_strip_v2->response_expected, response_expected ? 0xFF : 0, 2);

    return TF_E_OK;
}

int tf_led_strip_v2_set_led_values_low_level(TF_LEDStripV2 *led_strip_v2, uint16_t index, uint16_t value_length, uint16_t value_chunk_offset, const uint8_t value_chunk_data[58]) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_SET_LED_VALUES_LOW_LEVEL, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_LED_VALUES_LOW_LEVEL, 64, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    index = tf_leconvert_uint16_to(index); memcpy(_send_buf + 0, &index, 2);
    value_length = tf_leconvert_uint16_to(value_length); memcpy(_send_buf + 2, &value_length, 2);
    value_chunk_offset = tf_leconvert_uint16_to(value_chunk_offset); memcpy(_send_buf + 4, &value_chunk_offset, 2);
    memcpy(_send_buf + 6, value_chunk_data, 58);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_led_values_low_level(TF_LEDStripV2 *led_strip_v2, uint16_t index, uint16_t length, uint16_t *ret_value_length, uint16_t *ret_value_chunk_offset, uint8_t ret_value_chunk_data[60]) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_LED_VALUES_LOW_LEVEL, 4, _response_expected);

    size_t _i;
    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    index = tf_leconvert_uint16_to(index); memcpy(_send_buf + 0, &index, 2);
    length = tf_leconvert_uint16_to(length); memcpy(_send_buf + 2, &length, 2);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 64) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_value_length != NULL) { *ret_value_length = tf_packet_buffer_read_uint16_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 2); }
            if (ret_value_chunk_offset != NULL) { *ret_value_chunk_offset = tf_packet_buffer_read_uint16_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 2); }
            if (ret_value_chunk_data != NULL) { for (_i = 0; _i < 60; ++_i) ret_value_chunk_data[_i] = tf_packet_buffer_read_uint8_t(_recv_buf);} else { tf_packet_buffer_remove(_recv_buf, 60); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 64) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_set_frame_duration(TF_LEDStripV2 *led_strip_v2, uint16_t duration) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_SET_FRAME_DURATION, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_FRAME_DURATION, 2, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    duration = tf_leconvert_uint16_to(duration); memcpy(_send_buf + 0, &duration, 2);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_frame_duration(TF_LEDStripV2 *led_strip_v2, uint16_t *ret_duration) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_FRAME_DURATION, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 2) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_duration != NULL) { *ret_duration = tf_packet_buffer_read_uint16_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 2); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 2) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_supply_voltage(TF_LEDStripV2 *led_strip_v2, uint16_t *ret_voltage) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_SUPPLY_VOLTAGE, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 2) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_voltage != NULL) { *ret_voltage = tf_packet_buffer_read_uint16_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 2); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 2) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_set_clock_frequency(TF_LEDStripV2 *led_strip_v2, uint32_t frequency) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_SET_CLOCK_FREQUENCY, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_CLOCK_FREQUENCY, 4, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    frequency = tf_leconvert_uint32_to(frequency); memcpy(_send_buf + 0, &frequency, 4);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_clock_frequency(TF_LEDStripV2 *led_strip_v2, uint32_t *ret_frequency) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_CLOCK_FREQUENCY, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 4) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_frequency != NULL) { *ret_frequency = tf_packet_buffer_read_uint32_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 4); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 4) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_set_chip_type(TF_LEDStripV2 *led_strip_v2, uint16_t chip) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_SET_CHIP_TYPE, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_CHIP_TYPE, 2, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    chip = tf_leconvert_uint16_to(chip); memcpy(_send_buf + 0, &chip, 2);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_chip_type(TF_LEDStripV2 *led_strip_v2, uint16_t *ret_chip) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_CHIP_TYPE, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 2) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_chip != NULL) { *ret_chip = tf_packet_buffer_read_uint16_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 2); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 2) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_set_channel_mapping(TF_LEDStripV2 *led_strip_v2, uint8_t mapping) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_SET_CHANNEL_MAPPING, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_CHANNEL_MAPPING, 1, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    _send_buf[0] = (uint8_t)mapping;

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_channel_mapping(TF_LEDStripV2 *led_strip_v2, uint8_t *ret_mapping) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_CHANNEL_MAPPING, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 1) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_mapping != NULL) { *ret_mapping = tf_packet_buffer_read_uint8_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 1); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 1) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_set_frame_started_callback_configuration(TF_LEDStripV2 *led_strip_v2, bool enable) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_SET_FRAME_STARTED_CALLBACK_CONFIGURATION, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_FRAME_STARTED_CALLBACK_CONFIGURATION, 1, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    _send_buf[0] = enable ? 1 : 0;

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_frame_started_callback_configuration(TF_LEDStripV2 *led_strip_v2, bool *ret_enable) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_FRAME_STARTED_CALLBACK_CONFIGURATION, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 1) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_enable != NULL) { *ret_enable = tf_packet_buffer_read_bool(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 1); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 1) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_spitfp_error_count(TF_LEDStripV2 *led_strip_v2, uint32_t *ret_error_count_ack_checksum, uint32_t *ret_error_count_message_checksum, uint32_t *ret_error_count_frame, uint32_t *ret_error_count_overflow) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_SPITFP_ERROR_COUNT, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 16) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_error_count_ack_checksum != NULL) { *ret_error_count_ack_checksum = tf_packet_buffer_read_uint32_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 4); }
            if (ret_error_count_message_checksum != NULL) { *ret_error_count_message_checksum = tf_packet_buffer_read_uint32_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 4); }
            if (ret_error_count_frame != NULL) { *ret_error_count_frame = tf_packet_buffer_read_uint32_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 4); }
            if (ret_error_count_overflow != NULL) { *ret_error_count_overflow = tf_packet_buffer_read_uint32_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 4); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 16) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_set_bootloader_mode(TF_LEDStripV2 *led_strip_v2, uint8_t mode, uint8_t *ret_status) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_BOOTLOADER_MODE, 1, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    _send_buf[0] = (uint8_t)mode;

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 1) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 1); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 1) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_bootloader_mode(TF_LEDStripV2 *led_strip_v2, uint8_t *ret_mode) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_BOOTLOADER_MODE, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 1) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_mode != NULL) { *ret_mode = tf_packet_buffer_read_uint8_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 1); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 1) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_set_write_firmware_pointer(TF_LEDStripV2 *led_strip_v2, uint32_t pointer) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER, 4, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    pointer = tf_leconvert_uint32_to(pointer); memcpy(_send_buf + 0, &pointer, 4);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_write_firmware(TF_LEDStripV2 *led_strip_v2, const uint8_t data[64], uint8_t *ret_status) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_WRITE_FIRMWARE, 64, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    memcpy(_send_buf + 0, data, 64);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 1) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 1); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 1) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_set_status_led_config(TF_LEDStripV2 *led_strip_v2, uint8_t config) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_SET_STATUS_LED_CONFIG, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_SET_STATUS_LED_CONFIG, 1, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    _send_buf[0] = (uint8_t)config;

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_status_led_config(TF_LEDStripV2 *led_strip_v2, uint8_t *ret_config) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_STATUS_LED_CONFIG, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 1) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_config != NULL) { *ret_config = tf_packet_buffer_read_uint8_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 1); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 1) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_chip_temperature(TF_LEDStripV2 *led_strip_v2, int16_t *ret_temperature) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_CHIP_TEMPERATURE, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 2) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_temperature != NULL) { *ret_temperature = tf_packet_buffer_read_int16_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 2); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 2) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_reset(TF_LEDStripV2 *led_strip_v2) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_RESET, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_RESET, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_write_uid(TF_LEDStripV2 *led_strip_v2, uint32_t uid) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_led_strip_v2_get_response_expected(led_strip_v2, TF_LED_STRIP_V2_FUNCTION_WRITE_UID, &_response_expected);
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_WRITE_UID, 4, _response_expected);

    uint8_t *_send_buf = tf_tfp_get_send_payload_buffer(led_strip_v2->tfp);

    uid = tf_leconvert_uint32_to(uid); memcpy(_send_buf + 0, &uid, 4);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 0) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_read_uid(TF_LEDStripV2 *led_strip_v2, uint32_t *ret_uid) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_READ_UID, 0, _response_expected);

    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 4) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_uid != NULL) { *ret_uid = tf_packet_buffer_read_uint32_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 4); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 4) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

int tf_led_strip_v2_get_identity(TF_LEDStripV2 *led_strip_v2, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *_hal = led_strip_v2->tfp->spitfp->hal;

    if (tf_hal_get_common(_hal)->locked) {
        return TF_E_LOCKED;
    }

    bool _response_expected = true;
    tf_tfp_prepare_send(led_strip_v2->tfp, TF_LED_STRIP_V2_FUNCTION_GET_IDENTITY, 0, _response_expected);

    size_t _i;
    uint32_t _deadline = tf_hal_current_time_us(_hal) + tf_hal_get_common(_hal)->timeout;

    uint8_t _error_code = 0;
    uint8_t _length = 0;
    int _result = tf_tfp_send_packet(led_strip_v2->tfp, _response_expected, _deadline, &_error_code, &_length);

    if (_result < 0) {
        return _result;
    }

    if (_result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (_result & TF_TICK_PACKET_RECEIVED) {
        TF_PacketBuffer *_recv_buf = tf_tfp_get_receive_buffer(led_strip_v2->tfp);
        if (_error_code != 0 || _length != 25) {
            tf_packet_buffer_remove(_recv_buf, _length);
        } else {
            if (ret_uid != NULL) { tf_packet_buffer_pop_n(_recv_buf, (uint8_t *)ret_uid, 8);} else { tf_packet_buffer_remove(_recv_buf, 8); }
            if (ret_connected_uid != NULL) { tf_packet_buffer_pop_n(_recv_buf, (uint8_t *)ret_connected_uid, 8);} else { tf_packet_buffer_remove(_recv_buf, 8); }
            if (ret_position != NULL) { *ret_position = tf_packet_buffer_read_char(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 1); }
            if (ret_hardware_version != NULL) { for (_i = 0; _i < 3; ++_i) ret_hardware_version[_i] = tf_packet_buffer_read_uint8_t(_recv_buf);} else { tf_packet_buffer_remove(_recv_buf, 3); }
            if (ret_firmware_version != NULL) { for (_i = 0; _i < 3; ++_i) ret_firmware_version[_i] = tf_packet_buffer_read_uint8_t(_recv_buf);} else { tf_packet_buffer_remove(_recv_buf, 3); }
            if (ret_device_identifier != NULL) { *ret_device_identifier = tf_packet_buffer_read_uint16_t(_recv_buf); } else { tf_packet_buffer_remove(_recv_buf, 2); }
        }
        tf_tfp_packet_processed(led_strip_v2->tfp);
    }

    _result = tf_tfp_finish_send(led_strip_v2->tfp, _result, _deadline);

    if (_error_code == 0 && _length != 25) {
        return TF_E_WRONG_RESPONSE_LENGTH;
    }

    if (_result < 0) {
        return _result;
    }

    return tf_tfp_get_error(_error_code);
}

typedef struct TF_LEDStripV2_SetLEDValuesLLWrapperData {
    uint16_t index;
} TF_LEDStripV2_SetLEDValuesLLWrapperData;


static int tf_led_strip_v2_set_led_values_ll_wrapper(void *device, void *wrapper_data, uint32_t stream_length, uint32_t chunk_offset, void *chunk_data, uint32_t *ret_chunk_written) {
    TF_LEDStripV2_SetLEDValuesLLWrapperData *data = (TF_LEDStripV2_SetLEDValuesLLWrapperData *) wrapper_data;
    uint16_t value_chunk_offset = (uint16_t)chunk_offset;
    uint16_t value_length = (uint16_t)stream_length;
    uint32_t value_chunk_written = 58;

    uint8_t *value_chunk_data = (uint8_t *) chunk_data;
    int ret = tf_led_strip_v2_set_led_values_low_level((TF_LEDStripV2 *)device, data->index, value_length, value_chunk_offset, value_chunk_data);

    *ret_chunk_written = (uint32_t) value_chunk_written;
    return ret;
}

int tf_led_strip_v2_set_led_values(TF_LEDStripV2 *led_strip_v2, uint16_t index, const uint8_t *value, uint16_t value_length) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_LEDStripV2_SetLEDValuesLLWrapperData _wrapper_data;
    memset(&_wrapper_data, 0, sizeof(_wrapper_data));
    _wrapper_data.index = index;

    uint32_t _stream_length = value_length;
    uint32_t _value_written = 0;
    uint8_t _chunk_data[58];

    int ret = tf_stream_in(led_strip_v2, tf_led_strip_v2_set_led_values_ll_wrapper, &_wrapper_data, value, _stream_length, _chunk_data, &_value_written, 58, tf_copy_items_uint8_t);



    return ret;
}


typedef struct TF_LEDStripV2_GetLEDValuesLLWrapperData {
    uint16_t index;
    uint16_t length;
} TF_LEDStripV2_GetLEDValuesLLWrapperData;


static int tf_led_strip_v2_get_led_values_ll_wrapper(void *device, void *wrapper_data, uint32_t *ret_stream_length, uint32_t *ret_chunk_offset, void *chunk_data) {
    TF_LEDStripV2_GetLEDValuesLLWrapperData *data = (TF_LEDStripV2_GetLEDValuesLLWrapperData *) wrapper_data;
    uint16_t value_length = 0;
    uint16_t value_chunk_offset = 0;
    uint8_t *value_chunk_data = (uint8_t *) chunk_data;
    int ret = tf_led_strip_v2_get_led_values_low_level((TF_LEDStripV2 *)device, data->index, data->length, &value_length, &value_chunk_offset, value_chunk_data);

    *ret_stream_length = (uint32_t)value_length;
    *ret_chunk_offset = (uint32_t)value_chunk_offset;
    return ret;
}

int tf_led_strip_v2_get_led_values(TF_LEDStripV2 *led_strip_v2, uint16_t index, uint16_t length, uint8_t *ret_value, uint16_t *ret_value_length) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_LEDStripV2_GetLEDValuesLLWrapperData _wrapper_data;
    memset(&_wrapper_data, 0, sizeof(_wrapper_data));
    _wrapper_data.index = index;
    _wrapper_data.length = length;
    uint32_t _value_length = 0;
    uint8_t _value_chunk_data[60];

    int ret = tf_stream_out(led_strip_v2, tf_led_strip_v2_get_led_values_ll_wrapper, &_wrapper_data, ret_value, &_value_length, _value_chunk_data, 60, tf_copy_items_uint8_t);

    if (ret_value_length != NULL) {
        *ret_value_length = (uint16_t)_value_length;
    }
    return ret;
}
#if TF_IMPLEMENT_CALLBACKS != 0
int tf_led_strip_v2_register_frame_started_callback(TF_LEDStripV2 *led_strip_v2, TF_LEDStripV2_FrameStartedHandler handler, void *user_data) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    led_strip_v2->frame_started_handler = handler;
    led_strip_v2->frame_started_user_data = user_data;

    return TF_E_OK;
}
#endif
int tf_led_strip_v2_callback_tick(TF_LEDStripV2 *led_strip_v2, uint32_t timeout_us) {
    if (led_strip_v2 == NULL) {
        return TF_E_NULL;
    }

    if (led_strip_v2->magic != 0x5446 || led_strip_v2->tfp == NULL) {
        return TF_E_NOT_INITIALIZED;
    }

    TF_HAL *hal = led_strip_v2->tfp->spitfp->hal;

    return tf_tfp_callback_tick(led_strip_v2->tfp, tf_hal_current_time_us(hal) + timeout_us);
}

#ifdef __cplusplus
}
#endif
