#ifndef H_FIGFile
#define H_FIGFile
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
class FIGObject;
class FIGLexer;
class FIGCompound;
//---------------------------------------------------------------------------
class FIGFile
{
   public:
   /// Colors
   struct Color { int r,g,b; bool set; };

   private:
   /// Orientation
   enum Orientation { Orientation_Landscape, Orientation_Portrait };
   /// Orientation
   Orientation orientation;
   /// Justification
   enum Justification { Justification_Center, Justification_FlushLeft };
   /// Justification
   Justification justification;
   /// Units
   enum Units { Units_Metric, Units_Inches };
   /// Units
   Units units;
   /// Paper size
   enum Papersize { Papersize_Letter, Papersize_Legal, Papersize_Ledger, Papersize_Tabloid, Papersize_A, Papersize_B, Papersize_C, Papersize_D, Papersize_E, Papersize_A4, Papersize_A3, Papersize_A2, Papersize_A1, Papersize_A0, Papersize_B5 };
   /// Paper size
   Papersize papersize;
   /// Magnigication
   double magnification;
   /// Multiple page
   enum Multiplepage { Multiplepage_Single, Multiplepage_Multiple };
   /// Multiple page
   Multiplepage multiplepage;
   /// Transparent color
   int transparentcolor;
   /// Resolution
   int resolution;
   /// Coord system
   enum Coordsystem { Coordsystem_lowerleft, Coordsystem_upperleft };
   /// Coord system
   Coordsystem coordsystem;
   /// Colors
   Color colors[32+512];

   /// The objects
   FIGObject* objects;

   FIGFile(const FIGFile&);
   void operator=(const FIGFile&);

   /// Reset the colors
   void resetColors();

   /// Read objects
   FIGObject* readObjects(FIGLexer& in,bool inCompound);

   friend class FIGCompound;

   public:
   /// Constructor
   FIGFile();
   /// Destructor
   ~FIGFile();

   /// Read it
   void read(FIGLexer& in);
   /// Get the first object
   FIGObject* getObjects() { return objects; }
   /// Get the first object
   const FIGObject* getObjects() const { return objects; }
   /// Get the resolution
   int getResolution() const { return resolution; }
   /// Get a color
   const Color& getColor(unsigned id) const { return colors[id]; }
};
//---------------------------------------------------------------------------
#endif
