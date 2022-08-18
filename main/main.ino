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
#include <WiFi.h>
#include <BluetoothSerial.h>
#include <esp_wifi.h>
#include "driver/adc.h"
#include "seq_include.h"

void disableWiFi(){
        adc_power_off();
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
}

t_seq_keyboard keyboard;
t_seq_ring     keyboard_buffer;
t_seq_synth    synth2;
t_seq_synth_tuning tuning;
t_seq_sequencer sequencer;

void load_track(t_seq_sequencer *sequencer, uint8_t track_no, uint8_t note, uint8_t wait)
{
  t_seq_event event;
  event.idle_ticks = wait;
  event.message[1] = note;
  event.message[2] = 0;
  seq_ring_init(&(sequencer->tracks[track_no].ring), 7, 2500);

  //event.message[0] = (char)(144 + 6);
  //seq_ring_push(&(sequencer->tracks[track_no].ring), &event);
  event.message[0] = (char)(128 + 6);
  seq_ring_push(&(sequencer->tracks[track_no].ring), &event);

  sequencer->tracks[track_no].enable = 1;
}


void setup() {
  disableWiFi();
  Serial.begin(9600);
  seq_st7920_init();
  seq_keyboard_init(&keyboard);
  seq_ring_init(&keyboard_buffer, 3, 1000);
  keyboard.output_buffer = &keyboard_buffer;
  synth2.input_buffer = &keyboard_buffer;
  seq_synth_init(&synth2);
  keyboard.midi_channel = 9;
  synth2.tuning = &tuning;
  seq_tuning_12(&tuning);
  load_track(&sequencer, 0, 40, 120);
  load_track(&sequencer, 1, 40, 120);
  seq_sd_init_task(&sequencer);
  seq_sequencer_setup_timer(&sequencer, 180, SEQ_CONFIG_PRECISION_BASE);
  sequencer.output_buffer = &keyboard_buffer;
}

void loop() {
  seq_keyboard_loop(&keyboard);
  seq_synth_loop(&synth2);
  seq_sequencer_loop(&sequencer);
}
