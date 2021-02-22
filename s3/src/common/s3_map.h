#ifndef s3_map_included
#define s3_map_included
//----------------------------------------------------------------------

/*
  based on:
    http://www.codeproject.com/Articles/149879/Associative-Array-in-C
    by Arnav Mukhopadhyay
*/

//----------------------------------------------------------------------

#include "common/s3_array.h"

//----------

template <class _T>
class S3_Map {

  private:

    struct S3_MapItem {
      const char* name;
      _T value;
    };

    S3_Array<S3_MapItem> MItems;

  //------------------------------
  //
  //------------------------------

  public:

    uint32 size() {
      return MItems.size();
    }

    //----------

    bool hasItem(const char* name) {
      for (uint32 i=0; i<size(); i++) {
        if (MItems[i].name == name) return true; // strcmp
      }
      return false;
    }

    //----------

    int32 getItemIndex(const char* name) {
      for (uint32 i=0; i<size(); i++) {
        if (MItems[i].name == name) return i; // strcmp
      }
      return -1;
    }

    //----------

    const char* getItemName(uint32 index) {
      return MItems[index].name;
    }

    //----------

    _T& operator [] (uint32 index) {
      return MItems[index].value;
    }

    //----------

    _T& operator [] (const char* name) {
      for (uint32 i=0; i<size(); i++) {
        if (MItems[i].name == name) return MItems[i].value; // strcmp
      }
      uint32 idx = size();
      S3_MapItem d;
      d.name = name;
      d.value = 0;  // ???
      MItems.append(d);
      return MItems[idx].value;
    }

    //----------

    bool addItem(const char* name, _T value) {
      if (hasItem(name)) return false;
      S3_MapItem d;
      d.name = name;
      d.value = value;
      MItems.append(d);
      return true;
    }

    //----------

    bool removeItem(uint32 index) {
      MItems.remove(index);
      return true;
    }

    //----------

    bool removeItem(const char* name) {
      for (uint32 i=0; i<size(); i++) {
        if (MItems[i].name == name) { // strcmp
          MItems.remove(i);
          return true;
        }
      }
      return false;
    }

    //----------

};

//----------------------------------------------------------------------
#endif
