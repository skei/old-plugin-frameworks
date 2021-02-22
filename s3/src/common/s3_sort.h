#ifndef s3_sort_included
#define s3_sort_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

void S3_BubbleSort (int a[], int length);
void S3_InsertSort(int* a, int length);
void S3_RadixSort(long *source, long *dest, uint32 N, int byte);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

void S3_BubbleSort (int a[], int length) {
	int i, j, temp;
  for (i = 0; i < length; i++) {
    for (j = 0; j < length - 1; j++) {
      if (a[j + 1] < a[j]) {
        temp = a[j];
        a[j] = a[j + 1];
        a[j + 1] = temp;
      }
    }
  }
}

//----------

// nice for small-ish data sets.. ???

void S3_InsertSort(int* a, int length) {
  int i, j, value;
  for(i = 1 ; i < length ; i++) {
    value = a[i];
    for (j = i - 1; j >= 0 && a[j] > value; j--)
      a[j + 1] = a[j];
    a[j + 1] = value;
  }
}

//----------

//static __S3_UNUSED
void S3_RadixSort(long *source, long *dest, uint32 N, int byte) {
  uint32 i;
  long count[256];
  long index[256];
  S3_Memset(count, 0, sizeof(count));
  for (i=0; i<N; i++) count[ ((source[i]) >> (byte*8)) & 0xff ]++;
  index[0] = 0;
  for (i=1; i<256; i++) index[i] = index[i-1]+count[i-1];
  for (i=0; i<N; i++) dest[ index[ ((source[i])>>(byte*8))&0xff ]++ ] = source[i];
}

//----------------------------------------------------------------------
#endif
