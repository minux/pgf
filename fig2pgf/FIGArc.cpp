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
#include "FIGArc.hpp"
#include "FIGLexer.hpp"
//---------------------------------------------------------------------------
void FIGArc::read(FIGLexer& in,FIGFile& file)
   // Read the object
{
   int i=in.readInt();
   if (i==1) subtype=Subtype_Open; else
   if (i==2) subtype=Subtype_Closed; else
       in.error("unknown subtype");

   FIGLineObject::read(in,file);

   capstyle=in.readInt();

   i=in.readInt();
   if (i==0) direction=Direction_clockwise; else
   if (i==1) direction=Direction_counterclockwise; else
      in.error("unknown direction");

   forwardarrow.used=in.readBool();;
   backwardarrow.used=in.readBool();;

   center.read(in);
   p1.read(in);
   p2.read(in);
   p3.read(in);
   in.endLine();

   if (forwardarrow.used)
      forwardarrow.read(in);
   if (backwardarrow.used)
      backwardarrow.read(in);
}
//---------------------------------------------------------------------------
FIGObject::Bounds FIGArc::getBounds() const
   // Get the bounding box
{
   Bounds b;
   b.combine(center);
   b.combine(p1);
   b.combine(p2);
   b.combine(p3);
   return b;
}
//---------------------------------------------------------------------------
