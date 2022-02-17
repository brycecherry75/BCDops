/*

  BCDops demo by Bryce Cherry

  Serial command format:
  BCD_value_in_hexadecimal operation(+-/*%) value_to_manipulate

*/

#include <BCDops.h>

// ensures that the serial port is flushed fully on request
const unsigned long SerialPortRate = 9600;
const byte SerialPortRateTolerance = 5; // +percent - increase to 50 for rates above 115200 up to 4000000
const byte SerialPortBits = 10; // start (1), data (8), stop (1)
const unsigned long TimePerByte = ((((1000000ULL * SerialPortBits) / SerialPortRate) * (100 + SerialPortRateTolerance)) / 100); // calculated on serial port rate + tolerance and rounded down to the nearest uS, long caters for even the slowest serial port of 75 bps

const word commandSize = 50;
char command[commandSize];
const byte FieldSize = 30;

void FlushSerialBuffer() {
  while (true) {
    if (Serial.available() > 0) {
      byte dummy = Serial.read();
      while (Serial.available() > 0) { // flush additional bytes from serial buffer if present
        dummy = Serial.read();
      }
      if (TimePerByte <= 16383) {
        delayMicroseconds(TimePerByte); // delay in case another byte may be received via the serial port
      }
      else { // deal with delayMicroseconds limitation
        unsigned long DelayTime = TimePerByte;
        DelayTime /= 1000;
        if (DelayTime > 0) {
          delay(DelayTime);
        }
        DelayTime = TimePerByte;
        DelayTime %= 1000;
        if (DelayTime > 0) {
          delayMicroseconds(DelayTime);
        }
      }
    }
    else {
      break;
    }
  }
}

void getField (char* buffer, int index) {
  int commandPos = 0;
  int fieldPos = 0;
  int spaceCount = 0;
  while (commandPos < commandSize) {
    if (command[commandPos] == 0x20) {
      spaceCount++;
      commandPos++;
    }
    else if (command[commandPos] == 0x0D || command[commandPos] == 0x0A) {
      break;
    }
    if (spaceCount == index) {
      buffer[fieldPos] = command[commandPos];
      fieldPos++;
    }
    commandPos++;
  }
  for (int ch = 0; ch < strlen(buffer); ch++) { // correct case of command
    buffer[ch] = toupper(buffer[ch]);
  }
  buffer[fieldPos] = '\0';
}

void PrintStatus(byte value) {
  switch (value) {
    case NoBCDerror:
      Serial.println(F("OK"));
      break;
    case NotBCDvalue:
      Serial.println(F("BCD value does not contain 0-9 digits"));
      break;
    case BCDoutOfRange:
      Serial.println(F("Binary to BCD result is out of range"));
      break;
    case BCDunderflowValue:
      Serial.println(F("BCD manipulation underflow"));
      break;
    case BCDoverflowValue:
      Serial.println(F("BCD manipulation overflow"));
      break;
    case BCDdivideByZero:
      Serial.println(F("Attempt to divide by zero"));
      break;
  }
}

void setup() {
  Serial.begin(SerialPortRate);
}

void loop() {
  static int ByteCount = 0;
  if (Serial.available() > 0) {
    char inData = Serial.read();
    if (inData != '\n' && ByteCount < commandSize) {
      command[ByteCount] = inData;
      ByteCount++;
    }
    else {
      byte ErrorCode[1];
      ByteCount = 0;
      char field[FieldSize];
      getField(field, 0);
      unsigned long BCDvalue = strtoul(field, 0, 16);
      getField(field, 2);
      unsigned long BCDmanipulator = strtoul(field, 0, 10);
      BCDops.CheckBCD(BCDvalue, ErrorCode);
      if (ErrorCode[0] == NoBCDerror) {
        Serial.println(F("BCD check OK"));
      }
      unsigned long temp;
      temp = BCDops.BCDtoBinary(BCDvalue, ErrorCode);
      Serial.print(F("BCD input value in binary is "));
      Serial.print(temp, DEC);
      Serial.print(F(" "));
      PrintStatus(ErrorCode[0]);
      if (ErrorCode[0] == NoBCDerror) {
        temp = BCDops.BinaryToBCD(BCDmanipulator, ErrorCode);
        Serial.print(F("Binary manipulation value in BCD is "));
        Serial.print(temp, HEX);
        Serial.print(F(" "));
        PrintStatus(ErrorCode[0]);
        if (ErrorCode[0] == NoBCDerror) {
          bool ValidField = true;
          getField(field, 1);
          if (strcmp(field, "+") == 0) {
            BCDvalue = BCDops.BCDadd(BCDvalue, BCDmanipulator, ErrorCode);
          }
          else if (strcmp(field, "-") == 0) {
            BCDvalue = BCDops.BCDsubtract(BCDvalue, BCDmanipulator, ErrorCode);
          }
          else if (strcmp(field, "*") == 0) {
            BCDvalue = BCDops.BCDmultiply(BCDvalue, BCDmanipulator, ErrorCode);
          }
          else if (strcmp(field, "/") == 0) {
            BCDvalue = BCDops.BCDdivide(BCDvalue, BCDmanipulator, ErrorCode);
          }
          else if (strcmp(field, "%") == 0) {
            BCDvalue = BCDops.BCDmod(BCDvalue, BCDmanipulator, ErrorCode);
          }
          else {
            ValidField = false;
            Serial.println(F("Invalid operator"));
          }
          if (ValidField == true) {
            Serial.print(F("BCD result is "));
            Serial.print(BCDvalue, HEX);
            Serial.print(F(" "));
            PrintStatus(ErrorCode[0]);
          }
        }
      }
      FlushSerialBuffer();
    }
  }
}