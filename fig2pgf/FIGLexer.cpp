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
#include <cstdio>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
FIGLexer::FIGLexer()
   : handle(0),line(0)
   // Constructor
{
}
//---------------------------------------------------------------------------
FIGLexer::~FIGLexer()
   // Destructor
{
   if (handle)
      fclose(static_cast<FILE*>(handle));
}
//---------------------------------------------------------------------------
bool FIGLexer::open(const string& name)
   // Open
{
   if (handle)
      fclose(static_cast<FILE*>(handle));
   handle=fopen(name.c_str(),"rt");
   line=1;
   return handle;
}
//---------------------------------------------------------------------------
int FIGLexer::getNext()
   // Read a character
{
   int c=getc(static_cast<FILE*>(handle));
   if (c=='\n') line++;
   return c;
}
//---------------------------------------------------------------------------
void FIGLexer::unget(int c)
   // Put back a character
{
   if (c=='\n') line--;
   ungetc(c,static_cast<FILE*>(handle));
}
//---------------------------------------------------------------------------
void FIGLexer::skipNL()
   // Skip the newline
{
   while (true) {
      int c=getNext();
      if ((c=='\n')||(c=='\r')||(c==' ')||(c=='\t')) continue;
      if (c=='#') {
	 while (true) {
            c=getNext();
            if (c<0) return;
            if ((c=='\n')||(c=='\r')) break;
	 }
	 continue;
      }
      unget(c);
      return;
   }
}

//---------------------------------------------------------------------------
void FIGLexer::error(const string& s)
   // Report an error
{
   fprintf(stderr,"%s in line %d\n",s.c_str(),line);
   exit(1);
}
//---------------------------------------------------------------------------
bool FIGLexer::endOfFile()
   // Check if the end of file has been reached
{
   int c=getNext();
   unget(c);
   return c<0;
}
//---------------------------------------------------------------------------
void FIGLexer::readHeader()
   // Read the header
{
   string line;
   while (true) {
      int c=getNext();
      if (c<0) { line=""; break; }
      if ((c=='\n')||(c=='\r')) break;
      line+=static_cast<char>(c);
   }
   skipNL();

   if (line=="#FIG 3.2") return;
   if (line.substr(0,9)=="#FIG 3.2 ") return;

   error("unknown fig version '"+line+"'");
}
//---------------------------------------------------------------------------
void FIGLexer::acceptNL()
   // Consume NL if found
{
   int c=getNext();
   unget(c);
   if ((c=='\n')||(c=='\r'))
      skipNL();
}
//---------------------------------------------------------------------------
void FIGLexer::endLine()
   // Check for the end of the line
{
   int c=getNext();
   if ((c!='\n')&&(c!='\r'))
      error("end of line expected");
   skipNL();
}
//---------------------------------------------------------------------------
string FIGLexer::readEOL()
   // Read the rest of the line (without the end!)
{
   string line;
   while (true) {
      int c=getNext();
      if (c<0) break;
      if ((c=='\n')||(c=='\r')) {
	 unget(c);
	 break;
      }
      line+=static_cast<char>(c);
   }
   // Strip tailing blanks
   while ((line.length()>0)&&(line[line.length()-1]==' '))
      line=line.substr(0,line.length()-1);
   return line;
}
//---------------------------------------------------------------------------
double FIGLexer::readFloat()
   // Read a float
{
   string line;
   while (true) {
      int c=getNext();
      if (c<0) break;
      if ((c=='\n')||(c=='\r')||(c==' ')||(c=='\t')) {
	 while ((c==' ')||(c=='\t'))
	    c=getNext();
	 unget(c);
	 break;
      }
      line+=static_cast<char>(c);
   }
   const char* a=line.c_str();
   char* b;
   double result=strtod(a,&b);
   if ((result==0.0)&&(*b))
      error("float expected");
   return result;
}
//---------------------------------------------------------------------------
int FIGLexer::readInt()
   // Read an integer
{
   string line;
   while (true) {
      int c=getNext();
      if (c<0) break;
      if ((c=='\n')||(c=='\r')||(c==' ')||(c=='\t')) {
	 while ((c==' ')||(c=='\t'))
	    c=getNext();
	 unget(c);
	 break;
      }
      line+=static_cast<char>(c);
   }
   const char* a=line.c_str();
   char* b;
   int result=strtol(a,&b,10);
   if ((result==0.0)&&(*b))
      error("int expected");
   return result;
}
//---------------------------------------------------------------------------
bool FIGLexer::readBool()
   // Read a boolean
{
   int i=readInt();
   if (i==1) return true;
   if (i!=0) error("boolean expected");
   return false;
}
//---------------------------------------------------------------------------
string FIGLexer::readString()
   // Read a string
{
   string line;
   while (true) {
      int c=getNext();
      if (c<0) break;
      if (c=='\\') {
	 int c1=getNext();
	 if (c1=='\\') {
	    c='\\';
	 } else {
   	    int c2=getNext();
	    int c3=getNext();
	    int val=((c1-'0')*8*8)+((c2-'0')*8)+(c3-'0');
	    if (val==1) break;
	    c=val;
	 }
      }
      line+=static_cast<char>(c);
   }
   return line;
}
//---------------------------------------------------------------------------
