/*************************************************************************************
  Copyright (c) 2022, plopez230@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice, this
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

#include "../seq.h"

t_seq_keyboard *seq_keyboard_create()
{
  t_seq_keyboard *new_keyboard;

  new_keyboard = (t_seq_keyboard *)malloc(sizeof(t_seq_keyboard));
  if (!new_keyboard)
    return (NULL);
  return (new_keyboard);
}

void seq_keyboard_init(t_seq_keyboard *keyboard)
{
  Wire.begin(SEQ_MPR121_SDA_PIN, SEQ_MPR121_SCL_PIN);
  pinMode(SEQ_KEYBOARD_MODE_PIN, INPUT_PULLUP);
  keyboard->last_debounce_time = 0;
  keyboard->last_button_state = 0;
  keyboard->operation_mode = 1;
  seq_keyboard_update_mode(keyboard);
  digitalWrite(21, 0);
  digitalWrite(22, 0);
  keyboard->traspose = 12;
  keyboard->device_a = Adafruit_MPR121();
  keyboard->device_c = Adafruit_MPR121();
  keyboard->device_d = Adafruit_MPR121();
  keyboard->device_a.begin(0x5A);
  keyboard->device_c.begin(0x5C);
  keyboard->device_d.begin(0x5D);
  keyboard->midi_channel = 0;
  keyboard->velocity = 0;
}

const char seq_keyboard_alpha_layout[] = "abcdefghijklmnopqrstuvwxyz";
const char seq_keyboard_alpha_mayus_layout[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char seq_keyboard_num_layout[] = "0123456789-_().,;:{}[]<>?!";
const char seq_keyboard_num_mayus_layout[] = "+-*/^=|.$&%";

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
  char msg[3] = {(char)(144 + keyboard->midi_channel), (uint8_t)(key + keyboard->traspose), (uint8_t)(keyboard->velocity)};
  if (keyboard->output_buffer)
  {
    keyboard->output_buffer->push(msg);
  }
}

void seq_keyboard_instrument_release(t_seq_keyboard *keyboard, uint8_t key)
{
  char msg[3] = {(char)(128 + keyboard->midi_channel), (uint8_t)(key + keyboard->traspose), (char)(keyboard->velocity)};

  if (keyboard->output_buffer)
  {
    keyboard->output_buffer->push(msg);
  }
}

void seq_keyboard_set_instrument_mode(t_seq_keyboard *keyboard)
{
  keyboard->press_callback = seq_keyboard_instrument_press;
  keyboard->release_callback = seq_keyboard_instrument_release;
}

void seq_keyboard_control_press(t_seq_keyboard *keyboard, uint8_t key)
{
  char c;
  if (key == 26)
  {
    c = 17;
    seq_system.console->input_buffer->push(&c);
  }
  else if (key == 27)
  {
    c = 18;
    seq_system.console->input_buffer->push(&c);
  }
  else if (key == 28)
  {
    c = 19;
    seq_system.console->input_buffer->push(&c);
  }
  else if (key == 29)
  {
    c = 20;
    seq_system.console->input_buffer->push(&c);
  }
  else if (key == 30)
  {
    c = 28;
    seq_system.console->input_buffer->push(&c);
  }
  else if (key == 31)
  {
    c = ' ';
    seq_system.console->input_buffer->push(&c);
  }
  else if (key == 32)
  {
    c = '\n';
    seq_system.console->input_buffer->push(&c);
  }
  else if (key == 33)
  {
    keyboard->mayus_mode = !keyboard->mayus_mode;
  }
  else if (key == 34)
  {
    keyboard->num_mode = !keyboard->num_mode;
  }
  else if (key == 35)
  {
    c = 27;
    seq_system.console->input_buffer->push(&c);
  } else if (keyboard->num_mode)
  {
    if (keyboard->mayus_mode) {
      //seq_system.console->input_buffer->push(&seq_keyboard_num_mayus_layout[key]);
      if (key < 11) seq_system.console->input_buffer->push(&seq_keyboard_num_mayus_layout[key]); //Serial.print(seq_keyboard_num_mayus_layout[key]);
    } else {
      seq_system.console->input_buffer->push(&seq_keyboard_num_layout[key]);
      //Serial.print(seq_keyboard_num_layout[key]);
    }
  } else {
    if (keyboard->mayus_mode) {
      seq_system.console->input_buffer->push(&seq_keyboard_alpha_mayus_layout[key]);
      //Serial.print(seq_keyboard_alpha_mayus_layout[key]);
    } else {
      seq_system.console->input_buffer->push(&seq_keyboard_alpha_layout[key]);
      //Serial.print(seq_keyboard_alpha_layout[key]);
    }
  }
  xTaskNotifyGive(seq_system.console->task_handler);
}

