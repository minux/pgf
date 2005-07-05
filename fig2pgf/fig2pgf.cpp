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
#include "FIGLexer.hpp"
#include "FIGFile.hpp"
#include "PGFWriter.hpp"
#include "TikZWriter.hpp"
#include <cstdio>
#include <cstring>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
static void showHelp(const char* argv0)
{
   fprintf(stderr,"usage: %s [options] in out\n",argv0);
   fprintf(stderr,"supported options:\n");
   fprintf(stderr,"--tikz generate TikZ output (default)\n");
   fprintf(stderr,"--pgf  generate PGF output\n");
}
//---------------------------------------------------------------------------
int main(int argc,char* argv[])
{
   // Interpret options
   int start=1;
   enum { PGF, TikZ } format = TikZ;
   for (;start<argc;start++)
      if (strcmp(argv[start],"--pgf")==0) { format=PGF; } else
      if (strcmp(argv[start],"--tikz")==0) { format=TikZ; } else
         break;

   // Check for the correct number of arguments
   if (start+2!=argc) {
      showHelp(argv[0]);
      return 1;
   }
   // Read the fig file
   FIGLexer in;
   if (!in.open(argv[start])) {
      fprintf(stderr,"unable to open %s\n",argv[start]);
      return 1;
   }
   FIGFile fig;
   fig.read(in);

   // Create the output
   LatexWriter* out;
   bool created;
   switch (format) {
      case PGF: out=new PGFWriter(); break;
      case TikZ: out=new TikZWriter(); break;
   }
   if (!out->create(argv[start+1])) {
      fprintf(stderr,"unable to create %s\n",argv[start+1]);
      return 1;
   }

   out->process(fig);
}
//---------------------------------------------------------------------------
