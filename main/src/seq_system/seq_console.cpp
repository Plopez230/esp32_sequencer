#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../seq.h"
#define PRINT_BUFFER_LENGTH 25
#define CONSOLE_WIDTH 31
uint32_t str_length_up_to_newline(char *str)
{
  uint32_t r;

  r = 0;
  if (!str) return (0);
  while (str[r] && str[r] != '\n')
    r++;
  return (r);
}

char *previous_line_start(char *str, char *pos)
{
  uint32_t    str_length;
  char        *last_char_pos;

  str_length = str_length_up_to_newline(str);
  last_char_pos = str + str_length - 1;
  last_char_pos = pos;
  while (last_char_pos > str)
  {
    if (*last_char_pos == '\n')
    {
      last_char_pos++;
      break;
    }
    last_char_pos--;
  }
  return (last_char_pos);
}


char *get_full_line(char *str, char *pos, char **last_line_start, uint32_t *last_line_length)
{
  *last_line_start = previous_line_start(str, pos);
  *last_line_start += ((pos - *last_line_start) / CONSOLE_WIDTH) * CONSOLE_WIDTH;
  *last_line_length = str_length_up_to_newline(*last_line_start);
  if (*last_line_length > CONSOLE_WIDTH)
    *last_line_length = CONSOLE_WIDTH;
  if (*last_line_start == str)
    return (0);
  if (*(*last_line_start - 1) == '\n')
    return (*last_line_start - 2);
  return (*last_line_start - 1);
}

char *print_last_lines(char *test, char *temp, uint32_t n_lines, uint32_t offset)
{
  char *line_start;
  uint32_t line_length;
  char *previous;
  uint8_t h;
  uint8_t w;
  uint8_t h_base;
  char     char_buffer[2];

  previous = 0;
  char_buffer[1] = '\0';
  h_base = 12;
  h = 6;
  w = 0;
  n_lines += offset;
  if (offset) 
  {
    n_lines--;
    h--;
    h_base -= 2;
    u8g2.drawLine(0, 47, 123, 47);
    temp = get_full_line(test, temp, &line_start, &line_length);
    if (!previous) previous = temp;
      while (line_length--)
      {
        char_buffer[0] = *(line_start++);
        u8g2.drawStr(0 + 4 * w, 12 + 7 * 6, char_buffer);
        w++;
      }
      w = 0;
  }
  while (temp && n_lines--)
  {
    
    temp = get_full_line(test, temp, &line_start, &line_length);
    if (!previous) previous = temp;
    if (!offset) {
      while (line_length--)
      {
        char_buffer[0] = *(line_start++);
        u8g2.drawStr(0 + 4 * w, h_base + 7 * h, char_buffer);
        w++;
      }
      w = 0;
      h--;
    }
    else offset--;
  }
  return (previous);
}

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
  char *test = console->print_buffer;
  char *temp;
  uint8_t n = 7;
  temp = test + strlen(test) - 1;
  temp = print_last_lines(test, temp, 7, console->print_line_cursor);
}

void seq_draw_status_bar()
{
  uint16_t h;

  h = 62;
  u8g2.setColorIndex(1);
  u8g2.drawBox(0, 0, 128, 8);
  u8g2.setColorIndex(0);
  u8g2.drawBox(0, 8, 128, 1);
  u8g2.setColorIndex(1);
  u8g2.drawFrame(124, 9, 4, 46);
  u8g2.drawBox(0, 56, 128, 8);
  u8g2.setColorIndex(0);
  u8g2.drawStr(2, 6, "/");
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
}

void seq_console_draw()
{
  seq_draw_command(seq_system.console);
  seq_draw_status_bar();
}

void seq_console_task(void *parameter)
{
  t_seq_console *console;
  uint8_t input;
  uint8_t len;
  uint8_t print_len;
  uint8_t showing_output;

  console = (t_seq_console *)parameter;
  while (1)
  {
    while (!console->input_buffer->isEmpty())
    {
      console->input_buffer->pop(&input);
      len = strlen(console->command_buffer);
      print_len = strlen(console->print_buffer);
      if (input == 27)
      {
        console->command_buffer[0] = '\0';
        console->print_buffer[0] = '\0';
      }
      else if (input == 18)
      {
        //arriba
        console->print_line_cursor++;
      }
      else if (input == 19)
      {
        //abajo
        if (console->print_line_cursor) console->print_line_cursor--;
      }
      else if (input == '\n')
      {
        uint8_t more_output = pdTRUE;
        // Enviamos el comando
        while (more_output == pdTRUE)
        {
          more_output = FreeRTOS_CLIProcessCommand
                        (
                          ( const char * )console->command_buffer,   /* The command string.*/
                          console->return_buffer,  /* The output buffer. */
                          SEQ_CONFIG_CONSOLE_RETURN_BUFFER_SIZE/* The size of the output buffer. */
                        );
          console->command_buffer[0] = '\0';
          strlcat(console->print_buffer, " \n", SEQ_CONFIG_CONSOLE_PRINT_BUFFER_MAX_SIZE);
          strlcat(console->print_buffer, console->return_buffer, SEQ_CONFIG_CONSOLE_PRINT_BUFFER_MAX_SIZE);
        }
        strlcat(console->print_buffer, " \n> ", SEQ_CONFIG_CONSOLE_PRINT_BUFFER_MAX_SIZE);
      }
      else if (input == 28)
      {
        console->command_buffer[len - 1] = '\0';
        console->print_buffer[len - 1] = '\0';
      }
      else if (len < SEQ_CONFIG_CONSOLE_COMMAND_BUFFER_SIZE)
      {
        console->command_buffer[len] = input;
        console->command_buffer[len + 1] = '\0';
        console->print_buffer[print_len] = input;
        console->print_buffer[print_len + 1] = '\0';
      }
    }
    seq_st7920_draw(seq_console_draw);
    seq_draw_command(console);
    //vTaskDelay(0);
    ulTaskNotifyTake(pdTRUE, 25);
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
  console->print_buffer[0] = '\0';
  console->print_line_cursor = 0;
  strlcat(console->print_buffer, " \n> ", SEQ_CONFIG_CONSOLE_PRINT_BUFFER_MAX_SIZE);
  seq_console_init_task(console);
}

