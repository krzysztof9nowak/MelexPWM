#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

int main (void)
{
    PORTB |= (1 << PB1); // set PWM pin high
    DDRB |= (1 << PB1); // set PWM pin PB1 as output

    OCR0B = 255;
    TCCR0A = (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // correct PWM OCR0A
    TCCR0B = (1 << CS01); // x8 prescaling

    ADMUX = (1 << MUX0); // ADC1 on PB2
    ADCSRA = (1 << ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); // enable, 128x prescaler

    //Watchdog conf
    wdt_enable(WDTO_120MS);
    

    // 2.733 - 4.30 - 5.07
    const int32_t min_pot = 1024 * (2.8 / 5.07);
    const int32_t max_pot = 1024 * (4.30 / 5.07);

    while(1){
        wdt_reset();

        ADCSRA |= (1<<ADSC); // start conversion
        while(ADCSRA & (1<<ADSC)); // wait for result
        int32_t val = ADC;

        val = (255UL * (val - min_pot)) / (max_pot - min_pot) ;
        if(val < 0) val = 0;
        if(val > 255) val = 255;
        
        OCR0B = 255 - val;
        if(OCR0B == 0){
            TCCR0B = 0; // stop pwm
            PORTB &= ~(1 << PB1); // set PWM pin low
        } else{
            TCCR0B = (1 << CS01); // x8 prescaling
        }

        _delay_ms(10);
    }

    return 1;
}