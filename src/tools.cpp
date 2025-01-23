


#include <algorithm>
#include <vector>
#include <cmath>
#include "tools.h"
#include "geometry.h"

std::vector<Point> Tool::addOffsetUsingNormals(const std::vector<Point>& shape, double radius, bool isouter) {
    std::vector<Point> offsetShape;
    int n = shape.size();
    if (n < 3) return shape; // A shape must have at least 3 points

    double direction = -1.0;
    if (!isouter)
        direction = 1.0;
    for (int i = 0; i < n; ++i) {
        // Get the previous, current, and next points
        const Point& prev = shape[(i - 1 + n) % n];
        const Point& curr = shape[i];
        const Point& next = shape[(i + 1) % n];

        // Calculate the edge vectors
        double edgeX1 = curr.x - prev.x;
        double edgeY1 = curr.y - prev.y;

        double edgeX2 = next.x - curr.x;
        double edgeY2 = next.y - curr.y;

        // Normalize the edges to calculate normals
        double length1 = std::sqrt(edgeX1 * edgeX1 + edgeY1 * edgeY1);
        double length2 = std::sqrt(edgeX2 * edgeX2 + edgeY2 * edgeY2);

        double normX1 = edgeY1 / length1;
        double normY1 = -edgeX1 / length1;

        double normX2 = edgeY2 / length2;
        double normY2 = -edgeX2 / length2;

        // Calculate the average normal
        double avgNormX = (normX1 + normX2) / 2.0;
        double avgNormY = (normY1 + normY2) / 2.0;

        // Normalize the average normal
        double avgLength = std::sqrt(avgNormX * avgNormX + avgNormY * avgNormY);
        avgNormX /= avgLength;
        avgNormY /= avgLength;

        // Adjust the normal direction based on whether it's an outer border or a hole

        // Offset the point
        Point offsetPoint(curr.x + direction * avgNormX * radius, curr.y + direction * avgNormY * radius);
        offsetShape.push_back(offsetPoint);
    }

    return offsetShape;
}

// Process the entire structure of outer borders and holes
void Tool::processShapeWithHoles(
    const std::vector<std::vector<Point>>& outerBorders,
    const std::vector<std::vector<Point>>& holes,
    double radius,
    std::vector<std::vector<Point>>& offsetOuterBorders,
    std::vector<std::vector<Point>>& offsetHoles
) {
    // Process outer borders
    for (const auto& border : outerBorders) {
        offsetOuterBorders.push_back(addOffsetUsingNormals(border, radius, true));
    }
    // Process holes
    for (const auto& hole : holes) {
        offsetHoles.push_back(addOffsetUsingNormals(hole, radius, true));
    }
}
// Function to calculate the intersection of a line segment and a horizontal scan line
bool Tool::intersectSegmentWithScanLine(Point start, Point end, double yScan, Point& intersection) {
    double x1 = start.x, y1 = start.y;
    double x2 = end.x, y2 = end.y;

    // Check if the segment intersects the scan line
    if ((yScan < std::min(y1, y2)) || (yScan > std::max(y1, y2))) {
        return false;
    }

    // Handle horizontal segments
    if (y1 == y2) {
        return false; // Skip horizontal segments as they lie on the scan line
    }

    // Calculate the x-coordinate of the intersection
    double t = (yScan - y1) / (y2 - y1);
    intersection.x = x1 + t * (x2 - x1);
    intersection.y = yScan;

    return true;
}
// Function to generate scan-line tool paths for a shape with holes, considering shared vertices
std::vector<std::vector<Point>> Tool::ComputeScanLine(
    std::vector<std::vector<Point>>& outerBoundary,
    std::vector<std::vector<Point>>& holes,
    double yMin, double yMax, double spacing, double radius) {

    // Offset the boundaries to account for the radius
    std::vector<std::vector<Point>> offsetOuterBoundary, offsetHoles, scanlines;
    processShapeWithHoles(outerBoundary, holes, radius, offsetOuterBoundary, offsetHoles);

    for (double y = yMin; y <= yMax; y += spacing) {
        std::vector<Point> intersections;

        // Find intersections with the outer boundary

        for (auto& outer : offsetOuterBoundary) {
            //outer.erase(std::unique(outer.begin(), outer.end()), outer.end());
            for (size_t i = 0; i < outer.size(); i++) {
                Point s = outer[i];
                Point e = outer[(i + 1) % outer.size()];

                Point intersection{ 0,0 };
                if (intersectSegmentWithScanLine(s, e, y, intersection)) {
                    intersections.push_back(intersection);
                }
            }
        }
        // Find intersections with each hole
        for (auto& hole : offsetHoles) {
            //hole.erase(std::unique(hole.begin(), hole.end()), hole.end());
            for (size_t i = 0; i < hole.size(); i++) {
                Point s = hole[i];
                Point e = hole[(i + 1) % hole.size()];

                Point intersection;
                if (intersectSegmentWithScanLine(s, e, y, intersection)) {
                    intersections.push_back(intersection);
                }
            }
        }

        // Sort intersections by x-coordinate and remove duplicates
        std::sort(intersections.begin(), intersections.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
            });

        intersections.erase(std::unique(intersections.begin(), intersections.end()), intersections.end());

        // Generate tool paths by toggling states
        bool inside = false;
        std::vector<Point> scanLine;
        for (const auto& point : intersections) {
            if (inside) {
                // End the current segment
                scanLine.push_back(point);
                scanlines.push_back(scanLine);
                scanLine.clear();
            }
            else {
                // Start a new segment
                scanLine.push_back(point);
            }
            inside = !inside; // Toggle state
        }
    }
    return scanlines;
}
