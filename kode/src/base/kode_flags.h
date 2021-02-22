#ifndef kode_flags_included
#define kode_flags_included
//----------------------------------------------------------------------

#define KODE_ANCHOR_STRETCH_HORIZONTAL  9
#define KODE_ANCHOR_STRETCH_VERTICAL    10

#define KODE_BUTTON_NONE                0
#define KODE_BUTTON_LEFT                1
#define KODE_BUTTON_MIDDLE              2
#define KODE_BUTTON_RIGHT               3
#define KODE_BUTTON_SCROLL_UP           4
#define KODE_BUTTON_SCROLL_DOWN         5
#define KODE_BUTTON_BACKWARD            8
#define KODE_BUTTON_FORWARD             9

#define KODE_CHANNEL_BLUE               0
#define KODE_CHANNEL_GREEN              1
#define KODE_CHANNEL_RED                2
#define KODE_CHANNEL_ALPHA              3

#define KODE_CORNER_NONE                0
#define KODE_CORNER_LEFT_TOP            1
#define KODE_CORNER_LEFT_BOTTOM         2
#define KODE_CORNER_RIGHT_TOP           4
#define KODE_CORNER_RIGHT_BOTTOM        8

#define KODE_CURSOR_SHOW                -2
#define KODE_CURSOR_HIDE                -1
#define KODE_CURSOR_DEFAULT             0
#define KODE_CURSOR_ARROW               1
#define KODE_CURSOR_ARROWUP             2
#define KODE_CURSOR_ARROWDOWN           3
#define KODE_CURSOR_ARROWLEFT           4
#define KODE_CURSOR_ARROWRIGHT          5
#define KODE_CURSOR_ARROWUPDOWN         6
#define KODE_CURSOR_ARROWLEFTRIGHT      7
#define KODE_CURSOR_ARROWDIAGLEFT       8
#define KODE_CURSOR_ARROWDIAGRIGHT      9
#define KODE_CURSOR_MOVE                10
#define KODE_CURSOR_WAIT                11
#define KODE_CURSOR_ARROWWAIT           12
#define KODE_CURSOR_HAND                13
#define KODE_CURSOR_FINGER              14
#define KODE_CURSOR_CROSS               15
#define KODE_CURSOR_PENCIL              16
#define KODE_CURSOR_PLUS                17
#define KODE_CURSOR_QUESTION            18
#define KODE_CURSOR_IBEAM               19

#define KODE_EDGE_NONE                  0
#define KODE_EDGE_LEFT                  1
#define KODE_EDGE_TOP                   2
#define KODE_EDGE_RIGHT                 4
#define KODE_EDGE_BOTTOM                8

#define KODE_EVENT_UNKNOWN              0
#define KODE_EVENT_PARAMETER            1
#define KODE_EVENT_MIDI                 2

#define KOOE_FILTER_NONE                0
#define KOOE_FILTER_LOWPASS             1
#define KOOE_FILTER_BANDPASS            2
#define KOOE_FILTER_JIGHPASS            3
#define KOOE_FILTER_NOTCH               4
#define KOOE_FILTER_PEAK                5
#define KOOE_FILTER_ALLPASS             6
#define KOOE_FILTER_BELL                7
#define KOOE_FILTER_LOWSHELF            8
#define KOOE_FILTER_JIGHSHELF           9

#define KODE_HOST_EXE                   0
#define KODE_HOST_BITWIG                1
#define KODE_HOST_REAPER                2

#define KODE_KEY_NONE                   0x00
#define KODE_KEY_SHIFT                  0x01
#define KODE_KEY_CAPS                   0x02
#define KODE_KEY_CTRL                   0x04
#define KODE_KEY_ALT                    0x08
#define KODE_KEY_ALTGR                  0x10

#define KODE_MIDI_NOTE_OFF              0x80
#define KODE_MIDI_NOTE_ON               0x90
#define KODE_MIDI_POLY_AFTERTOUCH       0xA0
#define KODE_MIDI_CONTROL_CHANGE        0xB0
#define KODE_MIDI_PROGRAM_CHANGE        0xC0
#define KODE_MIDI_CHANNEL_AFTERTOUCH    0xD0
#define KODE_MIDI_PITCHBEND             0xE0
#define KODE_MIDI_SYS                   0xF0

#define KODE_PARAMETER_AUTOMATE         1

#define KODE_PLUGIN_NONE                0x00
#define KODE_PLUGIN_HAS_EDITOR          0x01
#define KODE_PLUGIN_IS_SYNTH            0x02
#define KODE_PLUGIN_SEND_MIDI           0x04
#define KODE_PLUGIN_RECEIVE_MIDI        0x08
#define KODE_PLUGIN_SYNC                0x10

