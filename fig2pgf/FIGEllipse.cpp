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
#include "FIGEllipse.hpp"
#include "FIGLexer.hpp"
//---------------------------------------------------------------------------
void FIGEllipse::read(FIGLexer& in,FIGFile& file)
   // Read the object
{
   int i=in.readInt();
   if (i==1) subtype=Subtype_EllipseByRadii; else
   if (i==2) subtype=Subtype_EllipseByDiameters; else
   if (i==3) subtype=Subtype_CircleByRadius; else
   if (i==4) subtype=Subtype_CircleByDiameter; else
      in.error("unknown subtype");

   FIGLineObject::read(in,file);

   direction=in.readInt();
   angle=in.readFloat();
   center.read(in);
   radius.read(in);
   start.read(in);
   end.read(in);

   in.endLine();
}
//---------------------------------------------------------------------------
double FIGEllipse::getMaxY() const
   // Return the maximum y coordinate
{
   return center.y+radius.y;
}
//---------------------------------------------------------------------------
