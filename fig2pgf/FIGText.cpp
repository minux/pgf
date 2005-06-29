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
#include "FIGText.hpp"
#include "FIGLexer.hpp"
//---------------------------------------------------------------------------
void FIGText::read(FIGLexer& in,FIGFile& /*file*/)
   // Read the object
{
   int i=in.readInt();
   if (i==0) subtype=Subtype_Left; else
   if (i==1) subtype=Subtype_Center; else
   if (i==2) subtype=Subtype_Right; else
      in.error("unknown subtype");
   color=in.readInt();
   depth=in.readInt();
   penstyle=in.readInt();
   font=in.readInt();
   fontsize=in.readFloat();
   angle=in.readFloat();

   int fontflags=in.readInt();
   rigidtext=fontflags&1;
   specialtext=fontflags&2;
   postscriptfont=fontflags&4;
   hiddentext=fontflags&8;

   height=in.readFloat();
   length=in.readFloat();

   base.read(in);
   text=in.readString();

   in.endLine();
}
//---------------------------------------------------------------------------
double FIGText::getMaxY() const
   // Return the maximum y coordinate
{
   return base.y;
}
//---------------------------------------------------------------------------
