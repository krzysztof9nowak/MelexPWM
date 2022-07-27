#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
    DDRB |= (1 << PB1); // set PWM pin PB1 as output
    TCCR0A = (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // correct PWM OCR0A
    TCCR0B = (1 << CS01); // x8 prescaling

    ADMUX = (1 << MUX0); // ADC1 on PB2
    ADCSRA = (1 << ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); // enable, 128x prescaler

    while(1){
        ADCSRA |= (1<<ADSC); // start conversion
        while(ADCSRA & (1<<ADSC)); // wait for result
        OCR0B = ADC / 4;
    }

    return 1;
}