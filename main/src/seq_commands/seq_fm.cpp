#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../seq.h"

#define SEQ_INT_BOUNDS(num, min, max) \
  {                                   \
    if (num > max)                    \
      num = max;                      \
    if (num < min)                    \
      num = min;                      \
  }

#define SEQ_DRAW_SLIDER(x, y, str, value)                        \
  {                                                              \
    u8g2.drawStr(x, y + 7, str);                                 \
    u8g2.drawLine(13 + x, y + 4, 28 + x, y + 4);                 \
    u8g2.setColorIndex(0);                                       \
    u8g2.drawBox(12 + x + value, y + 3, 3, 3);                   \
    u8g2.setColorIndex(1);                                       \
    u8g2.drawLine(13 + x + value, y + 3, 13 + x + value, y + 5); \
  }

#define SEQ_DRAW_CHECKBOX(x, y, str, value) \
  {                                         \
    u8g2.drawStr(x, y + 7, str);            \
    if (value)                              \
      u8g2.drawBox(x + 14, y + 2, 5, 5);    \
    else                                    \
      u8g2.drawFrame(x + 14, y + 2, 5, 5);  \
  }

int8_t seq_fm_selection_x;
int8_t seq_fm_selection_y;
int16_t seq_fm_divisions = 12;
int16_t seq_fm_base_frequency = 172;
int x = 62;
uint8_t checkbox_x = 41;
uint8_t slider_x = 9;

void seq_fm_gui_cursor()
{
  u8g2.setDrawColor(2);

  if (seq_fm_selection_y < 5)
  {
    if (seq_fm_selection_x == 0)
      u8g2.drawBox(slider_x - 1, 10 + 7 * seq_fm_selection_y, 31, 7);
    if (seq_fm_selection_x == 1)
      u8g2.drawBox(checkbox_x - 1, 10 + 7 * seq_fm_selection_y, 21, 7);
    if (seq_fm_selection_x == 2)
      u8g2.drawBox(slider_x - 1 + x, 10 + 7 * seq_fm_selection_y, 31, 7);
    if (seq_fm_selection_x == 3)
      u8g2.drawBox(checkbox_x - 1 + x, 10 + 7 * seq_fm_selection_y, 21, 7);
  }
  if (seq_fm_selection_y == 5)
  {
    if (seq_fm_selection_x == 0)
      u8g2.drawBox(slider_x - 1, 10 + 7 * seq_fm_selection_y, 31, 7);
    if (seq_fm_selection_x == 1)
      u8g2.drawBox(checkbox_x - 1, 10 + 7 * (seq_fm_selection_y - 1), 21, 7);
    if (seq_fm_selection_x == 2)
      u8g2.drawBox(slider_x - 1 + x, 10 + 7 * seq_fm_selection_y, 31, 7);
    if (seq_fm_selection_x == 3)
      u8g2.drawBox(checkbox_x - 1 + x, 10 + 7 * (seq_fm_selection_y - 1), 21, 7);
  }
  if (seq_fm_selection_y == 6)
  {
    if (seq_fm_selection_x == 0)
      u8g2.drawBox(slider_x - 7, 10 + 7 * seq_fm_selection_y + 3, 31, 7);
    if (seq_fm_selection_x == 1)
      u8g2.drawBox(checkbox_x - 7, 10 + 7 * seq_fm_selection_y + 3, 31, 7);
    if (seq_fm_selection_x == 2)
      u8g2.drawBox(checkbox_x - 7 + 32, 10 + 7 * seq_fm_selection_y + 3, 30, 7);
    if (seq_fm_selection_x == 3)
      u8g2.drawBox(checkbox_x - 7 + 63, 10 + 7 * seq_fm_selection_y + 3, 29, 7);
  }
  u8g2.setDrawColor(1);
}

