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
#include "FIGBackend.hpp"
#include "FIGFile.hpp"
#include "FIGObject.hpp"
#include "FIGArc.hpp"
#include "FIGCompound.hpp"
#include "FIGEllipse.hpp"
#include "FIGPolyline.hpp"
#include "FIGSpline.hpp"
#include "FIGText.hpp"
#include <map>
#include <vector>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
FIGBackend::FIGBackend()
   // Constructor
{
}
//---------------------------------------------------------------------------
FIGBackend::~FIGBackend()
   // Destructor
{
}
//---------------------------------------------------------------------------
static void sortObjects(map<int,vector<const FIGObject*> >& objects,const FIGObject* o)
   // Sort objects
{
   if (!o) return;
   if (o->getType()==FIGObject::Type_Compound) {
      const FIGCompound* c=static_cast<const FIGCompound*>(o);
      for (const FIGObject* iter=c->childs;iter;iter=iter->next)
         sortObjects(objects,iter);
   } else {
      objects[-(o->depth)].push_back(o);
   }
}
//---------------------------------------------------------------------------
void FIGBackend::process(const FIGFile& file)
   // Process the file
{
   // Sort all objects
   map<int,vector<const FIGObject*> > objects;
   for (const FIGObject* iter=file.getObjects();iter;iter=iter->next)
      sortObjects(objects,iter);
   // Process the objects
   for (map<int,vector<const FIGObject*> >::const_iterator iter=objects.begin();iter!=objects.end();++iter)
      for (vector<const FIGObject*>::const_iterator iter2=(*iter).second.begin();iter2!=(*iter).second.end();++iter2) {
	 const FIGObject* o=(*iter2);
	 switch (o->getType()) {
	    case FIGObject::Type_Arc: processArc(*static_cast<const FIGArc*>(o)); break;
	    case FIGObject::Type_Ellipse: processEllipse(*static_cast<const FIGEllipse*>(o)); break;
	    case FIGObject::Type_Polyline:
	       switch (static_cast<const FIGPolyline*>(o)->subtype) {
		  case FIGPolyline::Subtype_Polyline: processPolyline(*static_cast<const FIGPolyline*>(o),false); break;
		  case FIGPolyline::Subtype_Box: processPolyline(*static_cast<const FIGPolyline*>(o),true); break;
		  case FIGPolyline::Subtype_Polygon: processPolyline(*static_cast<const FIGPolyline*>(o),true); break;
		  case FIGPolyline::Subtype_ArcBox: processArcBox(*static_cast<const FIGPolyline*>(o)); break;
		  case FIGPolyline::Subtype_Picture: processPicture(*static_cast<const FIGPolyline*>(o)); break;
		  default: break;
	       }
	       break;
	    case FIGObject::Type_Spline: processSpline(*static_cast<const FIGSpline*>(o)); break;
	    case FIGObject::Type_Text: processText(*static_cast<const FIGText*>(o)); break;
	    default: break;
	 }
      }
}
//---------------------------------------------------------------------------
