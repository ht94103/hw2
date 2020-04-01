#include "mbed.h"

AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
Serial pc( USBTX, USBRX );
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(){

  int freq[3], sample = 1500, n = 0, t[2];
  float ADCdata[1500];

  wait(2);

  for (int i = 0; i < sample; i++){
    Aout = Ain;
    ADCdata[i] = Ain;
    wait(1.0/(float)sample);
  }

  /*for (int i = 0; i < sample; i++){
    pc.printf("%1.3f\r\n", ADCdata[i]);
    wait(0.1);
  }*/           // For FFT analysis. Uncommented when doing FFT analysis.

  for (int i = 0; i < sample; i++){
    if ((ADCdata[i + 1] > 1) && (1 > ADCdata[i - 1])){
      t[n] = i;
      n++;
    }
    if (n == 2){
      break;
    }
  }


  freq[0] = 100;//sample / (t[1] - t[0]);
  freq[3] = freq[0] % 10;
  freq[2] = ((freq[0] - freq[3]) / 10) % 10;
  freq[1] = ((freq[0] - freq[3] - freq[2] * 10 ) / 100 ) % 10;

  while(1){

    Aout = 0.5 + 0.5*cos(2*3.1415926*freq[0]*n/sample);
    n = n + 1;
    //pc.printf("%1.3f\r\n", ADCvalue);

    if (Switch == 0){

      redLED = 0;
      greenLED = 1;

      for (int i = 1; i <= 3; i++){
        if (i != 3){
            for (int j = 0; j < 10; j++){
              if (freq[i] == j){
                display = table[j];
                wait(1);
              }
          }
        }
        else {
          for (int j = 0; j < 10; j++){
              if (freq[3] == j){
                display = table[j] + 0x80;
                wait(1);
              }
          }
        }
      }

    }

    else{
      redLED = 1;
      greenLED = 0;
      display = 0x00;
    }

    wait(1.0/(float)sample);

  }

}