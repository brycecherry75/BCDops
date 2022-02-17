#include "BCDops.h"

void BCDopsClass::CheckBCD(uint32_t BCDvalue, uint8_t *ErrorCode) {
  ErrorCode[0] = NoBCDerror;
  for (int i = 0; i < 8; i++) {
    if ((BCDvalue & 0x0F) > 0x09) {
      ErrorCode[0] = NotBCDvalue;
      break;
    }
    BCDvalue >>= 4;
  }
}

uint32_t BCDopsClass::BinaryToBCD(uint32_t BinaryValue, uint8_t *ErrorCode) {
  ErrorCode[0] = NoBCDerror;
  if (BinaryValue <= 99999999UL) {
    uint32_t ValueToReturn = 0;
    uint32_t factor = 10000000UL;
    for (int i = 0; i < 8; i++) {
      ValueToReturn <<= 4;
      uint32_t temp = BinaryValue;
      temp /= factor;
      ValueToReturn |= temp;
      temp *= factor;
      BinaryValue -= temp;
      factor /= 10;
    }
    return ValueToReturn;
  }
  else {
    ErrorCode[0] = BCDoutOfRange;
    return BinaryValue;
  }
}

uint32_t BCDopsClass::BCDtoBinary(uint32_t CurrentBCDvalue, uint8_t *ErrorCode) {
  ErrorCode[0] = NoBCDerror;
  CheckBCD(CurrentBCDvalue, ErrorCode);
  if (ErrorCode[0] != NotBCDvalue) {
    uint32_t ValueToReturn = 0;
    uint32_t factor = 1;
    for (int i = 0; i < 8; i++) {
      uint32_t temp = CurrentBCDvalue;
      temp &= 0x0F;
      temp *= factor;
      ValueToReturn += temp;
      CurrentBCDvalue >>= 4;
      factor *= 10;
    }
    return ValueToReturn;
  }
  else {
    ErrorCode[0] = NotBCDvalue;
    return CurrentBCDvalue;
  }
}

uint32_t BCDopsClass::BCDadd(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode) {
  ErrorCode[0] = NoBCDerror;
  CheckBCD(CurrentBCDvalue, ErrorCode);
  if (ErrorCode[0] == NotBCDvalue) {
    return CurrentBCDvalue;
  }
  uint32_t ValueToReturn = BCDtoBinary(CurrentBCDvalue, ErrorCode);
  ValueToReturn += value;
  if (ValueToReturn <= 99999999UL) {
    return BinaryToBCD(ValueToReturn, ErrorCode);
  }
  else {
    ErrorCode[0] = BCDunderflowValue;
    return CurrentBCDvalue;
  }
}

uint32_t BCDopsClass::BCDsubtract(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode) {
  ErrorCode[0] = NoBCDerror;
  CheckBCD(CurrentBCDvalue, ErrorCode);
  if (ErrorCode[0] == NotBCDvalue) {
    return CurrentBCDvalue;
  }
  uint32_t ValueToReturn = BCDtoBinary(CurrentBCDvalue, ErrorCode);
  if (ValueToReturn >= value) {
    ValueToReturn -= value;
    return BinaryToBCD(ValueToReturn, ErrorCode);
  }
  else {
    ErrorCode[0] = BCDunderflowValue;
    return CurrentBCDvalue;
  }  
}

uint32_t BCDopsClass::BCDdivide(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode) {
  ErrorCode[0] = NoBCDerror;
  if (value != 0) { // avoid divide by zero
    BCDtoBinary(CurrentBCDvalue, ErrorCode);
    if (ErrorCode[0] == NotBCDvalue) {
      return CurrentBCDvalue;
    }
    uint32_t ValueToReturn = BCDtoBinary(CurrentBCDvalue, ErrorCode);
    if (ErrorCode[0] == BCDoverflowValue) {
      return CurrentBCDvalue;
    }
    ValueToReturn /= value;
    if (ValueToReturn <= 99999999UL) {
      return BinaryToBCD(ValueToReturn, ErrorCode);
    }
    else {
      ErrorCode[0] = BCDoverflowValue;
      return CurrentBCDvalue;
    }
  }
  else {
    ErrorCode[0] = BCDdivideByZero;
    return CurrentBCDvalue;
  }
}

uint32_t BCDopsClass::BCDmultiply(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode) {
  ErrorCode[0] = NoBCDerror;
  BCDtoBinary(CurrentBCDvalue, ErrorCode);
  if (ErrorCode[0] == NotBCDvalue) {
    return CurrentBCDvalue;
  }
  uint32_t ValueToReturn = BCDtoBinary(CurrentBCDvalue, ErrorCode);
  if (ErrorCode[0] == BCDoverflowValue) {
    return CurrentBCDvalue;
  }
  ValueToReturn *= value;
  if (ValueToReturn <= 99999999UL) {
    return BinaryToBCD(ValueToReturn, ErrorCode);
  }
  else {
    ErrorCode[0] = BCDoverflowValue;
    return CurrentBCDvalue;
  }  
}

uint32_t BCDopsClass::BCDmod(uint32_t CurrentBCDvalue, uint32_t value, uint8_t *ErrorCode) {
  ErrorCode[0] = NoBCDerror;
  BCDtoBinary(CurrentBCDvalue, ErrorCode);
  if (ErrorCode[0] == NotBCDvalue) {
    return CurrentBCDvalue;
  }
  uint32_t ValueToReturn = BCDtoBinary(CurrentBCDvalue, ErrorCode);
  if (ErrorCode[0] == BCDoverflowValue) {
    return CurrentBCDvalue;
  }
  ValueToReturn %= value;
  if (ValueToReturn <= 99999999UL) {
    return BinaryToBCD(ValueToReturn, ErrorCode);
  }
  else {
    ErrorCode[0] = BCDoverflowValue;
    return CurrentBCDvalue;
  }  
}

BCDopsClass BCDops;