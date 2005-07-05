#ifndef H_LatexWriter
#define H_LatexWriter
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
#include "FIGBackend.hpp"
#include "FIGObject.hpp"
#include <string>
#include <cstdio>
//---------------------------------------------------------------------------
class FIGFile;
//---------------------------------------------------------------------------
// Abstract base class for common Latex functionality
class LatexWriter : public FIGBackend
{
   private:
   /// The file
   std::FILE* handle;
   /// The complete fig
   const FIGFile* fig;
   protected:
   /// Bounding box
   FIGObject::Bounds bounds;
   /// Scale factors
   double scalex,scaley;

   /// Triggered before the real processing
   virtual void preparationDone();

   /// Escape Latex specials
   std::string escapeLatex(const std::string& str);
   /// Create a tex friendly decimal string
   std::string ftoa(double value);
   /// Write some text
   void write(const std::string& str);
   /// Get a color string
   std::string getColor(int id);
   /// Write a color
   void writeColor(int id);
   /// Get a fill color string
   std::string getFillColor(int id,int style);
   /// Write a fill color
   void writeFillColor(int id,int style);
   /// Write a font size
   void writeFontSize(double size);

   private:
   LatexWriter(const LatexWriter&);
   void operator=(const LatexWriter&);

   public:
   /// Constructor
   LatexWriter();
   /// Destructor
   ~LatexWriter();

   /// Create
   bool create(const std::string& name);
   /// Process the file
   virtual void process(const FIGFile& file);
};
//---------------------------------------------------------------------------
#endif
