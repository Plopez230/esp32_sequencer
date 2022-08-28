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
#include "src/seq.h"

void disableWiFi(){
    adc_power_off();
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
}

void seq_system_create()
{
  seq_system.sequencer = seq_sequencer_create();
  seq_system.keyboard = seq_keyboard_create();
  seq_system.synth = seq_synth_create();
  seq_system.console = seq_console_create();
}

void seq_system_init()
{
  seq_st7920_init();
  seq_keyboard_init(seq_system.keyboard);
  seq_system.keyboard->output_buffer = seq_system.synth->input_buffer;
  seq_synth_init(seq_system.synth);
  vTaskDelay(1);
  seq_console_init(seq_system.console);
}

void seq_system_loop()
{
  seq_keyboard_loop(seq_system.keyboard);
  seq_synth_loop(seq_system.synth);
  //seq_sequencer_loop(seq_system.sequencer);
}

t_seq_system seq_system;


void setup()
{
  disableWiFi();
  Serial.begin(9600);
  seq_system_create();
  seq_system_init();
}

void loop() {
  seq_system_loop();
}
