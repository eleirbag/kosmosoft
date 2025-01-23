#pragma once
#include <vector>

class Point;

class Tool {
private:
    std::vector<std::vector<Point>> scanlines;

public:
    static std::vector<Point> addOffsetUsingNormals(const std::vector<Point>& shape, double radius, bool isouter);

    // Process the entire structure of outer borders and holes
    static void processShapeWithHoles(
        const std::vector<std::vector<Point>>& outerBorders,
        const std::vector<std::vector<Point>>& holes,
        double radius,
        std::vector<std::vector<Point>>& offsetOuterBorders,
        std::vector<std::vector<Point>>& offsetHoles
    );

    // Function to generate scan-line tool paths for a shape with holes, considering shared vertices
    static std::vector<std::vector<Point>> ComputeScanLine(
        std::vector<std::vector<Point>>& outerBoundary,
        std::vector<std::vector<Point>>& holes,
        double yMin, double yMax, double spacing, double radius);
    static bool intersectSegmentWithScanLine(Point start, Point end, double yScan, Point& intersection);
};