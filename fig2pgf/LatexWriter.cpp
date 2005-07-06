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
#include "FIGFile.hpp"
#include "FIGEllipse.hpp"
#include "FIGPolyline.hpp"
#include "FIGText.hpp"
#include <cstdio>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
LatexWriter::LatexWriter()
   : handle(0),fig(0)
   // Constructor
{
}
//---------------------------------------------------------------------------
LatexWriter::~LatexWriter()
   // Destructor
{
   if (handle)
      fclose(handle);
}
//---------------------------------------------------------------------------
bool LatexWriter::create(const string& name)
   // Open
{
   if (handle)
      fclose(handle);
   handle=fopen(name.c_str(),"wt");
   return handle;
}
//---------------------------------------------------------------------------
void LatexWriter::preparationDone()
   // Triggered before the real processing
{
}
//---------------------------------------------------------------------------
void LatexWriter::process(const FIGFile& file)
   // Process the file
{
   fig=&file;
   // Scale to the specified size
   scalex=0.393700787/static_cast<double>(file.getResolution());
   scaley=scalex;
   // Determine the bounding box
   bounds=FIGObject::Bounds();
   for (const FIGObject* iter=file.getObjects();iter;iter=iter->next)
      bounds.combine(iter->getBounds());

   // Processing done
   preparationDone();

   // Process the data
   FIGBackend::process(file);
}
//---------------------------------------------------------------------------
string LatexWriter::escapeLatex(const string& str)
   // Escape Latex specials
{
   string result;
   for (string::const_iterator iter=str.begin();iter!=str.end();++iter)
      switch (*iter) {
	 case '$': result+="{\\$}"; break;
	 case '%': result+="{\\%}"; break;
	 case '_': result+="{\\_}"; break;
	 case '}': result+="{\\}}"; break;
	 case '&': result+="{\\&}"; break;
	 case '#': result+="{\\#}"; break;
	 case '{': result+="{\\{}"; break;
	 case '^': result+="{\\textasciicircum}"; break;
	 case '~': result+="{\\textasciitilde}"; break;
	 case '\\': result+="{\\textbackslash}"; break;
	 case '|': result+="{\\textbar}"; break;
	 case '<': result+="{\\textless}"; break;
	 case '>': result+="{\\textgreater}"; break;
	 case '\"': result+="{\\textquotedblright}"; break;
	 default: result+=*iter; break;
      }
   return result;
}
//---------------------------------------------------------------------------
string LatexWriter::ftoa(double value)
   // Create a tex friendly decimal string
{
   // Use f instead of g, as latex does not like exponential notation
   char buffer[50];
   snprintf(buffer,sizeof(buffer),"%20.10f",value);
   // Remove trailing spaces
   if (strchr(buffer,'.')) {
      char* end=buffer+strlen(buffer);
      while ((end>buffer)&&(end[-1]=='0'))
         *(--end)=0;
      if ((end>buffer)&&(end[-1]=='.'))
         *(--end)=0;
   }
   // Remove leading spaces
   char* start=buffer;
   while (start[0]==' ') start++;
   // Return the result
   return string(start);
}
//---------------------------------------------------------------------------
void LatexWriter::write(const std::string& str)
   // Write some text
{
   fprintf(handle,"%s",str.c_str());
}
//---------------------------------------------------------------------------
string LatexWriter::getColor(int slot)
   // Get a color string
{
   if (slot<0) return "{0,0,0}";
   FIGFile::Color c=fig->getColor(slot);
   if (!c.set) return "{0,0,0}";

   char buffer[50];
   double r=c.r,g=c.g,b=c.b;
   r/=255.0; g/=255.0; b/=255.0;
   snprintf(buffer,sizeof(buffer),"{%g,%g,%g}",r,g,b);
   return string(buffer);
}
//---------------------------------------------------------------------------
void LatexWriter::writeColor(int slot)
   // Write a color
{
   if (slot<0) return;
   FIGFile::Color c=fig->getColor(slot);
   if (!c.set) return;
   if ((!c.r)&&(!c.g)&&(!c.b)) return;

   write("\\color[rgb]"+getColor(slot));
}
//---------------------------------------------------------------------------
string LatexWriter::getFillColor(int slot,int style)
   // Get a fill color string
{
   if (slot<0) return "{1,1,1}";
   FIGFile::Color c=fig->getColor(slot);
   if (!c.set) return "{1,1,1}";
   // Adjust to style
   double r=c.r,g=c.g,b=c.b;
   r/=255.0; g/=255.0; b/=255.0;
   if ((style>=0)&&(style<=20)) {
      // Special for black
      if (!slot) {
	 r=g=b=(20-style)/20.0;
      } else {
         r*=style/20.0; g*=style/20.0; b*=style/20.0;
      }
   } else if ((style>=21)&&(style<=40)) {
      double scale=(style-20)/20.0;
      r=1.0-((1.0-r)*scale); g=1.0-((1.0-g)*scale); b=1.0-((1.0-b)*scale);
   }
   char buffer[50];
   snprintf(buffer,sizeof(buffer),"{%g,%g,%g}",r,g,b);
   return string(buffer);
}
//---------------------------------------------------------------------------
void LatexWriter::writeFillColor(int slot,int style)
   // Write a fill color
{
   if (slot<0) return;

   write("\\color[rgb]"+getFillColor(slot,style));
}
//---------------------------------------------------------------------------
void LatexWriter::writeFontSize(double size)
   // Write a color
{
   // Scale down a bit, should probably be an option...
   size/=1.2;

   fprintf(handle,"\\fontsize{%dpt}{%dpt}\\selectfont ",static_cast<int>(size),static_cast<int>(size*1.2));
}
//---------------------------------------------------------------------------
