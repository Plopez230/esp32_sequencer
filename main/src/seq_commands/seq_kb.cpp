#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../seq.h"

int8_t seq_kb_selection;

void seq_kb_gui_values()
{
  char buf[4];

  snprintf(buf, 4, "%u", seq_system.keyboard->midi_channel);
  u8g2.drawStr(80, 19, buf);
  snprintf(buf, 4, "%u", seq_system.keyboard->velocity);
  u8g2.drawStr(80, 31, buf);
  snprintf(buf, 4, "%u", seq_system.keyboard->traspose);
  u8g2.drawStr(80, 43, buf);
}

void seq_kb_gui_cursor()
{
  u8g2.drawFrame(43, 12 * (seq_kb_selection + 1) - 1, 82, 12);
}

void seq_kb_gui_doc()
{
  uint8_t h;
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawBox(0, 0, 128, 8);

  u8g2.setColorIndex(0);
  u8g2.drawStr(2, 6, "Keyboard configuration");

  u8g2.setColorIndex(1);
  h = 12;
  u8g2.drawStr(4, h + 7, "Channel");
  u8g2.drawStr(47, h + 7, "<");
  u8g2.drawStr(118, h + 7, ">");

  u8g2.drawFrame(44, h, 80, 10);
  u8g2.drawFrame(44, h, 10, 10);
  u8g2.drawFrame(114, h, 10, 10);
  h += 12;
  u8g2.drawStr(4, h + 7, "Velocity");
  u8g2.drawStr(47, h + 7, "<");
  u8g2.drawStr(118, h + 7, ">");
  u8g2.drawFrame(44, h, 80, 10);
  u8g2.drawFrame(44, h, 10, 10);
  u8g2.drawFrame(114, h, 10, 10);
  h += 12;
  u8g2.drawStr(4, h + 7, "Traspose");
  u8g2.drawStr(47, h + 7, "<");
  u8g2.drawStr(118, h + 7, ">");
  u8g2.drawFrame(44, h, 80, 10);
  u8g2.drawFrame(44, h, 10, 10);
  u8g2.drawFrame(114, h, 10, 10);
}

void seq_kb_gui_draw()
{
  seq_kb_gui_doc();
  seq_kb_gui_values();
  seq_kb_gui_cursor();
}

void seq_kb_gui_control_change(int change)
{
  if (seq_kb_selection == 0)
  {
    seq_system.keyboard->midi_channel += change;
    seq_system.keyboard->midi_channel = seq_system.keyboard->midi_channel % 16;
  }
  else if (seq_kb_selection == 1)
  {
    seq_system.keyboard->velocity += change * 8;
    seq_system.keyboard->velocity = seq_system.keyboard->velocity % 128;
  }
  else if (seq_kb_selection == 2)
  {
    seq_system.keyboard->traspose += change;
    seq_system.keyboard->traspose = seq_system.keyboard->traspose % 128;
  }
}

void seq_kb_gui_control()
{
  t_seq_console *console;
  uint8_t input;

  seq_keyboard_set_application_mode(seq_system.keyboard);
  console = seq_system.console;
  while (1)
  {
    while (!console->input_buffer->isEmpty())
    {
      console->input_buffer->pop(&input);
      if (input == 18)
      {
        seq_kb_selection --;
        seq_kb_selection = seq_kb_selection < 0 ? 0 : seq_kb_selection;
      }
      else if (input == 19)
      {
        seq_kb_selection ++;
        seq_kb_selection = seq_kb_selection > 2 ? 2 : seq_kb_selection;
      }
      else if (input == 17)
      {
        seq_kb_gui_control_change(-1);
      }
      else if (input == 20)
      {
        seq_kb_gui_control_change(1);
      }
      else if (input == 27)
      {
        seq_keyboard_set_control_mode(seq_system.keyboard);
        return;
      }
    }
    seq_st7920_draw(seq_kb_gui_draw);
    ulTaskNotifyTake(pdTRUE, 10000);
  }
}

BaseType_t seq_kb_command( char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string )
{
  seq_kb_gui_control();

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
