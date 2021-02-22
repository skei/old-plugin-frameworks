
#include "s3.h"
#include "common/s3_inifile.h"

int main(void) {
  S3_Inifile ini;
  ini.load();
  ini.unload();
  return 0;
}
