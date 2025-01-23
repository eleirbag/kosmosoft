#pragma once

class Point;
namespace QuadraticBezier{
    // Calculate a point on the quadratic Bézier curve for a given t
    Point Bezier(const Point& p0, const Point& p1, const Point& p2, double t);

    // Calculate the "flatness" of a quadratic Bézier curve by measuring the maximum distance to the line p0 -> p2
    double Flatness(Point p0, Point p1, Point p2);
    // De Casteljau's algorithm for quadratic Bézier curves (called from FreeType)
    void ConicTo(const Point& p0, const Point& p1, const Point& p2, void* user);
};

namespace CubicBezier {
    // Calculate a point on the cubic Bézier curve for a given t
    Point Bezier(const Point& p0, const Point& p1, const Point& p2, const Point& p3, double t);
    
    // Calculate the "flatness" of a cubic Bézier curve by measuring the maximum distance to the line p0 -> p3
    double Flatness(Point p0, Point p1, Point p2, Point p3);
    // Function to subdivide the cubic Bézier curve and process the segments
    void CubicTo(const Point& p0, const Point& p1, const Point& p2, const Point& p3, void* user);
};