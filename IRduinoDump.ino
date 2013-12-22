/*
 * IRremote: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */

#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

struct IrCode
{
  int type;
  int nbits;
  unsigned long ex1;
  unsigned long ex2;
  unsigned long long value;
};

IrCode code;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dump(void *v) {
//  decode_results *results = (decode_results *)v
void dump(decode_results *results) {
  memset((void*)&code, 0, sizeof(IrCode));

  code.type = results->decode_type;

  if (results->decode_type == PANASONIC)
    code.ex1 = results->panasonicAddress;

  code.nbits = results->bits;
  code.value = results->value;

  Serial.print(code.type);
  Serial.write(',');
  Serial.print(code.nbits);
  Serial.write(',');
  Serial.print(code.ex1, HEX);
  Serial.write(',');
  Serial.print(code.ex2, HEX);
  Serial.write(',');
  Serial.print((unsigned long int)code.value, HEX);
  Serial.print((unsigned long int)(code.value >> 32), HEX);
}

void loop() {
  if (irrecv.decode(&results)) {
    dump(&results);
    irrecv.resume(); // Receive the next value
  }
}
