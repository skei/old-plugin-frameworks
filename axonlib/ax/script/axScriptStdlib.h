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

#ifndef axScriptStdlib_included
#define axScriptStdlib_included
//----------------------------------------------------------------------
/*

: ; $
DUP DROP SWAP
= <> > <
EXIT
IF ELSE ENDIF
+ - * / 1+ 1-
.

*/
//----------------------------------------------------------------------

#include "script/axScript.h"

#define PUSH    mOwner->pushData
#define WRITE   mOwner->writeCode

#define POP     mOwner->popData()
#define NEXT    mOwner->next()
#define NEXTSTR mOwner->nextToken()
#define CODE    mOwner->codePos()

#define CALL    mOwner->codePos
#define RETURN  mOwner->popCall()

//----------------------------------------------------------------------
// base
//----------------------------------------------------------------------

// : word (gosub)

class opColon : public axOpcode
{
  public:
    opColon() : axOpcode(NULL,":") {}
    //virtual void compile(int aIndex) { mOwner->appendWord(new axWord( mOwner->nextToken(), mOwner->codePos() )); }
    virtual void compile(int aIndex) { mOwner->appendWord(new axWord( NEXTSTR, CODE )); }
    //virtual void execute(void) {)
};

//----------

// ; return

class opSemiColon : public axOpcode
{
  public:
    opSemiColon() : axOpcode(NULL,";") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { mOwner->popCall(); }
    virtual void execute(void) { RETURN; }
};

//----------

// $ label (goto)

class opDollar : public axOpcode
{
  public:
    opDollar() : axOpcode(NULL,"$") {}
    //virtual void compile(int aIndex) { mOwner->appendLabel(new axWord( mOwner->nextToken(), mOwner->codePos() )); }
    virtual void compile(int aIndex) { mOwner->appendLabel(new axWord(NEXTSTR,CODE)); }
    //virtual void execute(void) {)
};

//----------------------------------------------------------------------
// stack (data)
//----------------------------------------------------------------------

// DUP
class opDup : public axOpcode
{
  public:
    opDup() : axOpcode(NULL,"DUP") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    virtual void execute(void) { mOwner->dupData(); }
};

//----------

// DROP
class opDrop : public axOpcode
{
  public:
    opDrop() : axOpcode(NULL,"DROP") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    virtual void execute(void) { mOwner->dropData(); }
};

//----------

// SWAP
class opSwap : public axOpcode
{
  public:
    opSwap() : axOpcode(NULL,"SWAP") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    virtual void execute(void) { mOwner->swapData(); }
};



//----------------------------------------------------------------------
// conditionals
//----------------------------------------------------------------------

// =
class opEqual : public axOpcode
{
  public:
    opEqual() : axOpcode(NULL,"=") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { int n1=mOwner->popData(); int n2=mOwner->popData(); mOwner->pushData(n2==n1); }
    virtual void execute(void) { int n1=POP; int n2=POP; PUSH(n2==n1); }
};

//----------

// <>
class opNotEqual : public axOpcode
{
  public:
    opNotEqual() : axOpcode(NULL,"<>") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { int n1=mOwner->popData(); int n2=mOwner->popData(); mOwner->pushData(n2!=n1); }
    virtual void execute(void) { int n1=POP; int n2=POP; PUSH(n2!=n1); }
};

//----------

// >
class opGreater : public axOpcode
{
  public:
    opGreater() : axOpcode(NULL,">") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { int n1=mOwner->popData(); int n2=mOwner->popData(); mOwner->pushData(n2>n1); }
    virtual void execute(void) { int n1=POP; int n2=POP; PUSH(n2>n1); }
};

//----------

// <
class opLess : public axOpcode
{
  public:
    opLess() : axOpcode(NULL,"<") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { int n1=mOwner->popData(); int n2=mOwner->popData(); mOwner->pushData(n2<n1); }
    virtual void execute(void) { int n1=POP; int n2=POP; PUSH(n2<n1); }
};

//----------------------------------------------------------------------
// flow control
//----------------------------------------------------------------------

// EXIT
class opExit : public axOpcode
{
  public:
    opExit() : axOpcode(NULL,"EXIT") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Exit); }
    virtual void compile(int aIndex) { WRITE(op_Exit); }
    //virtual void execute(void) {}
};

//----------

