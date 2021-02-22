#ifndef s3_point_included
#define s3_point_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

struct S3_Point
{
  int32 x,y;
  S3_Point();
  S3_Point(int32 i);
  S3_Point(int32 ax, int32 ay);
  void set(int32 i);
  void set(int32 ax, int32 ay);
  void add(int32 ax, int32 ay);

};


//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Point::S3_Point() {
  x=0;
  y=0;
}

S3_Point::S3_Point(int32 i) {
  x=i;
  y=i;
}

S3_Point::S3_Point(int32 ax, int32 ay) {
  x=ax;
  y=ay;
}

void S3_Point::set(int32 i) {
  x=i;
  y=i;
}

void S3_Point::set(int32 ax, int32 ay) {
  x=ax;
  y=ay;
}

void S3_Point::add(int32 ax, int32 ay) {
  x+=ax;
  y+=ay;
}

//----------------------------------------------------------------------
#endif
