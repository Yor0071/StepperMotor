#ifndef StepperControl_h
#define StepperControl_h

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

class StepperControl {
  public:
    // constructors:
    StepperControl(int steps_per_rev, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    void setSpeed(int rpm);
    void step(int steps_to_move);
    
    //Timer functions
    static volatile unsigned long overflow_count;
    
    // Set up Timer 1 for normal mode
    static void init()
    {
        TCCR1A = 0; // Normal operation
        TCCR1B = (1 << CS11); // Prescaler 8, start timer
        TIMSK1 |= (1 << TOIE1); // Enable Timer1 overflow interrupt
    }

    static unsigned long getMicros()
    {
        unsigned long ticks;
        unsigned long overflow_ticks;
        
        // Disable interrupts while reading the timer value
        cli();
        ticks = TCNT1; // Read the timer value
        overflow_ticks = overflow_count;
        sei(); // Enable interrupts

        // Calculate total microseconds
        unsigned long total_ticks = (overflow_ticks << 16) + ticks;
        return total_ticks / 2; // Convert to microseconds (16MHz clock, prescaler 8)
    }
  private:
    void stepMotor(int thisStep);
    int direction;
    int step_number;
    unsigned long step_delay;
    int number_of_steps;
    uint8_t motor_pins[4];
    unsigned long last_step_time;
};

#endif