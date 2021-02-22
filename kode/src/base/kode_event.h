#ifndef kode_event_included
#define kode_event_included
//----------------------------------------------------------------------

struct KODE_MidiEvent {
  uint32_t  offset;
  uint8_t   msg1;
  uint8_t   msg2;
  uint8_t   msg3;
  uint8_t   dummy;
};

//----------

//struct KODE_SizeEvent {
//  uint32_t  width;
//  uint32_t  height;
//};

//----------

struct KODE_ParameterEvent {
  uint32_t  index;
  float     value;
};

//----------------------------------------------------------------------

struct KODE_Event {
  union {
    KODE_MidiEvent      midi;
    KODE_ParameterEvent param;
    //KODE_SizeEvent      size;
  };
};

//typedef vector<KODE_Event> KODE_Events;

//----------------------------------------------------------------------
#endif