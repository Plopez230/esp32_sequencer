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


void seq_tuning_equal_tempered(t_seq_synth_tuning *tuning, uint16_t divisions, uint16_t base_frequency)
{
  uint16_t tone_idx;
  double interval;
  double last_tone;

  interval = pow(2, 1.0 / divisions);
  tone_idx = 1;
  tuning->f_numbers[0] = base_frequency;
  last_tone = base_frequency;
  while (tone_idx < divisions)
  {
    last_tone = last_tone * interval;
    tuning->f_numbers[tone_idx] = (uint16_t)last_tone;
    tone_idx++;
  }
  tuning->divisions_per_octave = divisions;
}

void seq_tuning_12(t_seq_synth_tuning *tuning)
{
  tuning->f_numbers[0] = 172;
  tuning->f_numbers[1] = 181;
  tuning->f_numbers[2] = 192;
  tuning->f_numbers[3] = 204;
  tuning->f_numbers[4] = 216;
  tuning->f_numbers[5] = 229;
  tuning->f_numbers[6] = 242;
  tuning->f_numbers[7] = 257;
  tuning->f_numbers[8] = 272;
  tuning->f_numbers[9] = 288;
  tuning->f_numbers[10] = 305;
  tuning->f_numbers[11] = 323;
  tuning->divisions_per_octave = 12;
}

void seq_tuning_13(t_seq_synth_tuning *tuning)
{
  tuning->f_numbers[0] = 172;
  tuning->f_numbers[1] = 181;
  tuning->f_numbers[2] = 191;
  tuning->f_numbers[3] = 202;
  tuning->f_numbers[4] = 213;
  tuning->f_numbers[5] = 225;
  tuning->f_numbers[6] = 237;
  tuning->f_numbers[7] = 250;
  tuning->f_numbers[8] = 263;
  tuning->f_numbers[9] = 278;
  tuning->f_numbers[10] = 293;
  tuning->f_numbers[11] = 309;
  tuning->f_numbers[12] = 326;
  tuning->divisions_per_octave = 13;
}

void seq_tuning_14(t_seq_synth_tuning *tuning)
{
  tuning->f_numbers[0] = 172;
  tuning->f_numbers[1] = 181;
  tuning->f_numbers[2] = 190;
  tuning->f_numbers[3] = 200;
  tuning->f_numbers[4] = 210;
  tuning->f_numbers[5] = 220;
  tuning->f_numbers[6] = 231;
  tuning->f_numbers[7] = 243;
  tuning->f_numbers[8] = 256;
  tuning->f_numbers[9] = 269;
  tuning->f_numbers[10] = 282;
  tuning->f_numbers[11] = 297;
  tuning->f_numbers[12] = 312;
  tuning->f_numbers[13] = 327;
  tuning->divisions_per_octave = 14;
}

void seq_tuning_15(t_seq_synth_tuning *tuning)
{
  tuning->f_numbers[0] = 172;
  tuning->f_numbers[1] = 180;
  tuning->f_numbers[2] = 189;
  tuning->f_numbers[3] = 198;
  tuning->f_numbers[4] = 207;
  tuning->f_numbers[5] = 217;
  tuning->f_numbers[6] = 227;
  tuning->f_numbers[7] = 238;
  tuning->f_numbers[8] = 249;
  tuning->f_numbers[9] = 261;
  tuning->f_numbers[10] = 273;
  tuning->f_numbers[11] = 286;
  tuning->f_numbers[12] = 299;
  tuning->f_numbers[13] = 314;
  tuning->f_numbers[14] = 328;
  tuning->divisions_per_octave = 15;
}

void seq_tuning_16(t_seq_synth_tuning *tuning)
{
  tuning->f_numbers[0] = 172;
  tuning->f_numbers[1] = 180;
  tuning->f_numbers[2] = 188;
  tuning->f_numbers[3] = 196;
  tuning->f_numbers[4] = 205;
  tuning->f_numbers[5] = 214;
  tuning->f_numbers[6] = 223;
  tuning->f_numbers[7] = 233;
  tuning->f_numbers[8] = 243;
  tuning->f_numbers[9] = 254;
  tuning->f_numbers[10] = 265;
  tuning->f_numbers[11] = 277;
  tuning->f_numbers[12] = 289;
  tuning->f_numbers[13] = 302;
  tuning->f_numbers[14] = 315;
  tuning->f_numbers[15] = 329;
  tuning->divisions_per_octave = 16;
}

void seq_tuning_17(t_seq_synth_tuning *tuning)
{
  tuning->f_numbers[0] = 172;
  tuning->f_numbers[1] = 179;
  tuning->f_numbers[2] = 187;
  tuning->f_numbers[3] = 194;
  tuning->f_numbers[4] = 202;
  tuning->f_numbers[5] = 211;
  tuning->f_numbers[6] = 220;
  tuning->f_numbers[7] = 229;
  tuning->f_numbers[8] = 238;
  tuning->f_numbers[9] = 248;
  tuning->f_numbers[10] = 259;
  tuning->f_numbers[11] = 269;
  tuning->f_numbers[12] = 281;
  tuning->f_numbers[13] = 292;
  tuning->f_numbers[14] = 304;
  tuning->f_numbers[15] = 317;
  tuning->f_numbers[16] = 330;
  tuning->divisions_per_octave = 17;
}
