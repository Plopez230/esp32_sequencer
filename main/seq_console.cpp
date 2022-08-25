#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seq_include.h"
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

char *print_last_lines(char *test, char *temp)
{
  char *line_start;
  uint32_t line_length;
  char *previous;
  uint8_t n_lines;
  uint8_t h;
  uint8_t w;
  char     char_buffer[2];

  previous = 0;
  n_lines = 7;
  char_buffer[1] = '\0';
  h = 6;
  w = 0;
  while (temp && n_lines--)
  {
    temp = get_full_line(test, temp, &line_start, &line_length);
    if (!previous) previous = temp;

    while (line_length--)
    {
      char_buffer[0] = *(line_start++);
      u8g2.drawStr(0 + 4 * w, 12 + 7 * h, char_buffer);
      w++;
    }
    w = 0;
    h--;
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
  char *test = console->command_buffer;
  char *temp;
  uint8_t n = 7;
  temp = test+strlen(test)-1;
  temp = print_last_lines(test, temp);
  /*uint16_t command_length;
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
    u8g2.drawStr(0 + 4 * w, 14 + 7 * h, char_buffer);
    char_idx++;
    w++;
    if (w >= 31)
    {
      w = 0;
      h ++;
    }
  }
  char_buffer[0] = '_';
  if ((millis() / 700) % 2)
    u8g2.drawStr(0 + 4 * w, 14 + 7 * h, char_buffer);*/
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
        console->command_buffer[len - 1] = '\0';
      }
      else if (len < SEQ_CONFIG_CONSOLE_COMMAND_BUFFER_SIZE)
      {
        console->command_buffer[len] = input;
        console->command_buffer[len + 1] = '\0';
      }
    }
    seq_st7920_draw(seq_console_draw);
    seq_draw_command(console);
    //vTaskDelay(0);
    ulTaskNotifyTake(pdTRUE, 25);
  }
}

/*char *seq_console_last_line_length(t_seq_console *console, char *last_char)
  {
  char *current_char;
  char *first_char;

  if (last_char < console->print_buffer || last_char >= console->print_buffer + SEQ_CONFIG_CONSOLE_PRINT_BUFFER_MAX_SIZE)
    return (NULL);
  current_char = last_char;
  while (current_char != console->print_buffer_end)
  {
    first_char = current_char;
    current_char--;
    if (current_char < console->print_buffer)
      current_char += SEQ_CONFIG_CONSOLE_PRINT_BUFFER_MAX_SIZE;
    if (*current_char == '\n')
      return (last_char - first_char);
  }
  return (0);
  }*/

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
  console->print_buffer_end = console->print_buffer;
  seq_console_init_task(console);
}


void seq_console_print(char *str);
void seq_console_println(char *str);
void seq_console_input(char *str);
