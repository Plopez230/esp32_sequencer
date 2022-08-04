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

#ifndef SEQ_PERIPHERAL_H
# define SEQ_PERIPHERAL_H

/**
 * SD Card pins
 */
# define SEQ_SD_MOSI_PIN       15
# define SEQ_SD_MISO_PIN       19
# define SEQ_SD_SCK_PIN        14
# define SEQ_SD_CS_PIN         13

/**
 * ST7920 Screen pins
 */
# define SEQ_ST7920_CLK_PIN    18
# define SEQ_ST7920_DATA_PIN   23
# define SEQ_ST7920_CS_PIN     5
# define SEQ_ST7920_RESET_PIN  -1    // Reset pin is not connected

/**
 * MPR121 Keyboard pins
 */
# define SEQ_MPR121_SDA_PIN    21
# define SEQ_MPR121_SCL_PIN    22

/**
 * YM2413 Synthesizer pins
 */
# define SEQ_YM2413_DATA_PIN   27
# define SEQ_YM2413_CLK_PIN    0
# define SEQ_YM2413_LTC_PIN    4
# define SEQ_YM2413_CS_PIN     32
# define SEQ_YM2413_AO_PIN     26
# define SEQ_YM2413_IC_PIN     33
# define SEQ_YM2413_WE_PIN     25

/**
 * U8G2 Handler class
 */
# define SEQ_U8G2_CLASS U8G2_ST7920_128X64_F_SW_SPI

/**
 * seq_st7920.cpp prototypes
 */
void seq_st7920_prepare(void);
void seq_st7920_init(void);
void seq_st7920_draw(void (*draw_callback)(SEQ_U8G2_CLASS));

#endif
