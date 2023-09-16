#pragma once

#include "stb_image.h"

class TextureItem
{
public:
   unsigned int ID;
   
   TextureItem(); 
   void Load(const char* path, bool generateMipMap);
};
