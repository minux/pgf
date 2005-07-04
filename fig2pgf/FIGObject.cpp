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
FIGObject::Bounds::Bounds()
   // Constructor
{
   upperLeft.x=1; upperLeft.y=0;
   lowerRight.x=0; lowerRight.y=0;
}
//---------------------------------------------------------------------------
void FIGObject::Bounds::combine(const FloatPoint& p)
   // Combine
{
   if (empty()) {
      upperLeft=p; lowerRight=p;
   } else {
      if (p.x<upperLeft.x) upperLeft.x=p.x;
      if (p.y<upperLeft.y) upperLeft.y=p.y;
      if (p.x>lowerRight.x) lowerRight.x=p.x;
      if (p.y>lowerRight.y) lowerRight.y=p.y;
   }
}
//---------------------------------------------------------------------------
void FIGObject::Bounds::combine(const IntPoint& p)
   // Combine
{
   FloatPoint p2; p2.x=p.x; p2.y=p.y;
   combine(p2);
}
//---------------------------------------------------------------------------
void FIGObject::Bounds::combine(const Bounds& p)
   // Combine
{
   if (p.empty()) return;

   combine(p.upperLeft);
   combine(p.lowerRight);
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
