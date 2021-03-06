/*
   hhugboy Game Boy emulator
   copyright 2013 taizou

   Based on GEST
   Copyright (C) 2003-2010 TM

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "render.h"
#include "../GB.h"
#include <cstring>

bool asmRendering = true;

void fill_gfx_buffers(unsigned long val)
{
   int count = 160*144;
      
   if(GB->gfx_bit_count == 16)
   {
      fill_line16((WORD*)GB->gfx_buffer1, val,count);
      fill_line16((WORD*)GB->gfx_buffer2, val,count);
      fill_line16((WORD*)GB->gfx_buffer3, val,count);
      fill_line16((WORD*)GB->gfx_buffer4, val,count);
   } else
   {
      fill_line32((DWORD*)GB->gfx_buffer1, val,count);
      fill_line32((DWORD*)GB->gfx_buffer2, val,count);
      fill_line32((DWORD*)GB->gfx_buffer3, val,count);
      fill_line32((DWORD*)GB->gfx_buffer4, val,count);
   }
}

void LCDoff_fill_gfx_buffer(unsigned long val)
{
   int count = 160*144;

   if(GB->gfx_bit_count == 16)
   {
      fill_line16((WORD*)GB->gfx_buffer, val,count);
   } else
   {
      fill_line32((DWORD*)GB->gfx_buffer, val,count);
   }
}

void fill_line16(unsigned short* adr, unsigned long val, int count)
{
    if ( asmRendering ) {
        __asm__ __volatile__ 
        (    
          "cld\n"  
          "rep\n"
          "stosl\n"    
          : "=c" (count), "=D" (val)  // dummy values, because register is corrupted
          : "c" (count>>1), "a" (val), "D" (adr)
          : "memory" );
    } else {
       for(int x=0;x<count;x++) {
	       adr[x] = val;
	   }
    }
}

void fill_line32(unsigned long* adr, unsigned long val, int count)
{            
	if ( asmRendering ) {
	   __asm__ __volatile__ 
       (       
          "cld\n"  
          "rep\n"
          "stosl\n"    
          : "=c" (count), "=D" (val)  // dummy values, because register is corrupted
          : "c" (count), "a" (val), "D" (adr)
          : "memory" );
	} else {
    	for(int x=0;x<count;x++) {
    		adr[x] = val;
    	}
	}
}

void copy_line16(unsigned short* target, unsigned short* src, int count)
{
    if ( asmRendering ) {
       unsigned long dummy;
       __asm__ __volatile__
       (
          "cld\n"
          "rep\n"
          "movsd\n"
          : "=c" (count), "=D" (dummy), "=S" (dummy)  // dummy values, because register is corrupted
          : "c" (count>>1), "S" (src), "D" (target)
          : "memory" );
    } else {
        memcpy(target,src,count*sizeof(short));
    }
}

void copy_line32(unsigned long* target, unsigned long* src, int count)
{
    if ( asmRendering ) {
       unsigned long dummy;
       __asm__ __volatile__
       (
          "cld\n"
          "rep\n"
          "movsd\n"
          : "=c" (count), "=D" (dummy), "=S" (dummy)  // dummy values, because register is corrupted
          : "c" (count), "S" (src), "D" (target)
          : "memory" );
    } else {
        memcpy(target,src,count*sizeof(long));
    	return;
    }

	

}

