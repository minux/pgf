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
#include "FIGLexer.hpp"
//---------------------------------------------------------------------------
void FIGLineObject::read(FIGLexer& in,FIGFile& /*file*/)
   // Read the object
{
   linestyle.read(in);
   linethickness=in.readInt();
   pencolor=in.readInt();
   fillcolor=in.readInt();
   depth=in.readInt();
   penstyle=in.readInt();
   areafill=in.readInt();
   stylevalue=in.readFloat();
}
//---------------------------------------------------------------------------