void seq_keyboard_control_release(t_seq_keyboard *keyboard, uint8_t key)
{

}

void seq_keyboard_set_control_mode(t_seq_keyboard *keyboard)
{
  keyboard->press_callback = seq_keyboard_control_press;
  keyboard->release_callback = seq_keyboard_control_release;
}

void seq_keyboard_update_mode(t_seq_keyboard *keyboard)
{
  if (keyboard->operation_mode == 0)
  {
    seq_keyboard_set_instrument_mode(keyboard);
  }
  else if (keyboard->operation_mode == 1)
  {
    seq_keyboard_set_control_mode(keyboard);
  }
}

void seq_keyboard_text_press(t_seq_keyboard *keyboard, uint8_t key)
{
  Serial.print((char)('a' + key));
}

void seq_keyboard_text_release(t_seq_keyboard *keyboard, uint8_t key)
{

}

void seq_keyboard_application_press(t_seq_keyboard *keyboard, uint8_t key)
{
  if (key > 25)
    seq_keyboard_control_press(keyboard, key);
  else if (key <= 25)
    seq_keyboard_instrument_press(keyboard, key);
}

void seq_keyboard_application_release(t_seq_keyboard *keyboard, uint8_t key)
{
  if (key > 25)
    seq_keyboard_control_release(keyboard, key);
  else if (key <= 25)
    seq_keyboard_instrument_release(keyboard, key);
}

void seq_keyboard_set_application_mode(t_seq_keyboard *keyboard)
{
  keyboard->press_callback = seq_keyboard_application_press;
  keyboard->release_callback = seq_keyboard_application_release;
}

void seq_keyboard_set_text_mode(t_seq_keyboard *keyboard)
{
  keyboard->press_callback = seq_keyboard_text_press;
  keyboard->release_callback = seq_keyboard_text_release;
}

void seq_keyboard_loop(t_seq_keyboard *keyboard)
{
  seq_keyboard_check_mode_button(keyboard);
  seq_keyboard_device_touched(keyboard, &keyboard->device_a, &(keyboard->last_touched_a), 12);
  seq_keyboard_device_touched(keyboard, &keyboard->device_c, &(keyboard->last_touched_c), 0);
  seq_keyboard_device_touched(keyboard, &keyboard->device_d, &(keyboard->last_touched_d), 24);

}

void seq_keyboard_check_mode_button(t_seq_keyboard *keyboard) {
  int reading = digitalRead(SEQ_KEYBOARD_MODE_PIN);
  if (reading != keyboard->last_button_state) {
    keyboard->last_debounce_time = millis();
  }
  if ((millis() - keyboard->last_debounce_time) > SEQ_CONFIG_DEBOUNCE_DELAY) {
    if (reading != keyboard->button_state) {
      keyboard->button_state = reading;
      if (keyboard->button_state == HIGH) {
        keyboard->operation_mode = !keyboard->operation_mode;
        keyboard->num_mode = 0;
        keyboard->mayus_mode = 0;
        seq_keyboard_update_mode(keyboard);
      }
    }
  }
  keyboard->last_button_state = reading;
}
