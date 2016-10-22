FLUIDSYNTH_OBJS=$(patsubst %.c,%.o,$(wildcard src/*.c))
# fluidsynth/drivers/fluid_alsa.o fluidsynth/drivers/fluid_pulse.o
CFLAGS=-O3 -g -I. -Isrc -D_GNU_SOURCE
CFLAGS += -DFLUID_CALC_FORMAT_FLOAT -DFLUID_NEW_GEN_API 
CFLAGS += -DFLUID_NO_NAMES -DFLUID_ALTSFONT
CFLAGS += -DFLUID_SAMPLE_MMAP # TODO linux
#CFLAGS +=-DFLUID_SAMPLE_READ_DISK -DFLUID_SAMPLE_GC 
#CFLAGS += -DFLUID_SAMPLE_READ_CHUNK
CFLAGS += -DFLUID_NEW_VOICE_MOD_API
CFLAGS += -DFLUID_NEW_VOICE_GEN_API
CFLAGS += -DFLUID_NO_NRPN_EXT
#CFLAGS += -DFLUID_FIXED_POINT
#CFLAGS += -DFLUID_ARM_OPT
CFLAGS += -DFLUID_SIMPLE_IIR
CFLAGS += -DFLUID_AVX_OPT 
CFLAGS+=-march=native -mavx 
#CFLAGS+=-ftree-vectorize -ffast-math -fsingle-precision-constant
#SYNTH_CFLAGS=-Irt -D__LINUX_PULSE__ -D__LINUX_ALSA__ 


# for massif and callgrind
SF2_PROF_FILE=ct4mgm.sf2

all: $(FLUIDSYNTH_OBJS) rt/RtAudio.o rt/RtMidi.o
	g++ $(CFLAGS) $(SYNTH_CFLAGS) synth.cpp -o synth $^ -lc -lm -lpthread -lasound -lpulse -lpulse-simple
	objdump -sSt synth >synth.lst


rt/RtAudio.o:
	g++ -Irt -D__LINUX_PULSE__ -c -o rt/RtAudio.o rt/RtAudio.cpp

rt/RtMidi.o:
	g++ -Irt -D__LINUX_ALSA__ -c -o rt/RtMidi.o rt/RtMidi.cpp


timgm6mb.sf2:
	wget https://launchpad.net/ubuntu/+archive/primary/+files/timgm6mb-soundfont_1.3.orig.tar.gz
	tar xvzf timgm6mb-soundfont_1.3.orig.tar.gz
	mv timgm6mb-soundfont_1.3/TimGM6mb.sf2 timgm6mb.sf2
	rm -fr timgm6mb-soundfont_1.3
	rm -f timgm6mb-soundfont_1.3.orig.tar.gz

test: timgm6mb.sf2 $(FLUIDSYNTH_OBJS)
	gcc $(CFLAGS) test.c -o test $(FLUIDSYNTH_OBJS) -lc -lm

test_altsfont: $(FLUIDSYNTH_OBJS)
	gcc $(CFLAGS) test_altsfont.c -o test_altsfont $(FLUIDSYNTH_OBJS) -lc -lm

callgrind: synth
	valgrind --dsymutil=yes --tool=callgrind --dump-instr=yes --collect-jumps=yes ./synth $(SF2_PROF_FILE)

massif: synth
	valgrind --tool=massif  --heap-admin=1 --depth=50 --peak-inaccuracy=0.0 --detailed-freq=1 --threshold=0.0 --time-unit=B --massif-out-file=massif.out ./synth $(SF2_PROF_FILE)
	ms_print massif.out > massif.log

test_massif: test
	valgrind --tool=massif  --heap-admin=1 --depth=50 --peak-inaccuracy=0.0 --detailed-freq=1 --threshold=0.0 --time-unit=B --massif-out-file=massif.out ./test
	ms_print massif.out > massif.log

test_altsfont_massif: test_altsfont
	valgrind --tool=massif  --heap-admin=1 --depth=50 --peak-inaccuracy=0.0 --detailed-freq=1 --threshold=0.0 --time-unit=B --massif-out-file=massif.out ./test_altsfont
	ms_print massif.out > massif.log

clean:
	rm -f src/*.o
	rm -f rt/*.o
	rm -f massif.*
#	rm -f callgrind.*
	rm -f synth
	rm -f test
	rm -f *.lst