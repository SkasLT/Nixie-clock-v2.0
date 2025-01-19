#include <uRTCLib.h>
#include <AVR_utils.h>
#include "defines.h"
#include "buttonDebounce.h"
#include "debug.h"

// uRTCLib rtc;
uRTCLib rtc(0x68);

// debounced buttons objects
buttonDebounce modeButton(&MODE_BUTTON_PIN_DDRx, &MODE_BUTTON_PIN_PINx, &MODE_BUTTON_PIN_PORTx, MODE_BUTTON_PIN_PORTxn);
buttonDebounce incrementButton(&INCREMENT_BUTTON_PIN_DDRx, &INCREMENT_BUTTON_PIN_PINx, &INCREMENT_BUTTON_PIN_PORTx, INCREMENT_BUTTON_PIN_PORTxn);
buttonDebounce decrementButton(&DECREMENT_BUTTON_PIN_DDRx, &DECREMENT_BUTTON_PIN_PINx, &DECREMENT_BUTTON_PIN_PORTx, DECREMENT_BUTTON_PIN_PORTxn);

uint8_t menuPage = 0;           // menupage
uint8_t lastMinute = 100;       // lastminute
uint8_t minuteCounter = -1;     // minutecounter

uint8_t hour, minute, second;
uint8_t hour1, hour2, minute1, minute2, second1, second2;
uint8_t lastSecond = 100;

void calculateTime(void)
{
    rtc.refresh();

    hour = rtc.hour();
    hour1 = hour / 10;
    hour2 = hour % 10;

    minute = rtc.minute();
    minute1 = minute / 10;
    minute2 = minute % 10;

    second = rtc.second();
    second1 = second / 10;
    second2 = second % 10;

    if(lastSecond != second && (DEBUG == 1))
    {
        lastSecond = second;

        debug("Current time: ");
        debug(hour1);
        debug(hour2);
        debug(":");
        debug(minute1);
        debug(minute2);
        debug(":");
        debug(second1);
        debugln(second2);
    }
}

uint32_t startTime = millis();

bool state = true;

void blinkPin(volatile uint8_t *pinPORTx, uint8_t pinPORTxn, uint32_t timeDelay)
{
    if((millis() - startTime) >= timeDelay)
    {
        state = !state;

        setPin(*pinPORTx, pinPORTxn, state);

        startTime = millis();
    }
}

void shiftOutBits(volatile uint8_t *dataPinPORTx, volatile uint8_t dataPinPORTxn, volatile uint8_t *clockPinPORTx, volatile uint8_t clockPinPORTxn, uint8_t bitOrder, uint8_t value, uint8_t bits,
                  bool enableBlanking)
{
    if(enableBlanking == false)
    {
        for(int i = 0; i < bits; i++)
        {
            if(bitOrder == LSB_FIRST)
                setPin(*dataPinPORTx, dataPinPORTxn, (i == value));

            else
                setPin(*dataPinPORTx, dataPinPORTxn, ((bits - 1 - i) == value));

            setPin(*clockPinPORTx, clockPinPORTxn, HIGH);
            setPin(*clockPinPORTx, clockPinPORTxn, LOW);
        }
    }

    else
    {
        setPin(*dataPinPORTx, dataPinPORTxn, LOW);

        for(int i = 0; i < bits; i++)
        {
            setPin(*clockPinPORTx, clockPinPORTxn, HIGH);
            setPin(*clockPinPORTx, clockPinPORTxn, LOW);
        }
    }
}

void cathodeRoutine(uint32_t duration, uint32_t digitDelay)
{
    uint32_t startTime = millis();

    debugln("cathode routine has starded...");

    while((millis() - startTime) <= duration)
    {
        for(int i = 0; i < 10; i++)
        {
            setPin(LATCH_PIN_PORTx, LATCH_PIN_PORTxn, LOW);
            shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, i, 10, false);
            shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, i, 10, false);
            shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, i, 10, false);
            shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, i, 10, false);
            setPin(LATCH_PIN_PORTx, LATCH_PIN_PORTxn, HIGH);
            delay(digitDelay);
        }

        for(int i = 8; i > 0; i--)
        {
            setPin(LATCH_PIN_PORTx, LATCH_PIN_PORTxn, LOW);
            shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, i, 10, false);
            shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, i, 10, false);
            shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, i, 10, false);
            shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, i, 10, false);
            setPin(LATCH_PIN_PORTx, LATCH_PIN_PORTxn, HIGH);
            delay(digitDelay);
        }
    }

    debugln("cathode routine has completed");
}

void updateDisplayedTime(uint8_t blankDigit)
{
    setPin(LATCH_PIN_PORTx, LATCH_PIN_PORTxn, LOW);
    shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, minute2, 10, blankDigit == MINUTE_2);
    shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, minute1, 10, blankDigit == MINUTE_1);
    shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, hour2, 10, blankDigit == HOUR_2);
    shiftOutBits(&DATA_PIN_PORTx, DATA_PIN_PORTxn, &CLOCK_PIN_PORTx, CLOCK_PIN_PORTxn, LSBFIRST, hour1, 10, blankDigit == HOUR_1);
    setPin(LATCH_PIN_PORTx, LATCH_PIN_PORTxn, HIGH);
}

