#ifndef H_FIGText
#define H_FIGText
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
struct FIGText : public FIGObject
{
   enum Subtype { Subtype_Left, Subtype_Center, Subtype_Right };
   Subtype subtype;
   int color;
   int penstyle;
   int font;
   double fontsize;
   double angle;
   bool rigidtext;
   bool specialtext;
   bool postscriptfont;
   bool hiddentext;
   double height;
   double length;
   FloatPoint base;
   std::string text;

   /// Get the type
   Type getType() const { return Type_Text; }
   /// Read the object
   void read(FIGLexer& in,FIGFile& file);
   /// Get the bounding box
   Bounds getBounds() const;
};
//---------------------------------------------------------------------------
#endif
