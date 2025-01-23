#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <vector>

class Point;
class Bezier;
class Font;


class ContoursEtraction {
private:
    static constexpr double MM_PER_INCH = 25.4;
    static constexpr int DEFAULT_DPI = 72;
    int dpi;
    std::vector<std::vector<Point>> edges;

    // Callback functions for outline decomposition
    static int MoveTo(const FT_Vector* to, void* user);

    static int LineTo(const FT_Vector* to, void* user);
    

    static int CubicOutline(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);
    static int ConicOutline(const FT_Vector* control, const FT_Vector* to, void* user);
public:
    double yMin, yMax, xMin, xMax, advanceX;
    ContoursEtraction();
    std::vector < std::vector<Point>>& getPathSegments() { return edges; }


    void ExtractOutlines(Font* font);
};


class ClassifyContours {
private:

    // Calculate the signed area of a polygon
    double CalculateSignedArea(const std::vector<Point>& polygon);

public:
    std::vector<std::vector<Point>> outers;
    std::vector<std::vector<Point>> holes;
    ClassifyContours();
    // Classify contours as outer boundaries or holes
    void DefineInnerAndOuter(const std::vector<std::vector<Point>>& contours);

};
