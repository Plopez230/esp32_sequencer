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

#include <Arduino.h>
#include <U8g2lib.h>
//#include <SPI.h>
#include "seq_peripheral.h"

/**
 * Define and create screen handler as global variable
 */
SEQ_U8G2_CLASS u8g2(
  U8G2_R0, 
  SEQ_ST7920_CLK_PIN, 
  SEQ_ST7920_DATA_PIN, 
  SEQ_ST7920_CS_PIN, 
  SEQ_ST7920_RESET_PIN
  );

/**
 * Set default print style
 */
void seq_st7920_prepare(void) {
  u8g2.setFont(u8g2_font_5x7_tf );
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

/**
 * Initialize screen
 */
void seq_st7920_init(void)
{
  u8g2.begin();
}

/**
 * Refresh screen and draw
 * draw_callback calls the printing functions.
 */
//void seq_st7920_draw(void (*draw_callback)(SEQ_U8G2_CLASS))
//{
//  u8g2.clearBuffer();
//  draw_callback(u8g2);
//  u8g2.sendBuffer();
//}
