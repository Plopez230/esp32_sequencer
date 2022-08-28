/*************************************************************************************
  Copyright (c) 2022, plopez230@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
**************************************************************************************/

#ifndef SEQ_PERIPHERAL_H
# define SEQ_PERIPHERAL_H

# include <U8g2lib.h>
# include "Adafruit_MPR121.h"

/**
 * SD Card pins
 */
# define SEQ_SD_MOSI_PIN       15
# define SEQ_SD_MISO_PIN       19
# define SEQ_SD_SCK_PIN        14
# define SEQ_SD_CS_PIN         13

/**
 * ST7920 Screen pins
 */
# define SEQ_ST7920_CLK_PIN    18
# define SEQ_ST7920_DATA_PIN   23
# define SEQ_ST7920_CS_PIN     5
# define SEQ_ST7920_RESET_PIN  -1    // Reset pin is not connected

/**
 * MPR121 Keyboard pins
 */
# define SEQ_MPR121_SDA_PIN    21
# define SEQ_MPR121_SCL_PIN    22
# define SEQ_KEYBOARD_MODE_PIN 16

/**
 * YM2413 Synthesizer pins
 */
# define SEQ_YM2413_DATA_PIN   27
# define SEQ_YM2413_CLK_PIN    0
# define SEQ_YM2413_LTC_PIN    4
# define SEQ_YM2413_CS_PIN     32
# define SEQ_YM2413_AO_PIN     26
# define SEQ_YM2413_IC_PIN     33
# define SEQ_YM2413_WE_PIN     25

/**
 * U8G2 Handler class
 */
# define SEQ_U8G2_CLASS U8G2_ST7920_128X64_F_SW_SPI

extern SEQ_U8G2_CLASS u8g2;

/**
 * Keyboard structure
 */
typedef struct    s_seq_keyboard
{
  Adafruit_MPR121 device_a;
  Adafruit_MPR121 device_c;
  Adafruit_MPR121 device_d;
  cppQueue        *output_buffer;
  uint16_t        last_touched_a;
  uint16_t        last_touched_c;
  uint16_t        last_touched_d;
  uint8_t         operation_mode;
  uint8_t         button_state;
  uint8_t         last_button_state;
  uint32_t        last_debounce_time;
  uint8_t         velocity;
  uint8_t         traspose;
  uint8_t         num_mode;
  uint8_t         mayus_mode;
  uint8_t         midi_channel;
  void            (*press_callback)(s_seq_keyboard *, uint8_t);
  void            (*release_callback)(s_seq_keyboard *, uint8_t);
}                 t_seq_keyboard;

t_seq_keyboard *seq_keyboard_create();
void seq_keyboard_init(t_seq_keyboard *keyboard);
void seq_keyboard_device_touched(t_seq_keyboard *keyboard, Adafruit_MPR121 *seq_mpr121, uint16_t *last_touched, uint8_t key_offset);
void seq_keyboard_instrument_press(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_instrument_release(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_set_instrument_mode(t_seq_keyboard *keyboard);
void seq_keyboard_control_press(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_control_release(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_set_control_mode(t_seq_keyboard *keyboard);
void seq_keyboard_update_mode(t_seq_keyboard *keyboard);
void seq_keyboard_text_press(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_text_release(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_application_press(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_application_release(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_set_application_mode(t_seq_keyboard *keyboard);
void seq_keyboard_set_text_mode(t_seq_keyboard *keyboard);
void seq_keyboard_loop(t_seq_keyboard *keyboard);
void seq_keyboard_check_mode_button(t_seq_keyboard *keyboard);

void seq_st7920_prepare(void);
void seq_st7920_draw_logo(void);
void seq_st7920_init(void);
void seq_st7920_draw(void (*draw_callback)());

void seq_ym2413_shift (uint8_t value);
void seq_ym2413_setup_pins();
void seq_ym2413_select_chip();
void seq_ym2413_reset_chip();
void seq_ym2413_setup();
void seq_ym2413_write(byte address, byte data);
void seq_ym2413_play_note(s_seq_synth_tuning *tuning, int channel, int note, int instrument, int vol);
void seq_ym2413_stop_note(int channel, boolean sustain);
#endif
