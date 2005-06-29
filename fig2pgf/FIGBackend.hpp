#ifndef H_FIGBackend
#define H_FIGBackend
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
class FIGFile;
class FIGArc;
class FIGEllipse;
class FIGPolyline;
class FIGSpline;
class FIGText;
//---------------------------------------------------------------------------
/// Base class for FIG processing backends
class FIGBackend
{
   protected:
   /// Process an element
   virtual void processArc(const FIGArc& o) = 0;
   /// Process an element
   virtual void processEllipse(const FIGEllipse& o) = 0;
   /// Process an element
   virtual void processPolyline(const FIGPolyline& o,bool close) = 0;
   /// Process an element
   virtual void processArcBox(const FIGPolyline& o) = 0;
   /// Process an element
   virtual void processPicture(const FIGPolyline& o) = 0;
   /// Process an element
   virtual void processSpline(const FIGSpline& o) = 0;
   /// Process an element
   virtual void processText(const FIGText& o) = 0;

   public:
   /// Constructor
   FIGBackend();
   /// Destructor
   virtual ~FIGBackend();

   /// Process the file
   virtual void process(const FIGFile& file);
};
//---------------------------------------------------------------------------
#endif
