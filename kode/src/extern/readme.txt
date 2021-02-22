
because of various licensing issues, you have to provide some files yourself,
depending on what you use/include.. be sure to read the licenses for all
external code you use!

----------
vst2
----------

by default, the steinberg official sdk is selected, but you still need to
have the needed files.. steinberg have stopped distributing them..

but you can optionally use the (included) vestige or fst header

needed files:

vst2/
  aeffect.h
  aeffectx.h
  vstfxstore.h

fst.h can be found here:
  https://git.iem.at/zmoelnig/FST

vestige.h can be found here:
  https://github.com/osxmidi/LinVst/blob/master/vestige.h
  https://github.com/rncbc/qtractor/blob/master/src/vestige/vestige.h
  https://github.com/x42/lv2vst/blob/master/include/vestige.h
  
  
----------
vst3
----------

download:
  https://www.steinberg.net/en/company/developers.html

copy the entire pluginterfaces directory from the vst3 sdk..
it should look something like this:
  
vst3/
  pluginterfaces/
    base/
    gui/
    test/
    vst/
    
(we could possibly incude the needed files, since they are gpl3?)

