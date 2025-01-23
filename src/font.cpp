#include "font.h"

Font::Font() : face(NULL), horizontal_shift(0.0), fontsize(0), dpi(0) {
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        throw std::runtime_error("Error initializing FreeType library\n");
    }
}

Font::~Font() {
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

int Font::LoadFont(std::string font_path) {
    FT_Error error;
    error = FT_New_Face(library, font_path.c_str(), 0, &face);
    if (error) {
        FT_Done_FreeType(library);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int Font::SetFontSize(int fontsize, int dpi) {
    this->fontsize = fontsize;
    this->dpi = dpi;
    FT_Error error;
    error = FT_Set_Char_Size(face, 0, fontsize * 64, dpi, dpi);
    if (error) {
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int Font::LoadChar(const char c) {

    FT_Error error;
    // Get glyph index
    FT_UInt glyph_index = FT_Get_Char_Index(face, c);
    if (glyph_index == 0) {
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return EXIT_FAILURE;
    }

    // Load the glyph
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_SCALE);
    if (error) {
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return EXIT_FAILURE;
    }

    horizontal_shift = face->glyph->advance.x;
    return EXIT_SUCCESS;
}

FT_Outline Font::GetFontOutline() {
    return face->glyph->outline;
}

double Font::GetHorizontalShift() { return horizontal_shift; }