#define KODE_PLUGIN_FORMAT_NONE         0
#define KODE_PLUGIN_FORMAT_EXE          1
#define KODE_PLUGIN_FORMAT_LADSPA       2
#define KODE_PLUGIN_FORMAT_DSSI         3
#define KODE_PLUGIN_FORMAT_LV2          4
#define KODE_PLUGIN_FORMAT_VST2         5
#define KODE_PLUGIN_FORMAT_VST3         6

#define KODE_PLUGIN_PLAYSTATE_NONE      0x00
#define KODE_PLUGIN_PLAYSTATE_PLAYING   0x01
#define KODE_PLUGIN_PLAYSTATE_RECORDING 0x02
#define KODE_PLUGIN_PLAYSTATE_LOOPING   0x04

//#define KODE_PLUGIN_PLAYSTATE_STARTED_PLAYING   0x08
//#define KODE_PLUGIN_PLAYSTATE_STARTED_RECORDING 0x10
//#define KODE_PLUGIN_PLAYSTATE_STARTED_LOOPING   0x11

#define KODE_PLUGIN_STATE_OPEN          1
#define KODE_PLUGIN_STATE_CLOSE         2
#define KODE_PLUGIN_STATE_INITIALIZE    3
#define KODE_PLUGIN_STATE_TERMINATE     4
#define KODE_PLUGIN_STATE_ACTIVATE      5
#define KODE_PLUGIN_STATE_DEACTIVATE    6

#define KODE_SYMBOL_NONE                1
#define KODE_SYMBOL_DRAW_RECT           2
#define KODE_SYMBOL_FILL_RECT           3
#define KODE_SYMBOL_DRAW_TRI_DOWN       4
#define KODE_SYMBOL_FILL_TRI_DOWN       5
#define KODE_SYMBOL_DRAW_ELLIPSE        6
#define KODE_SYMBOL_FILL_ELLIPSE        7

#define KODE_TEXT_ALIGN_NONE            0
#define KODE_TEXT_ALIGN_TOP             (1 << 0)
#define KODE_TEXT_ALIGN_BOTTOM          (1 << 1)
#define KODE_TEXT_ALIGN_LEFT            (1 << 2)
#define KODE_TEXT_ALIGN_RIGHT           (1 << 3)
#define KODE_TEXT_ALIGN_CENTER          (1 << 4)

#define KODE_THREAD_NONE                0
#define KODE_THREAD_KILL                666
#define KODE_THREAD_TIMER               667
#define KODE_THREAD_IDLE                668

#define KODE_WIDGET_ACTIVE              (1 << 0) 
#define KODE_WIDGET_VISIBLE             (1 << 1)
#define KODE_WIDGET_OPAQUE              (1 << 2)
#define KODE_WIDGET_VERTICAL            (1 << 3)
#define KODE_WIDGET_SETCURSOR           (1 << 4)
#define KODE_WIDGET_GRABCURSOR          (1 << 5)
#define KODE_WIDGET_HIDECURSOR          (1 << 6)
#define KODE_WIDGET_HOVER               (1 << 7)
#define KODE_WIDGET_REALIGN             (1 << 8)
#define KODE_WIDGET_CLIP                (1 << 9)
//#define KODE_WIDGET_DRAG                (1 << 10)
//#define KODE_WIDGET_DROP                (1 << 11)

#define KODE_WIDGET_ALIGN_NONE          0
#define KODE_WIDGET_ALIGN_PARENT        1
#define KODE_WIDGET_ALIGN_LEFT          2
#define KODE_WIDGET_ALIGN_LEFT_FILL     3
#define KODE_WIDGET_ALIGN_LEFT_TOP      4
#define KODE_WIDGET_ALIGN_LEFT_BOTTOM   5
#define KODE_WIDGET_ALIGN_RIGHT         6
#define KODE_WIDGET_ALIGN_RIGHT_FILL    7
#define KODE_WIDGET_ALIGN_RIGHT_TOP     8
#define KODE_WIDGET_ALIGN_RIGHT_BOTTOM  9
#define KODE_WIDGET_ALIGN_TOP           10
#define KODE_WIDGET_ALIGN_TOP_FILL      11
#define KODE_WIDGET_ALIGN_TOP_LEFT      12
#define KODE_WIDGET_ALIGN_TOP_RIGHT     13
#define KODE_WIDGET_ALIGN_BOTTOM        14
#define KODE_WIDGET_ALIGN_BOTTOM_FILL   15
#define KODE_WIDGET_ALIGN_BOTTOM_LEFT   16
#define KODE_WIDGET_ALIGN_BOTTOM_RIGHT  17
#define KODE_WIDGET_ALIGN_CLIENT        18
#define KODE_WIDGET_ALIGN_CLIENT_FILL   19
#define KODE_WIDGET_ALIGN_STACK_HORIZ   20
#define KODE_WIDGET_ALIGN_STACK_VERT    21
#define KODE_WIDGET_ALIGN_CENTER        22

//----------------------------------------------------------------------
#endif