#ifndef s3_edges_included
#define s3_edges_included
//----------------------------------------------------------------------

struct S3_Edges {
  int32   left;
  int32   top;
  int32   right;
  int32   bottom;
  /* */
  S3_Edges() { left=0; top=0; right=0; bottom=0; }
  S3_Edges(int32 i) { left=i; top=i; right=i; bottom=i; }
  S3_Edges(int32 ALeft, int32 ATop, int32 ARight, int32 ABottom) { left=ALeft, top=ATop, right=ARight, bottom=ABottom; }

};

//----------------------------------------------------------------------
#endif
