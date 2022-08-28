/*************************************************************************************
  Copyright (c) 2022, plopez230@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
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

#ifndef SEQ_CONFIGURATION_H
# define SEQ_CONFIGURATION_H

# define SEQ_CONFIG_MAX_TRACKS 12
# define SEQ_CONFIG_MAX_BUFFER_ELEMENTS 500
# define SEQ_CONFIG_MAX_MESSAGE_LENGTH 3
# define SEQ_CONFIG_PRECISION_DIVIDER 1
# define SEQ_CONFIG_PRECISION_BASE 960
# define SEQ_CONFIG_TRACK_BUFFER_SIZE 256
# define SEQ_CONFIG_SYNTH_BUFFER_SIZE 256
# define SEQ_CONFIG_CONSOLE_INPUT_BUFFER_SIZE 2560
# define SEQ_CONFIG_CONSOLE_COMMAND_BUFFER_SIZE 2560
# define SEQ_CONFIG_CONSOLE_PRINT_BUFFER_MAX_SIZE 8192
# define SEQ_CONFIG_CONSOLE_RETURN_BUFFER_SIZE 2560
# define SEQ_CONFIG_CONSOLE_MAX_LINES 256
# define SEQ_CONFIG_MAX_FILE_NAME_SIZE 256

# define SEQ_CONFIG_DEBOUNCE_DELAY 50

#endif
