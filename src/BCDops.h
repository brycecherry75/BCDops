#ifndef BCDops_h
#define BCDops_h

#include <Arduino.h>

const uint8_t NoBCDerror = 0;
const uint8_t NotBCDvalue = 1;
const uint8_t BCDoutOfRange = 2;
const uint8_t BCDunderflowValue = 3;
const uint8_t BCDoverflowValue = 4;
const uint8_t BCDdivideByZero = 5;

class BCDopsClass {
  public:
    void CheckBCD(uint32_t BCDvalue, uint8_t *ErrorCode);
    uint32_t BinaryToBCD(uint32_t BinaryValue, uint8_t *ErrorCode);
    uint32_t BCDtoBinary(uint32_t CurrentBCDvalue, uint8_t *ErrorCode);
    uint32_t BCDadd(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode);
    uint32_t BCDsubtract(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode);
    uint32_t BCDmultiply(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode);
    uint32_t BCDdivide(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode);
    uint32_t BCDmod(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode);
};

extern BCDopsClass BCDops;

#endif