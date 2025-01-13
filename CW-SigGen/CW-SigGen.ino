// https://github.com/ta2bgh/cw-beacon/blob/main/cw-beacon.ino

#include "si5351.h"

Si5351 si5351;

uint16_t duration = 85;  // Morse code typing speed in milliseconds - higher number means slower typing
uint64_t target_freq = 28023000ULL * 100ULL;
String cw_message = "VVV CQ CQ CQ DE VU3FOE";

void setup() {
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 16999);
  si5351.output_enable(SI5351_CLK0, 0);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(10000);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  si5351.set_freq(target_freq, SI5351_CLK0);
  cw_string_proc(cw_message);
  cw(false);
  digitalWrite(LED_BUILTIN, HIGH);
  si5351.output_enable(SI5351_CLK0, 0);
  delay(5000);
}

void cw_string_proc(String str) {  // Processing string to characters
  for (uint8_t j = 0; j < str.length(); j++)
    cw_char_proc(str[j]);
}

void cw_char_proc(char m) {  // Processing characters to Morse symbols
  String s;

  if (m == ' ') {  // Pause between words
    word_space();
    return;
  }

  if (m > 96)  // ACSII, case change a-z to A-Z
    if (m < 123)
      m -= 32;

  switch (m) {  // Morse
    case 'A': s = ".-#"; break;
    case 'B': s = "-...#"; break;
    case 'C': s = "-.-.#"; break;
    case 'D': s = "-..#"; break;
    case 'E': s = ".#"; break;
    case 'F': s = "..-.#"; break;
    case 'G': s = "--.#"; break;
    case 'H': s = "....#"; break;
    case 'I': s = "..#"; break;
    case 'J': s = ".---#"; break;
    case 'K': s = "-.-#"; break;
    case 'L': s = ".-..#"; break;
    case 'M': s = "--#"; break;
    case 'N': s = "-.#"; break;
    case 'O': s = "---#"; break;
    case 'P': s = ".--.#"; break;
    case 'Q': s = "--.-#"; break;
    case 'R': s = ".-.#"; break;
    case 'S': s = "...#"; break;
    case 'T': s = "-#"; break;
    case 'U': s = "..-#"; break;
    case 'V': s = "...-#"; break;
    case 'W': s = ".--#"; break;
    case 'X': s = "-..-#"; break;
    case 'Y': s = "-.--#"; break;
    case 'Z': s = "--..#"; break;

    case '1': s = ".----#"; break;
    case '2': s = "..---#"; break;
    case '3': s = "...--#"; break;
    case '4': s = "....-#"; break;
    case '5': s = ".....#"; break;
    case '6': s = "-....#"; break;
    case '7': s = "--...#"; break;
    case '8': s = "---..#"; break;
    case '9': s = "----.#"; break;
    case '0': s = "-----#"; break;

    case '?': s = "..--..#"; break;
    case '=': s = "-...-#"; break;
    case ',': s = "--..--#"; break;
    case '/': s = "-..-.#"; break;
  }

  for (uint8_t i = 0; i < 7; i++) {
    switch (s[i]) {
      case '.': di(); break;           // di
      case '-': dah(); break;          // dah
      case '#': char_space(); return;  // end of morse symbol
    }
  }
}

void di() {
  cw(true);  // TX di
  delay(duration);

  cw(false);  // stop TX di
  delay(duration);
}

void dah() {
  cw(true);  // TX dah
  delay(3 * duration);

  cw(false);  // stop TX dah
  delay(duration);
}

void char_space() {     // 3x, pause between letters
  delay(2 * duration);  // 1x from end of character + 2x from the beginning of new character
}

void word_space() {     // 7x, pause between words
  delay(6 * duration);  // 1x from end of the word + 6x from the beginning of new word
}

void cw(bool state) {  // TX-CW, TX-LED, 750 Hz sound
  if (state) {
    si5351.set_freq(target_freq, SI5351_CLK0);
    si5351.output_enable(SI5351_CLK0, 1);
  } else {
    si5351.set_freq(target_freq, SI5351_CLK0);
    si5351.output_enable(SI5351_CLK0, 0);
  }
}
