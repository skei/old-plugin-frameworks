#ifndef s3_list_included
#define s3_list_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_ListNode
{
  friend class S3_List;
  private:
    S3_ListNode* MPrev;
    S3_ListNode* MNext;
  public:
    S3_ListNode();
    S3_ListNode*  next(void);
    S3_ListNode*  prev(void);
    void          next(S3_ListNode* ANode);
    void          prev(S3_ListNode* ANode);
};

//----------

class S3_List
{
  private:
    int         MSize;
    S3_ListNode* MHead;
    S3_ListNode* MTail;
  public:
    S3_List();
    S3_ListNode*  head(void);
    S3_ListNode*  tail(void);
    void          head(S3_ListNode* ANode);
    void          tail(S3_ListNode* ANode);
    void          reset(void);
    bool          empty(void);
    bool          notEmpty(void);
    void          insertHead(S3_ListNode* ANode);
    void          insertTail(S3_ListNode* ANode);
    void          removeHead(void);
    void          removeTail(void);
    void          append(S3_ListNode* ANode);
    void          remove(S3_ListNode* ANode);
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

/*
  a templates (ala array)data-container)
  b multiple inheritance (ala widgetListener)

  todo: (?)
  - sort
  - find
  - swap two items
  - move item to head/tail
*/

S3_ListNode::S3_ListNode() {
  MPrev = S3_NULL;
  MNext = S3_NULL;
}

S3_ListNode* S3_ListNode::next(void) {
  return MNext;
}

S3_ListNode* S3_ListNode::prev(void) {
  return MPrev;
}

void S3_ListNode::next(S3_ListNode* ANode) {
  MNext = ANode;
}

void S3_ListNode::prev(S3_ListNode* ANode) {
  MPrev = ANode;
}

//----------------------------------------
//
//----------------------------------------

S3_List::S3_List() {
  reset();
}

S3_ListNode* S3_List::head(void) {
  return MHead;
}
S3_ListNode* S3_List::tail(void) {
  return MTail;
}
void S3_List::head(S3_ListNode* ANode) {
  MHead = ANode;
}
void S3_List::tail(S3_ListNode* ANode) {
  MTail = ANode;
}

void S3_List::reset(void) {
  MHead = S3_NULL;
  MTail = S3_NULL;
}

bool S3_List::empty(void) {
  return (MHead==S3_NULL);
}

bool S3_List::notEmpty(void) {
  return (MHead!=S3_NULL);
}

void S3_List::insertHead(S3_ListNode* ANode) {
  ANode->MPrev = S3_NULL;
  if (MTail==S3_NULL) {
    MTail = ANode;
    ANode->MNext = S3_NULL;
  }
  else {
    MHead->MPrev = ANode;
    ANode->MNext = MHead;
  }
  MHead = ANode;
  MSize++;
}

void S3_List::insertTail(S3_ListNode* ANode) {
  ANode->MNext = S3_NULL;
  if (MHead==S3_NULL) {
    MHead = ANode;
    ANode->MPrev = S3_NULL;
  }
  else {
    MTail->MNext = ANode;
    ANode->MPrev = MTail;
  }
  MTail = ANode;
  MSize++;
}

void S3_List::removeHead(void) {
  if (MHead==MTail) reset();
  else {
    MHead = MHead->MNext;
    MHead->MPrev = S3_NULL;
  }
}

void S3_List::removeTail(void) {
  if (MHead==MTail) reset();
  else {
    MTail = MTail->MPrev;
    MTail->MNext = S3_NULL;
  }
}

void S3_List::append(S3_ListNode* ANode) {
  insertTail(ANode);
}

void S3_List::remove(S3_ListNode* ANode) {
  if (ANode==MHead) removeHead();
  else if (ANode==MTail) removeTail();
  else {
    ANode->MNext->MPrev = ANode->MPrev;
    ANode->MPrev->MNext = ANode->MNext;
  }
}

//----------------------------------------------------------------------
#endif

/*

class KDoubleListNode {
  private:
    KDoubleListNode* MPrevListNode;
    KDoubleListNode* MNextListNode;
  public:
    inline void prev(KDoubleListNode* ANode) { MPrevListNode=ANode; }
    inline void next(KDoubleListNode* ANode) { MNextListNode=ANode; }
    inline KDoubleListNode* prev(void) { return MPrevListNode; }
    inline KDoubleListNode* next(void) { return MNextListNode; }
};

//----------

class KDoubleList {
  private:
    KDoubleListNode* MListHead;
    KDoubleListNode* MListTail;

  public:

    KDoubleList() {
      MListHead = S3_NULL;
      MListTail = S3_NULL;
    }

    virtual ~KDoubleList() {
    }

    inline void head(KDoubleListNode* ANode) { MListHead=ANode; }
    inline void tail(KDoubleListNode* ANode) { MListTail=ANode; }
    inline KDoubleListNode* head(void) { return MListHead; }
    inline KDoubleListNode* tail(void) { return MListTail; }

    // initialize the list
    virtual void clear(void) {
      MListHead = S3_NULL;
      MListTail = S3_NULL;
    }

    // delete all nodes...
    void deleteNodes(void) {
      KDoubleListNode* node = MListHead;
      KDoubleListNode* next;
      while (node) {
        next = node->next();
        delete node;
        node = next;
      }
      MListHead = S3_NULL;
      MListTail = S3_NULL;
    }

    // append to tail...
    void appendTail(KDoubleListNode* ANode) {
      if (MListTail) {
        // xx XX --
        ANode->prev(MListTail);
        ANode->next(S3_NULL);
        MListTail->next(ANode);
        MListTail = ANode;
      }
      else {
        // -- XX --
        MListHead = ANode;
        ANode->prev(S3_NULL);
        ANode->next(S3_NULL);
        MListTail = ANode;
      }
    }

    void removeHead(void) {
      //TODO
    }

    void removeTail(void) {
      //TODO
    }

    // insert ANewNode after ANode
    void insertNode(KDoubleListNode* ANode, KDoubleListNode* ANewNode) {
      KDoubleListNode* next = ANode->next();
      KDoubleListNode* prev = ANode->prev();
      if (prev) {
        if (next) {
          //  xx XX xx
          prev->next(ANewNode);
          ANewNode->prev(prev);
          ANewNode->next(next);
          next->prev(ANewNode);
        }
        else {
          // xx XX --
          prev->next(ANewNode);
          ANewNode->prev(prev);
          ANewNode->next(S3_NULL);
          MListTail = ANewNode;
        }
      }
      else {
        // !head
        if (next) {
          // -- XX xx
          MListHead = ANewNode;
          ANewNode->prev(S3_NULL);
          ANewNode->next(next);
          next->prev(ANewNode);
        }
        else {
          // -- XX --
          MListHead = ANewNode;
          ANewNode->prev(S3_NULL);
          ANewNode->next(S3_NULL);
          MListTail = ANewNode;
        }
      } // !next
    }

    // remove node
    void removeNode(KDoubleListNode* ANode) {
      KDoubleListNode* next = ANode->next();
      KDoubleListNode* prev = ANode->prev();
      if (prev) {
        if (next) {
          //  xx XX xx
          // prev, next
          prev->next(next);
          next->prev(prev);
        }
        else {
          // xx XX --
          // prev, NULL
          // tail
          prev->next(S3_NULL);
          MListTail = prev;
        }
      }
      else {
        // !head
        if (next) {
          // -- XX xx
          // NULL,next
          // head
          MListHead = next;
          next->prev(S3_NULL);
        }
        else {
          // -- XX --
          // NULL,NULL
          // single
          MListHead = S3_NULL;
          MListTail = S3_NULL;
        }
      } // !next
    }

};

//----------

typedef SDoubleListNode  SListNode;
typedef SDoubleList      SList;

*/

