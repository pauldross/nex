/**
 * @author Paul Rossouw
 * @brief A very simple class that abstracts the area calculation of cells. The input is unordered points
 * of a polygon. The points are ALWAYS coplanar. The cell class abstracts this. Makes use of the shoelace algorithm
 * to calculate areas. This class has nothing to do with msfn and only stores geometry data. the cell class stores the
 * field data.
 */

#ifndef GLYPH3D_SUBCELL_H
#define GLYPH3D_SUBCELL_H
#include "Point.h"
#include <vector>

#define PI 3.141592653589793238463

using namespace std;

class subCell {
    public:
        /**
         * @brief inits class. Mainly used for testing
         * @param pts points that make up the subCell (all coplanar)
         * @param center the center to use for ordering the points
         * @param norm the normal to the plane the points belong to
         */
        subCell(vector<simplePoint> pts, simplePoint center, simplePoint norm);
        /**
         * @overload overloads default constructor
         * @param norm the normal to the plane the points belong to
         */
        subCell(simplePoint norm);
        /**
         * @brief returns area of subcell using the shoelace algorithm
         * @return
         */
        double getArea();
        /**
         * @brief add point to internal (private) point vector
         * @param pt the point you wish to add to the subcell
         */
        void pushPoint(simplePoint pt);
        /**
         * @brief used for debugging: logs points and normal of subcell
         */
        void printPoints();
        int getNumPoints();
        /**
         * @bug not working
         * TODO fix or remove
         * @param index
         * @return
         */
        Point getPoint(int index);
    private:
        simplePoint mid;
        vector<simplePoint> points;
    public:
        const vector<simplePoint> &getPoints() const;

    private:
        simplePoint normal;
        simplePoint u; //Line to first point
        simplePoint v; // perpendicular to first point and plane normal
        double area;
        bool areaComputed;
        bool uv_comp;

        void _orderPts();
        void _translate();
        void _translateBack();
        double _computeArea();
        double _angleUV(simplePoint p1);
        bool comparePt(simplePoint p1, simplePoint p2);
};


#endif //GLYPH3D_SUBCELL_H
