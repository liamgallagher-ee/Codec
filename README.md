# Codec Template
---
This is a code template, setting up the Hatysa DSP platform and initializing a STM32G431RBTx NUCLEO-64 board to work with it.

This board uses the TLV320AIC3104 stereo in/stereo out audio codec, along with output driving hardware capable of driving both line out and HP out, set by the output jumpers. Audio jacks are TRS 3.5mm standard input and output, and three potentiometers are present to provide 3 user-controlled parameters.

Configuration of the codec is done via I2C, and the main audio stream uses 96kHz audio, but this system sets up the audio as 16-bit bit depth.
 