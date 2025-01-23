#include "serialize.h"
#include "geometry.h"

DxfWriter::DxfWriter() {}
DxfWriter::~DxfWriter() {}
void DxfWriter::AddHeader() {

    // Write the DXF header
    dxf << "0\nSECTION\n2\nHEADER\n0\nENDSEC\n";

    // Write the DXF tables
    dxf << "0\nSECTION\n2\nTABLES\n0\nENDSEC\n";

    // Write the DXF blocks
    dxf << "0\nSECTION\n2\nBLOCKS\n0\nENDSEC\n";

    // Write the DXF entities section
    dxf << "0\nSECTION\n2\nENTITIES\n";
}

void DxfWriter::AddPath(const std::vector<std::vector<Point>>& segments) {
    // Loop over the segments and write each line
    for (const auto& segment : segments) {
        auto current_point = segment[0];
        current_point.y = -current_point.y;
        for (size_t i = 1; i < segment.size(); i++) {
            Point p1 = segment[i];
            p1.y = -p1.y;
            // Write each line as an entity
            dxf << "0\nLINE\n";
            dxf << "8\n0\n"; // Layer 0
            dxf << "62\n1\n"; // Color (1 = red)
            dxf << "370\n" << 10 << "\n"; // Lineweight in hundredths of mm
            dxf << "10\n" << current_point.x << "\n";  // Start X
            dxf << "20\n" << current_point.y << "\n";  // Start Y
            dxf << "30\n" << 0.0 << "\n";  // Start Z
            dxf << "11\n" << p1.x << "\n";  // End X
            dxf << "21\n" << p1.y << "\n";  // End Y
            dxf << "31\n" << 0.0 << "\n";  // Start Z
            current_point = p1;

        }

    }

}
void DxfWriter::EndFile() {

    // Write the DXF end section
    dxf << "0\nENDSEC\n";

    // Write the end of the DXF file
    dxf << "0\nSECTION\n2\nOBJECTS\n0\nENDSEC\n";
    dxf << "0\nENDSEC\n";
    dxf << "0\nEOF\n";

}
void DxfWriter::Save(const std::string& filename) {
    std::ofstream file(filename);
    file << dxf.str();
    file.close();
}


SvgWriter::SvgWriter() {}
void SvgWriter::UpdateBounds(double x, double y) {
    minX = std::min(minX, x);
    minY = std::min(minY, y);
    maxX = std::max(maxX, x);
    maxY = std::max(maxY, y);
}
void SvgWriter::AddPath(const std::vector<std::vector<Point>>& segments, double thickness) {
    std::stringstream path;
    path << "  <path d=\"";

    for (const auto& segment : segments) {
        path << "M " << segment[0].x << " " << segment[0].y << " ";
        for (size_t i = 1; i < segment.size(); i++) {
            path << "L " << segment[i].x << " " << segment[i].y << " ";
            this->UpdateBounds(segment[i].x, segment[i].y);
        }
    }

    path << "Z\" fill=\"none\" stroke=\"red\" stroke-width=\"" + std::to_string(thickness) + "\"/>\n";
    svg << path.str();
}

void SvgWriter::Save(const std::string& filename) {
    
    double width = maxX - minX;
    double height = maxY - minY;

    std::ofstream file(filename);
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
        << "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
        << "     width=\"" << width << "mm\"\n"
        << "     height=\"" << height << "mm\"\n"
        << "     viewBox=\"" << minX << " " << minY << " " << width << " " << height << "\">\n"
        << svg.str()
        << "</svg>\n";
    file.close();
}