// Drain modulator code

#include "RP2040_PWM.h"

//creates pwm instance
RP2040_PWM* PWM_Instance;

float frequency;
float dutyCycle;

#define pinToUse 16

#define AUDIO_INPUT_PIN A0    // Analog input pin for audio signal
// #define PWM_OUTPUT_PIN 9      // PWM output pin (must be PWM-capable)

const int SAMPLE_WINDOW = 50; // Sample window in ms (20 Hz)
const int PWM_FREQ = 31250;   // PWM frequency in Hz
const int PWM_RANGE = 255;    // PWM range (8-bit resolution)

// Raspberry Pi Pico
const int PWM_PIN = 16;
const int PWM_RESOLUTION = 8;  // PWM resolution in bits

unsigned long lastSample = 0;
int minAudioLevel = 1023;
int maxAudioLevel = 0;

void setup() {
  pinMode(AUDIO_INPUT_PIN, INPUT);
  // pinMode(PWM_OUTPUT_PIN, OUTPUT);

  // Initialize PWM
  // analogWriteFreq(PWM_FREQ);
  // analogWriteRange(1 << PWM_RESOLUTION);

  // Set up the PWM pin
  // pinMode(PWM_PIN, OUTPUT);

  // Start the PWM signal at 50% duty cycle
  // analogWrite(PWM_PIN, (1 << PWM_RESOLUTION) / 2);

  // Set PWM frequency (31250 Hz on pins 9 and 10)
  // TCCR1B = (TCCR1B & 0b11111000) | 0x01; // For Arduino Nano and similar

  PWM_Instance = new RP2040_PWM(pinToUse, PWM_FREQ, 50);
  frequency = PWM_FREQ;
  dutyCycle = 50;
  PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
  Serial.begin(115200);  // For debugging
}

void loop() {
  unsigned long currentMillis = millis();

  // Sample the audio input
  if (currentMillis - lastSample >= SAMPLE_WINDOW) {
    int pwmValue = sampleAudio();
    // analogWrite(PWM_PIN, pwmValue);

    // https://github.com/khoih-prog/RP2040_PWM/blob/main/examples/PWM_Basic/PWM_Basic.ino
    PWM_Instance->setPWM(pinToUse, frequency, pwmValue);

    // Print the PWM value for debugging
    // Serial.println(pwmValue);

    lastSample = currentMillis;
  }
}

int sampleAudio() {
  unsigned long startMillis = millis();
  int signalMax = 0;
  int signalMin = 1023;

  // Collect samples for SAMPLE_WINDOW milliseconds
  while (millis() - startMillis < SAMPLE_WINDOW) {
    int sample = analogRead(AUDIO_INPUT_PIN);
    if (sample > signalMax) {
      signalMax = sample;
    } else if (sample < signalMin) {
      signalMin = sample;
    }
  }

  int sample = analogRead(AUDIO_INPUT_PIN);
  Serial.println(sample);

  // Calculate peak-to-peak amplitude
  /* int peakToPeak = signalMax - signalMin;

    // Update global min/max levels (for automatic gain control)
    if (peakToPeak > maxAudioLevel) maxAudioLevel = peakToPeak;
    if (peakToPeak < minAudioLevel) minAudioLevel = peakToPeak; */

  // Map the amplitude to PWM range with some headroom
  // int pwmValue = map(peakToPeak, minAudioLevel, maxAudioLevel, 0, PWM_RANGE * 0.8);
  // pwmValue = constrain(pwmValue, 0, PWM_RANGE);

  int pwmValue = map(sample, 0, 1023, 0, PWM_RANGE * 0.8);
  pwmValue = constrain(pwmValue, 0, PWM_RANGE);

  // Invert PWM value for P-channel MOSFET
  // return PWM_RANGE - pwmValue;

  // We have a 2N3904 in front of P-channel MOSFET, so no PWM inversion is
  // needed!
  return pwmValue;
}