void idleState()
{
    calculateTime();

    if(lastMinute != rtc.minute())
    {
        minuteCounter++;
        lastMinute = rtc.minute();

        if(minuteCounter == CATHODE_ROUTINE_TIME_INTERVAL)
        {
            minuteCounter = 0;
            cathodeRoutine(CATHODE_ROUTINE_DURATION, CATHODE_ROUTINE_DIGIT_DELAY);
        }

        updateDisplayedTime(false);
    }

    if(modeButton.debouncePulse(DEBOUNCE_TIME) == LOW)
        menuPage++;
}

// menu page for changing hours
void firstMenuPage()
{
    while(menuPage == 1)
    {
        blinkPin(&HOUR_LED_PIN_PORTx, HOUR_LED_PIN_PORTxn, 200);

        if(modeButton.debouncePulse(DEBOUNCE_TIME) == LOW)
            menuPage++;

        if(incrementButton.debouncePulse(DEBOUNCE_TIME) == LOW)
        {
            hour = (hour + 1) % 24;

            hour1 = hour / 10;
            hour2 = hour % 10;
        }

        if(decrementButton.debouncePulse(DEBOUNCE_TIME) == LOW)
        {
            if(hour > 0)
                hour--;
            else
                hour = 23;

            hour1 = hour / 10;
            hour2 = hour % 10;
        }

        updateDisplayedTime(false);

        debug("Set hours : ");
        debugln(hour);
    }

    setPin(HOUR_LED_PIN_PORTx, HOUR_LED_PIN_PORTxn, LOW);
}

// menu page for changing minutes
void secondMenuPage()
{
    while(menuPage == 2)
    {
        blinkPin(&MINUTE_LED_PIN_PORTx, MINUTE_LED_PIN_PORTxn, 200);

        if(modeButton.debouncePulse(DEBOUNCE_TIME) == LOW)
            menuPage++;

        if(incrementButton.debouncePulse(DEBOUNCE_TIME) == LOW)
        {
            minute = (minute + 1) % 60;

            minute1 = minute / 10;
            minute2 = minute % 10;
        }

        if(decrementButton.debouncePulse(DEBOUNCE_TIME) == LOW)
        {
            if(minute > 0)
                minute--;
            else
                minute = 59;

            minute1 = minute / 10;
            minute2 = minute % 10;
        }

        updateDisplayedTime(false);

        debug("Set minutes: ");
        debugln(minute);
    }

    setPin(MINUTE_LED_PIN_PORTx, MINUTE_LED_PIN_PORTxn, LOW);
}

// last menu page that sets adjusted time in the RTC module
void lastMenuPage()
{
    // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
    rtc.set(0, minute, hour, rtc.dayOfWeek(), rtc.dayOfWeek(), rtc.month(), rtc.year());

    menuPage = 0;
}

void setup()
{
    debugBegin(9600);

    // begin i2c with rtc
    URTCLIB_WIRE.begin();

    // set buttons as inputs
    modeButton.input(ENABLE_INTERNAL_PULLUP);
    incrementButton.input(ENABLE_INTERNAL_PULLUP);
    decrementButton.input(ENABLE_INTERNAL_PULLUP);

    // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
    // rtc.set(50, 54, 5, 7, 24, 7, 23);

    // set pins as outputs
    outputPin(SECONDS_INDICATOR_PIN_DDRx, SECONDS_INDICATOR_PIN_PORTxn);
    outputPin(HOUR_LED_PIN_DDRx, HOUR_LED_PIN_PORTxn);
    outputPin(MINUTE_LED_PIN_DDRx, MINUTE_LED_PIN_PORTxn);
    outputPin(DISPLAY_EN_PIN_DDRx, DISPLAY_EN_PIN_PORTxn);
    outputPin(MASTER_RESET_PIN_DDRx, MASTER_RESET_PIN_PORTxn);
    outputPin(CLOCK_PIN_DDRx, CLOCK_PIN_PORTxn);
    outputPin(LATCH_PIN_DDRx, LATCH_PIN_PORTxn);
    outputPin(DATA_PIN_DDRx, DATA_PIN_PORTxn);

    // set pins as high
    setPin(MASTER_RESET_PIN_PORTx, MASTER_RESET_PIN_PORTxn, HIGH);
    setPin(DISPLAY_EN_PIN_PORTx, DISPLAY_EN_PIN_PORTxn, LOW);

    // set pin as high
    setPin(SECONDS_INDICATOR_PIN_PORTx, SECONDS_INDICATOR_PIN_PORTxn, HIGH);

    // do cathode routine when starting
    cathodeRoutine(3000, CATHODE_ROUTINE_DIGIT_DELAY);
}

void loop()
{
    idleState();

    switch(menuPage)
    {
        case 1:
            firstMenuPage();
        case 2:
            secondMenuPage();
        case 3:
            lastMenuPage();
    }
}