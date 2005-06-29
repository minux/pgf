#ifndef H_FIGEllipse
#define H_FIGEllipse
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
struct FIGEllipse : public FIGLineObject
{
   enum Subtype { Subtype_EllipseByRadii, Subtype_EllipseByDiameters, Subtype_CircleByRadius, Subtype_CircleByDiameter };
   Subtype subtype;
   int direction;
   double angle;
   IntPoint center;
   IntPoint radius;
   IntPoint start;
   IntPoint end;

   /// Get the type
   Type getType() const { return Type_Ellipse; }
   /// Read the object
   void read(FIGLexer& in,FIGFile& file);

   /// Get the maximum y coordinate
   double getMaxY() const;
};
//---------------------------------------------------------------------------
#endif