// IF
class opIf : public axOpcode
{
  public:
    opIf() : axOpcode(NULL,"IF") {}
    virtual void compile(int aIndex)
      {
        //mOwner->writeCode(op_Opcode);
        //mOwner->writeCode(aIndex);
        WRITE(op_Opcode);
        WRITE(aIndex);
        mOwner->pushCond();
        //mOwner->writeCode(0);   // placeholder
        WRITE(0);
      }
    virtual void execute(void)
      {
        //int cond = mOwner->popData();
        //int pos  = mOwner->next();
        int cond = POP;
        int pos  = NEXT;
        //if (!cond) mOwner->codePos(pos);
        if (!cond) CALL(pos);
      }
};

//----------

// ELSE
class opElse : public axOpcode
{
  public:
    opElse() : axOpcode(NULL,"ELSE") {}
    virtual void compile(int aIndex)
      {
        int pos = mOwner->popCond();
        //mOwner->writeCode(op_Opcode);
        //mOwner->writeCode(aIndex);
        WRITE(op_Opcode);
        WRITE(aIndex);
        mOwner->pushCond();
        //mOwner->writeCode(0);   // placeholder
        WRITE(0);
        //mOwner->writeCodeAt(pos,mOwner->codePos());
        mOwner->writeCodeAt(pos,CODE);
      }
    //virtual void execute(void) { mOwner->codePos( mOwner->next() ); }
    virtual void execute(void) { CALL(NEXT); }
};

// ENDIF
class opEndif : public axOpcode
{
  public:
    opEndif() : axOpcode(NULL,"ENDIF") {}
    virtual void compile(int aIndex)
      {
        int pos = mOwner->popCond();
        //mOwner->writeCodeAt(pos,mOwner->codePos());
        mOwner->writeCodeAt(pos,CODE);
      }
    //virtual void execute(void) {}
};


//----------------------------------------------------------------------
// arithmetic (int)
//----------------------------------------------------------------------

// +
class opAdd : public axOpcode
{
  public:
    opAdd() : axOpcode(NULL,"+") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { mOwner->pushData( mOwner->popData() + mOwner->popData() ); }
    virtual void execute(void) { int n1=POP; int n2=POP; PUSH(n2+n1); }
};

//----------

// -
class opSub : public axOpcode
{
  public:
    opSub() : axOpcode(NULL,"-") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { mOwner->pushData( mOwner->popData() - mOwner->popData() ); }
    virtual void execute(void) { int n1=POP; int n2=POP; PUSH(n2-n1); }
};

//----------

// *
class opMul : public axOpcode
{
  public:
    opMul() : axOpcode(NULL,"*") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { mOwner->pushData( mOwner->popData() * mOwner->popData() ); }
    virtual void execute(void) { int n1=POP; int n2=POP; PUSH(n2*n1); }
};

//----------

// /
class opDiv : public axOpcode
{
  public:
    opDiv() : axOpcode(NULL,"/") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { mOwner->pushData( mOwner->popData() / mOwner->popData() ); }
    virtual void execute(void) { int n1=POP; int n2=POP; PUSH(n2/n1); }
};

//----------

// 1+
class opInc : public axOpcode
{
  public:
    opInc() : axOpcode(NULL,"1+") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { mOwner->pushData( mOwner->popData() + mOwner->popData() ); }
    virtual void execute(void) { mOwner->incData(); }
};

//----------

// 1-
class opDec : public axOpcode
{
  public:
    opDec() : axOpcode(NULL,"1-") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { mOwner->pushData( mOwner->popData() + mOwner->popData() ); }
    virtual void execute(void) { mOwner->decData(); }
};


//----------------------------------------------------------------------
// io
//----------------------------------------------------------------------

// .
class opPrintInt : public axOpcode
{
  public:
    opPrintInt() : axOpcode(NULL,".") {}
    //virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void compile(int aIndex) { WRITE(op_Opcode); WRITE(aIndex); }
    //virtual void execute(void) { printf("%i\n",mOwner->popData()); }
    virtual void execute(void) { printf("%i\n",POP); }
};

//----------------------------------------
// to be sure there's no conflict with other parts of the library

#undef PUSH
#undef WRITE
#undef POP
#undef NEXT
#undef NEXTSTR
#undef CODE
#undef CALL
#undef RETURN


//----------------------------------------------------------------------
#endif
