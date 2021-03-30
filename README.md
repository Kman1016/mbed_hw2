# mbed_hw2

main.c++:
// library
#include "mbed.h"
#include "TextLCD.h"

//LCD pin
TextLCD lcd(D0, D1, D2, D3, D4, D5);    // RS, E, DB4-DB7

//button pin
DigitalIn b0(D10);
DigitalIn b1(D11);
DigitalIn b2(D12);

// wave generate pin and result input pin
AnalogOut aout(PA_4);
AnalogIn ain(A0);

// sample our result wave
void samplef();

// control frequency, D10 is confirm, D11: +10, D12: -10
int button();

// show frequency value in ideal, included in button()
void LCD(int sel);

// when we confirm frequency, will call out this function, to actually change current frequency
void Fre_change();

// generate the wave
void wave_g();

// i need 4 thread to deal with different task
Thread t1;
Thread t2;
Thread t3;
Thread t4;

// interrupt when i confirm freq..
InterruptIn YES(D10);
EventQueue queue(16 * EVENTS_EVENT_SIZE);

int main()
{
  t1.start(samplef);
  t2.start(button);
  
  t3.start(callback(&queue, &EventQueue::dispatch_forever));
  YES.rise(queue.event(Fre_change));
  
  t4.start(wave_g);
}
