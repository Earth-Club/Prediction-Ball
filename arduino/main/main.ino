
// Library of Trigram.
// 1. roll 3 dices at the same time to generate 6 random numbers in [0,3).
// 2. convert those 6 numbers by applying f(x)=x%2 function to a group of yao, which is a trigram (0bABCDEF).
// 3. lookup the Five Element names by Earthly Branch of Yao (inner or outer).
// 4. lookup the Six Relative names by Eight Gong of Trigram.
// 5. lookup the 世爻/应爻 by Five Element of Trigram.
/*
We use an int32 to represent a well formated trigram as the following diagram in network bitroders:

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-----------+-----------+-----------+---------------------------+
|  six yao  | six qin   |  gan zhi  | five elem |r s           v|
+-----------+-----------+-----------+---------------------------+

*/

#define TRIGRAM int

TRIGRAM trigram_new() {
  return 1;
}

int trigram_get_bit_value(TRIGRAM trigram, int idx) {
  while(idx > 0) {
     trigram = trigram >> 1;
     idx--;
  }
  return trigram & 0x1;
}

int trigram_get_six_yao(TRIGRAM trigram, int idx) {
  if (idx < 0 || idx > 6) {
    return -1;
  }
  return trigram_get_bit_value(trigram, idx);
}

int trigram_get_six_relative(TRIGRAM trigram, int idx) {
  if (trigram >= 0) {
    return -1;
  }
  if (idx < 0 || idx > 6) {
    return -1;
  }
  return trigram_get_bit_value(trigram, idx + 6);
}

int trigram_get_five_element(TRIGRAM trigram, int idx) {
  if (trigram >= 0) {
    return -1;
  }
  if (idx < 0 || idx > 6) {
    return -1;
  }
  return trigram_get_bit_value(trigram, idx + 6*3);
}

int trigram_get__element(TRIGRAM trigram, int idx) {
  if (trigram >= 0) {
    return -1;
  }
  if (idx < 0 || idx > 6) {
    return -1;
  }
  return trigram_get_bit_value(trigram, idx + 6*3);
}


// Main.
#define DEBUG 1

void setup() {
  // enable console.
  if (DEBUG) {
    Serial.begin(9600);
    Serial.println("Main application is launching...");
  }
}


void loop() {

}
