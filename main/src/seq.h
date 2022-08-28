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

#ifndef SEQ_INCLUDE_H
# define SEQ_INCLUDE_H

# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <cppQueue.h>
# include <FreeRTOS_CLI_vESP32.h>
# include "driver/timer.h"
# include "freertos/task.h"
# include "freertos/timers.h"
#include "freertos/FreeRTOS.h"

# include "seq_configuration.h"
# include "seq_system/seq_system.h"
# include "seq_hardware/seq_hardware.h"
# include "seq_commands/seq_commands.h"


/**
   System global variable.
*/
typedef struct        s_seq_system
{
  t_seq_keyboard      *keyboard;
  t_seq_synth         *synth;
  t_seq_sequencer     *sequencer;
  t_seq_console       *console;
}                     t_seq_system;

extern t_seq_system seq_system;


#endif