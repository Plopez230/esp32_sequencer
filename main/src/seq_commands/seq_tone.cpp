#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../seq.h"

int8_t seq_tone_selection;
int16_t seq_tone_divisions = 12;
int16_t seq_tone_base_frequency = 172;

void seq_tone_gui_values()
{
  char buf[4];

  snprintf(buf, 4, "%u", seq_tone_divisions);
  u8g2.drawStr(80, 19, buf);
  snprintf(buf, 4, "%u", seq_tone_base_frequency);
  u8g2.drawStr(80, 31, buf);
}

void seq_tone_gui_cursor()
{
  u8g2.drawFrame(43, 12 * (seq_tone_selection + 1) - 1, 82, 12);
}

void seq_tone_gui_doc()
{
  uint8_t h;
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawBox(0, 0, 128, 8);

  u8g2.setColorIndex(0);
  u8g2.drawStr(2, 6, "Tuning configuration");

  u8g2.setColorIndex(1);
  h = 12;
  u8g2.drawStr(4, h + 7, "Divisions");
  u8g2.drawStr(47, h + 7, "<");
  u8g2.drawStr(118, h + 7, ">");

  u8g2.drawFrame(44, h, 80, 10);
  u8g2.drawFrame(44, h, 10, 10);
  u8g2.drawFrame(114, h, 10, 10);
  h += 12;
  u8g2.drawStr(4, h + 7, "Base Freq");
  u8g2.drawStr(47, h + 7, "<");
  u8g2.drawStr(118, h + 7, ">");
  u8g2.drawFrame(44, h, 80, 10);
  u8g2.drawFrame(44, h, 10, 10);
  u8g2.drawFrame(114, h, 10, 10);
}

void seq_tone_gui_draw()
{
  seq_tone_gui_doc();
  seq_tone_gui_values();
  seq_tone_gui_cursor();
}

void seq_tone_gui_control_change(int change)
{
  if (seq_tone_selection == 0)
  {
    seq_tone_divisions += change;
    if (seq_tone_divisions > 32) seq_tone_divisions = 32;
    if (seq_tone_divisions < 1) seq_tone_divisions = 1;
  }
  else if (seq_tone_selection == 1)
  {
    seq_tone_base_frequency += change;
    if (seq_tone_base_frequency > 260) seq_tone_base_frequency = 260;
    if (seq_tone_base_frequency < 100) seq_tone_base_frequency = 100;
  }
}

void seq_tone_gui_control()
{
  t_seq_console *console;
  t_seq_synth_tuning *tuning;
  uint8_t input;

  seq_keyboard_set_application_mode(seq_system.keyboard);
  console = seq_system.console;
  tuning = seq_system.synth->tuning;
  while (1)
  {
    while (!console->input_buffer->isEmpty())
    {
      console->input_buffer->pop(&input);
      if (input == 18)
      {
        seq_tone_selection --;
        seq_tone_selection = seq_tone_selection < 0 ? 0 : seq_tone_selection;
      }
      else if (input == 19)
      {
        seq_tone_selection ++;
        seq_tone_selection = seq_tone_selection > 1 ? 1 : seq_tone_selection;
      }
      else if (input == 17)
      {
        seq_tone_gui_control_change(-1);
        seq_tuning_equal_tempered(tuning, seq_tone_divisions, seq_tone_base_frequency);
      }
      else if (input == 20)
      {
        seq_tone_gui_control_change(1);
        seq_tuning_equal_tempered(tuning, seq_tone_divisions, seq_tone_base_frequency);
      }
      else if (input == 27)
      {
        seq_keyboard_set_control_mode(seq_system.keyboard);
        return;
      }
    }
    seq_st7920_draw(seq_tone_gui_draw);
    ulTaskNotifyTake(pdTRUE, 10000);
  }
}

BaseType_t seq_tone_command( char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string )
{
  seq_tone_gui_control();

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
  //snprintf( pcWriteBuffer, xWriteBufferLen, "keyboard configuration tool");
  return pdFALSE;
}
