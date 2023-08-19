#pragma once

#include "stb_image.h"

class Texture
{
public:
   unsigned int ID;
   
   Texture(); 
   void Load(const char* path, bool generateMipMap);
};
