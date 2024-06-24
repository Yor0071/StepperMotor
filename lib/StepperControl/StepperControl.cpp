#include "StepperControl.h"

volatile unsigned long StepperControl::overflow_count = 0;

ISR(TIMER1_OVF_vect) {
    StepperControl::overflow_count++;
}

StepperControl::StepperControl(int steps_per_rev, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
{
    this->step_number = 0;
    this->last_step_time = 0;
    this->number_of_steps = steps_per_rev;
    this->direction = 0;
    this->step_delay = 0;
    
    // Motor pins
    this->motor_pins[0] = pin1;
    this->motor_pins[1] = pin2;
    this->motor_pins[2] = pin3;
    this->motor_pins[3] = pin4;

    // Set the motor pins as output
    DDRB |= (1 << pin1) | (1 << pin2) | (1 << pin3) | (1 << pin4);
}

void StepperControl::setSpeed(int rpm)
{
    this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / rpm;
}

void StepperControl::step(int steps_to_move)
{
    int steps_left = abs(steps_to_move); // How many steps to take

    // Determine direction based on whether steps_to_move is positive or negative
    if (steps_to_move > 0) { this->direction = 1; }
    if (steps_to_move < 0) { this->direction = 0; }

    // Decrement the number of steps, moving one step each time
    while (steps_left > 0)
    {
        unsigned long now = StepperControl::getMicros();
        // move only if the appropriate delay has passed:
        if (now - this->last_step_time >= this->step_delay)
        {
            // get the timeStamp of when you stepped:
            this->last_step_time = now;
            // increment or decrement the step number,
            // depending on direction:
            if (this->direction == 1)
            {
                this->step_number++;
                if (this->step_number == this->number_of_steps) {
                    this->step_number = 0;
                }
            }
            else
            {
                if (this->step_number == 0) 
                {
                    this->step_number = this->number_of_steps;
                }
                    this->step_number--;
            }
            // decrement the steps left:
            steps_left--;

            stepMotor(this->step_number % 4);
        }
    }
}

void StepperControl::stepMotor(int thisStep)
{
    switch (thisStep) {
    case 0:  // 1010
        PORTB |= (1 << this->motor_pins[0]);  
        PORTB &= ~(1 << this->motor_pins[1]); 
        PORTB |= (1 << this->motor_pins[2]);  
        PORTB &= ~(1 << this->motor_pins[3]); 
        break;
    case 1:  // 0110
        PORTB &= ~(1 << this->motor_pins[0]); 
        PORTB |= (1 << this->motor_pins[1]);  
        PORTB |= (1 << this->motor_pins[2]);  
        PORTB &= ~(1 << this->motor_pins[3]); 
        break;
    case 2:  // 0101
        PORTB &= ~(1 << this->motor_pins[0]); 
        PORTB |= (1 << this->motor_pins[1]);  
        PORTB &= ~(1 << this->motor_pins[2]); 
        PORTB |= (1 << this->motor_pins[3]);  
        break;
    case 3:  // 1001
        PORTB |= (1 << this->motor_pins[0]);  
        PORTB &= ~(1 << this->motor_pins[1]); 
        PORTB &= ~(1 << this->motor_pins[2]); 
        PORTB |= (1 << this->motor_pins[3]);  
        break;
    }
}