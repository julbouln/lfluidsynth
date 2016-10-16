/* simple test */
#include "lfluidsynth.h"
#include "fluid_defsfont.h"
#include "fluid_gen.h"
#include "fluid_list.h"


int main() {
	fluid_synth_t* synth;
	fluid_settings_t* settings;
	int sfont_id;


  printf("sizeof(fluid_inst_zone_t) %d\n",sizeof(fluid_inst_zone_t));
  printf("sizeof(fluid_sample_t) %d\n",sizeof(fluid_sample_t));
  printf("sizeof(fluid_gen_t) %d\n",sizeof(fluid_gen_t));
  printf("sizeof(fluid_mod_t) %d\n",sizeof(fluid_mod_t));
  printf("sizeof(fluid_list_t) %d\n",sizeof(fluid_list_t));
  printf("GEN_LAST (%d) * sizeof(fluid_gen_t) %d\n",GEN_LAST,GEN_LAST*sizeof(fluid_gen_t));
  printf("FLUID_NUM_MOD (%d) * sizeof(fluid_mod_t) %d\n",FLUID_NUM_MOD,FLUID_NUM_MOD*sizeof(fluid_mod_t));
  printf("sizeof(fluid_mod_t) %d\n",sizeof(fluid_mod_t));
  printf("sizeof(fluid_voice_t) %d\n",sizeof(fluid_voice_t));
  printf("sizeof(fluid_synth_t) %d\n",sizeof(fluid_synth_t));
//  printf("sizeof(fluid_chorus_t) %d\n",sizeof(fluid_chorus_t));
//  printf("sizeof(fluid_rev_t) %d\n",sizeof(fluid_rev_t));

	/* Create the settings. */
	settings = new_fluid_settings();

//	fluid_settings_setstr(settings, "synth.reverb.active", "no");
//    fluid_settings_setstr(settings, "synth.chorus.active", "no");
    fluid_settings_setint(settings, "synth.polyphony", 64);

	/* Create the synthesizer. */
	synth = new_fluid_synth(settings);

	sfont_id = fluid_synth_sfload(synth, "merlin.sf2", 1);
	fluid_synth_set_interp_method(synth, -1, FLUID_INTERP_NONE);


	fluid_synth_noteon(synth, 1, 60, 127);
	printf("NOTE ON\n");

	fluid_synth_noteoff(synth, 1, 60);
	printf("NOTE OFF\n");

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

	fluid_synth_sfunload(synth, sfont_id, 1);
	delete_fluid_synth(synth);
	delete_fluid_settings(settings);
	return 0;
}