#ifndef H_FIGSpline
#define H_FIGSpline
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
struct FIGSpline : public FIGLineObject
{
   enum Subtype { Subtype_OpenApproximated, Subtype_ClosedApproximated, Subtype_OpenInterpolated, Subtype_ClosedInterpolated, Subtype_OpenXSpline, Subtype_ClosedXSpline };
   Subtype subtype;
   int capstyle;

   Arrow forwardarrow;
   Arrow backwardarrow;
   std::vector<IntPoint> points;
   std::vector<double> controlPoints;

   /// Get the type
   Type getType() const { return Type_Spline; }
   /// Read the object
   void read(FIGLexer& in,FIGFile& file);
   /// Get the bounding box
   Bounds getBounds() const;
};
//---------------------------------------------------------------------------
#endif
