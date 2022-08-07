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
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "seq_include.h"

Adafruit_MPR121 seq_mpr121_a;
Adafruit_MPR121 seq_mpr121_c;
Adafruit_MPR121 seq_mpr121_d;

void seq_keyboard_init()
{
  Wire.begin(I2C_SDA, I2C_SCL);
  digitalWrite(I2C_SDA, 0);
  digitalWrite(I2C_SCL, 0);
  seq_mpr121_a.begin(0x5A);
  seq_mpr121_c.begin(0x5C);
  seq_mpr121_d.begin(0x5D);
}

{
  currtouched = seq_mpr121_a.touched();
  for (uint8_t i = 0; i < 12; i++) {
    //it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      event.init_note_on_event(instrument, note + i, 0);
      if (output_queue) output_queue->push(&event);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      event.init_note_off_event(instrument, note + i, 0);
      if (output_queue) output_queue->push(&event);
    }
  }
  // reset our state
  lasttouched = currtouched;

  currtouched = seq_mpr121_c.touched();
  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      event.init_note_on_event(instrument, note - 12 + i, 0);
      if (output_queue) output_queue->push(&event);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      event.init_note_off_event(instrument, note - 12 + i, 0);
      if (output_queue) output_queue->push(&event);
    }
  }
  // reset our state
  lasttouched = currtouched;

  currtouched = seq_mpr121_d.touched();
  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      event.init_note_on_event(instrument, note + 12 + i, 0);
      if (i==11) event.init_event(STOP, 0, 0);
      if (output_queue) output_queue->push(&event);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      event.init_note_off_event(instrument, note + 12 + i, 0);
      if (output_queue) output_queue->push(&event);
    }
  }
  // reset our state
  lasttouched = currtouched;
}
