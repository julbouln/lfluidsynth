FLUIDSYNTH_OBJS=$(patsubst %.c,%.o,$(wildcard src/*.c))
# fluidsynth/drivers/fluid_alsa.o fluidsynth/drivers/fluid_pulse.o
CFLAGS=-O3 -DFLUID_SAMPLE_FORMAT_FLOAT -DFLUID_SAMPLE_READ_DISK -DFLUID_SAMPLE_GC -DFLUID_SAMPLE_READ_CHUNK -I. -Isrc
SYNTH_CFLAGS=-Irt -D__LINUX_ALSA__ 
#-D__LINUX_PULSE__ 
# -DALSA_SUPPORT -DPULSE_SUPPORT 
#-DPULSE_SUPPORT -DFLUID_ENABLE_THREAD

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
	pasuspender -- valgrind --dsymutil=yes --tool=callgrind ./synth merlin.sf2

massif: synth
	pasuspender -- valgrind --tool=massif  --heap-admin=1 --depth=50 --peak-inaccuracy=0.0 --detailed-freq=1 --threshold=0.0 --time-unit=B --massif-out-file=massif.out ./synth merlin.sf2
	ms_print massif.out > massif.log

clean:
	rm -f src/*.o
	rm -f massif.*
#	rm -f callgrind.*
	rm -f synth
	rm -f test
	rm -f *.lst