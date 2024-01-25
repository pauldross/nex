//
// Created by paul on 1/25/24.
//

#ifndef NEX_NOISEEX_CUSTOM_PARSER_H
#define NEX_NOISEEX_CUSTOM_PARSER_H

#include <string>
#include "Point.h"
#include "cell.h"
#include "../VTKParser/depend/VTKParser.h"

using namespace std;

class noiseEX_custom_parser {
    public:
        /**
         * Class constructor
         * @param fileName: String, the file path to the desired vtk file.
         */

        explicit noiseEX_custom_parser(string fileName);

        ~noiseEX_custom_parser();

        /**
         * Runs the algorithm
         * @warning: Init must be run before this can be run
         */
        void run();


        /**
         * @brief So this is currently the only way to call the lower level methods to perform point ordering. Consider
         * a cleaner implementation or just renaming this to something like computeAreas or something. Adding
         * TODO read brief future Paul!
         * @return surface area of the object
         */
        double getArea();

        double  getMSFN(double current);

    private:
        void _init();
        string fileName;
        vector<shared_ptr<cell>> Cells;
        VTKparser parser;

};


#endif //NEX_NOISEEX_CUSTOM_PARSER_H
