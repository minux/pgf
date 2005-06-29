#ifndef H_FIGArc
#define H_FIGArc
//---------------------------------------------------------------------------
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
#include "FIGLineObject.hpp"
//---------------------------------------------------------------------------
struct FIGArc : public FIGLineObject
{
   enum Subtype { Subtype_Open, Subtype_Closed };
   Subtype subtype;
   int capstyle;
   enum Direction { Direction_clockwise, Direction_counterclockwise };
   Direction direction;
   FloatPoint center,p1,p2,p3;

   Arrow forwardarrow;
   Arrow backwardarrow;

   /// Get the type
   Type getType() const { return Type_Arc; }
   /// Read the object
   void read(FIGLexer& in,FIGFile& file);
   /// Get the maximum y coordinate
   double getMaxY() const;
};
//---------------------------------------------------------------------------
#endif
