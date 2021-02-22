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

#ifndef axScript_included
#define axScript_included
//----------------------------------------------------------------------
// TODO

// forth inspired stack-based script
// TODO:
// - error checking / memory safety
// - strings/pointers ." "string"
// - variables
// - repeat/until, while/wend, loop

// TODO: JIT compiler
// - http://code.google.com/p/asmjit/
// - http://homepage1.nifty.com/herumi/soft/xbyak_e.html

#include <stdio.h>
#include <stdlib.h> // atoi..
#include <string.h>
#include "core/axString.h"

//----------------------------------------------------------------------

//  ! # % & ? $ @
//  _ ^ ~ '
//  () [] {} <>
//  .: ,;

#define MAX_CODESIZE  65536
#define MAX_STACKSIZE 1024

// opcodes
#define op_None   0
#define op_Opcode 1
#define op_Word   2
#define op_Label  3
#define op_Int    4
#define op_Float  5
#define op_Exit   0xffff

// word  = call/return
// label = jump

// token types
#define tty_Unknown 0
#define tty_Word    1
#define tty_Int     2
#define tty_Float   3
#define tty_String  4
#define tty_Ptr     5

//// script states
//#define sst_Compile 1
//#define sst_Execute 2

class axScript;

//----------------------------------------------------------------------
// token
//----------------------------------------------------------------------

class axToken
{
  protected:
    axScript* mOwner;
    char      mName[16];
    //int       mType;
    //union     mValue
    //{
    //  int   i; // long?
    //  float f;
    //  void* p;
    //};
  public:
    axToken(axScript* aOwner, char* aName/*, int aType*/) { mOwner=aOwner; strcpy(mName,aName); /*mType=aType;*/ }
    inline char* name(void) { return mName; }
};

typedef axArray<axToken*> axTokens;

//----------------------------------------------------------------------
// word
//----------------------------------------------------------------------
// also used for: label, (string)

class axWord
{
  protected:
    char  mName[16];
    int   mPos;
  public:
    axWord(char* aName, int aPos) { strcpy(mName,aName); mPos=aPos; }
    inline char* name(void) { return mName; }
    inline void  pos(int aPos) { mPos=aPos; }
    inline int   pos(void) { return mPos; }
};

typedef axArray<axWord*> axWords;

//----------------------------------------------------------------------
// opcode
//----------------------------------------------------------------------

class axOpcode
{
  protected:
    axScript* mOwner;
    int       mIndex;
    char      mName[16];
  public:
    axOpcode(axScript* aOwner, axString aName) { mOwner=aOwner; strcpy(mName,aName.ptr()); }
    inline char* name(void) { return mName; }
    inline void  setOwner(axScript* aOwner) { mOwner=aOwner; }
    inline void  setIndex(int aIndex) { mIndex=aIndex; }
    virtual void compile(int aIndex) {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) {}
};

typedef axArray<axOpcode*> axOpcodes;

//----------------------------------------------------------------------
//
// script
//
//----------------------------------------------------------------------

class axScript
{
  protected:
    char*     mSource;
    axTokens  mTokens;
    axOpcodes mOpcodes;
    axWords   mWords;
    axWords   mLabels;
    int*      mCode;
    int*      mDataStack;
    int*      mCallStack;
    int*      mCondStack;

    int       mSrcSize;
    int       mCodeSize;

    int       mCodePos;
    int       mDataPos;
    int       mCallPos;
    int       mCondPos;
    int       mCurToken;

  public:

    axScript()
      {
        trace("axScript.constructor");
        mSrcSize    = 0;
        mSource     = NULL;
        mCode       = new int[MAX_CODESIZE];
        mDataStack  = new int[MAX_STACKSIZE];
        mCallStack  = new int[MAX_STACKSIZE];
        mCondStack  = new int[MAX_STACKSIZE];
        mCodePos    = 0;
        mDataPos    = 0;
        mCallPos    = 0;
        mCondPos    = 0;
        mCurToken   = 0;
        mTokens.clear();
        mOpcodes.clear();
        mWords.clear();
        mLabels.clear();
        #ifdef AX_SCRIPT_STDLIB
        append_stdlib();
        #endif
      }

    //----------

