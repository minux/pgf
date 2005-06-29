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
#include "FIGCompound.hpp"
#include "FIGLexer.hpp"
#include "FIGFile.hpp"
//---------------------------------------------------------------------------
FIGCompound::FIGCompound()
   : childs(0)
   // Constructor
{
}
//---------------------------------------------------------------------------
FIGCompound::~FIGCompound()
   // Destructor
{
   delete childs;
}
//---------------------------------------------------------------------------
void FIGCompound::read(FIGLexer& in,FIGFile& file)
   // Read the object
{
   upperleft.read(in);
   lowerright.read(in);
   in.endLine();

   delete childs;
   childs=0;
   childs=file.readObjects(in,true);

   depth=0;
   for (FIGObject* iter=childs;iter;iter=iter->next)
       if (iter->depth>depth)
          depth=iter->depth;
}
//---------------------------------------------------------------------------
double FIGCompound::getMaxY() const
   // Return the maximum y coordinate
{
   if (childs) {
      double m=childs->getMaxY();
      for (FIGObject* iter=childs->next;iter;iter=iter->next) {
	 double m2=iter->getMaxY();
	 if (m2>m) m=m2;
      }
      return m;
   } else return 0;
}
//---------------------------------------------------------------------------
