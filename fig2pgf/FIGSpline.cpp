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
#include "FIGSpline.hpp"
#include "FIGLexer.hpp"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
void FIGSpline::read(FIGLexer& in,FIGFile& file)
   // Read the object
{
   int i=in.readInt();
   if (i==0) subtype=Subtype_OpenApproximated; else
   if (i==1) subtype=Subtype_ClosedApproximated; else
   if (i==2) subtype=Subtype_OpenInterpolated; else
   if (i==3) subtype=Subtype_ClosedInterpolated; else
   if (i==4) subtype=Subtype_OpenXSpline; else
   if (i==5) subtype=Subtype_ClosedXSpline; else
       in.error("unknown subtype");

   FIGLineObject::read(in,file);

   capstyle=in.readInt();
   forwardarrow.used=in.readBool();
   backwardarrow.used=in.readBool();
   int npoints=in.readInt();
   points.resize(npoints);
   controlPoints.resize(npoints);
   in.endLine();

   if (forwardarrow.used)
      forwardarrow.read(in);
   if (backwardarrow.used)
      backwardarrow.read(in);

   if (npoints) {
      for (vector<IntPoint>::iterator iter=points.begin();iter!=points.end();++iter) {
	 in.acceptNL();
         (*iter).read(in);
      }

      for (vector<double>::iterator iter=controlPoints.begin();iter!=controlPoints.end();++iter) {
	 in.acceptNL();
         (*iter)=in.readFloat();
      }

      in.endLine();
   }
}
//---------------------------------------------------------------------------
double FIGSpline::getMaxY() const
   // Return the maximum y coordinate
{
   if (points.size()>0) {
      double m=(*(points.begin())).y;
      for (vector<IntPoint>::const_iterator iter=points.begin();iter!=points.end();++iter)
         if ((*iter).y>m)
            m=(*iter).y;
      return m;
   } else return 0;
}
//---------------------------------------------------------------------------
