#include "buttonDebounce.h"

buttonDebounce::buttonDebounce(volatile uint8_t *button_DDRx, volatile uint8_t *button_PINx, volatile uint8_t *button_PORTx, uint8_t button_PORTxn)
{
    _button_DDRx = button_DDRx;
    _button_PINx = button_PINx;
    _button_PORTx = button_PORTx;
    _button_PORTxn = button_PORTxn;
}

void buttonDebounce::input(bool internalPullup)
{
    // set button pin as input
    *_button_DDRx &= ~(1 << _button_PORTxn);

    if(internalPullup == true)
        *_button_PORTx |= (1 << _button_PORTxn);     // enable internal pullup resistor if true
}

bool buttonDebounce::debouncePulse(const unsigned long debounceDelay)
{
    bool reading = ((*_button_PINx & (1 << _button_PORTxn)) >> _button_PORTxn);

    if(reading != _lastPulseButtonState)
        _previousPulseTime = millis();     // reset the debouncing timer

    if((millis() - _previousPulseTime) > debounceDelay)
    {
        if(reading == false && oneShot == true)
        {
            oneShot = false;
            return false;
        }
        else if(reading == true)
            oneShot = true;
    }
    _lastPulseButtonState = reading;

    return true;
}

bool buttonDebounce::debounceHold(const unsigned long debounceDelay)
{
    bool reading = ((*_button_PINx & (1 << _button_PORTxn)) >> _button_PORTxn);

    if(reading != _lastHoldButtonState)
        _previousHoldTime = millis();     // reset the debouncing timer

    if((millis() - _previousHoldTime) > debounceDelay)
    {
        if(reading == false)
            return false;
    }
    _lastHoldButtonState = reading;

    return true;
}