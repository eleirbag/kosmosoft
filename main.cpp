#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <chrono>


#include "font.h"
#include "serialize.h"
#include "geometry.h"
#include "tools.h"
#include "bezier.h"
#include "contours.h"



#include <vector>
#include <cmath>



int main(int argc, char* argv[]) {
    Font* font = new Font();
    if (argc < 6) {
        std::cout << "Usage: ./pttomm /path/to/font/font.tff radius spacing fontsize text";
        exit(EXIT_FAILURE);
    }

    double radius = std::atof(argv[2]);
    double spacing = std::atof(argv[3]);
    int fontsize = std::atoi(argv[4]);

    if (font->LoadFont(argv[1])) {
        throw std::runtime_error("Error loading font file\n");
    }

    if (font->SetFontSize(fontsize)) {
         throw std::runtime_error("Error setting character size\n");
    }
    std::string text = argv[5];
    std::set<char> singlechars;
    std::copy(text.begin(), text.end(),
        std::inserter(singlechars, singlechars.end()));
    std::unordered_map<char, ContoursEtraction> contours;
    std::unordered_map<char, std::vector<std::vector<Point>>> charscanline;

    auto start = std::chrono::high_resolution_clock::now();
    try {
        for (auto c : singlechars) {
            if (font->LoadChar(c)) {
                throw std::runtime_error("Error loading char info\n");
            }

            ContoursEtraction converter;

            converter.ExtractOutlines(font);
            auto &shapes = converter.getPathSegments(); 
            
            ClassifyContours cc;
            cc.DefineInnerAndOuter(shapes);
             
            auto scanlines = Tool::ComputeScanLine(cc.outers, cc.holes, converter.yMin, converter.yMax, spacing, radius);

            charscanline.insert({ c, scanlines });
            contours.insert({ c, converter });
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    // Record the end time
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Print the elapsed time in milliseconds
    std::cout << "Function execution took " << duration.count() << " ms" << std::endl;
    double width = 0.0;

    DxfWriter dxf;

    dxf.AddHeader();
    for (auto c : text) {

        auto contour = contours[c];
        auto sl = charscanline[c];
        auto& border = contour.getPathSegments();
        for (auto& segment : border) {
            for (auto& p : segment) {
                p.x += width;
            }
            segment.push_back(segment[0]);
        }

        dxf.AddPath(border);
        for (auto& segment : sl) {
            for (auto& p : segment) {
                p.x += width;
            }
        }
        dxf.AddPath(sl);
        width += contour.advanceX;
    }
    dxf.EndFile();
    dxf.Save("output.dxf");

    delete font;
    return 0;
}