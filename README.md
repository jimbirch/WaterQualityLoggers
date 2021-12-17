# WaterQualityLoggers
Code for a series of simple atmega-based water quality dataloggers
Most code, with the exception of real time data logging code can be run on an off-the-shelf Atmega168- or Atmega328p-based Arduino. Timing may need to be adjusted. All code is tested directly on an Atmega328p running at 1 mHz on the internal 8 mHz oscillator, real time logging is accomplished with a 32.768 kHz oscillator connected to XTAL pins.
Current capabilities include:
1. Temperature using two ADC pins
2. Conductivity using three ADC pins
3. Turbidity (in development)

Stay tuned for circuit descriptions.
