/* simple test */
#include "lfluidsynth.h"

int main() {
	fluid_synth_t* synth;
	fluid_settings_t* settings;
	int sfont_id;

	/* Create the settings. */
	settings = new_fluid_settings();

	/* Create the synthesizer. */
	synth = new_fluid_synth(settings);

	sfont_id = fluid_synth_sfload(synth, "timgm6mb.sf2", 1);
	fluid_synth_set_interp_method(synth, -1, FLUID_INTERP_LINEAR);
/*
	fluid_synth_noteon(synth, 1, 60, 127);
	printf("NOTE ON\n");
	fluid_synth_noteoff(synth, 1, 60);
	printf("NOTE OFF\n");

	int i;
	float buf[1024];
	for(i=0;i<10;i++) {

		int n = fluid_synth_write_float(synth, 512, buf, 0, 2, buf, 1, 2);
		printf("WRITE BUFFER\n");

	}
*/
	fluid_synth_sfunload(synth, sfont_id, 1);
	delete_fluid_synth(synth);
	delete_fluid_settings(settings);

}