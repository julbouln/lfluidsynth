/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *  
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */

#ifndef _FLUID_EVENT_H
#define _FLUID_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fluid_sys.h"

/* Private data for event */
/* ?? should be optimized in size, using unions */
struct _fluid_event_t {
  uint32_t time;
  int type;
  int16_t src;
  int16_t dest;
  int channel;
  int16_t key;
  int16_t vel;
  int16_t control;
  int16_t value;
  int16_t id; //?? unused ?
  int pitch;
  uint32_t duration;
  void* data;
};

unsigned int fluid_event_get_time(fluid_event_t* evt);
void fluid_event_set_time(fluid_event_t* evt, unsigned int time);

/* private data for sorter + heap */
enum fluid_evt_entry_type {
  FLUID_EVT_ENTRY_INSERT = 0,
  FLUID_EVT_ENTRY_REMOVE
};

typedef struct _fluid_evt_entry fluid_evt_entry;
struct _fluid_evt_entry {
  fluid_evt_entry *next;
  int16_t entryType;
  fluid_event_t evt;
};

#define HEAP_WITH_DYNALLOC 1
/* #undef HEAP_WITH_DYNALLOC */

typedef struct _fluid_evt_heap_t {
#ifdef HEAP_WITH_DYNALLOC
  fluid_evt_entry* freelist;
  fluid_mutex_t mutex;
#else
  fluid_evt_entry* head;
  fluid_evt_entry* tail;
  fluid_evt_entry pool;
#endif
} fluid_evt_heap_t;

fluid_evt_heap_t* _fluid_evt_heap_init(int nbEvents);
void _fluid_evt_heap_free(fluid_evt_heap_t* heap);
fluid_evt_entry* _fluid_seq_heap_get_free(fluid_evt_heap_t* heap);
void _fluid_seq_heap_set_free(fluid_evt_heap_t* heap, fluid_evt_entry* evt);

/**
 * Sequencer event type enumeration.
 */
enum fluid_seq_event_type {
  FLUID_SEQ_NOTE = 0,		/**< Note event (DOCME) */
  FLUID_SEQ_NOTEON,		/**< Note on event */
  FLUID_SEQ_NOTEOFF,		/**< Note off event */
  FLUID_SEQ_ALLSOUNDSOFF,	/**< All sounds off event */
  FLUID_SEQ_ALLNOTESOFF,	/**< All notes off event */
  FLUID_SEQ_BANKSELECT,		/**< Bank select message */
  FLUID_SEQ_PROGRAMCHANGE,	/**< Program change message */
  FLUID_SEQ_PROGRAMSELECT,	/**< Program select message (DOCME) */
  FLUID_SEQ_PITCHBEND,		/**< Pitch bend message */
  FLUID_SEQ_PITCHWHHELSENS,	/**< Pitch wheel sensitivity set message */
  FLUID_SEQ_MODULATION,		/**< Modulation controller event */
  FLUID_SEQ_SUSTAIN,		/**< Sustain controller event */
  FLUID_SEQ_CONTROLCHANGE,	/**< MIDI control change event */
  FLUID_SEQ_PAN,		/**< Stereo pan set event */
  FLUID_SEQ_VOLUME,		/**< Volume set event */
  FLUID_SEQ_REVERBSEND,		/**< Reverb send set event */
  FLUID_SEQ_CHORUSSEND,		/**< Chorus send set event */
  FLUID_SEQ_TIMER,		/**< Timer event (DOCME) */
  FLUID_SEQ_ANYCONTROLCHANGE,	/**< DOCME (used for remove_events only) */
  FLUID_SEQ_LASTEVENT		/**< Defines the count of event enums */
};

/* Event alloc/free */
fluid_event_t* new_fluid_event(void);
void delete_fluid_event(fluid_event_t* evt);

/* Initializing events */
void fluid_event_set_source(fluid_event_t* evt, short src);
void fluid_event_set_dest(fluid_event_t* evt, short dest);

/* Timer events */
void fluid_event_timer(fluid_event_t* evt, void* data);

/* Note events */
void fluid_event_note(fluid_event_t* evt, int channel, 
				   short key, short vel, 
				   unsigned int duration);

void fluid_event_noteon(fluid_event_t* evt, int channel, short key, short vel);
void fluid_event_noteoff(fluid_event_t* evt, int channel, short key);
void fluid_event_all_sounds_off(fluid_event_t* evt, int channel);
void fluid_event_all_notes_off(fluid_event_t* evt, int channel);

/* Instrument selection */
void fluid_event_bank_select(fluid_event_t* evt, int channel, short bank_num);
void fluid_event_program_change(fluid_event_t* evt, int channel, short preset_num);
void fluid_event_program_select(fluid_event_t* evt, int channel, unsigned int sfont_id, short bank_num, short preset_num);

/* Real-time generic instrument controllers */

void fluid_event_control_change(fluid_event_t* evt, int channel, short control, short val);

/* Real-time instrument controllers shortcuts */
void fluid_event_pitch_bend(fluid_event_t* evt, int channel, int val);
void fluid_event_pitch_wheelsens(fluid_event_t* evt, int channel, short val);
void fluid_event_modulation(fluid_event_t* evt, int channel, short val);
void fluid_event_sustain(fluid_event_t* evt, int channel, short val);
void fluid_event_pan(fluid_event_t* evt, int channel, short val);
void fluid_event_volume(fluid_event_t* evt, int channel, short val);
void fluid_event_reverb_send(fluid_event_t* evt, int channel, short val);
void fluid_event_chorus_send(fluid_event_t* evt, int channel, short val);

/* Only for removing events */
void fluid_event_any_control_change(fluid_event_t* evt, int channel);

/* Accessing event data */
int fluid_event_get_type(fluid_event_t* evt);
short fluid_event_get_source(fluid_event_t* evt);
short fluid_event_get_dest(fluid_event_t* evt);
int fluid_event_get_channel(fluid_event_t* evt);
short fluid_event_get_key(fluid_event_t* evt);
short fluid_event_get_velocity(fluid_event_t* evt);
short fluid_event_get_control(fluid_event_t* evt);
short fluid_event_get_value(fluid_event_t* evt);
short fluid_event_get_program(fluid_event_t* evt);
void* fluid_event_get_data(fluid_event_t* evt);
unsigned int fluid_event_get_duration(fluid_event_t* evt);
short fluid_event_get_bank(fluid_event_t* evt);
int fluid_event_get_pitch(fluid_event_t* evt);
unsigned int fluid_event_get_sfont_id(fluid_event_t* evt);

#ifdef __cplusplus
}
#endif
#endif /* _FLUIDSYNTH_EVENT_H */
