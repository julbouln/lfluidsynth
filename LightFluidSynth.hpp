#ifndef LightFluidSynth_H
#define LightFluidSynth_H

#include "lfluidsynth.h"

//#define POLYPHONY 24 // standard GM
#define POLYPHONY 64
//#define POLYPHONY 64
//#define SAMPLE_RATE 48000
#define SAMPLE_RATE 44100


class LightFluidSynth {
  fluid_synth_t* synth;
  fluid_settings_t* settings;
  int sfont_id;
public:
  LightFluidSynth() {

    /* Create the settings. */
    settings = new_fluid_settings();

    /* Change the settings if necessary*/
    fluid_settings_setnum(settings, "synth.sample-rate", SAMPLE_RATE);

//  fluid_settings_setstr(settings, "audio.period-size", "1024");

    fluid_settings_setstr(settings, "synth.reverb.active", "no");
    fluid_settings_setstr(settings, "synth.chorus.active", "no");
    fluid_settings_setint(settings, "synth.polyphony", POLYPHONY);

//    fluid_settings_setstr(settings, "synth.verbose", "yes");

    /* Create the synthesizer. */
    synth = new_fluid_synth(settings);

//    fluid_synth_set_interp_method(synth, -1, FLUID_INTERP_LINEAR);
    fluid_synth_set_interp_method(synth, -1, FLUID_INTERP_NONE);

  }


  ~LightFluidSynth() {
    fluid_synth_sfunload(synth,sfont_id,1);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
  }

  void loadSF2(char *filename)
  {
    sfont_id = fluid_synth_sfload(synth, filename, 1);

  }

  void noteOff(int chan, int n) {
    fluid_synth_noteoff(synth, chan, n);
  }

  void noteOn(int chan, int n, int v) {
    fluid_synth_noteon(synth, chan, n, v);
  }

  void controlChange(int chan, int b1, int b2) {
    fluid_synth_cc(synth, chan, b1, b2);
  }

  void programChange(int chan, int p) {
    fluid_synth_program_change(synth, chan, p);
  }

  void pitchBend(int chan, int b) {
    fluid_synth_pitch_bend(synth, chan, b);
  }

  void channelPresure(int chan, int b) {
      fluid_synth_channel_pressure(synth, chan, b);
  }

  void writeStereoFloat(float *buf, int n) {
    fluid_synth_write_float(synth, n, buf, 0, 2, buf, 1, 2);

  }

  void writeStereoS16(int16_t *buf, int n) {
    fluid_synth_write_s16(synth, n, (int16_t *)buf, 0, 2, (int16_t *)buf, 1, 2);

  }
  void writeStereoS32(int32_t *buf, int n) {
    fluid_synth_write_s32(synth, n, (int32_t *)buf, 0, 2, (int32_t *)buf, 1, 2);
  }


};

#endif