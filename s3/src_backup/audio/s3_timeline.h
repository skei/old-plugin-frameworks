#ifndef s2_timeline_included
#define s2_timeline_included
//----------------------------------------------------------------------

#include "common/s2_list.h"

//----------------------------------------------------------------------
// segment
//----------------------------------------------------------------------

class S2_Timeline_Segment
: public S2_ListNode {

  private:

    const char* MName;
    float       MStartPos;
    float       MEndPos;
    bool        MSelected;

  public:

    void name(const char* AName)      { MName = AName; }
    void start(float APos)            { MStartPos = APos; }
    void end(float APos)              { MEndPos = APos; }
    void select(float ASelected=true) { MSelected = ASelected; }

    const char* name(void)      { return MName; }
    float       start(void)     { return MStartPos; }
    float       end(void)       { return MEndPos; }
    bool        selected(void)  { return MSelected; }

  public:

    S2_Timeline_Segment(const char* AName, float AStart, float AEnd)
    : S2_ListNode() {
      MName     = AName;
      MStartPos = AStart;
      MEndPos   = AEnd;
      MSelected = false;
    }

    //----------

    virtual ~S2_Timeline_Segment() {
    }

    //----------

    bool isInside(float ATime) {
      if ((ATime >= MStartPos) && (ATime <= MEndPos)) return true;
      else return false;
    }

};

//----------------------------------------------------------------------
// track
//----------------------------------------------------------------------

class S2_Timeline_Track
: public S2_ListNode {

  private:

    char*     MName;
    S2_List   MSegments;
    bool      MSelected;

  public:

    char*     name(void) { return MName; }
    void      name(char* AName) { MName = AName; }
    S2_List*  segments(void) { return &MSegments; }
    void      select(float ASelected=true) { MSelected = ASelected; }
    bool      selected(void) { return MSelected; }

  public:

    S2_Timeline_Track(char* AName)
    : S2_ListNode() {
      MName = AName;
      MSelected = false;
    }

    //----------

    virtual ~S2_Timeline_Track() {
      // todo: if no autodelete - deletesegments
    }

    //----------

    void addSegment(S2_Timeline_Segment* ASegment) {
      MSegments.append(ASegment);
    }

    //----------

    void deleteSegments(void) {
      S2_ListNode* node = MSegments.head();
      while (node) {
        S2_Timeline_Segment* segment = (S2_Timeline_Segment*)node;
        node = node->next();
        delete segment;
      }
    }

};

//----------------------------------------------------------------------
// timeline
//----------------------------------------------------------------------

class S2_Timeline
: public S2_List {

  private:

    S2_List MTracks;
    int32   MCursor;

  public:

    S2_List*  tracks(void)          { return &MTracks; }
    int32     cursor(void)          { return MCursor; }
    void      cursor(int32 ACursor) { MCursor=ACursor; }

  public:

    S2_Timeline()
    : S2_List() {
      MCursor = 0;
    }

    //----------

    virtual ~S2_Timeline() {
    }

    //----------

    void addTrack(S2_Timeline_Track* ATrack) {
      MTracks.append(ATrack);
    }

    //----------

    void deleteTracks(void) {
      S2_ListNode* node = MTracks.head();
      while (node) {
        S2_Timeline_Track* track = (S2_Timeline_Track*)node;
        node = node->next();
        track->deleteSegments();
        delete track;
      }
    }

    //----------

};

//----------------------------------------------------------------------
#endif

