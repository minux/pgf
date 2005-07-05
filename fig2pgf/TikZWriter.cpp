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
#include "TikZWriter.hpp"
#include "FIGFile.hpp"
#include "FIGEllipse.hpp"
#include "FIGPolyline.hpp"
#include "FIGText.hpp"
#include <cstdio>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
TikZWriter::TikZWriter()
   // Constructor
{
}
//---------------------------------------------------------------------------
TikZWriter::~TikZWriter()
   // Destructor
{
}
//---------------------------------------------------------------------------
void TikZWriter::writePoint(const FIGObject::FloatPoint& point)
   // Write a point
{
   write("("+ftoa((point.x-bounds.upperLeft.x)*scalex)+","+ftoa((bounds.lowerRight.y-point.y)*scaley)+")");
}
//---------------------------------------------------------------------------
void TikZWriter::writePoint(const FIGObject::IntPoint& point)
   // Write a point
{
   FIGObject::FloatPoint p; p.x=point.x; p.y=point.y;
   writePoint(p);
}
//---------------------------------------------------------------------------
void TikZWriter::writeVect(const FIGObject::FloatPoint& point)
   // Write a vector
{
   write("("+ftoa(point.x*scalex)+"/"+ftoa(point.y*scaley)+")");
}
//---------------------------------------------------------------------------
void TikZWriter::writeVect(const FIGObject::IntPoint& point)
   // Write a vector
{
   FIGObject::FloatPoint p; p.x=point.x; p.y=point.y;
   writeVect(p);
}
//---------------------------------------------------------------------------
static string arrowCode(const FIGObject::Arrow& arrow,bool /*forward*/)
   // Get arrow code
{
   if (!arrow.used) return "";
   switch (arrow.type) {
      case 0: return "to";
      case 1: return "latex";
      case 2: return "stealth";
      case 3: return "diamond";
      default: return "";
   }
}
//---------------------------------------------------------------------------
static string linestyleCode(FIGObject::LineStyle style)
   // Get code for the line style
{
   switch (style.style) {
      case 1: return ",style=dashed"; // dashed
      case 2: return ",style=dotted"; // dotted
      case 3: return ",dash pattern=on 1mm off 1mm on 0.1mm off 1.9mm"; // dash-dotted
      case 4: return ",dash pattern=on 1mm off 1mm on 0.1mm off 0.9mm on 0.1mm off 0.9mm"; // dash-double-dotted
      case 5: return ",dash pattern=on 1mm off 1mm on 0.1mm off 0.9mm on 0.1mm off 0.9mm on 0.1mm off 0.9mm"; // dash-triple-dotted
      default: return ""; // solid
   }
}
//---------------------------------------------------------------------------
void TikZWriter::processArc(const FIGArc& /*o*/)
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
void TikZWriter::processEllipse(const FIGEllipse& o)
   // Process an element
{
   write("{");

   // Line color
   string intro="\\path[";
   string lineColor=getColor(o.pencolor);
   if (lineColor!="{0,0,0}") {
      write("\\definecolor{fig2pgf_linecolor}{rgb}"+lineColor);
      intro+="draw=fig2pgf_linecolor";
   } else {
      intro+="draw";
   }
   // Fill the area, if needed
   if ((o.areafill>=0)&&(o.fillcolor>=0)) {
      write("\\definecolor{fig2pgf_fillcolor}{rgb}"+getFillColor(o.fillcolor,o.areafill));
      intro+=",fill=fig2pgf_fillcolor";
   }

   // Handle line style
   intro+=linestyleCode(o.linestyle);

   // Draw the ellipse
   write(intro); write("] ");
   writePoint(o.center);
   write(" ellipse ");
   writeVect(o.radius);
   write(";");

   write("}\n");
}
//---------------------------------------------------------------------------
void TikZWriter::processPolyline(const FIGPolyline& o,bool close)
   // Process an element
{
   if (!o.points.size()) return;

   write("{");

   // Line color
   string intro="\\path[";
   string lineColor=getColor(o.pencolor);
   if (lineColor!="{0,0,0}") {
      write("\\definecolor{fig2pgf_linecolor}{rgb}"+lineColor);
      intro+="draw=fig2pgf_linecolor";
   } else {
      intro+="draw";
   }
   // Fill the area, if needed
   if ((o.areafill>=0)&&(o.fillcolor>=0)&&(o.points.size()>2)) {
      write("\\definecolor{fig2pgf_fillcolor}{rgb}"+getFillColor(o.fillcolor,o.areafill));
      intro+=",fill=fig2pgf_fillcolor";
   }

   // Handle arrows
   if (o.forwardarrow.used||o.backwardarrow.used)
      intro+=",arrows="+arrowCode(o.backwardarrow,false)+"-"+arrowCode(o.forwardarrow,true);

   // Handle line style
   intro+=linestyleCode(o.linestyle);

   // Draw the line
   vector<FIGObject::IntPoint>::const_iterator iter=o.points.begin();
   write(intro); write("] ");
   writePoint(*iter);
   for (++iter;iter!=o.points.end();++iter) {
      write(" -- ");
      writePoint(*iter);
   }
   if (close) write(" -- cycle ");
   write(";");

   write("}\n");
}
//---------------------------------------------------------------------------
void TikZWriter::processArcBox(const FIGPolyline& /*o*/)
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
void TikZWriter::processPicture(const FIGPolyline& /*o*/)
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
void TikZWriter::processSpline(const FIGSpline& /*o*/)
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
void TikZWriter::processText(const FIGText& o)
   // Process an element
{
   // Hidden?
   if (o.hiddentext) return;

   write("{");

   // Color
   string intro="\\path[draw,inner sep=0pt";
   string lineColor=getColor(o.color);
   if (lineColor!="{0,0,0}") {
      write("\\definecolor{fig2pgf_linecolor}{rgb}"+lineColor);
      intro+=",color=fig2pgf_linecolor";
   }

   write(intro);
   write("] ");
   // Position the text
   writePoint(o.base);
   write(" node[");
   switch (o.subtype) {
      case FIGText::Subtype_Left: write("anchor=base west"); break;
      case FIGText::Subtype_Center: write("anchor=base"); break;
      case FIGText::Subtype_Right: write("anchor=base east"); break;
      default: write("anchor=base"); break;
   }
   write("] {");

   // Font size
   writeFontSize(o.fontsize);

   // Output the text itself
   if (o.specialtext) {
      write(o.text);
   } else {
      write(escapeLatex(o.text));
   }
   write("};}\n");
}
//---------------------------------------------------------------------------
