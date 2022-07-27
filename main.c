#include <avr/io.h>
#include <util/delay.h>


#define PWM_PIN (1 << 1)


int main (void)
{
    DDRB |= PWM_PIN; // set PWM pin as output
    TCCR0A = (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // correct PWM OCR0A
    TCCR0B = (1 << CS00); // no prescaling

    while(1){
        OCR0A++;
        OCR0B++;

        _delay_ms(50);
    }

    return 1;
}