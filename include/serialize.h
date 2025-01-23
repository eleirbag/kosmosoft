#pragma once
#include <sstream>
#include <fstream>
#include <vector>
class Point;

class DxfWriter {
private:
    std::stringstream dxf;
public:
    DxfWriter();
    ~DxfWriter();
    void AddHeader();

    void AddPath(const std::vector<std::vector<Point>>& segments);
    void EndFile();
    void Save(const std::string& filename);
};

class SvgWriter {
private:
    std::stringstream svg;
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();
    void UpdateBounds(double x, double y);

public:
    SvgWriter();

    void AddPath(const std::vector<std::vector<Point>>& segments, double thickness);

    void Save(const std::string& filename);
};
