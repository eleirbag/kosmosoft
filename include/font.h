#pragma once
#include <string>
#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H


class Font {
private:
    FT_Library library;
    FT_Face face;
    double horizontal_shift;
    int fontsize, dpi;
    FT_BBox bbox;
public:
    Font();
    ~Font();

    int LoadFont(std::string font_path);
    int SetFontSize(int fontsize, int dpi = 72);
    int LoadChar(const char c);
    FT_Outline GetFontOutline();
    double GetHorizontalShift();


};