    virtual ~axScript()
      {
        delete[] mCode;
        delete[] mDataStack;
        delete[] mCallStack;
        delete[] mCondStack;
        deleteTokens();
        deleteOpcodes();
//        deleteWords();
//        deleteLabels();
        if (mSource) delete[] mSource;
      }

    //--------------------------------------------------

    void dumpCode(void)
      {
        printf("code size: %i\n",mCodeSize);
        for (int i=0; i<mCodeSize; i++)
        {
          int n = mCode[i];
          if ( ((i&7)==7) || (i==mCodeSize-1) ) printf("%08x\n",n);
          else printf("%08x ",n);
        }
      }

    //--------------------------------------------------

    inline int        codePos(void)       { return mCodePos; }
    inline void       codePos(int aPos)   { mCodePos = aPos; }
    inline int        next(void)          { return mCode[mCodePos++]; };
    inline void       writeCode(int aCode)  { mCode[mCodePos++] = aCode; }
    inline void       writeCodeAt(int aPos, int aCode)  { mCode[aPos] = aCode; }

    inline char*      nextToken(void)     { return mTokens[mCurToken++]->name(); }
    inline void       deleteTokens(void)  { for (int i=0; i<mTokens.size(); i++) delete mTokens[i]; }
    inline void       deleteWords(void)   { for (int i=0; i<mWords.size(); i++) delete mWords[i]; }

    inline int        numOpcodes(void)    { return mOpcodes.size(); }
    inline axOpcode*  opcode(int aIndex)  { return mOpcodes[aIndex]; }
    inline void       deleteOpcodes(void) { for (int i=0; i<mOpcodes.size(); i++) delete mOpcodes[i]; }

    inline void       pushData(int aData)   { mDataStack[mDataPos++] = aData; }
    inline void       pushCall(void)        { mCallStack[mCallPos++] = mCodePos; }
    inline int        popData(void)         { return mDataStack[--mDataPos]; }
    inline void       popCall(void)         { mCodePos = mCallStack[--mCallPos]; }

    inline int        peekData(void)        { return mDataStack[mDataPos-1]; }

    inline int        wordPos(int aWord)    { return mWords[aWord]->pos(); }
    inline int        labelPos(int aLabel)  { return mLabels[aLabel]->pos(); }
  //inline void       callWord(int aWord)   { pushCall(); mCodePos = mWords[aWord]->pos(); }

    inline void       pushCond(void)        { mCondStack[mCondPos++] = mCodePos; }
    inline int        popCond(void)         { return mCondStack[--mCondPos]; }

    //----------

    inline void   dupData(void)   { mDataStack[mDataPos++] = mDataStack[mDataPos-1]; }
    inline void   dropData(void)  { mDataPos--; }
    inline void   swapData(void)
      {
        int temp = mDataStack[mDataPos-1];
        mDataStack[mDataPos-1] = mDataStack[mDataPos-2];
        mDataStack[mDataPos-2] = temp;
      }
    inline void   incData(void)   { mDataStack[mDataPos-1]++; }
    inline void   decData(void)   { mDataStack[mDataPos-1]--; }

    //----------

    void appendOpcode(axOpcode* aOpcode)
      {
        aOpcode->setOwner( this );
        aOpcode->setIndex( mOpcodes.size() );
        mOpcodes.append(aOpcode);
      }

    //----------

    int findOpcode(char* token)
      {
        for (int i=0; i<mOpcodes.size(); i++) { if (strcmp(token,mOpcodes[i]->name())==0) return i; }
        return -1;
      }

    //----------

    void appendWord(axWord* aWord)
      {
        aWord->pos( mCodePos );
        mWords.append(aWord);
      }

    int findWord(char* token)
      {
        for (int i=0; i<mWords.size(); i++) { if (strcmp(token,mWords[i]->name())==0) return i; }
        return -1;
      }

    //----------

    void appendLabel(axWord* aWord)
      {
        aWord->pos( mCodePos );
        mLabels.append(aWord);
      }

    int findLabel(char* token)
      {
        for (int i=0; i<mLabels.size(); i++) { if (strcmp(token,mLabels[i]->name())==0) return i; }
        return -1;
      }

    //--------------------------------------------------

