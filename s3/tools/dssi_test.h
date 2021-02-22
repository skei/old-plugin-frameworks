
//----------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <alsa/seq_event.h>

#include "s3.h"

//----------------------------------------------------------------------

#define NUM_ALL_PORTS		  2
#define PORTNUM_LEFT_OUT	0
#define PORTNUM_RIGHT_OUT 1
#define LADSPA_ID_NUM     666
#define PLUGIN_STR_NAME   "Dssi Test"
#define PLUGIN_CLASS_NAME "DssiTest"

//----------------------------------------------------------------------

#define printf S3_DTrace

//----------------------------------------------------------------------

typedef struct {
	float *			Data[NUM_ALL_PORTS];
	char				Test[4];
} MIDI_INS;

typedef MIDI_INS *	LADSPA_Handle;

typedef struct {
	int	Hint;
	float	Lower, Upper;
} PORTRANGEHINT;

typedef struct _LADSPA_Descriptor {
	unsigned long		UniqueID;
	const char *		Label;
	int					Props;
	const char *		Name;
	const char *		Maker;
	const char *		Copyright;
	unsigned long		PortCount;
	const int *			PortDescs;
	const char * const * PortNames;
	const PORTRANGEHINT *	PortHints;
	void *				ImpData;
	LADSPA_Handle   (*instantiateF)(const struct _LADSPA_Descriptor *, unsigned long);
	void (*connect_port)(LADSPA_Handle, unsigned long, float *);
	void (*activateF)(LADSPA_Handle);
	void (*runF)(LADSPA_Handle, unsigned long);
	void (*run_2)(void);
	void (*run_3)(void);
	void (*deactivateF)(LADSPA_Handle);
	void (*cleanupF)(LADSPA_Handle);
} LADSPA_Descriptor;

typedef struct {
    int DSSI_API_Version;
    const LADSPA_Descriptor *LADSPA_Plugin;
    void *	configure;
    void *	get_program;
    void *	select_program;
    int (*get_midi_controller_for_port)(LADSPA_Handle, unsigned long Port);
    void (*run_synth)(LADSPA_Handle, unsigned long, snd_seq_event_t *, unsigned long);
    void *	run_synth_adding;
    void *	run_multiple_synths;
    void *	run_multiple_synths_adding;
} DSSI_Descriptor;

static void chk_handle(register MIDI_INS * plugin)
{
	printf("Host has passed a ");
	if (plugin)
	{
		if (plugin->Test[0] == 'T' && plugin->Test[1] == 'e' && plugin->Test[2] == 's' && plugin->Test[3] == 't')
			printf("good");
		else
			printf("bad");
	}
	else
		printf("NULL");
	printf(" plugin handle.\r\n");
}

static void cleanup(LADSPA_Handle instance)
{
	register MIDI_INS *	plugin;

	plugin = instance;

	printf("\r\n\n=============================\r\nHost is calling plugin's cleanup()...\r\n");
	chk_handle(plugin);

	if (plugin)
	{
		printf("Attempting to free the plugin handle...\r\n");
		memset(plugin, 0, sizeof(MIDI_INS));
   	free(plugin);
		printf("The plugin handle has been successfully freed.\r\n");
	}
	printf("Plugin's cleanup() has finished and is returning to the host...\r\n=============================\r\n");
}

static int chk_descriptor(const void * descriptor);

static LADSPA_Handle instantiate(const LADSPA_Descriptor * descriptor, unsigned long s_rate)
{
	register MIDI_INS *	plugin;

	printf("\r\n\n=============================\r\nHost is calling plugin's instantiate()...\r\nHost has passed the ");

	if (chk_descriptor(descriptor))
		printf("correct");
	else
		printf("wrong");
	printf("descriptor.\r\n");
	printf("Host has passed a sample rate of %lu.\r\n", s_rate);

	if ((plugin = (MIDI_INS *)malloc(sizeof(MIDI_INS))))
	{
		printf("The plugin handle has been successfully allocated.\r\n");
		memset(plugin, 0, sizeof(MIDI_INS));
		plugin->Test[0] = 'T';
		plugin->Test[1] = 'e';
		plugin->Test[2] = 's';
		plugin->Test[3] = 't';
	}

	printf("Plugin's instantiate() has finished and is returning to the host...\r\n=============================\r\n");
	return (LADSPA_Handle) plugin;
}

static void activate(LADSPA_Handle instance)
{
	register MIDI_INS *	plugin;

	plugin = instance;

	printf("\r\n\n=============================\r\nHost is calling plugin's activate()...\r\n");
	chk_handle(plugin);
	printf("Plugin's activate() has finished and is returning to the host...\r\n=============================\r\n");
}

