/* ////////////////////////////////////////////////////////////

YS Class Library
Copyright (c) 2014 Soji Yamakawa.  All rights reserved.

YS Class Library is an open-source class-library project since 1999.
It has been used and battle-tested in commercial, research, and free programs.
Please visit the following URL for more details.

http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File Name: ysmalloc.h
//////////////////////////////////////////////////////////// */

#ifndef YSMALLOC_IS_INCLUDED
#define YSMALLOC_IS_INCLUDED
/* { */

#include "ysdef.h"
#include "ysexception.h"

template <class T,const int minimum> class YsOneMemoryBlock
{
public:
	YsOneMemoryBlock();
	virtual ~YsOneMemoryBlock();

	T *GetMemoryBlock(YSSIZE_T requiredSize);

protected:
	T minBuf[minimum];
	T *ptr;
};

template <class T,const int minimum>
YsOneMemoryBlock <T,minimum>::YsOneMemoryBlock()
{
	ptr=NULL;
}

template <class T,const int minimum>
YsOneMemoryBlock <T,minimum>::~YsOneMemoryBlock()
{
	if(ptr!=NULL && ptr!=minBuf)
	{
		delete [] ptr;
	}
}

template <class T,const int minimum>
T *YsOneMemoryBlock <T,minimum>::GetMemoryBlock(YSSIZE_T requiredSize)
{
	if(ptr==NULL)
	{
		if(requiredSize<=minimum)
		{
			ptr=minBuf;
		}
		else
		{
			ptr=new T[requiredSize];
			if(ptr==NULL)
			{
				YsExceptionHandler::Exception(YsExceptionHandler::OUTOFMEMORY,"YsOneMemoryBlock::GetMemoryBlock()");
			}
		}
		return ptr;
	}
	YsErrOut(
	    "YsOneMemoryBlock::GetMemoryBlock\n"
	    "  Attempted doubely get memory block from one YsOneMemoryBlock\n");
	return NULL;
}

/* } */
#endif
