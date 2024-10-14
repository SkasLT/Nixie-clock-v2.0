#ifndef AVR_UTILS_H_
#define AVR_UTILS_H_

// port manipulation
#define setPin(PORTx, PORTxn, state) (state == true) ? (PORTx |= (1 << PORTxn)) : (PORTx &= ~(1 << PORTxn))
#define readPin(PINx, PORTxn)        (PINx & (1 << PORTxn)) >> PORTxn
#define togglePin(PORTx, PORTxn)     PORTx ^= (1 << PORTxn)

// setting ports for input/output mode
#define outputPin(DDRx, PORTxn) DDRx |= (1 << PORTxn)
#define inputPin(DDRx, PORTxn)  DDRx &= ~(1 << PORTxn)

// bit manipulation
#define setBitReg(reg, bit)   reg |= (1 << bit)
#define resetBitReg(reg, bit) reg &= ~(1 << bit)

#endif