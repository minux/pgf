/*
   Copyright (C) 2005 Thomas Neumann

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */
//---------------------------------------------------------------------------
#include "FIGObject.hpp"
#include "FIGLexer.hpp"
//---------------------------------------------------------------------------
void FIGObject::Arrow::read(FIGLexer& in)
   // Read it
{
   type=in.readInt();
   style=in.readInt();
   thickness=in.readFloat();
   width=in.readFloat();
   height=in.readFloat();
   in.endLine();
}
//---------------------------------------------------------------------------
void FIGObject::IntPoint::read(FIGLexer& in)
   // Read it
{
   x=in.readInt();
   y=in.readInt();
}
//---------------------------------------------------------------------------
void FIGObject::FloatPoint::read(FIGLexer& in)
   // Read it
{
   x=in.readFloat();
   y=in.readFloat();
}
//---------------------------------------------------------------------------
void FIGObject::LineStyle::read(FIGLexer& in)
   // Read it
{
   style=in.readInt();
}
//---------------------------------------------------------------------------
FIGObject::FIGObject()
   : next(0)
   // Constructor
{
}
//---------------------------------------------------------------------------
FIGObject::~FIGObject()
   // Destructor
{
   while (next) {
      FIGObject* n=next->next;
      next->next=0;
      delete next;
      next=n;
   }
}
//---------------------------------------------------------------------------
