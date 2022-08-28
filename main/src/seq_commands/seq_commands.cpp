#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../seq.h"

int8_t selection;

BaseType_t seq_mem_command( char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string )
{
  snprintf(pcWriteBuffer, xWriteBufferLen, "Free heap: %d bytes", ESP.getFreeHeap());
  return pdFALSE;
}


void seq_register_commands()
{
  FreeRTOS_CLIRegisterCommand(&seq_kb_command_def);
  FreeRTOS_CLIRegisterCommand(&seq_mem_command_def);
  FreeRTOS_CLIRegisterCommand(&seq_tone_command_def);
}
