/*
   hhugboy Game Boy emulator
   copyright 2013-2016 taizou
   Based on GEST
   Copyright (C) 2003-2010 TM
   Incorporating code from VisualBoyAdvance
   Copyright (C) 1999-2004 by Forgotten

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
#include "MbcUnlSachen8in1.h"

void MbcUnlSachen8in1::writeMemory(unsigned short address, register byte data) {
    if(address < 0x2000)
        return;

    if(address < 0x4000) // Is it a ROM bank switch?
    {
        data &= 0x1f;
        if(data == 0)
            data = 1;

        rom_bank = data;

        int cadr = (data<<14)+(MBChi<<14);
        cadr &= rom_size_mask[(*gbRom)->ROMsize];
        gbMemMap[0x4] = &(*gbCartridge)[cadr];
        gbMemMap[0x5] = &(*gbCartridge)[cadr+0x1000];
        gbMemMap[0x6] = &(*gbCartridge)[cadr+0x2000];
        gbMemMap[0x7] = &(*gbCartridge)[cadr+0x3000];
        return;
    }

    if(address < 0x6000) // Is it a RAM bank switch?
    {
        if(address == 0x4000 && bc_select < 3) // game select
        {
            ++bc_select;

            MBClo = 0;

            MBChi = (data&0x1f);

            cart_address = MBChi<<14;
            gbMemMap[0x0] = &(*gbCartridge)[cart_address];
            gbMemMap[0x1] = &(*gbCartridge)[cart_address+0x1000];
            gbMemMap[0x2] = &(*gbCartridge)[cart_address+0x2000];
            gbMemMap[0x3] = &(*gbCartridge)[cart_address+0x3000];

            gbMemMap[0x4] = &(*gbCartridge)[cart_address+0x4000];
            gbMemMap[0x5] = &(*gbCartridge)[cart_address+0x5000];
            gbMemMap[0x6] = &(*gbCartridge)[cart_address+0x6000];
            gbMemMap[0x7] = &(*gbCartridge)[cart_address+0x7000];
            return;
        }

        data &= 0x03;

        ram_bank = data;

        int madr = data<<13;
        gbMemMap[0xA] = &(*gbCartRam)[madr];
        gbMemMap[0xB] = &(*gbCartRam)[madr+0x1000];
        return;
    }

    if(address < 0x8000)
        return;

    // Always allow RAM writes.

    gbMemMap[address>>12][address&0x0FFF] = data;
}
