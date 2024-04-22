#include <math.h>
#include <jack/jack.h>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

jack_port_t *my_output_port1, *my_output_port2;

#define TABLE_SIZE   (200)
typedef struct
{
	float sine[TABLE_SIZE];
	int left_phase;
	int right_phase;
}
paTestData;

paTestData data;
void* setup_process(jack_port_t *_output_port1, jack_port_t *_output_port2) {
	int i;
	for( i=0; i<TABLE_SIZE; i++ )
	{
		data.sine[i] = 0.01 * (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 4. );
	}
	data.left_phase = data.right_phase = 0;
	
	my_output_port1 = _output_port1;
	my_output_port2 = _output_port2;

	return &data;
}

int process (jack_nframes_t nframes, void *arg)
{
	jack_default_audio_sample_t *out1, *out2;
	paTestData *data = (paTestData*)arg;
	int i;
	
	out1 = (jack_default_audio_sample_t*)jack_port_get_buffer (my_output_port1, nframes);
	out2 = (jack_default_audio_sample_t*)jack_port_get_buffer (my_output_port2, nframes);
	
	for( i=0; i<nframes; i++ )
	{
		out1[i] = data->sine[data->left_phase];  /* left */
		out2[i] = data->sine[data->right_phase];  /* right */
		data->left_phase += 1;
		if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
		data->right_phase += 3; /* higher pitch so we can distinguish left and right. */
		if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
	}
	
	return 0;      
}