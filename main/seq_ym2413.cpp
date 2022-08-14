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

#include "seq_include.h"

void seq_ym2413_shift (uint8_t value) {
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

void seq_synth_reset_channels(t_seq_synth *synth)
{
  uint8_t channel_counter;

  channel_counter = 0;
  while (channel_counter < 8)
  {
    synth->channels[channel_counter].instrument = 0;
    synth->channels[channel_counter].note = 0;
    synth->channels[channel_counter].velocity = 0;
    synth->channels[channel_counter].used = 0;
    channel_counter++;
  }
}

int8_t  seq_synth_get_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  int8_t channel_counter;

  channel_counter = 0;
  while (channel_counter < 9)
  {
    if (!synth->channels[channel_counter].used)
    {
      synth->channels[channel_counter].instrument = instrument;
      synth->channels[channel_counter].note = note;
      synth->channels[channel_counter].velocity = velocity;
      synth->channels[channel_counter].used = 1;
      return (channel_counter);
    }
    channel_counter++;
  }
  synth->channels[0].instrument = instrument;
  synth->channels[0].note = note;
  synth->channels[0].velocity = velocity;
  synth->channels[0].used = 1;
  return (0);
}

int8_t  seq_synth_release_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  int x = 0;

  while (x < 9){
    if (synth->channels[x].used && synth->channels[x].note == note && synth->channels[x].instrument == instrument) {
      synth->channels[x].used = false;
      return x;
    }
    x++;
  }
  return -1;
}

//int databus[8] = {27, 29, 31, 33, 35, 37, 39, 41};
//int controlbus[4] = {32, 26, 33, 25};

//int temperamento = 12;

void seq_ym2413_setup_pins() {
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

void seq_ym2413_play_note(int channel, int pitch, int oct, int instrument, int vol) {
  // f-number that go with those notes
  int fnumbers[12] = {172, 181, 192, 204, 216, 229, 242, 257, 272, 288, 305 , 323};
  int fnumber = 0;
    fnumber = fnumbers[pitch];
  // 0E D0..D4 = Rhythm instrument on / off
  // D5 -> 1= Rhythm sound mode, 0= melody sound mode
  // Drums off, all channels are melody
  seq_ym2413_write(0x0E , 0);
  // 10 = F-Number LSB 8 bits
  seq_ym2413_write((0x10 + channel), lowByte(fnumber)); // );
  // NoteON & Oct & Note
  // 20 -> D0 = MSB fnumber
  //       D1..D3 = octave 0 to 7
  //       D4 = key On/Off
  byte yB20 = 0;
  bitWrite(yB20, 0, bitRead(fnumber, 8)); // MSB of F-Number
  bitWrite(yB20, 1, bitRead(oct, 0) ); // octave bit 0
  bitWrite(yB20, 2, bitRead(oct, 1) ); // octave bit 1
  bitWrite(yB20, 3, bitRead(oct, 2) ); // octave bit 2
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

int note_(int note) {
  return (int) note % 12;
}

int octave_ (int note) {
  return (int) note / 12;
}

void seq_synth_play_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  uint8_t channel = seq_synth_get_channel(synth, instrument, note, velocity);
  seq_ym2413_play_note(channel, note_(note), octave_(note), instrument, 0);
}

void seq_synth_stop_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  uint8_t channel = seq_synth_release_channel(synth, instrument, note, velocity);
  if (channel == -1)
    return;
  seq_ym2413_stop_note(channel, false);
}

void seq_synth_init(t_seq_synth *synth)
{
  seq_ym2413_setup();
  seq_synth_reset_channels(synth);
}

void seq_synth_loop(t_seq_synth *synth)
{
  char    event[3];
  uint8_t command;
  uint8_t channel;
  
  if (!synth->input_buffer)
    return;
  while (seq_ring_elements_used(synth->input_buffer))
  {
    seq_ring_pop(synth->input_buffer, event);
    command = event[0] & 0xf0;
    channel = event[0] & 0x0f;
    if (command == 144) {
      seq_synth_play_note(synth, channel, event[1], 0);
    }
    if (command == 128) {
      seq_synth_stop_note(synth, channel, event[1], 0);
    }
  }
}
