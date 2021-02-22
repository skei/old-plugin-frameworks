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

#ifndef axProgram_included
#define axProgram_included
//----------------------------------------------------------------------

class axProgram
{
  private:
    char      mName[33];
    int       mNumVal;
    float*    mValues;

  public:

    axProgram(axString aName, int aNumVal, float* aValues)
      {
        axStrncpy(mName,aName.ptr(),32);
        mNumVal = aNumVal;
        mValues = (float*)axMalloc( aNumVal*sizeof(float) );
        axMemcpy(mValues,aValues,aNumVal*sizeof(float));
      }

    axProgram(axString aName, int aNumVal)
      {
        axStrncpy(mName,aName.ptr(),32);
        mNumVal = aNumVal;
        mValues = (float*)axMalloc( aNumVal*sizeof(float) );
      }

    virtual ~axProgram()
      {
        if (mValues) axFree(mValues);
      }

    void      setName(axString aName)             { axStrncpy(mName,aName.ptr(),32); }
    axString  getName(void)                       { return mName; }
    void      setValue(int aIndex, float aValue)  { mValues[aIndex]=aValue; }
    float     getValue(int aIndex)                { return mValues[aIndex]; }

};

typedef axArray<axProgram*> axPrograms;

//----------------------------------------------------------------------
#endif
