#ifndef H_PGFWriter
#define H_PGFWriter
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
#include "LatexWriter.hpp"
//---------------------------------------------------------------------------
class PGFWriter : public LatexWriter
{
   private:
   PGFWriter(const PGFWriter&);
   void operator=(const PGFWriter&);

   /// Write a point
   void writePoint(const FIGObject::FloatPoint& point);
   /// Write a point
   void writePoint(const FIGObject::IntPoint& point);
   /// Write a vector
   void writeVect(const FIGObject::FloatPoint& point);
   /// Write a vector
   void writeVect(const FIGObject::IntPoint& point);

   /// Triggered before the real processing
   void preparationDone();

   /// Process an element
   void processArc(const FIGArc& o);
   /// Process an element
   void processEllipse(const FIGEllipse& o);
   /// Process an element
   void processPolyline(const FIGPolyline& o,bool close);
   /// Process an element
   void processArcBox(const FIGPolyline& o);
   /// Process an element
   void processPicture(const FIGPolyline& o);
   /// Process an element
   void processSpline(const FIGSpline& o);
   /// Process an element
   void processText(const FIGText& o);

   public:
   /// Constructor
   PGFWriter();
   /// Destructor
   ~PGFWriter();
};
//---------------------------------------------------------------------------
#endif