    int parse(axString aSource)
      {
        int res = 0;
        mSrcSize = aSource.length();
        mSource = new char[mSrcSize];
        memcpy(mSource,aSource.ptr(),mSrcSize);
        for (int i=0; i<mSrcSize; i++) if (mSource[i]<32) mSource[i]=' ';
        char* temp;
        char* delimiters = (char*)" ";
        char* token = NULL;
        //token  = strtok_r(mSource," ",&temp);  // the _r means reentrant, is this needed?
        token  = axStrtok(mSource," ");  // the _r means reentrant, is this needed?
        do { mTokens.append(new axToken(this,token/*,tty_Unknown*/)); }
        //while ((token=strtok_r(NULL,delimiters,&temp)));
        while ((token=axStrtok(NULL,delimiters)));
        return res;
      }

    //--------------------------------------------------

    virtual void compileOpcode(int aOpcode)
      {
        mOpcodes[aOpcode]->compile(aOpcode);
      }

    //----------

    virtual void compileWord(int aWord)
      {
        writeCode(op_Word);
        //writeCode( aWord );
        writeCode( wordPos(aWord) );
      }

    //----------

    virtual void compileLabel(int aLabel)
      {
        writeCode(op_Label);
        //writeCode( aWord );
        writeCode( labelPos(aLabel) );
      }

    //----------

    virtual void compileInt(int aValue)
      {
        writeCode(op_Int);
        writeCode(aValue);
      }

    //----------

    virtual void compileFloat(float aValue)
      {
        int value = *(int*)(&aValue);
        writeCode(op_Float);
        writeCode(value);
      }

    //--------------------------------------------------

    virtual int compile(axString aSource)
      {
        parse(aSource);
        mCurToken = 0;
        do
        {
          char* token = nextToken();
          int op = findOpcode(token);
          if (op>=0) compileOpcode(op);
          else
          {
            int wo = findWord(token);
            if (wo>=0) compileWord(wo);
            else
            {

              int la = findLabel(token);
              if (la>=0) compileLabel(la);
              else
              {
                if (strchr(token,'.')!=NULL) compileFloat(atof(token));
                else compileInt(atoi(token));
              }
            }
          }
        } while ( mCurToken<mTokens.size() );
        writeCode(op_Exit);
        mCodeSize = mCodePos;
        return mCodeSize;
      }

    //--------------------------------------------------
    /*
    the 'heart' of the vm (virtual machine).
    pretty rudimentary at the moment, even though we
    already have program flow, conditionals,
    functions,

    */

    // todo/idea:
    // split opcode bits into some kind of binary tree,

    virtual int execute(int aStart=0)
      {
        int result = 0;
        mCodePos = aStart;
        mDataPos = 0;
        mCallPos = 0;
        mCondPos = 0;
        int val;
        int opcode = next();
        while (opcode!=op_Exit)
        {
          switch(opcode)
          {
            //case op_None:
            //  break;
            case op_Opcode:
              val = next();
              mOpcodes[val]->execute();
              break;
            case op_Word: // gosub
              val = next();
              pushCall();
              mCodePos = val;
              break;
            case op_Label: // goto
              val = next();
              //pushCall();
              mCodePos = val;
              break;
            case op_Int:
              pushData(next());
              break;
            case op_Float:
              pushData(next());
              break;
            //case op_Exit:
            //  break;
          }
          opcode = next();
        }
        return result;
      }

    //--------------------------------------------------

    #ifdef AX_SCRIPT_STDLIB

    #include "axScriptStdlib.h"
    void append_stdlib(void)
      {
        // base
        appendOpcode( new opColon() );
        appendOpcode( new opSemiColon() );
        appendOpcode( new opDollar() );
        // stack (data)
        appendOpcode( new opDup() );
        appendOpcode( new opDrop() );
        appendOpcode( new opSwap() );
        // conditionals
        appendOpcode( new opEqual() );
        appendOpcode( new opNotEqual() );
        appendOpcode( new opGreater() );
        appendOpcode( new opLess() );
        // flow control
        appendOpcode( new opExit() );
        appendOpcode( new opIf() );
        appendOpcode( new opElse() );
        appendOpcode( new opEndif() );
        // arithmetic (int)
        appendOpcode( new opAdd() );
        appendOpcode( new opSub() );
        appendOpcode( new opMul() );
        appendOpcode( new opDiv() );
        appendOpcode( new opInc() );
        appendOpcode( new opDec() );
//        // io
        appendOpcode( new opPrintInt() );

      }

    #endif

};


//----------------------------------------------------------------------
#endif
