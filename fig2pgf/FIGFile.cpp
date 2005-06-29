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
#include "FIGFile.hpp"
#include "FIGLexer.hpp"
#include "FIGObject.hpp"
#include "FIGEllipse.hpp"
#include "FIGPolyline.hpp"
#include "FIGSpline.hpp"
#include "FIGText.hpp"
#include "FIGArc.hpp"
#include "FIGCompound.hpp"
#include <cstdio>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
FIGFile::FIGFile()
   : orientation(Orientation_Landscape),justification(Justification_Center),
     units(Units_Metric),papersize(Papersize_A4),magnification(100),
     multiplepage(Multiplepage_Single),transparentcolor(-2),resolution(1200),
     coordsystem(Coordsystem_upperleft),objects(0)
   // Constructor
{
   resetColors();
}
//---------------------------------------------------------------------------
FIGFile::~FIGFile()
   // Destructor
{
   delete objects;
}
//---------------------------------------------------------------------------
void FIGFile::resetColors()
{
   // Default colors
   static const unsigned char defaultColors[32][3]={{0,0,0},{0,0,255},{0,255,0},{0,255,255},{255,0,0},{255,0,255},{255,255,0},{255,255,255},{0,0,144},{0,0,176},{0,0,208},{135,206,255},{0,144,0},{0,176,0},{0,208,0},{0,144,144},{0,176,176},{0,208,208},{144,0,0},{176,0,0},{208,0,0},{144,0,144},{176,0,176},{208,0,208},{128,48,0},{160,64,0},{192,96,0},{255,128,128},{255,160,160},{255,192,192},{255,224,224},{255,215,0}};
   for (unsigned index=0;index<32;index++) {
      colors[index].r=defaultColors[index][0];
      colors[index].g=defaultColors[index][1];
      colors[index].b=defaultColors[index][2];
      colors[index].set=true;
   }

   // User colors
   Color notSet; notSet.r=0; notSet.g=0; notSet.b=0; notSet.set=false;
   for (int index=0;index<512;index++) {
      colors[index+32]=notSet;
   }
}
//---------------------------------------------------------------------------
static int parseColor(const string& s)
{
   if ((s.length()!=7)||(s[0]!='#'))
      return -1;
   int value=0;
   for (int index=1;index<7;index++) {
      char c=s[index];
      if ((c>='0')&&(c<='9')) value=16*value+(c-'0'); else
      if ((c>='a')&&(c<='f')) value=16*value+(c-'a')+10; else
      if ((c>='A')&&(c<='F')) value=16*value+(c-'A')+10; else
         return -1;
   }
   return value;
}
//---------------------------------------------------------------------------
void FIGFile::read(FIGLexer& in)
{
   delete objects;
   objects=0;

   in.readHeader();
   // Orientation
   string s=in.readEOL(); in.endLine();
   if (s=="Landscape") orientation=Orientation_Landscape; else
   if (s=="Portrait") orientation=Orientation_Portrait; else
     in.error("unknown orientation "+s);
   // Justification
   s=in.readEOL(); in.endLine();
   if (s=="Center") justification=Justification_Center; else
   if ((s=="Flush Left")||(s=="Flush left")) justification=Justification_FlushLeft; else
      in.error("unknown justification "+s);
   // Units
   s=in.readEOL(); in.endLine();
   if (s=="Metric") units=Units_Metric; else
   if (s=="Inches") units=Units_Inches; else
      in.error("unknown units "+s);
   // Paper size
   s=in.readEOL(); in.endLine();
   if (s=="Letter") papersize=Papersize_Letter; else
   if (s=="Legal") papersize=Papersize_Legal; else
   if (s=="Ledger") papersize=Papersize_Ledger; else
   if (s=="Tabloid") papersize=Papersize_Tabloid; else
   if (s=="A") papersize=Papersize_A; else
   if (s=="B") papersize=Papersize_B; else
   if (s=="C") papersize=Papersize_C; else
   if (s=="D") papersize=Papersize_D; else
   if (s=="E") papersize=Papersize_E; else
   if (s=="A4") papersize=Papersize_A4; else
   if (s=="A3") papersize=Papersize_A3; else
   if (s=="A2") papersize=Papersize_A2; else
   if (s=="A1") papersize=Papersize_A1; else
   if (s=="A0") papersize=Papersize_A0; else
   if (s=="B5") papersize=Papersize_B5; else
      in.error("unknown paper size '"+s+"'");
   // Magnifiation
   magnification=in.readFloat(); in.endLine();
   // Multiple page
   s=in.readEOL(); in.endLine();
   if (s=="Single") multiplepage=Multiplepage_Single; else
   if (s=="Multiple") multiplepage=Multiplepage_Multiple; else
      in.error("unknown multiple page "+s);
   // Transparent colot
   transparentcolor=in.readInt(); in.endLine();
   // Resolution
   resolution=in.readInt();
   // Coord system
   int i=in.readInt(); in.endLine();
   if (i==1) coordsystem=Coordsystem_lowerleft; else
   if (i==2) coordsystem=Coordsystem_upperleft; else
      in.error("unknown coord system");
   // Reset the colors
   resetColors();

   // Read the objects
   objects=readObjects(in,false);
}
//---------------------------------------------------------------------------
FIGObject* FIGFile::readObjects(FIGLexer& in,bool inCompound)
   // Read objects
{
   FIGObject* objects=0;
   FIGObject* last=0;
   while (true) {
      if (in.endOfFile()) {
	 if (inCompound)
	    in.error("compound not closed");
	 break;
      }
      int type=in.readInt();
      // End of compound?
      if (type==-6) {
	 in.endLine();
	 if (!inCompound)
	    in.error("not in compound");
	 break;
      }
      // Handle colors separately
      if (type==0) {
	 int slot=in.readInt();
	 int color=parseColor(in.readEOL());
	 in.endLine();
	 if ((slot<32)||(slot>=(512+32))||(color<0))
	    in.error("invalid color");
	 colors[slot].r=((color>>16)&0xFF);
	 colors[slot].g=((color>>8)&0xFF);
	 colors[slot].b=((color>>0)&0xFF);
	 colors[slot].set=true;
	 continue;
      }
      // Normal objects
      FIGObject* o=0;
      switch (type) {
	 case 1: o=new FIGEllipse(); break;
	 case 2: o=new FIGPolyline(); break;
	 case 3: o=new FIGSpline(); break;
	 case 4: o=new FIGText(); break;
	 case 5: o=new FIGArc(); break;
	 case 6: o=new FIGCompound(); break;
	 default: {
	    char buffer[50];
	    sprintf(buffer,"unknown object type %d",type);
	    in.error(buffer);
	 }
      }
      o->read(in,*this);
      if (last)
         last->next=o; else
         objects=o;
      last=o;
   }
   return objects;
}
//---------------------------------------------------------------------------
