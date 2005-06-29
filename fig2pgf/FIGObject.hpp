#ifndef H_FIGObject
#define H_FIGObject
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
#include <string>
//---------------------------------------------------------------------------
class FIGLexer;
class FIGFile;
//---------------------------------------------------------------------------
struct FIGObject
{
   /// Possible types
   enum Type { Type_Arc, Type_Compound, Type_Ellipse, Type_Polyline, Type_Spline, Type_Text };

   /// An arrow
   struct Arrow {
      /// The type
      int type;
      /// The style
      int style;
      /// The thickness
      double thickness;
      /// The width;
      double width;
      /// The height
      double height;
      /// Used?
      bool used;

      /// Constructor
      Arrow() : used(false) {}

      /// Read it
      void read(FIGLexer& in);
   };
   /// A point
   struct IntPoint {
      int x,y;

      /// Read it
      void read(FIGLexer& in);
   };
   /// A point
   struct FloatPoint {
      double x,y;

      /// Read it
      void read(FIGLexer& in);
   };
   /// Linestyles
   struct LineStyle {
      int style;

      /// Read it
      void read(FIGLexer& in);
   };

   /// The next entry
   FIGObject* next;
   /// The depth
   int depth;

   public:
   /// Constructor
   FIGObject();
   /// Destructor
   virtual ~FIGObject();

   /// Get the type
   virtual Type getType() const = 0;
   /// Read the object
   virtual void read(FIGLexer& in,FIGFile& file) = 0;
   /// Get the maximum y coordinate
   virtual double getMaxY() const = 0;
};
//---------------------------------------------------------------------------
#endif
