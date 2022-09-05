
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

BaseType_t seq_fm_command( char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string );
static const CLI_Command_Definition_t seq_fm_command_def =
{
  "fm",
  "fm <option> <value>",
  seq_fm_command,
  -1
};

BaseType_t seq_file_command(char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string);
static const CLI_Command_Definition_t seq_file_command_def =
{
  "file",
  "file <option> <value>",
  seq_file_command,
  -1
};