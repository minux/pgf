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
#include <cstdio>
//---------------------------------------------------------------------------
int main(int argc,char* argv[])
{
   if (argc!=3) {
      fprintf(stderr,"usage: %s in out\n",argv[0]);
      return 1;
   }
   FIGLexer in;
   if (!in.open(argv[1])) {
      fprintf(stderr,"unable to open %s\n",argv[1]);
      return 1;
   }
   FIGFile fig;
   fig.read(in);

   PGFWriter out;
   if (!out.create(argv[2])) {
      fprintf(stderr,"unable to create %s\n",argv[2]);
      return 1;
   }

   out.process(fig);
}
//---------------------------------------------------------------------------
