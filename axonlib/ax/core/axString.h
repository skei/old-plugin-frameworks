/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axString_included
#define axString_included
//----------------------------------------------------------------------
// char* wrapper

//#include <string.h>
#include "core/axStdlib.h"

//----------------------------------------------------------------------

class axString
{
  private:
    char* mText;
  public:

    axString()                            { mText = (char*)""; }
    axString(char* txt)                   { mText = txt; }
    axString(const char* txt)             { mText = (char*)txt; }
  //axString(axString* str)               { mText = str->mText; }

    char*    ptr()                        { return mText; }
    void     set(char* txt)               { mText = txt; }
    bool     empty()                      { return mText[0]==0; }
    int      length()                     { return axStrlen(mText); } // return strlen(mText);

    char     operator [] (unsigned int i)          { return mText[i]; }

    void     operator = (axString str)    { mText = str.mText; }
  //void     operator = (axString& str)   { mText = str.mText; }
    void     operator = (axString* str)   { mText = str->mText; }
    void     operator = (char* txt)       { mText = txt; }
    void     operator = (const char* txt) { mText = (char*)txt; }

};

//----------------------------------------------------------------------

static axString STR_EMPTY((char*)"");
static axString STR_NONE((char*)"none");

#include "axArray.h"
typedef axArray<axString> axStrings;

//----------------------------------------------------------------------
#endif
