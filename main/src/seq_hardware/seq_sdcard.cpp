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

#include <Arduino.h>
#include "SD.h"

#include "../seq.h"

File myFile;
SPIClass SPISD(HSPI);
TaskHandle_t  seq_sd_task_handler;

int8_t seq_sd_init()
{
  SPISD.begin(SEQ_SD_SCK_PIN, SEQ_SD_MISO_PIN, SEQ_SD_MOSI_PIN);
  if (!SD.begin(SEQ_SD_CS_PIN, SPISD))
  {
    Serial.println(F("SD init failed!"));
    return (-1);
  }
  else
  {
    Serial.println(F("SD read!"));
    return (0);
  }
}


void SD_Init(void *parameter)
{
  uint32_t bytes_read = 0;
  t_seq_sequencer *sequencer = (t_seq_sequencer *)parameter;
  t_seq_track *track = sequencer->tracks[0];
  seq_sd_init();
  s_seq_event event;
  while (1) {
    myFile = SD.open("/track_2.hex", "r"); //read from file
    if (myFile)
    {
      Serial.println("track_0.seq:");
      while (myFile.available())
      {
        if (!track)
        {
          Serial.println("No hay track");
          vTaskDelay(1000);
        } 
        else if (!track->track_buffer)
        {
          Serial.println("No hay track buffer");
          vTaskDelay(1000);
        }
        else if (track->track_buffer->isFull())
        {
          Serial.println("Esta lleno");
          track->enable = 1;
          vTaskDelay(1000);
        }

        // bool full = !track->track_buffer->isFull();/*)*/
        if (!track->track_buffer->isFull()){
        //Serial.println("Dentro:");
        myFile.read((uint8_t*)&event, 7);
        bytes_read += 7;
        track->track_buffer->push(&event);
        }

        if (bytes_read > 2000)
        {
          vTaskDelay(10);
        }
        else
        {
          vTaskDelay(10);
        }
      }
      myFile.close();
      Serial.println("se ha cerrado el fichero");
    }
    else
    {
      Serial.println("error opening track_10.seq to read");
    }
    Serial.printf("fin de l lectura %u bytes leidos", bytes_read);
  }
}


void seq_sd_init_task(t_seq_sequencer *sequencer)
{
  xTaskCreatePinnedToCore(
    SD_Init, /* Function to implement the task */
    "sd task", /* Name of the task */
    10000,  /* Stack size in words */
    sequencer,  /* Task input parameter */
    0,  /* Priority of the task */
    &seq_sd_task_handler,  /* Task handle. */
    0); /* Core where the task should run */
}
