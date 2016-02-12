FLUIDSYNTH_OBJS=$(patsubst %.c,%.o,$(wildcard src/*.c))
# fluidsynth/drivers/fluid_alsa.o fluidsynth/drivers/fluid_pulse.o
CFLAGS=-O3 -g -I. -Isrc
CFLAGS+= -DFLUID_SAMPLE_FORMAT_FLOAT -DFLUID_SAMPLE_READ_DISK -DFLUID_SAMPLE_GC -DFLUID_SAMPLE_READ_CHUNK -DFLUID_NEW_GEN_API -DFLUID_NEW_VOICE_MOD_API
#CFLAGS      += -DFLUID_ARM_OPT 
#CFLAGS+=-march=native
#CFLAGS+=-ftree-vectorize -ffast-math -fsingle-precision-constant
SYNTH_CFLAGS=-Irt -D__LINUX_ALSA__ 

# for massif and callgrind
SF2_PROF_FILE=merlin.sf2

all: $(FLUIDSYNTH_OBJS)
	g++ $(CFLAGS) $(SYNTH_CFLAGS) rt/RtMidi.cpp rt/RtAudio.cpp synth.cpp -o synth $^ -lc -lm -lpthread -lasound
	objdump -St synth >synth.lst

timgm6mb.sf2:
	wget https://launchpad.net/ubuntu/+archive/primary/+files/timgm6mb-soundfont_1.3.orig.tar.gz
	tar xvzf timgm6mb-soundfont_1.3.orig.tar.gz
	mv timgm6mb-soundfont_1.3/TimGM6mb.sf2 timgm6mb.sf2
	rm -fr timgm6mb-soundfont_1.3
	rm -f timgm6mb-soundfont_1.3.orig.tar.gz

test: timgm6mb.sf2 $(FLUIDSYNTH_OBJS)
	gcc $(CFLAGS) test.c -o test $(FLUIDSYNTH_OBJS) -lc -lm

callgrind: synth
	pasuspender -- valgrind --dsymutil=yes --tool=callgrind --dump-instr=yes --collect-jumps=yes ./synth $(SF2_PROF_FILE)

massif: synth
	pasuspender -- valgrind --tool=massif  --heap-admin=1 --depth=50 --peak-inaccuracy=0.0 --detailed-freq=1 --threshold=0.0 --time-unit=B --massif-out-file=massif.out ./synth $(SF2_PROF_FILE)
	ms_print massif.out > massif.log

clean:
	rm -f src/*.o
	rm -f massif.*
#	rm -f callgrind.*
	rm -f synth
	rm -f test
	rm -f *.lst