static void run(LADSPA_Handle instance, unsigned long numSamples, snd_seq_event_t * midiEventsArray, unsigned long numMidiEvents)
{
	register MIDI_INS *	plugin;
	float * 					outputL;
	float * 					outputR;
	register uint32_t		begin, end;

	plugin = instance;

	printf("\r\n\n=============================\r\nHost is calling plugin's run()...\r\n");
	chk_handle(plugin);

	if (plugin)
	{
		outputL = plugin->Data[PORTNUM_LEFT_OUT];
		outputR = plugin->Data[PORTNUM_RIGHT_OUT];

		if (outputL)
			memset(outputL, 0, numSamples * sizeof(float));
		else
			printf("Host has not supplied the left channel buffer.\r\n");
		if (outputR)
			memset(outputR, 0, numSamples * sizeof(float));
		else
			printf("Host has not supplied the right channel buffer.\r\n");

		if (midiEventsArray)
			printf("Host has passed %lu midi events.\r\n", numMidiEvents);
		else
			printf("Host has not supplied a buffer of midi events.\r\n");

		begin = 0;
		while (begin < numSamples)
		{
			end = numSamples;
			while (numMidiEvents)
			{
				if (midiEventsArray->time.tick > begin)
				{
					end = midiEventsArray->time.tick;
					if (end > numSamples) end = numSamples;
					break;
				}

				switch (midiEventsArray->type)
				{
					case SND_SEQ_EVENT_NOTEON:
					{
						if (!midiEventsArray->data.note.velocity) goto ntf;
						printf("Midi note %u is turned ON.\r\n", midiEventsArray->data.note.note);
						break;
					}
					case SND_SEQ_EVENT_NOTEOFF:
					{
ntf:					printf("Midi note %u is turned off.\r\n", midiEventsArray->data.note.note);
					}
				}

				midiEventsArray++;
				numMidiEvents--;
			}

			begin = end;
		}

		if (numMidiEvents) printf("%lu midi events weren't handled because their tick time > %lu.\r\n", numSamples);
	}
	printf("Plugin's run() has finished and is returning to the host...\r\n=============================\r\n");
}

static void runWrapper(LADSPA_Handle instance, unsigned long numSamples)
{
    run(instance, numSamples, NULL, 0);
}

static void connectPort(LADSPA_Handle instance, unsigned long port, float * data)
{
	register MIDI_INS *	plugin;

	plugin = instance;

	printf("\r\n\n=============================\r\nHost is calling plugin's connectPort()...\r\n");
	chk_handle(plugin);

	if (plugin)
	{
		if (port < NUM_ALL_PORTS)
		{
			printf("Host has supplied the %s channel buffer at address %p.\r\n", port ? "right" : "left", data);
			plugin->Data[port] = data;
		}
		else
			printf("Host has passed an illegal port number (%lu).\r\n", port);
	}
	printf("Plugin's connectPort() has finished and is returning to the host...\r\n=============================\r\n");
}

static int getController(LADSPA_Handle instance, unsigned long port)
{
	return 0;
}

static const char Blank[] = "";

static const char *	PortNames[NUM_ALL_PORTS] = {
	&Blank[0],	// chan 1
	&Blank[0],	// chan 2
};

static const PORTRANGEHINT PortRangeHints[NUM_ALL_PORTS] = {
	{0, 0.0f, 1.0f},
	{0, 0.0f, 1.0f},
};

static const int				PortDescriptors[NUM_ALL_PORTS] = {
	10,	// Output audio port
	10,
};

static const LADSPA_Descriptor LadDescriptor = {
	LADSPA_ID_NUM,
	PLUGIN_CLASS_NAME,
	4,
	PLUGIN_STR_NAME,
	"Jeff Glatt",
	"GPL3",
	NUM_ALL_PORTS,
	&PortDescriptors[0],
	&PortNames[0],
	&PortRangeHints[0],
	0,
	instantiate,
	connectPort,
	activate,
	runWrapper,
	0, 0, 0,
	cleanup,
};

static const DSSI_Descriptor DssiDescriptor = {1, &LadDescriptor, 0, 0, 0, getController, run, 0, 0, 0};

const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	return (index ? 0 : &LadDescriptor);
}

_S3_DLLEXPORT
const DSSI_Descriptor * dssi_descriptor(unsigned long index)
{
	return (index ? 0 : &DssiDescriptor);
}

static int chk_descriptor(const void * descriptor)
{
	if (descriptor == &DssiDescriptor) return 1;
	return 0;
}

//----------------------------------------------------------------------

#undef printf

//----------------------------------------------------------------------

