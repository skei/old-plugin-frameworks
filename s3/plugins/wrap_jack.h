
#ifndef jack_included
#define jack_included
//----------------------------------------------------------------------

/*
  todo:
  - ring-buffer
*/


//----------------------------------------------------------------------

//#define S3_DEBUG_MEM
#ifdef S3_PLUGIN_VST
  //#define S3_DEBUG_VST
  #define S3_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------

#include <jack/jack.h>

#include "s3.h"
#include "plugin/s3_plugin.h"
#include "common/s3_random.h"

#define JACK_BUFFER_SIZE  192000
#define JACK_CLIENT_NAME  "fx_jack"

static int process_jack(jack_nframes_t, void*);

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin : public S3_Plugin
{

  friend int process_jack(jack_nframes_t nframes, void* data);

  private:

    jack_client_t*  MJackClient;
    jack_port_t*    MJackInput0;
    jack_port_t*    MJackInput1;
    jack_port_t*    MJackOutput0;
    jack_port_t*    MJackOutput1;
    float           MJackInputBuffer0[JACK_BUFFER_SIZE];
    float           MJackInputBuffer1[JACK_BUFFER_SIZE];
    float           MJackOutputBuffer0[JACK_BUFFER_SIZE];
    float           MJackOutputBuffer1[JACK_BUFFER_SIZE];

  //----------------------------------------
  // internal
  //----------------------------------------

  /*

    jack_client_t* jack_client_open(
      const char*     client_name,
      jack_options_t  options,
      jack_status_t*  status,
      ...
    )

    Open an external client session with a JACK server. This interface is more
    complex but more powerful than jack_client_new(). With it, clients may
    choose which of several servers to connect, and control whether and how to
    start the server automatically, if it was not already running. There is
    also an option for JACK to generate a unique client name, when necessary.

    Parameters:
      client_name         of at most jack_client_name_size() characters. The
                          name scope is local to each server. Unless forbidden
                          by the JackUseExactName option, the server will
                          modify this name to create a unique variant, if
                          needed.
      options             formed by OR-ing together JackOptions bits. Only the
                          JackOpenOptions bits are allowed.
      status(if non-NULL) an address for JACK to return information from the
                          open operation. This status word is formed by OR-ing
                          together the relevant JackStatus bits.

      Optional parameters depending on corresponding [options bits] additional
                          parameters may follow status (in this order).

        [JackServerName]  (char *) server_name selects from among several
                          possible concurrent server instances. Server names
                          are unique to each user. If unspecified, use
                          "default" unless $JACK_DEFAULT_SERVER is defined in
                          the process environment.

    Returns:
      Opaque client handle if successful. If this is NULL, the open operation
      failed, *status includes JackFailure and the caller is not a JACK client.

    ----------

    jack_port_t* jack_port_register(
      jack_client_t* client,
      const char*   port_name,
      const char*   port_type,
      unsigned long flags,
      unsigned long buffer_size
    )

    Create a new port for the client. This is an object used for moving data of
    any type in or out of the client. Ports may be connected in various ways.
    Each port has a short name. The port's full name contains the name of the
    client concatenated with a colon (:) followed by its short name. The
    jack_port_name_size() is the maximum length of this full name. Exceeding
    that will cause the port registration to fail and return NULL.
    The port_name must be unique among all ports owned by this client. If the
    name is not unique, the registration will fail.
    All ports have a type, which may be any non-NULL and non-zero length
    string, passed as an argument. Some port types are built into the JACK API,
    like JACK_DEFAULT_AUDIO_TYPE or JACK_DEFAULT_MIDI_TYPE

    Parameters:
      client      pointer to JACK client structure.
      port_name   non-empty short name for the new port (not including the
                  leading "client_name:"). Must be unique.
      port_type   port type name. If longer than jack_port_type_size(), only
                  that many characters are significant.
      flags       JackPortFlags bit mask.
      buffer_size must be non-zero if this is not a built-in port_type.
                  Otherwise, it is ignored.

    Returns:
      jack_port_t pointer on success, otherwise NULL.

    ----------

    int jack_set_process_callback(
      jack_client_t* client,
      JackProcessCallback process_callback,
      void* arg
    )

    Tell the Jack server to call process_callback whenever there is work be
    done, passing arg as the second argument.
    The code in the supplied function must be suitable for real-time execution.
    That means that it cannot call functions that might block for a long time.
    This includes all I/O functions (disk, TTY, network), malloc, free, printf,
    pthread_mutex_lock, sleep, wait, poll, select, pthread_join,
    pthread_cond_wait, etc, etc.

    Returns:
      0 on success, otherwise a non-zero error code, causing JACK to remove
      that client from the process() graph.

  */

  private:

    void init_jack(void) {
      MJackClient = jack_client_open(JACK_CLIENT_NAME,JackNoStartServer,NULL,NULL);
      if (MJackClient) {
        MJackInput0  = jack_port_register(MJackClient,"left in",  JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);
        MJackInput1  = jack_port_register(MJackClient,"right in", JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);
        MJackOutput0 = jack_port_register(MJackClient,"left out", JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
        MJackOutput1 = jack_port_register(MJackClient,"right out",JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
        jack_set_process_callback(MJackClient,&process_jack,(void*)this);
        //nframes = (unsigned long)jack_get_buffer_size(MJackClient);
        S3_Memset(MJackInputBuffer0,0,sizeof(MJackInputBuffer0));
        S3_Memset(MJackInputBuffer1,0,sizeof(MJackInputBuffer1));
        S3_Memset(MJackOutputBuffer0,0,sizeof(MJackOutputBuffer0));
        S3_Memset(MJackOutputBuffer1,0,sizeof(MJackOutputBuffer1));
      }
    }

    //----------

    /*
      int jack_client_close(jack_client_t* client)
      Disconnects an external client from a JACK server.
      Returns:
        0 on success, otherwise a non-zero error code
    */

    void exit_jack(void) {
      jack_client_close(MJackClient);
    }

    //----------

    /*
      int jack_activate(jack_client_t* client)
      Tell the Jack server that the program is ready to start processing audio.
      Returns:
        0 on success, otherwise a non-zero error code
    */

    void activate_jack(void) {
      if (MJackClient) jack_activate(MJackClient);
    }

    //----------

    /*
      int jack_deactivate(jack_client_t* client)
      Tell the Jack server to remove this client from the process graph. Also,
      disconnect all ports belonging to it, since inactive clients have no port
      connections.
      Returns:
        0 on success, otherwise a non-zero error code
    */

    void deactivate_jack(void) {
      if (MJackClient) jack_deactivate(MJackClient);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) final {
      MName       = "fx_jack";
      MAuthor     = "s3";
      MDescription    = MName;
      MUniqueId   = S3_MAGIC + 0x0000;
      MNumInputs  = 2;
      MNumOutputs = 2;
      //
      init_jack();
    }

    //----------

    //virtual
    void on_destroy(void) final {
      exit_jack();
    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    //virtual
    void on_stateChange(uint32 AState) final {
      switch(AState) {
        case s3_ps_suspend:
          deactivate_jack();
          break;
        case s3_ps_resume:
          activate_jack();
          break;
      }
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      float* in0  = (float*)jack_port_get_buffer(MJackInput0,  ANumSamples);
      float* in1  = (float*)jack_port_get_buffer(MJackInput1,  ANumSamples);
      float* out0 = (float*)jack_port_get_buffer(MJackOutput0, ANumSamples);
      float* out1 = (float*)jack_port_get_buffer(MJackOutput1, ANumSamples);
      if (MJackClient) {
        //SMemcpy(AOutputs[0],in0,ANumSamples*sizeof(SSample));
        //SMemcpy(AOutputs[1],in1,ANumSamples*sizeof(SSample));
        //SMemcpy(out0,AInputs[0],ANumSamples*sizeof(SSample));
        //SMemcpy(out1,AInputs[1],ANumSamples*sizeof(SSample));
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  typedef int(* JackProcessCallback)(jack_nframes_t nframes, void *arg)

  Prototype for the client supplied function that is called by the engine anytime there is work to be done.

  Precondition:
    nframes == jack_get_buffer_size()
    nframes == pow(2,x)
  Parameters:
    nframes number of frames to process
    arg     pointer to a client supplied data
  Returns:
    zero on success, non-zero on error
*/

static int process_jack(jack_nframes_t nframes, void* data) {
  //jack_nframes_t i;
  myPlugin* self = (myPlugin*)data;
  jack_default_audio_sample_t* in0  = (float*)jack_port_get_buffer(self->MJackInput0, nframes);
  jack_default_audio_sample_t* in1  = (float*)jack_port_get_buffer(self->MJackInput1, nframes);
  jack_default_audio_sample_t* out0 = (float*)jack_port_get_buffer(self->MJackOutput0, nframes);
  jack_default_audio_sample_t* out1 = (float*)jack_port_get_buffer(self->MJackOutput1, nframes);

  for (uint32 i=0; i<nframes; i++) {
    self->MJackOutputBuffer0[i] = S3_RandomSigned();
    self->MJackOutputBuffer1[i] = S3_RandomSigned();
  }

  S3_Memcpy(self->MJackInputBuffer0,in0,nframes*sizeof(float));
  S3_Memcpy(self->MJackInputBuffer1,in1,nframes*sizeof(float));
  S3_Memcpy(out0,self->MJackOutputBuffer0,nframes*sizeof(float));
  S3_Memcpy(out1,self->MJackOutputBuffer1,nframes*sizeof(float));

  return 0;
}

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif


