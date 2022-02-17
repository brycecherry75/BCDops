# BCDops

v1.0.0 First release

## Introduction
This library performs Binary Coded Decimal (BCD) operations including arithmetric and conversion.

## Features
+ Conversion from binary/BCD (up to 8 digits)
+ Result will be unchanged on an error
+ All basic arithmetric (+ - * / %) can be used to manipulate a BCD value

## Library Use
Example programs using the library are provided in the source directory.

All values are uint32_t length except for *ErrorCode which is a 1 element uint8_t array.

CheckBCD(BCDvalue, *ErrorCode) - checks if BCDvalue contains valid BCD digits (0-9)

BinaryToBCD(BinaryValue, *ErrorCode) - convert a binary number to BCD and returns a uint32_t result

BCDtoBinary(CurrentBCDvalue, *ErrorCode) - convert a BCD value to a binary number and returns a uint32_t result

BCD(add/subtract/multiply/divide/mod)(CurrentBCDvalue, value, *ErrorCode) - perform addition/subtraction/multiplication/division/modulo on CurrentBCDvalue with a binary value and returns a uint32_t result

*ErrorCode values at Element 0: NoBCDerror/NotBCDvalue/BCDunderflowValue/BCDoverflowValue/BCDdivideByZero

## Installation
Copy the `src/` directory to your Arduino sketchbook directory (named the directory `BCDopsDemo`), and install the libraries in your Arduino library directory.  You can also install the BCDops files separatly as a library.