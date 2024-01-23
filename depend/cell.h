/**
 * @author: Paul Rossouw
 * @brief: A simple class for storing subcell objects.
 *  Interact with class by adding points using the points push method (automaticall adds it to correct cell vector)
 *  Sorts subcells by the plane they belong to.
 */

#ifndef GLYPH3D_CELL_H
#define GLYPH3D_CELL_H
#include "subCell.h"
#include <vector>
#include <unordered_map>
#include <memory>


using namespace std;

class cell {
    public:
        cell();
        /**
         *
         * @param p The point you wish to push
         * @param norm The normal of the plain the point belongs to (for point grouping)
         */
        void pushPoints(Point p, Point norm);

        /**
         *
         * @return surface area of cell
         */
        double getArea();

        /**
         * @brief Prints points (and normals) to console
         */
        void printPoints();
        /**
         * @warning Not implemented
         * @param p1
         * @param p2
         * @return nothing bro
         */
        Point isPlaneInMap(Point p1, Point p2);
        int getNumPoints();
        /**
         * @return number of subcells (and thus unique planes) belonging to the current cell
         */
        int getNumSubCell();
        /**
         * @bug Not working 100%. Don't use
         * TODO: Fix or remove
         * @param index
         * @return
         */
        Point getPoint(int index);

        void setBfield(Point p1);

        /**
         * Get the  B field times the node area. Returns all components  (Bx^2, By^2, Bz^2)*A
         * @return B^2 * A (A is the area of the node(s) making up this cell)
         * @warning untested. Todo test this method
         */
        Point getB2A();
    private:
        Point B;
        vector<pair<Point, shared_ptr<subCell>>> ptsMap;
    public:
        [[nodiscard]] const vector<pair<Point, shared_ptr<subCell>>> &getPtsMap() const;
};


#endif //GLYPH3D_CELL_H
