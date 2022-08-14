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

#include <Arduino.h>
#include <Wire.h>

#include "seq_include.h"

void seq_keyboard_init(t_seq_keyboard *keyboard)
{
  Wire.begin(SEQ_MPR121_SDA_PIN, SEQ_MPR121_SCL_PIN);
  digitalWrite(21, 0);
  digitalWrite(22, 0);
  keyboard->device_a = Adafruit_MPR121();
  keyboard->device_c = Adafruit_MPR121();
  keyboard->device_d = Adafruit_MPR121();
  keyboard->device_a.begin(0x5A);
  keyboard->device_c.begin(0x5C);
  keyboard->device_d.begin(0x5D);
  keyboard->midi_channel = 6;
  keyboard->velocity = 0;
  seq_keyboard_set_instrument_mode(keyboard);
}

void seq_keyboard_device_touched(t_seq_keyboard *keyboard, Adafruit_MPR121 *seq_mpr121, uint16_t *last_touched, uint8_t key_offset)
{
  uint16_t curr_touched = seq_mpr121->touched();
  for (uint8_t i = 0; i < 12; i++) {
    if ((curr_touched & _BV(i)) && !(*last_touched & _BV(i)) ) {
      keyboard->press_callback(keyboard, i + key_offset);
    }
    if (!(curr_touched & _BV(i)) && (*last_touched & _BV(i)) ) {
      keyboard->release_callback(keyboard, i + key_offset);
    }
  }
  *last_touched = curr_touched;
}

void seq_keyboard_instrument_press(t_seq_keyboard *keyboard, uint8_t key)
{
  char msg[3] = {(char)(144 + keyboard->midi_channel), key, (char)(keyboard->velocity)};
  seq_ring_push(keyboard->output_buffer, msg);
}

void seq_keyboard_instrument_release(t_seq_keyboard *keyboard, uint8_t key)
{
  char msg[3] = {(char)(128 + keyboard->midi_channel), key, (char)(keyboard->velocity)};
  seq_ring_push(keyboard->output_buffer, msg);
}

void seq_keyboard_set_instrument_mode(t_seq_keyboard *keyboard)
{
   keyboard->press_callback = seq_keyboard_instrument_press;
   keyboard->release_callback = seq_keyboard_instrument_release;
}

void seq_keyboard_control_press(t_seq_keyboard *keyboard, uint8_t key)
{
  Serial.print("Press   ");
  Serial.println(key);
}

void seq_keyboard_control_release(t_seq_keyboard *keyboard, uint8_t key)
{
  Serial.print("Release ");
  Serial.println(key);
}

void seq_keyboard_set_control_mode(t_seq_keyboard *keyboard)
{
   keyboard->press_callback = seq_keyboard_control_press;
   keyboard->release_callback = seq_keyboard_control_release;
}

void seq_keyboard_text_press(t_seq_keyboard *keyboard, uint8_t key)
{
  Serial.print((char)('a'+key));
}

void seq_keyboard_text_release(t_seq_keyboard *keyboard, uint8_t key)
{
  
}

void seq_keyboard_set_text_mode(t_seq_keyboard *keyboard)
{
  keyboard->press_callback = seq_keyboard_text_press;
  keyboard->release_callback = seq_keyboard_text_release;
}

void seq_keyboard_touched(t_seq_keyboard *keyboard)
{
  seq_keyboard_device_touched(keyboard, &keyboard->device_a, &(keyboard->last_touched_a), 12);
  seq_keyboard_device_touched(keyboard, &keyboard->device_c, &(keyboard->last_touched_c), 0);
  seq_keyboard_device_touched(keyboard, &keyboard->device_d, &(keyboard->last_touched_d), 24);
}
