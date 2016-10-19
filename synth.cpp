/* complete linux synthesizer using RtAudio and RtMidi */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>

#include "LightFluidSynth.hpp"

#include "RtAudio.h"
#include "RtMidi.h"

//#define AUDIO_FORMAT RTAUDIO_FLOAT32
#define AUDIO_FORMAT RTAUDIO_SINT16

#define FRAME_SIZE 128

using namespace std;

void midiCallback(double deltatime, vector<uint8_t>* msg, void* userData)
{
  LightFluidSynth *synth = (LightFluidSynth *)userData;

  int chan = (*msg)[0] & 0xf;
  int msgtype = (*msg)[0] & 0xf0;
  int b1 =  (*msg)[1];
  int b2 =  (*msg)[2];
  int b = ((b2 & 0x7f) << 7) | (b1 & 0x7f);

//  printf("%f ", deltatime);

  switch (msgtype) {
  case 0x80:
//    std::cout << "MIDI Note OFF  C: " << chan << " N: " << b1 << std::endl;
    synth->noteOff(chan, b1);
    break;
  case 0x90:
//    std::cout << "MIDI Note ON   C: " << chan << " N: " << b1 << " V: " << b2 << std::endl;
    synth->noteOn(chan, b1, b2);
    break;
  case 0xB0:
//    std::cout << "MIDI Control change   C: " << chan << " B1: " << b1 << " B2: " << b2 << std::endl;
    synth->controlChange(chan, b1, b2);
    break;
  case 0xC0:
//    std::cout << "MIDI Program change   C: " << chan << " P: " << b1 << std::endl;
    synth->programChange(chan, b1);
    break;
  case 0xD0:
//    std::cout << "MIDI Channel presure   C: " << chan << " P: " << b1 << std::endl;
    synth->channelPresure(chan, b1);
    break;
  case 0xE0:
//    std::cout << "MIDI Pitch Bend   C: " << chan << " P: " << b << std::endl;
    synth->pitchBend(chan, b);
    break;
  default:
    std::cout << "MIDI msg   C: " << chan << " B1: " << b1 << " B2: " << b2 << std::endl;
    break;

  }
}

int audioCallback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                   double streamTime, RtAudioStreamStatus status, void *userData )
{
  LightFluidSynth *synth = (LightFluidSynth *)userData;

  switch (AUDIO_FORMAT) {
  case RTAUDIO_FLOAT32:
  {
    float *buf = (float *)outputBuffer;
    synth->writeStereoFloat(buf, nBufferFrames);

  }
  break;
  case RTAUDIO_SINT16:
  {
    int16_t *buf = (int16_t *)outputBuffer;
    synth->writeStereoS16((short *)buf, nBufferFrames);
  }
  break;
  case RTAUDIO_SINT32:
  {
    int32_t *buf = (int32_t *)outputBuffer;
    synth->writeStereoS32((int *)buf, nBufferFrames);

  }
  break;
  }

  return 0;
}

int main(int argc, char** argv)
{

  if (argc != 2) {
    printf("Usage: synth file.sf2\n");
    exit(0);
  }

  LightFluidSynth *usynth;

  usynth = new LightFluidSynth();

  usynth->loadSF2(argv[1]);
//  usynth->loadSF2("tim.sf2");

  RtMidiIn *midiIn = new RtMidiIn();
  if (midiIn->getPortCount() == 0) {
    std::cout << "No MIDI ports available!\n";
  }
  midiIn->openPort(0);
  midiIn->setCallback( &midiCallback, (void *)usynth );
  midiIn->ignoreTypes( false, false, false );

//   RtAudio dac(RtAudio::LINUX_PULSE);
  RtAudio dac;
  RtAudio::StreamParameters rtParams;

  // Determine the number of devices available
  unsigned int devices = dac.getDeviceCount();
  // Scan through devices for various capabilities
  RtAudio::DeviceInfo info;
  for ( unsigned int i = 0; i < devices; i++ ) {
    info = dac.getDeviceInfo( i );
    if ( info.probed == true ) {
      std::cout << "device " << " = " << info.name;
      std::cout << ": maximum output channels = " << info.outputChannels << "\n";
    }
  }
//  rtParams.deviceId = 3;
  rtParams.deviceId = dac.getDefaultOutputDevice();
  rtParams.nChannels = 2;
  unsigned int bufferFrames = FRAME_SIZE;

  RtAudio::StreamOptions options;
  options.flags = RTAUDIO_SCHEDULE_REALTIME;

  dac.openStream( &rtParams, NULL, AUDIO_FORMAT, SAMPLE_RATE, &bufferFrames, &audioCallback, (void *)usynth, &options );
  dac.startStream();

  printf("\n\nPress Enter to stop\n\n");
  cin.get();
  dac.stopStream();

  delete(usynth);
  return 0;
}