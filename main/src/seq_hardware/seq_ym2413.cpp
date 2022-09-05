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

void seq_ym2413_shift(uint8_t value)
{
  digitalWrite(SEQ_YM2413_LTC_PIN, LOW);
  for (int x = 0; x < 8; x++)
  {
    digitalWrite(SEQ_YM2413_DATA_PIN, LOW);
    if (bitRead(value, x))
    {
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

void seq_ym2413_select_chip()
{
  digitalWrite(SEQ_YM2413_CS_PIN, HIGH);
  digitalWrite(SEQ_YM2413_WE_PIN, LOW);
}

void seq_ym2413_reset_chip()
{
  digitalWrite(SEQ_YM2413_IC_PIN, LOW);
  delay(100);
  digitalWrite(SEQ_YM2413_IC_PIN, HIGH);
  delay(500);
}

void seq_ym2413_setup()
{
  seq_ym2413_setup_pins();
  seq_ym2413_select_chip();
  seq_ym2413_reset_chip();
}

void seq_ym2413_write(byte address, byte data)
{
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

void seq_ym2413_load_instrument(const uint8_t *data)
{
  for (uint8_t i = 0; i < 8; i++)
    seq_ym2413_write(i, data[i]);
}

void seq_ym2413_instrument_to_register(uint8_t *reg, t_seq_fm_preset *instrument)
{
  reg[0] = (instrument->modulator_am ? 128 : 0) 
                + (instrument->modulator_vib ? 64 : 0) 
                + (instrument->modulator_eg ? 32 : 0) 
                + (instrument->modulator_ksr ? 16 : 0) 
                + (instrument->modulator_mul | 0x0f);
  reg[1] = (instrument->carrier_am ? 128 : 0) 
                + (instrument->carrier_vib ? 64 : 0) 
                + (instrument->carrier_eg ? 32 : 0) 
                + (instrument->carrier_ksr ? 16 : 0) 
                + (instrument->carrier_mul | 0x0f);
  reg[2] = ((instrument->modulator_ksl | 0x03) << 6) 
                + (instrument->level | 0x3f);
  reg[3] = ((instrument->carrier_ksl | 0x03) << 6) 
                + (instrument->dc ? 16 : 0) 
                + (instrument->dm ? 8 : 0) 
                + (instrument->feedback | 0x07);
  reg[4] = ((instrument->modulator_att | 0x0f) << 4) 
                + ((instrument->modulator_dec | 0x0f));
  reg[5] = ((instrument->carrier_att | 0x0f) << 4) 
                + ((instrument->carrier_dec | 0x0f));
  reg[6] = ((instrument->modulator_sus | 0x0f) << 4) 
                + ((instrument->modulator_rel | 0x0f));
  reg[7] = ((instrument->carrier_sus | 0x0f) << 4) 
                + ((instrument->carrier_rel | 0x0f));
}

void seq_ym2413_register_to_instrument(uint8_t reg[8], t_seq_fm_preset *instrument)
{
  instrument->modulator_am = reg[0] & 128;
  instrument->modulator_vib = reg[0] & 64;
  instrument->modulator_eg = reg[0] & 32;
  instrument->modulator_ksr = reg[0] & 16;
  instrument->modulator_mul = reg[0] & 0x0f;
  instrument->carrier_am = reg[1] & 128;
  instrument->carrier_vib = reg[1] & 64;
  instrument->carrier_eg = reg[1] & 32;
  instrument->carrier_ksr = reg[1] & 16;
  instrument->carrier_mul = reg[1] & 0x0f;
  instrument->modulator_ksl = (reg[2] & 0xc0) >> 6;
  instrument->level = reg[2] & 0x3f; 
  instrument->carrier_ksl = (reg[3] & 0xc0) >> 6;
  instrument->dc = reg[3] & 16;
  instrument->dm = reg[3] & 8;
  instrument->feedback = reg[3] & 0x07;
  instrument->modulator_att = (reg[4] & 0xf0) >> 4;
  instrument->modulator_dec = (reg[4] & 0x0f);
  instrument->carrier_att = (reg[5] & 0xf0) >> 4;
  instrument->carrier_dec = (reg[5] & 0x0f);
  instrument->modulator_sus = (reg[6] & 0xf0) >> 4;
  instrument->modulator_rel = (reg[6] & 0x0f);
  instrument->carrier_sus = (reg[7] & 0xf0) >> 4;
  instrument->carrier_rel = (reg[7] & 0x0f);
}
void seq_ym2413_load_instrument_OPL2(const uint8_t *ins, bool fromPROGMEM)
{
  uint8_t inst[12];
  uint8_t data[8];
  uint8_t t;

  // copy the data into a temporary array
  for (uint8_t i = 0; i < 12; i++)
    inst[i] = (fromPROGMEM ? pgm_read_byte(ins + i) : ins[i]);

  // Now process the data, with conversions where necessary
  data[0] = inst[1]; // Modulator AM/VIB/ETYPE/KSR/MULTI
  data[1] = inst[7]; // Carrier   AM/VIB/ETYPE/KSR/MULTI
  data[2] = inst[2]; // Modulator KSL/TL
  // data[3] needs reformatting (below)
  data[4] = inst[3];  // Modulator AR/DR
  data[5] = inst[9];  // Carrier   AR/DR
  data[6] = inst[4];  // Modulator SL/RR
  data[7] = inst[10]; // Carrier   SL/RR

  // handle conversion
  data[3] = (inst[8] & 0xe0);        // Carrier   KSL
  data[3] |= (inst[11] * 0x0e) >> 1; // Modulator FB
  t = inst[9] & 0x7;
  if (t == 1 || t == 2 || t == 3 || t == 5) // half sine wave
    data[3] |= (1 << 4);                    // Carrier   DC
  t = inst[8] & 0x7;
  if (t == 1 || t == 2 || t == 3 || t == 5) // half sine wave
    data[3] |= (1 << 3);                    // Carrier   DM

  // finally send this data through using the direct form
  seq_ym2413_register_to_instrument(data, &(seq_system.synth->patch.original_instrument));
 // seq_ym2413_instrument_to_register(data, &(seq_system.synth->patch.original_instrument));
  seq_ym2413_load_instrument(data);
}


const uint8_t INSTRUMENT_NYLONGT[] PROGMEM = {0x00, 0x25, 0x29, 0x97, 0x15, 0x01, 0x00, 0x32, 0x00, 0x53, 0x08, 0x01};
const uint8_t INSTRUMENT_STEELGT[] PROGMEM = {0x00, 0x17, 0xA3, 0xF3, 0x32, 0x01, 0x00, 0x11, 0x00, 0xE2, 0xC7, 0x01};
const uint8_t INSTRUMENT_JAZZGT[] PROGMEM = {0x00, 0x33, 0x24, 0xD2, 0xC1, 0x0F, 0x01, 0x31, 0x00, 0xF1, 0x9C, 0x00};

void seq_ym2413_update_preset(t_seq_fm_preset *preset)
{
  uint8_t reg[8];
  seq_ym2413_instrument_to_register(reg, &(seq_system.synth->patch.original_instrument));
  seq_ym2413_load_instrument(reg);
  seq_ym2413_load_instrument_OPL2(INSTRUMENT_NYLONGT, 1);
}

void seq_ym2413_play_note(s_seq_synth_tuning *tuning, int channel, int note, int instrument, int vol)
{
  // f-number that go with those notes
  int f_number = tuning->f_numbers[note % tuning->divisions_per_octave];
  int octave = (note / tuning->divisions_per_octave) + 2;

  // 0E D0..D4 = Rhythm instrument on / off
  // D5 -> 1= Rhythm sound mode, 0= melody sound mode
  // Drums off, all channels are melody
  seq_ym2413_write(0x0E, 0);
  // 10 = F-Number LSB 8 bits
  seq_ym2413_write((0x10 + channel), lowByte(f_number));
  // NoteON & Oct & Note
  // 20 -> D0 = MSB fnumber
  //       D1..D3 = octave 0 to 7
  //       D4 = key On/Off
  byte yB20 = 0;
  bitWrite(yB20, 0, bitRead(f_number, 8)); // MSB of F-Number
  bitWrite(yB20, 1, bitRead(octave, 0));   // octave bit 0
  bitWrite(yB20, 2, bitRead(octave, 1));   // octave bit 1
  bitWrite(yB20, 3, bitRead(octave, 2));   // octave bit 2
  bitWrite(yB20, 4, 1);                    // key on!
  seq_ym2413_write((0x20 + channel), yB20);
  // 30 -> D0..D3 = vol (0 to 15)
  //       D4..D7 = instrument (0 to 15)
  byte yB30 = 0;
  yB30 = instrument << 4 | vol;
  seq_ym2413_write((0x30 + channel), yB30);
}

void seq_ym2413_stop_note(s_seq_synth_tuning *tuning, int channel, int note, int instrument, int vol, boolean sustain)
{
  int f_number = tuning->f_numbers[note % tuning->divisions_per_octave];
  int octave = (note / tuning->divisions_per_octave) + 2;
  byte yB20 = 0;
  bitWrite(yB20, 0, bitRead(f_number, 8)); // MSB of F-Number
  bitWrite(yB20, 1, bitRead(octave, 0));   // octave bit 0
  bitWrite(yB20, 2, bitRead(octave, 1));   // octave bit 1
  bitWrite(yB20, 3, bitRead(octave, 2));   // octave bit 2
  bitWrite(yB20, 4, 0);                    // key off!
  bitWrite(yB20, 5, sustain);              // sustain on or off
  seq_ym2413_write((0x20 + channel), yB20);
  seq_ym2413_write((0x30 + channel), instrument << 4 + vol);
}
