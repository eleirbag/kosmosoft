#include "bezier.h"
#include "geometry.h"

#include <cmath>
#include <algorithm>
#include <vector>
// Calculate a point on the quadratic Bézier curve for a given t
Point QuadraticBezier::Bezier(const Point& p0, const Point& p1, const Point& p2, double t) {
    double u = 1.0 - t;
    double tt = t * t;
    double uu = u * u;

    Point p;
    p.x = uu * p0.x + 2 * u * t * p1.x + tt * p2.x;
    p.y = uu * p0.y + 2 * u * t * p1.y + tt * p2.y;
    return p;
}

// Calculate the "flatness" of a quadratic Bézier curve by measuring the maximum distance to the line p0 -> p2
double QuadraticBezier::Flatness(Point p0, Point p1, Point p2) {
    double maxDist = 0.0;

    // Check several points on the Bézier curve at different t values (e.g., 0.1, 0.2, ..., 0.9)
    for (double t = 0.01; t < 1.0; t += 0.01) {
        Point p = Bezier(p0, p1, p2, t);
        double dist = Utilities::pointLineDistance(p, p0, p2);
        maxDist = std::max(maxDist, dist);
    }

    return maxDist;
}

void QuadraticBezier::ConicTo(const Point& p0, const Point& p1, const Point& p2, void* user) {

    auto segments = static_cast<std::vector<std::vector<Point>>*>(user);
    double flatness = QuadraticBezier::Flatness(p0, p1, p2);

    // Check if the flatness is above the threshold, if so, subdivide further
    if (flatness > 0.01) {
        // Subdivide the curve at the midpoint
        Point mid01 = Utilities::midpoint(p0, p1);
        Point mid12 = Utilities::midpoint(p1, p2);
        Point mid0_12 = Utilities::midpoint(mid01, mid12);

        // Recursively subdivide the left and right segments
        ConicTo(p0, mid01, mid0_12, user);  // Left segment
        ConicTo(mid0_12, mid12, p2, user);  // Right segment
    }
    else {
        //segments->rbegin()->push_back(p0);
        segments->rbegin()->push_back(p2) ;

    }
}

Point CubicBezier::Bezier(const Point& p0, const Point& p1, const Point& p2, const Point& p3, double t) {
    double u = 1.0 - t;
    double tt = t * t;
    double uu = u * u;
    double uuu = uu * u;
    double ttt = tt * t;

    Point p;
    p.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
    p.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
    return p;
}
// Calculate the "flatness" of a cubic Bézier curve by measuring the maximum distance to the line p0 -> p3
double CubicBezier::Flatness(Point p0, Point p1, Point p2, Point p3) {
    double maxDist = 0.0;

    // Check several points on the Bézier curve at different t values (e.g., 0.1, 0.2, ..., 0.9)
    for (double t = 0.1; t < 1.0; t += 0.1) {
        Point p = Bezier(p0, p1, p2, p3, t);
        double dist = Utilities::pointLineDistance(p, p0, p3);
        maxDist = std::max(maxDist, dist);
    }

    return maxDist;
}
// Function to subdivide the cubic Bézier curve and process the segments

void  CubicBezier::CubicTo(const Point& p0, const Point& p1, const Point& p2, const Point& p3, void* user) {
    auto segments = static_cast<std::vector<std::vector<Point>>*>(user);

    // Calculate the flatness of the curve
    double flatness = CubicBezier::Flatness(p0, p1, p2, p3);

    // Check if the flatness is above the threshold, if so, subdivide further
    if (flatness > flatness) {
        // Subdivide the curve at the midpoint
        Point mid01 = Utilities::midpoint(p0, p1);
        Point mid12 = Utilities::midpoint(p1, p2);
        Point mid23 = Utilities::midpoint(p2, p3);
        Point mid012 = Utilities::midpoint(mid01, mid12);
        Point mid123 = Utilities::midpoint(mid12, mid23);
        Point mid0123 = Utilities::midpoint(mid012, mid123);

        // Recursively subdivide the left and right segments
        CubicTo(p0, mid01, mid012, mid0123, user);  // Left segment
        CubicTo(mid0123, mid123, mid23, p3, user);  // Right segment
    }
    else {
        segments->rbegin()->push_back(p0);
        segments->rbegin()->push_back(p1);
        segments->rbegin()->push_back(p2);
    }
}