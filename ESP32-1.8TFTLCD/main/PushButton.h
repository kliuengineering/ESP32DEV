/**
 * @file PushButton.h
 * @brief Header file for constructing debounced PBs.
 *
 * This file contains all the definitions and function declarations needed
 * to interface PB with MCUs.
 * It is encapsulated inside a class.
 *
 * @author Kevin Liu 
 * @date Created on: 2024-05-06
 * @version 1.0
 *
 * @note This code is intended for educational purposes and can be used
 *       as a base for further development and integration within various
 *       projects.
 *
 * COPYRIGHT NOTICE: All rights reserved. Unauthorized copying of this file,
 * via any medium is strictly prohibited without the express permission of the
 * authour.
 */


 #ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

#include <Arduino.h>

class PushButton 
{
  private:
    int _pin;
    unsigned long _debounce_delay;
    unsigned long _last_debounce_time;
    bool _button_state;
    bool _last_button_state;
    bool _stable_state;
    bool _pressed_event;  // Event flag for button press
    bool _released_event; // Event flag for button release

  public:
    PushButton(int pin_pb, unsigned long delay = 50) :
              _pin(pin_pb), _debounce_delay(delay), _last_debounce_time(0), 
              _stable_state(false), _pressed_event(false), _released_event(false) 
              {
                pinMode(_pin, INPUT_PULLUP);
                _last_button_state = digitalRead(_pin);
                _button_state = _last_button_state;
              }

    // Read and process button events
    void read() 
    {
        bool current_state = digitalRead(_pin);

        // checks the pin voltage, then updates the time stamp of the check
        if (current_state != _last_button_state) 
        {
            _last_debounce_time = millis();
        }

        // edge detection algorithm; it depends on the preset delay from the parametric constructor
        if ( ( millis() - _last_debounce_time ) > _debounce_delay ) 
        {
            // if you reach here, then it means it is a valid rising/falling edge
            if ( current_state != _stable_state ) 
            {
                _stable_state = current_state;

                if (_stable_state == LOW) 
                {
                    _pressed_event = true;  // Button was pressed
                } 
                else 
                {
                    _released_event = true; // Button was released
                }
            }
        }

        _last_button_state = current_state;
    }

    bool is_pressed() 
    {
        if (_pressed_event) 
        {
            _pressed_event = false;  // Reset the event flag
            return true;
        }
        return false;
    }

    bool is_released() 
    {
        if (_released_event) 
        {
            _released_event = false;  // Reset the event flag
            return true;
        }
        return false;
    }
};

#endif



// #ifndef __PUSHBUTTON_H__
// #define __PUSHBUTTON_H__

// #include <Arduino.h>

// class PushButton // debounced
// {
//   private:
//     int _pin;  // Pin the button is connected to
//     unsigned long _last_debounce_time;  // Last time the output pin was toggled
//     unsigned long _debounce_delay;      // Debounce delay in milliseconds
//     int _button_state;                  // Current reading from the input pin
//     int _last_button_state;             // Previous reading from the input pin
//     int _stable_state;                  // Stable state after debouncing

//   public:
//     // Parametric constructor
//     PushButton(int pin_pb, unsigned long delay = 50) : 
//       _pin(pin_pb), _debounce_delay(delay), _last_debounce_time(0), _stable_state(LOW) 
//     {
//       pinMode(_pin, INPUT_PULLUP);
//       _last_button_state = digitalRead(_pin);
//       _button_state = _last_button_state;
//     }

//     // Enhanced read function
//     bool read(void)
//     {
//         bool state_changed = false;
//         int reading = digitalRead(_pin);

//         // Check if the button state has changed since the last read
//         if (reading != _last_button_state) {
//             _last_debounce_time = millis(); // Reset the debouncing timer
//         }

//         // If the button state has not changed for the debounce interval
//         if ((millis() - _last_debounce_time) > _debounce_delay) {
//             // And the button state is different from the stable state
//             if (reading != _stable_state) {
//                 _stable_state = reading; // Update the stable state
//                 state_changed = true;    // Indicate that the state has changed
//             }
//         }

//         _last_button_state = reading; // Save the reading for next time
//         return state_changed;         // Return true if the state has changed
//     }

//     // Return the stable state
//     int get_state()
//     {
//       return _stable_state;
//     }
// };

// #endif



// class PushButton // debounced
// {
//   private:
    
//     int PIN;
//     unsigned long debounce_time;
//     unsigned long debounce_delay;
//     int state_button;
//     int state_button_prev;

//   public:

//     // parametric constructor
//     PushButton(int pin_pb, unsigned long delay = 24) : 
//       PIN(pin_pb), debounce_time(0), state_button_prev( digitalRead(pin_pb) )
//     {
//       pinMode(PIN, INPUT_PULLUP);
//       state_button = state_button_prev;
//     }


//     // debounce is implemented on the hardware level
//     bool read(void)
//     {
//         int reading = digitalRead(PIN);
//         if (reading != state_button) 
//         {
//             state_button = reading;
//             return true; // State has changed
//         }
//         return false; // State has not changed
//     }


//     int get_state()
//     {
//       return state_button;
//     }
// };