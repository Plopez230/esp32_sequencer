#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seq_include.h"

t_seq_console *seq_console_create()
{
  t_seq_console *new_console;

  new_console = (t_seq_console *) malloc(sizeof(t_seq_console));
  if (!new_console)
    return (NULL);
  new_console->command_buffer = (char *)malloc(SEQ_CONFIG_CONSOLE_COMMAND_BUFFER_SIZE + 1);
  if (!new_console->command_buffer)
  {
    free(new_console);
    return (NULL);
  }
  new_console->return_buffer = (char *)malloc(SEQ_CONFIG_CONSOLE_RETURN_BUFFER_SIZE + 1);
  if (!new_console->return_buffer)
  {
    free(new_console->command_buffer);
    free(new_console);
    return (NULL);
  }
  new_console->input_buffer = new cppQueue(1, SEQ_CONFIG_CONSOLE_INPUT_BUFFER_SIZE, FIFO, true);
  if (!new_console->input_buffer)
  {
    free(new_console->return_buffer);
    free(new_console->command_buffer);
    free(new_console);
    return (NULL);
  }
  return (new_console);
}

void seq_draw_command(t_seq_console *console)
{
  uint16_t command_length;
  uint16_t char_idx;
  uint16_t h;
  uint16_t w;
  char     char_buffer[2];

  command_length = strlen(console->command_buffer);
  char_idx = 0;
  char_buffer[1] = '\0';
  h = 0;
  w = 2;
  while (char_idx < command_length)
  {
    char_buffer[0] = console->command_buffer[char_idx];
    u8g2.drawStr(2 + 5 * w, 19 + 8 * h, char_buffer);
    char_idx++;
    w++;
    if (w >= 25)
    {
      w = 0;
      h ++;
    }
  }
  char_buffer[0] = '_';
  if ((millis() / 700)%2)
    u8g2.drawStr(2 + 5 * w, 19 + 8 * h, char_buffer);
}

void seq_draw_status_bar()
{
  uint16_t h;

  h = 62;
  u8g2.setFont(u8g2_font_5x7_tf );
  u8g2.setColorIndex(1);
  u8g2.drawBox(0, 0, 128, 10);
  u8g2.drawBox(0, 54, 128, 10);
  u8g2.setColorIndex(0);
  u8g2.drawStr(2, 8, "/");
  if (seq_system.keyboard->operation_mode == 0)
    u8g2.drawStr(2, h, "ins");
  else if (seq_system.keyboard->num_mode && seq_system.keyboard->mayus_mode)
    u8g2.drawStr(2, h, "+-*");
  else if (seq_system.keyboard->num_mode && !seq_system.keyboard->mayus_mode)
    u8g2.drawStr(2, h, "123");
  else if (!seq_system.keyboard->num_mode && seq_system.keyboard->mayus_mode)
    u8g2.drawStr(2, h, "ABC");
  else if (!seq_system.keyboard->num_mode && !seq_system.keyboard->mayus_mode)
    u8g2.drawStr(2, h, "abc");
  u8g2.setColorIndex(1);
  for (int n = 0; n < 1; n++)
  {
    u8g2.drawStr(2, 19 + 8 * n, "> ");
  }
}

void seq_console_draw()
{
  seq_draw_status_bar();
  seq_draw_command(seq_system.console);
}

void seq_console_task(void *parameter)
{
  t_seq_console *console;
  uint8_t input;
  uint8_t len;
  uint8_t showing_output;

  console = (t_seq_console *)parameter;
  while (1)
  {
        while (!console->input_buffer->isEmpty())
        {
          if (showing_output)
          {
            console->command_buffer[0] = '\0';
            showing_output = !showing_output;
          }
          console->input_buffer->pop(&input);
          len = strlen(console->command_buffer);
          if (input == 27)
          {
            console->command_buffer[0] = '\0';
          }
          else if (input == '\n')
          {
            // Enviamos el comando
            FreeRTOS_CLIProcessCommand
                              (
                                  ( const char * )console->command_buffer,   /* The command string.*/
                                  console->return_buffer,  /* The output buffer. */
                                  SEQ_CONFIG_CONSOLE_RETURN_BUFFER_SIZE/* The size of the output buffer. */
                              );
            strlcpy(console->command_buffer, console->return_buffer, SEQ_CONFIG_CONSOLE_COMMAND_BUFFER_SIZE);
            showing_output = 1;
          }
          else if (input == 28)
          {
            console->command_buffer[len-1] = '\0';
          }
          else if (len < SEQ_CONFIG_CONSOLE_COMMAND_BUFFER_SIZE)
          {
            Serial.println(input);
            console->command_buffer[len] = input;
            console->command_buffer[len + 1] = '\0';
            
            Serial.println(console->command_buffer);
          }
        }
    seq_st7920_draw(seq_console_draw);
    seq_draw_command(console);
    vTaskDelay(0);
  }
}

void seq_console_init_task(t_seq_console *console)
{
  xTaskCreatePinnedToCore(
    seq_console_task, /* Function to implement the task */
    "console task", /* Name of the task */
    10000,  /* Stack size in words */
    console,  /* Task input parameter */
    0,  /* Priority of the task */
    & (console->task_handler), /* Task handle. */
    0); /* Core where the task should run */
}

void seq_console_init(t_seq_console *console)
{
  seq_register_commands();
  console->command_buffer[0] = '\0';
  seq_console_init_task(console);
}
