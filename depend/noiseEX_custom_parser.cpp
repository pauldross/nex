//
// Created by paul on 1/25/24.
//
// spin density in units m^-2
#define SPIN_DENS 5e17

// Bohr magneton in units JT^-1
#define MU_B 9.2740100783e-24

//flux quantumn
#define PHI_0 2.067833848e-15

#define m2toum2 1e12

//#define B2toPHI2 3.35237503e-12

#define B_PT(v) {v[0], v[1], v[3]}
#define PT(pt) pt.x , pt.y, pt.z

#include "noiseEX_custom_parser.h"

#include <utility>
//#include "../VTKParser/depend/VTKParser.h"

noiseEx::noiseEx(string fileName) : parser(std::move(fileName)) {
    parser.parse();
    this->_init();
    this->run();
}

void noiseEx::run() {
    int num_pts = parser.getNumPoints();
    for (int i = 0; i < num_pts; i++ ){
//        make new cell for each point
//        for(int i = n; i < n+1; i++){
        Cells.emplace_back(make_shared<cell>());
        // get data from D_array:
        std::shared_ptr<std::vector<double>> d_vec = parser.getBFromPID(i);
        Point B_f = Point(d_vec->at(0),d_vec->at(1),d_vec->at(2));
        Cells.back()->setBfield(B_f);
        // This gets each cell connected to this point (each triangle)
        //reused variables
        int ncells;
        auto tcells = parser.getCellIDsFromPointIDs(i,ncells);
//        vtkIdType ncells;
//        vtkIdType * tcells;
//        output->GetPointCells(i, ncells, tcells);
        // point vector that hopefully gets cleared lol
        vector<Point> point_vec;
        for (int k = 0; k < ncells; k++) {
            int npts;
//            vtkIdType const * pts;
            // Then use cell ids to get points
//            output->GetCellPoints(tcells[k],npts,pts);

            auto pts = parser.getPointIDsFromCellId(tcells->at(k),npts);

//            Point cellCentrePoint = noiseEx::vtkPtsToPoint(PointArray, i);
            auto p = parser.getVec3FromPID(i);
            Point cellCentrePoint = {1E6*p->at(0),1E6*p->at(1),1E6*p->at(2),i};

            for (int j = 0; j< npts; j++){
                if(pts->at(j) != i){
                    // where all my points are
                    auto pt = parser.getVec3FromPID(pts->at(j));
                    point_vec.emplace_back(1E6*pt->at(0),1E6*pt->at(1),1E6*pt->at(2));
                }
            }
            Point normal = cross(point_vec[0] - cellCentrePoint, point_vec[1] - cellCentrePoint);
            normal = normal/ normal.norm();
            //push points to cell object
            for (auto pt : point_vec){
                Cells.back()->pushPoints((pt + cellCentrePoint)/2,normal);
            }
            point_vec.clear();
            // compute circumcenter
            auto pt = parser.getVec3FromPID(pts->at(0));
            Point p1(1E6*pt->at(0),1E6*pt->at(1),1E6*pt->at(2));
            pt = parser.getVec3FromPID(pts->at(1));
            Point p2(1E6*pt->at(0),1E6*pt->at(1),1E6*pt->at(2));
            pt = parser.getVec3FromPID(pts->at(2));
            Point p3(1E6*pt->at(0),1E6*pt->at(1),1E6*pt->at(2));
//            Point p1 = noiseEx::vtkPtsToPoint(PointArray,(int)pts[0]);
//            Point p2 = noiseEx::vtkPtsToPoint(PointArray,(int)pts[1]);
//            Point p3 = noiseEx::vtkPtsToPoint(PointArray,(int)pts[2]);

            Point bottom = cross(p1-p2,p2-p3);
            double bot = (bottom*bottom)*2;
            double alpha = ((p2 - p3)*(p2 - p3))*((p1-p2)*(p1-p3))/bot;
            double beta = ((p1 - p3)*(p1 - p3))*((p2-p1)*(p2-p3))/bot;
            double gamma = ((p1 - p2)*(p1 - p2))*((p3-p1)*(p3-p2))/bot;
            Point center = alpha *p1 + beta*p2 + gamma*p3;

            Cells.back()->pushPoints(center,normal);

        }

        // loop through all cells here and determine if more than 1 plane exists. If this is the case add the charpt to each plane
        if (Cells.back()->getNumSubCell() > 1) {
            // more than 1 face push charPt
            for (const auto& c : Cells.back()->getPtsMap()){
                auto pt = parser.getVec3FromPID(i);
                Cells.back()->pushPoints(Point(1E6*pt->at(0),1E6*pt->at(1),1E6*pt->at(2)),c.first);
            }
        }

    }

}


double noiseEx::getArea() {
    double sum = 0;
    for(const auto& c : Cells){
        sum += c->getArea();
    }
    return sum;
}

double noiseEx::getMSFN(double current) {
    Point totB= {0,0,0};
    for (const auto& c : Cells){
        totB = totB + c->getB2A();
    }
    totB = totB/m2toum2  * (SPIN_DENS * MU_B*MU_B / (3*current*current*PHI_0*PHI_0));
    return totB.x + totB.y + totB.z;
}

void noiseEx::_init() {

}

noiseEx::~noiseEx() = default;