void seq_fm_gui_doc()
{
  uint8_t h;
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawBox(0, 0, 128, 8);

  u8g2.setColorIndex(0);
  u8g2.drawStr(2, 6, "FM synth configuration");

  u8g2.setColorIndex(1);
  h = 9;
  u8g2.drawFrame(slider_x - 6, h + 1, 60, 44);
  u8g2.setColorIndex(0);
  u8g2.drawBox(slider_x - 6, h + 1, 59, 43);
  u8g2.setColorIndex(1);
  u8g2.drawFrame(x + slider_x - 6, h + 1, 60, 44);
  u8g2.setColorIndex(0);
  u8g2.drawBox(x + slider_x - 6, h + 1, 59, 43);
  u8g2.setColorIndex(1);

  SEQ_DRAW_SLIDER(slider_x, h, "KSL", (seq_system.synth->patch.original_instrument.carrier_ksl << 2));
  SEQ_DRAW_SLIDER(slider_x + x, h, "KSL", (seq_system.synth->patch.original_instrument.modulator_ksl << 2));
  SEQ_DRAW_CHECKBOX(checkbox_x, h, "AM", seq_system.synth->patch.original_instrument.carrier_am);
  SEQ_DRAW_CHECKBOX(checkbox_x + x, h, "AM", seq_system.synth->patch.original_instrument.modulator_am);
  h += 7;
  SEQ_DRAW_SLIDER(slider_x, h, "REL", seq_system.synth->patch.original_instrument.carrier_rel);
  SEQ_DRAW_SLIDER(slider_x + x, h, "REL", seq_system.synth->patch.original_instrument.modulator_rel);
  SEQ_DRAW_CHECKBOX(checkbox_x, h, "KSR", seq_system.synth->patch.original_instrument.carrier_ksr);
  SEQ_DRAW_CHECKBOX(checkbox_x + x, h, "KSR", seq_system.synth->patch.original_instrument.modulator_ksr);
  h += 7;
  SEQ_DRAW_SLIDER(slider_x, h, "MUL", seq_system.synth->patch.original_instrument.carrier_mul);
  SEQ_DRAW_SLIDER(slider_x + x, h, "MUL", seq_system.synth->patch.original_instrument.modulator_mul);
  SEQ_DRAW_CHECKBOX(checkbox_x, h, "DC", seq_system.synth->patch.original_instrument.dc);
  SEQ_DRAW_CHECKBOX(checkbox_x + x, h, "DC", seq_system.synth->patch.original_instrument.dm);
  h += 7;
  SEQ_DRAW_SLIDER(slider_x, h, "SUS", seq_system.synth->patch.original_instrument.carrier_sus);
  SEQ_DRAW_SLIDER(slider_x + x, h, "SUS", seq_system.synth->patch.original_instrument.modulator_sus);
  SEQ_DRAW_CHECKBOX(checkbox_x, h, "VIB", seq_system.synth->patch.original_instrument.carrier_vib);
  SEQ_DRAW_CHECKBOX(checkbox_x + x, h, "VIB", seq_system.synth->patch.original_instrument.modulator_vib);
  h += 7;
  SEQ_DRAW_SLIDER(slider_x, h, "ATT", seq_system.synth->patch.original_instrument.carrier_att);
  SEQ_DRAW_SLIDER(slider_x + x, h, "ATT", seq_system.synth->patch.original_instrument.modulator_att);
  SEQ_DRAW_CHECKBOX(checkbox_x, h, "EG", seq_system.synth->patch.original_instrument.carrier_eg);
  SEQ_DRAW_CHECKBOX(checkbox_x + x, h, "EG", seq_system.synth->patch.original_instrument.modulator_eg);
  h += 7;
  SEQ_DRAW_SLIDER(slider_x, h, "DEC", seq_system.synth->patch.original_instrument.carrier_dec);
  SEQ_DRAW_SLIDER(slider_x + x, h, "DEC", seq_system.synth->patch.original_instrument.modulator_dec);

  u8g2.setFontDirection(3);
  u8g2.drawStr(slider_x - 2, h + 7, "CARRIER");
  u8g2.drawStr(slider_x - 2 + x, h + 7, "MODULATOR");
  u8g2.setFontDirection(0);

  h += 10;
  SEQ_DRAW_SLIDER(slider_x - 6, h, "FB", (seq_system.synth->patch.original_instrument.feedback << 1));
  SEQ_DRAW_SLIDER(slider_x + x - 36, h, "LVL", (seq_system.synth->patch.original_instrument.level >> 2));
  u8g2.drawStr(slider_x + 64, h + 7, "Save");
  u8g2.drawStr(slider_x + 94, h + 7, "Load");
}

void seq_fm_gui_draw()
{
  seq_fm_gui_doc();
  seq_fm_gui_cursor();
}

