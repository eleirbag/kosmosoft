
#include "bezier.h"
#include "contours.h"
#include "geometry.h"
#include "font.h"

#include <cmath>
#include <algorithm>


int ContoursEtraction::CubicOutline(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user) {
    auto segments = static_cast<std::vector<std::vector<Point>>*>(user);
    CubicBezier::CubicTo(*(segments->rbegin())->rbegin(), Point{ double(control1->x), double(control1->y) }, Point{ double(control2->x), double(control2->y) }, Point{ double(to->x), double(to->y) }, user);
    return 0;
}

int ContoursEtraction::ConicOutline(const FT_Vector* control, const FT_Vector* to, void* user) {
    auto segments = static_cast<std::vector<std::vector<Point>>*>(user);
    QuadraticBezier::ConicTo(*(segments->rbegin())->rbegin(), Point{ double(control->x), double(control->y) }, Point{ double(to->x), double(to->y) }, user);

    return 0;
}

// Callback functions for outline decomposition
int ContoursEtraction::MoveTo(const FT_Vector* to, void* user) {
    auto segments = static_cast<std::vector<std::vector<Point>>*>(user);
    segments->push_back({ Point{double(to->x), double(to->y)} });
    return 0;
}

int ContoursEtraction::LineTo(const FT_Vector* to, void* user) {
    auto segments = static_cast<std::vector<std::vector<Point>>*>(user);
    segments->rbegin()->push_back(Point{ double(to->x), double(to->y) });

    return 0;
}

void ContoursEtraction::ExtractOutlines(Font* font) {

    FT_Outline_Funcs funcs;
    funcs.move_to = MoveTo;
    funcs.line_to = LineTo;
    funcs.conic_to = ConicOutline;
    funcs.cubic_to = CubicOutline;
    funcs.shift = 0;
    funcs.delta = 0;
    FT_Outline outline = font->GetFontOutline();
    FT_Error error = FT_Outline_Decompose(&outline, &funcs, &edges);
    
    if (error) {
        throw std::runtime_error("Failed to decompose outline");
    }

    for (auto& borders : edges) {
        borders.pop_back();
        for (auto& point : borders) {
            point.y = -point.y;
            yMin = std::min(yMin, point.y);
            yMax = std::max(yMax, point.y);
            xMin = std::min(xMin, point.x);
            xMax = std::max(xMax, point.x);
        }
    }
    advanceX = font->GetHorizontalShift();
}

ContoursEtraction::ContoursEtraction() : dpi(DEFAULT_DPI), yMax(-DBL_MAX), yMin(DBL_MAX), xMax(DBL_MIN), xMin(DBL_MAX), advanceX(0) {}

ClassifyContours::ClassifyContours() {}

// Calculate the signed area of a polygon
double ClassifyContours::CalculateSignedArea(const std::vector<Point>& polygon) {
    double area = 0.0;
    size_t n = polygon.size();
    for (size_t i = 0; i < n; ++i) {
        const Point& p1 = polygon[i];
        const Point& p2 = polygon[(i + 1) % n]; // Wrap around
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return area / 2.0;
}

// Classify contours as outer boundaries or holes
void ClassifyContours::DefineInnerAndOuter(const std::vector<std::vector<Point>>& contours) {
    for (const auto& contour : contours) {
        if (this->CalculateSignedArea(contour) < 0) {
            holes.push_back(contour);
        }
        else {
            outers.push_back(contour);
        }
    }
}

