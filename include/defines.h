#ifndef defines_h
#define defines_h

// seconds indicator pin defines
#define SECONDS_INDICATOR_PIN_DDRx   DDRD
#define SECONDS_INDICATOR_PIN_PORTx  PORTD
#define SECONDS_INDICATOR_PIN_PORTxn PORTD2

// hour led pin defines
#define HOUR_LED_PIN_DDRx   DDRD
#define HOUR_LED_PIN_PORTx  PORTD
#define HOUR_LED_PIN_PORTxn PORTD3

// minute led pin defines
#define MINUTE_LED_PIN_DDRx   DDRD
#define MINUTE_LED_PIN_PORTx  PORTD
#define MINUTE_LED_PIN_PORTxn PORTD4

// decrement button pin defines
#define DECREMENT_BUTTON_PIN_PINx   PIND
#define DECREMENT_BUTTON_PIN_DDRx   DDRD
#define DECREMENT_BUTTON_PIN_PORTx  PORTD
#define DECREMENT_BUTTON_PIN_PORTxn PORTD5

// increment button pin defines
#define INCREMENT_BUTTON_PIN_PINx   PIND
#define INCREMENT_BUTTON_PIN_DDRx   DDRD
#define INCREMENT_BUTTON_PIN_PORTx  PORTD
#define INCREMENT_BUTTON_PIN_PORTxn PORTD6

// mode button pin defines
#define MODE_BUTTON_PIN_PINx   PIND
#define MODE_BUTTON_PIN_DDRx   DDRD
#define MODE_BUTTON_PIN_PORTx  PORTD
#define MODE_BUTTON_PIN_PORTxn PORTD7

// display enable pin defines
#define DISPLAY_EN_PIN_DDRx   DDRB
#define DISPLAY_EN_PIN_PORTx  PORTB
#define DISPLAY_EN_PIN_PORTxn PORTB1     // PORTB0 for V6.1

// master reset pin defines
#define MASTER_RESET_PIN_DDRx   DDRB
#define MASTER_RESET_PIN_PORTx  PORTB
#define MASTER_RESET_PIN_PORTxn PORTB2

// clock pin defines
#define CLOCK_PIN_DDRx   DDRB
#define CLOCK_PIN_PORTx  PORTB
#define CLOCK_PIN_PORTxn PORTB3

// latch pin defines
#define LATCH_PIN_DDRx   DDRB
#define LATCH_PIN_PORTx  PORTB
#define LATCH_PIN_PORTxn PORTB4

// data pin defines
#define DATA_PIN_DDRx   DDRB
#define DATA_PIN_PORTx  PORTB
#define DATA_PIN_PORTxn PORTB5

// least or most significant bit first
#define LSB_FIRST 0
#define MSB_FIRST 1

// defines for digit blanking
#define HOUR_1   1
#define HOUR_2   2
#define MINUTE_1 3
#define MINUTE_2 4

// cathode routine defines
#define CATHODE_ROUTINE_TIME_INTERVAL 10        // defined in minutes
#define CATHODE_ROUTINE_DURATION      27000     // defined in milliseconds
#define CATHODE_ROUTINE_DIGIT_DELAY   75        // defined in milliseconds

// button debounce time defines
#define DEBOUNCE_TIME 25     // defined in milliseconds

#endif