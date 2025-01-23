#pragma once
#include <cmath>
// Represents a 2D point
struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}
    inline bool operator==(const Point& other) const {
        return std::abs(x - other.x) < 1e-6 && std::abs(y - other.y) < 1e-6;
    }
    
};

class Utilities {
public:
    static Point lerp(Point p1, Point p2, double t) {
        return Point(
            p1.x * (1 - t) + p2.x * t,  // interpolate x coordinate
            p1.y * (1 - t) + p2.y * t   // interpolate y coordinate
        );
    }
    static Point midpoint(Point p1, Point p2) {
        return lerp(p1, p2, 0.5);
    }
    // Calculate the distance between a point and a line formed by points p0 and p2
    static double pointLineDistance(const Point& p, const Point& p0, const Point& p2) {
        double num = std::fabs((p2.y - p0.y) * p.x - (p2.x - p0.x) * p.y + p2.x * p0.y - p2.y * p0.x);
        double den = std::sqrt((p2.y - p0.y) * (p2.y - p0.y) + (p2.x - p0.x) * (p2.x - p0.x));
        return num / den;
    }
};
