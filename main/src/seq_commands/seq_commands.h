
#include "../seq.h"

void seq_register_commands();

// Commands

BaseType_t seq_mem_command( char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string );

static const CLI_Command_Definition_t seq_mem_command_def =
{
  "mem",
  "mem",
  seq_mem_command,
  0
};

void seq_kb_gui_values();
void seq_kb_gui_cursor();
void seq_kb_gui_doc();
void seq_kb_gui_draw();
void seq_kb_gui_control_change(int change);
void seq_kb_gui_control();
BaseType_t seq_kb_command( char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string );

static const CLI_Command_Definition_t seq_kb_command_def =
{
  "kb",
  "kb <option> <value>",
  seq_kb_command,
  -1
};

BaseType_t seq_tone_command( char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string );
static const CLI_Command_Definition_t seq_tone_command_def =
{
  "tone",
  "tone <option> <value>",
  seq_tone_command,
  -1
};