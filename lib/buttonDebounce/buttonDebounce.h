#ifndef buttonDebounce_h
#define buttonDebounce_h

#include <Arduino.h>

#define ENABLE_INTERNAL_PULLUP  true
#define DISABLE_INTERNAL_PULLUP false

class buttonDebounce
{
  public:
    buttonDebounce(volatile uint8_t *button_DDRx, volatile uint8_t *button_PINx, volatile uint8_t *button_PORTx, uint8_t button_PORTxn);
    void input(bool enableInternalPullup);
    bool debouncePulse(const unsigned long debounceDelay);
    bool debounceHold(const unsigned long debounceDelay);

  private:
    volatile uint8_t *_button_DDRx;
    volatile uint8_t *_button_PINx;
    volatile uint8_t *_button_PORTx;
    uint8_t _button_PORTxn;

    bool oneShot = true;
    bool _pulseButtonState;
    bool _lastPulseButtonState;
    unsigned long _previousPulseTime;

    bool _holdButtonState;
    bool _lastHoldButtonState;
    unsigned long _previousHoldTime;
};

#endif