void seq_fm_gui_control_change(int change)
{
  if (seq_fm_selection_x == 0 && seq_fm_selection_y == 0)
  {
    seq_system.synth->patch.original_instrument.carrier_ksl += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.carrier_ksl, 0, 3);
  }
  if (seq_fm_selection_x == 0 && seq_fm_selection_y == 1)
  {
    seq_system.synth->patch.original_instrument.carrier_rel += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.carrier_rel, 0, 15);
  }
  if (seq_fm_selection_x == 0 && seq_fm_selection_y == 2)
  {
    seq_system.synth->patch.original_instrument.carrier_mul += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.carrier_mul, 0, 15);
  }
  if (seq_fm_selection_x == 0 && seq_fm_selection_y == 3)
  {
    seq_system.synth->patch.original_instrument.carrier_sus += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.carrier_sus, 0, 15);
  }
  if (seq_fm_selection_x == 0 && seq_fm_selection_y == 4)
  {
    seq_system.synth->patch.original_instrument.carrier_att += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.carrier_att, 0, 15);
  }
  if (seq_fm_selection_x == 0 && seq_fm_selection_y == 5)
  {
    seq_system.synth->patch.original_instrument.carrier_dec += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.carrier_dec, 0, 15);
  }
  if (seq_fm_selection_x == 2 && seq_fm_selection_y == 0)
  {
    seq_system.synth->patch.original_instrument.modulator_ksl += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.modulator_ksl, 0, 3);
  }
  if (seq_fm_selection_x == 2 && seq_fm_selection_y == 1)
  {
    seq_system.synth->patch.original_instrument.modulator_rel += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.modulator_rel, 0, 15);
  }
  if (seq_fm_selection_x == 2 && seq_fm_selection_y == 2)
  {
    seq_system.synth->patch.original_instrument.modulator_mul += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.modulator_mul, 0, 15);
  }
  if (seq_fm_selection_x == 2 && seq_fm_selection_y == 3)
  {
    seq_system.synth->patch.original_instrument.modulator_sus += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.modulator_sus, 0, 15);
  }
  if (seq_fm_selection_x == 2 && seq_fm_selection_y == 4)
  {
    seq_system.synth->patch.original_instrument.modulator_att += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.modulator_att, 0, 15);
  }
  if (seq_fm_selection_x == 2 && seq_fm_selection_y == 5)
  {
    seq_system.synth->patch.original_instrument.modulator_dec += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.modulator_dec, 0, 15);
  }

  if (seq_fm_selection_x == 1 && seq_fm_selection_y == 0)
  {
    seq_system.synth->patch.original_instrument.carrier_am = !seq_system.synth->patch.original_instrument.carrier_am;
  }
  if (seq_fm_selection_x == 1 && seq_fm_selection_y == 1)
  {
    seq_system.synth->patch.original_instrument.carrier_ksr = !seq_system.synth->patch.original_instrument.carrier_ksr;
  }
  if (seq_fm_selection_x == 1 && seq_fm_selection_y == 2)
  {
    seq_system.synth->patch.original_instrument.dc = !seq_system.synth->patch.original_instrument.dc;
  }
  if (seq_fm_selection_x == 1 && seq_fm_selection_y == 3)
  {
    seq_system.synth->patch.original_instrument.carrier_vib = !seq_system.synth->patch.original_instrument.carrier_vib;
  }
  if (seq_fm_selection_x == 1 && (seq_fm_selection_y == 4 || seq_fm_selection_y == 5))
  {
    seq_system.synth->patch.original_instrument.carrier_eg = !seq_system.synth->patch.original_instrument.carrier_eg;
  }

  if (seq_fm_selection_x == 3 && seq_fm_selection_y == 0)
  {
    seq_system.synth->patch.original_instrument.modulator_am = !seq_system.synth->patch.original_instrument.modulator_am;
  }
  if (seq_fm_selection_x == 3 && seq_fm_selection_y == 1)
  {
    seq_system.synth->patch.original_instrument.modulator_ksr = !seq_system.synth->patch.original_instrument.modulator_ksr;
  }
  if (seq_fm_selection_x == 3 && seq_fm_selection_y == 2)
  {
    seq_system.synth->patch.original_instrument.dm = !seq_system.synth->patch.original_instrument.dm;
  }
  if (seq_fm_selection_x == 3 && seq_fm_selection_y == 3)
  {
    seq_system.synth->patch.original_instrument.modulator_vib = !seq_system.synth->patch.original_instrument.modulator_vib;
  }
  if (seq_fm_selection_x == 3 && (seq_fm_selection_y == 4 || seq_fm_selection_y == 5))
  {
    seq_system.synth->patch.original_instrument.modulator_eg = !seq_system.synth->patch.original_instrument.modulator_eg;
  }
  if (seq_fm_selection_x == 0 && seq_fm_selection_y == 6)
  {
    seq_system.synth->patch.original_instrument.feedback += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.feedback, 0, 7);
  }
  if (seq_fm_selection_x == 1 && seq_fm_selection_y == 6)
  {
    seq_system.synth->patch.original_instrument.level += change;
    SEQ_INT_BOUNDS(seq_system.synth->patch.original_instrument.level, 0, 63);
  }
  seq_ym2413_update_preset(&(seq_system.synth->patch.original_instrument));
}

