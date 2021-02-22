/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/
//----------------------------------------------------------------------
#ifndef h_Format_Exe_included
#define h_Format_Exe_included
//----------------------------------------------------------------------

#include "src/h_Descriptor.h"
#include "src/h_Instance.h"

class h_Format
{
  private:
    h_Platform* m_Platform;

  public:
    h_Format();
    ~h_Format();
    h_Descriptor* createDescriptor(void);
    h_Instance*   createInstance(h_Host* a_Host,h_Descriptor* a_Descriptor);
    int           entrypoint(void* a_Ptr);
    h_String      getName(void) { return "exe"; }
};

//----------------------------------------------------------------------
#endif

