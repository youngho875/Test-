#ifndef GRAPHICS_BUFFER_H_
#define GRAPHICS_BUFFER_H_

struct SYMBOL_GRAPHICS
{	
	Gdiplus::Bitmap					BMP;
	Gdiplus::Graphics				GRP;
	
	SYMBOL_GRAPHICS(Gdiplus::Graphics& g, INT nWidth, INT nHeight) 
		:BMP(nWidth, nHeight, &g)
		, GRP(&BMP) 
	{
	}
	~SYMBOL_GRAPHICS(void) 
	{
	}
};

enum E_2525B_SYMBOL_SIZE
{
	SYMBOL_SYZE_1 = 1,
	SYMBOL_SYZE_2,
	SYMBOL_SYZE_3,
	SYMBOL_SYZE_4,
	SYMBOL_SYZE_5,
};


enum E_2525B_SYMBOL_SIZE_EX
{
	SYMBOL_SYZE_LOD_1 = 1,
	SYMBOL_SYZE_LOD_2,
	SYMBOL_SYZE_LOD_3,
	SYMBOL_SYZE_LOD_4,
	SYMBOL_SYZE_LOD_5,
};

#endif




