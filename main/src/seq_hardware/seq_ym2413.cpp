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

#include "../seq.h"

void seq_ym2413_shift (uint8_t value)
{
  digitalWrite(SEQ_YM2413_LTC_PIN, LOW);
  for (int x = 0; x < 8; x++) {
    digitalWrite(SEQ_YM2413_DATA_PIN, LOW);
    if (bitRead(value, x)) {
      digitalWrite(SEQ_YM2413_DATA_PIN, HIGH);
    }
    digitalWrite(SEQ_YM2413_CLK_PIN, HIGH);
    digitalWrite(SEQ_YM2413_DATA_PIN, LOW);
    digitalWrite(SEQ_YM2413_CLK_PIN, LOW);
  }
  digitalWrite(SEQ_YM2413_LTC_PIN, HIGH);
}

void seq_ym2413_setup_pins()
{
  pinMode(SEQ_YM2413_DATA_PIN, OUTPUT);
  pinMode(SEQ_YM2413_CLK_PIN, OUTPUT);
  pinMode(SEQ_YM2413_LTC_PIN, OUTPUT);
  pinMode(SEQ_YM2413_CS_PIN, OUTPUT);
  pinMode(SEQ_YM2413_AO_PIN, OUTPUT);
  pinMode(SEQ_YM2413_IC_PIN, OUTPUT);
  pinMode(SEQ_YM2413_WE_PIN, OUTPUT);
}

void seq_ym2413_select_chip() {
  digitalWrite(SEQ_YM2413_CS_PIN, HIGH);
  digitalWrite(SEQ_YM2413_WE_PIN, LOW);
}

void seq_ym2413_reset_chip() {
  digitalWrite(SEQ_YM2413_IC_PIN, LOW);
  delay(100);
  digitalWrite(SEQ_YM2413_IC_PIN, HIGH);
  delay(500);
}

void seq_ym2413_setup() {
  seq_ym2413_setup_pins();
  seq_ym2413_select_chip();
  seq_ym2413_reset_chip();
}

void seq_ym2413_write(byte address, byte data) {
  // write register address
  digitalWrite(SEQ_YM2413_CS_PIN, HIGH);
  digitalWrite(SEQ_YM2413_AO_PIN, LOW);
  seq_ym2413_shift(address);
  digitalWrite(SEQ_YM2413_CS_PIN, LOW);
  // wait for 12 master clock cycles (at 3.5Mhz that is 4 microseconds, rounded up)
  delayMicroseconds(4);
  digitalWrite(SEQ_YM2413_CS_PIN, HIGH);
  digitalWrite(SEQ_YM2413_AO_PIN, HIGH);
  // write register data
  seq_ym2413_shift(data);
  digitalWrite(SEQ_YM2413_CS_PIN, LOW);
  // wait for 84 master clock cycles (at 3.5Mhz that is 25 microseconds, rounded up)
  delayMicroseconds(25);
  digitalWrite(SEQ_YM2413_CS_PIN, HIGH);
}

void seq_ym2413_play_note(s_seq_synth_tuning *tuning, int channel, int note, int instrument, int vol) {
  // f-number that go with those notes
  int f_number = tuning->f_numbers[note % tuning->divisions_per_octave];
  int octave = (note / tuning->divisions_per_octave) + 2;

  // 0E D0..D4 = Rhythm instrument on / off
  // D5 -> 1= Rhythm sound mode, 0= melody sound mode
  // Drums off, all channels are melody
  seq_ym2413_write(0x0E , 0);
  // 10 = F-Number LSB 8 bits
  seq_ym2413_write((0x10 + channel), lowByte(f_number));
  // NoteON & Oct & Note
  // 20 -> D0 = MSB fnumber
  //       D1..D3 = octave 0 to 7
  //       D4 = key On/Off
  byte yB20 = 0;
  bitWrite(yB20, 0, bitRead(f_number, 8)); // MSB of F-Number
  bitWrite(yB20, 1, bitRead(octave, 0) ); // octave bit 0
  bitWrite(yB20, 2, bitRead(octave, 1) ); // octave bit 1
  bitWrite(yB20, 3, bitRead(octave, 2) ); // octave bit 2
  bitWrite(yB20, 4, 1); // key on!
  seq_ym2413_write((0x20 + channel), yB20);
  // 30 -> D0..D3 = vol (0 to 15)
  //       D4..D7 = instrument (0 to 15)
  byte yB30 = 0;
  yB30 = instrument << 4 | vol ;
  seq_ym2413_write((0x30 + channel), yB30);
}

void seq_ym2413_stop_note(int channel, boolean sustain) {
  byte yB20 = 0;
  bitWrite(yB20, 5, sustain); // sustain on or off
  seq_ym2413_write((0x20 + channel), yB20);
}
