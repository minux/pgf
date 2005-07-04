#ifndef H_FIGPolyline
#define H_FIGPolyline
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
#include <vector>
//---------------------------------------------------------------------------
struct FIGPolyline : public FIGLineObject
{
   enum Subtype { Subtype_Polyline, Subtype_Box, Subtype_Polygon, Subtype_ArcBox, Subtype_Picture };
   Subtype subtype;
   int joinstyle;
   int capstyle;
   int radius;
   bool flipped;
   std::string filename;

   Arrow forwardarrow;
   Arrow backwardarrow;
   std::vector<IntPoint> points;

   /// Constructor
   FIGPolyline();
   /// Destructor
   ~FIGPolyline();

   /// Get the type
   Type getType() const { return Type_Polyline; }
   /// Read the object
   void read(FIGLexer& in,FIGFile& file);
   /// Get the bounding box
   Bounds getBounds() const;
};
//---------------------------------------------------------------------------
#endif
