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
#include "PGFWriter.hpp"
#include "FIGFile.hpp"
#include "FIGEllipse.hpp"
#include "FIGPolyline.hpp"
#include "FIGText.hpp"
#include <cstdio>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
PGFWriter::PGFWriter()
   : handle(0),fig(0)
   // Constructor
{
}
//---------------------------------------------------------------------------
PGFWriter::~PGFWriter()
   // Destructor
{
   if (handle)
      fclose(handle);
}
//---------------------------------------------------------------------------
bool PGFWriter::create(const string& name)
   // Open
{
   if (handle)
      fclose(handle);
   handle=fopen(name.c_str(),"wt");
   return handle;
}
//---------------------------------------------------------------------------
void PGFWriter::process(const FIGFile& file)
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
   // Output the expected bounding box
   fprintf(handle,"%% bb {%gmm}{%gmm}\n",
      (bounds.lowerRight.x-bounds.upperLeft.x)*scalex*10.0,
      (bounds.lowerRight.y-bounds.upperLeft.y)*scaley*10.0);

   // Process the data
   FIGBackend::process(file);
}
//---------------------------------------------------------------------------
static string escapeLatex(const string& str)
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
void PGFWriter::write(const std::string& str)
   // Write some text
{
   fprintf(handle,"%s",str.c_str());
}
//---------------------------------------------------------------------------
void PGFWriter::write(const FIGObject::FloatPoint& point)
   // Write a point
{
   fprintf(handle,"\\pgfxy(%g,%g)",(point.x-bounds.upperLeft.x)*scalex,(bounds.lowerRight.y-point.y)*scaley);
}
//---------------------------------------------------------------------------
void PGFWriter::write(const FIGObject::IntPoint& point)
   // Write a point
{
   FIGObject::FloatPoint p; p.x=point.x; p.y=point.y;
   write(p);
}
//---------------------------------------------------------------------------
void PGFWriter::writeVect(const FIGObject::FloatPoint& point)
   // Write a vector
{
   fprintf(handle,"\\pgfxy(%g,%g)",point.x*scalex,point.y*scaley);
}
//---------------------------------------------------------------------------
void PGFWriter::writeVect(const FIGObject::IntPoint& point)
   // Write a vector
{
   FIGObject::FloatPoint p; p.x=point.x; p.y=point.y;
   writeVect(p);
}
//---------------------------------------------------------------------------
void PGFWriter::writeColor(int slot)
   // Write a color
{
   if (slot<0) return;
   FIGFile::Color c=fig->getColor(slot);
   if (!c.set) return;
   if ((!c.r)&&(!c.g)&&(!c.b)) return;
   double r=c.r,g=c.g,b=c.b;
   r/=255.0; g/=255.0; b/=255.0;
   fprintf(handle,"\\color[rgb]{%g,%g,%g}",r,g,b);
}
//---------------------------------------------------------------------------
void PGFWriter::writeFillColor(int slot,int style)
   // Write a fill color
{
   if (slot<0) return;
   FIGFile::Color c=fig->getColor(slot);
   if (!c.set) return;
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
   fprintf(handle,"\\color[rgb]{%g,%g,%g}",r,g,b);
}
//---------------------------------------------------------------------------
void PGFWriter::writeFontSize(double size)
   // Write a color
{
   fprintf(handle,"\\fontsize{%d}{%d}",static_cast<int>(size),static_cast<int>(size*1.2));
}
//---------------------------------------------------------------------------
static string arrowCode(const FIGObject::Arrow& arrow,bool forward)
   // Get arrow code
{
   if (!arrow.used) return "";
   string type;
   switch (arrow.type) {
      case 0: type="\\pgfarrowpointed"; break;
      case 1: type="\\pgfarrowtriangle{2pt}"; break;
      case 2: type="\\pgfarrowsingle"; break;
      case 3: type="\\pgfarrowdiamong"; break;
      default: return "";
   }
   if (forward)
      return "\\pgfsetendarrow{"+type+"}"; else
      return "\\pgfsetstartarrow{"+type+"}";
}
//---------------------------------------------------------------------------
static string linestyleCode(FIGObject::LineStyle style)
   // Get code for the line style
{
   switch (style.style) {
      case 1: return "\\pgfsetdash{{1mm}{1mm}}{0mm}"; // dashed
      case 2: return "\\pgfsetdash{{0.1mm}{0.9mm}}{0mm}"; // dotted
      case 3: return "\\pgfsetdash{{1mm}{1mm}{0.1mm}{1.9mm}}{0mm}"; // dash-dotted
      case 4: return "\\pgfsetdash{{1mm}{1mm}{0.1mm}{0.9mm}{0.1mm}{0.9mm}}{0mm}"; // dash-double-dotted
      case 5: return "\\pgfsetdash{{1mm}{1mm}{0.1mm}{0.9mm}{0.1mm}{0.9mm}{0.1mm}{0.9mm}}{0mm}"; // dash-triple-dotted
      default: return "";
   }
}
//---------------------------------------------------------------------------
void PGFWriter::processArc(const FIGArc& /*o*/)
   // Process an element
{
   // XXX
   static bool warned = false;
   if (!warned) {
      warned=true;
      fprintf(stderr,"warning: arcs currently not supported\n");
   }
}
//---------------------------------------------------------------------------
void PGFWriter::processEllipse(const FIGEllipse& o)
   // Process an element
{
   write("{");

   FIGObject::IntPoint v1=o.radius; v1.y=0;
   FIGObject::IntPoint v2=o.radius; v2.x=0;

   // Handle line style
   string specialLine;
   specialLine+=linestyleCode(o.linestyle);

   // Write header if needed
   if (specialLine!="")
      write("\\begin{pgfscope}\n");

   // Fill the area, if needed
   if ((o.areafill>=0)&&(o.fillcolor>=0)) {
      write("{");
      writeFillColor(o.fillcolor,o.areafill);
      write("\\pgfellipse[fill]{");
      write(o.center);
      write("}{");
      writeVect(v1);
      write("}{");
      writeVect(v2);
      write("}}");
   }

   // Intro for the line
   writeColor(o.pencolor);
   write(specialLine);

   // Draw the elipse
   write("\\pgfellipse[stroke]{");
   write(o.center);
   write("}{");
   writeVect(v1);
   write("}{");
   writeVect(v2);
   write("}");

   // Close scope if needed
   if (specialLine!="")
       write("\n\\end{pgfscope}");

   write("}\n");
}
//---------------------------------------------------------------------------
void PGFWriter::processPolyline(const FIGPolyline& o,bool close)
   // Process an element
{
   if (!o.points.size()) return;

   write("{");

   // Handle arrows
   string specialLine;
   specialLine+=arrowCode(o.forwardarrow,true);
   specialLine+=arrowCode(o.backwardarrow,false);

   // Handle line style
   specialLine+=linestyleCode(o.linestyle);

   // Write header if needed
   if (specialLine!="")
      write("\\begin{pgfscope}\n");

   // Fill the area, if needed
   if ((o.areafill>=0)&&(o.fillcolor>=0)&&(o.points.size()>2)) {
      write("{");
      writeFillColor(o.fillcolor,o.areafill);
      vector<FIGObject::IntPoint>::const_iterator iter=o.points.begin();
      write("\\pgfmoveto{"); write(*iter); write("}");
      for (++iter;iter!=o.points.end();++iter) {
	 write("\\pgflineto{"); write(*iter); write("}");
      }
      write("\\pgfeofill}");
   }

   // Intro for the line
   writeColor(o.pencolor);
   write(specialLine);

   // Draw the line
   vector<FIGObject::IntPoint>::const_iterator iter=o.points.begin();
   write("\\pgfmoveto");
   write("{"); write(*iter); write("}");
   for (++iter;iter!=o.points.end();++iter) {
      write("\\pgflineto");
      write("{"); write(*iter); write("}");
   }
   if (close)
      write("\\pgfclosestroke"); else
      write("\\pgfstroke");

   // Close scope if needed
   if (specialLine!="")
       write("\n\\end{pgfscope}");

   write("}\n");
}
//---------------------------------------------------------------------------
void PGFWriter::processArcBox(const FIGPolyline& /*o*/)
   // Process an element
{
   // XXX
   static bool warned = false;
   if (!warned) {
      warned=true;
      fprintf(stderr,"warning: arc boxes currently not supported\n");
   }
}
//---------------------------------------------------------------------------
void PGFWriter::processPicture(const FIGPolyline& /*o*/)
   // Process an element
{
   // XXX
   static bool warned = false;
   if (!warned) {
      warned=true;
      fprintf(stderr,"warning: pictures currently not supported\n");
   }
}
//---------------------------------------------------------------------------
void PGFWriter::processSpline(const FIGSpline& /*o*/)
   // Process an element
{
   // XXX
   static bool warned = false;
   if (!warned) {
      warned=true;
      fprintf(stderr,"warning: splines currently not supported\n");
   }
}
//---------------------------------------------------------------------------
void PGFWriter::processText(const FIGText& o)
   // Process an element
{
   // Hidden?
   if (o.hiddentext) return;

   // Position the text
   write("\\pgfputat{"); write(o.base); write("}{\\pgfbox[");
   switch (o.subtype) {
      case FIGText::Subtype_Left: write("left"); break;
      case FIGText::Subtype_Center: write("center"); break;
      case FIGText::Subtype_Right: write("right"); break;
      default: write("left"); break;
   }
   write(",base]{");
   writeColor(o.color);

   // Font size
   writeFontSize(o.fontsize);

   // Output the text itself
   if (o.specialtext) {
      write(o.text);
   } else {
      write(escapeLatex(o.text));
   }

   write("}}\n");
}
//---------------------------------------------------------------------------
