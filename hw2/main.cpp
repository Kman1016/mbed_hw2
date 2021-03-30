#include "mbed.h"
#include "TextLCD.h"

using namespace std::chrono;

TextLCD lcd(D0, D1, D2, D3, D4, D5);    // RS, E, DB4-DB7

DigitalIn b0(D10);
DigitalIn b1(D11);
DigitalIn b2(D12);

AnalogOut aout(PA_4);
AnalogIn ain(A0);

Thread t1;
Thread t2;
Thread t3;
Thread t4;
InterruptIn YES(D10);
EventQueue queue(16 * EVENTS_EVENT_SIZE);

void samplef();
int button();
void LCD(int sel);
void Fre_change();
void wave_g();

float Fre = 100;
int T = 10;
int r = 0;

int main()
{
  t1.start(samplef);
  t2.start(button);
  t3.start(callback(&queue, &EventQueue::dispatch_forever));
  YES.rise(queue.event(Fre_change));
  t4.start(wave_g);
}

void wave_g()
{
  float i;
  while (1) {
      // change the voltage on the digital output pin by 0.1 * (sleep time_
    for (i = 0.0f; i < 1.0f; i += 0.1f) {
      aout = i;
      wait_us(T * 24);
    }
    for (; i > 0.0f; i -= 0.1f) {
      aout = i;
      wait_us(T * 16);
    }    
  }
}

void samplef()
{
  int sample = 200;
  float ain_data;
  int rate = 1000 / sample;
  while(1) {
    ain_data = ain;
    printf("%f\r\n", ain_data);
    ThisThread::sleep_for(rate * 1ms);
  }
}

int button()
{
  while (1)
  {
    
    if (b1.read() == 1 && Fre < 500) 
    {
      Fre = Fre + 10;
      ThisThread::sleep_for(500ms);
    }
    else if (b2.read() == 1 && Fre > 10)
    {
      Fre = Fre - 10;
      ThisThread::sleep_for(500ms);
    }
    LCD(Fre);
  }
}
void LCD(int sel)
{
        lcd.locate(0,1);
        lcd.printf("%d", sel);
        ThisThread::sleep_for(1ms);
}
void Fre_change()
{
    T = (1 / Fre) * 1000;
    ThisThread::sleep_for(300ms);
}