#ifndef H_FIGLexer
#define H_FIGLexer
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
#include <string>
//---------------------------------------------------------------------------
class FIGLexer
{
   private:
   /// The file
   void* handle;
   /// The current line
   int line;

   FIGLexer(const FIGLexer&);
   void operator=(const FIGLexer&);

   /// Get the next character
   int getNext();
   /// Unget a character
   void unget(int c);
   /// Skip the new line and blanks
   void skipNL();


   public:
   /// Constructor
   FIGLexer();
   /// Destructor
   ~FIGLexer();

   /// Open
   bool open(const std::string& name);

   /// Check if the end of file has been reached
   bool endOfFile();
   /// Read the fig header
   void readHeader();
   /// Consume NL if found
   void acceptNL();
   /// Check for the end of the line
   void endLine();
   /// Read the rest of the line (without the end!)
   std::string readEOL();
   /// Read a float
   double readFloat();
   /// Read an integer
   int readInt();
   /// Read a boolean
   bool readBool();
   /// Read a string
   std::string readString();

   /// Report an error
   void error(const std::string& message);
};
//---------------------------------------------------------------------------
#endif
