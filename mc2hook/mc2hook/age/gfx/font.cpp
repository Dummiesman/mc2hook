#include "font.h"

void gfxFont::DrawFont(int x, int y, const char* string, void* a4)
{
    hook::StaticThunk<0x5F9E40>::Call<void>(x, y, string, a4);
}