void seq_fm_gui_control()
{
  t_seq_console *console;
  t_seq_synth_tuning *tuning;
  t_seq_fm_preset *preset;
  uint8_t input;

  seq_keyboard_set_application_mode(seq_system.keyboard);
  console = seq_system.console;
  tuning = seq_system.synth->tuning;
  seq_system.keyboard->midi_channel = 0;
  preset = &(seq_system.synth->patch.original_instrument);
  while (1)
  {
    while (!console->input_buffer->isEmpty())
    {
      console->input_buffer->pop(&input);
      if (input == 18)
      {
        seq_fm_selection_y--;
        seq_fm_selection_y = seq_fm_selection_y < 0 ? 0 : seq_fm_selection_y;
      }
      else if (input == 19)
      {
        seq_fm_selection_y++;
        seq_fm_selection_y = seq_fm_selection_y > 6 ? 6 : seq_fm_selection_y;
      }
      else if (input == 17)
      {
        seq_fm_selection_x--;
        seq_fm_selection_x = seq_fm_selection_x < 0 ? 0 : seq_fm_selection_x;
      }
      else if (input == 20)
      {
        seq_fm_selection_x++;
        seq_fm_selection_x = seq_fm_selection_x > 3 ? 3 : seq_fm_selection_x;
      }
      else if (input == 28)
      {
        seq_fm_gui_control_change(-1);
      }
      else if (input == ' ')
      {
        seq_fm_gui_control_change(1);
      }
      else if (input == 27)
      {
        seq_keyboard_set_control_mode(seq_system.keyboard);
        return;
      }
      else if (input == '\n')
      {
        if (seq_fm_selection_x == 3 && seq_fm_selection_y == 6)
        {
          char file_buffer[2048];
          uint8_t instruement_buffer[8];
          seq_file_gui_control("/instruments", file_buffer, 2048);
          File file = SD.open(file_buffer, "r");
          file.read(instruement_buffer, 8);
          seq_ym2413_load_instrument(instruement_buffer);
        }
      }
    }
    seq_st7920_draw(seq_fm_gui_draw);
    ulTaskNotifyTake(pdTRUE, 10000);
  }
}

BaseType_t seq_fm_command(char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string)
{
  seq_fm_gui_control();

  return (pdFALSE);

  int value;
  uint8_t option;
  char *parameter = strtok((char *)command_string, " ");
  snprintf(pcWriteBuffer, xWriteBufferLen, parameter);
  parameter = strtok(NULL, " ");
  option = parameter[0];
  parameter = strtok(NULL, " ");
  value = atoi(parameter);
  snprintf(pcWriteBuffer, xWriteBufferLen, "change %c to value %i", option, value);
  if (option == 'c')
  {
    seq_system.keyboard->midi_channel = value % 16;
    snprintf(pcWriteBuffer, xWriteBufferLen, "Updated keyboard channel to %i", value % 16);
  }
  else if (option == 't')
  {
    seq_system.keyboard->traspose = value % 128;
    snprintf(pcWriteBuffer, xWriteBufferLen, "Updated keyboard traspose to %i", value % 128);
  }
  else if (option == 'v')
  {
    seq_system.keyboard->velocity = value % 128;
    snprintf(pcWriteBuffer, xWriteBufferLen, "Updated keyboard velocity to %i", value % 128);
  }
  else
  {
    snprintf(pcWriteBuffer, xWriteBufferLen, "Option '%c' not recognized", option);
  }
  // snprintf( pcWriteBuffer, xWriteBufferLen, "keyboard configuration tool");
  return pdFALSE;
}
