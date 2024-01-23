//
// Created by paul on 9/11/23.
//
//#define debug
#define DEBUG_SUB_CELL 3
#include "cell.h"

#ifdef debug
//vector<pair<Point, shared_ptr<subCell>>> c;
#endif

cell::cell() {

}

// automatically organizes subcells by the plane they belong to.
void cell::pushPoints(Point p, Point norm) {
    bool found = false;
    for (const auto& ptPair : ptsMap){
        if(ptPair.first == norm){
            ptPair.second->pushPoint((simplePoint)p);
            found = true;
            break;
        }
    }
    if(!found){
     // Create new subCell
        ptsMap.emplace_back(norm, make_shared<subCell>((simplePoint) norm));
        ptsMap.back().second->pushPoint((simplePoint) p);
    }
}

void cell::printPoints() {
    for (const auto& entry: ptsMap){
        std::cout << "Key (" << entry.first.x << ", " << entry.first.y << ", " << entry.first.z << "): ";
        entry.second->printPoints();
        std::cout << std::endl;
    }
}

double cell::getArea() {
    double area = 0;
    for (const auto& entry: ptsMap){
        area += entry.second->getArea();
    }
#ifdef debug
    area = ptsMap[DEBUG_SUB_CELL].second->getArea();
#endif
    return area;
}

Point cell::isPlaneInMap(Point p1, Point p2) {
//    Point comp1 = cross(p1,p2);
//    comp1 = comp1/comp1.norm();
//    Point comp2 = cross(p2,p1);
//    comp2 = comp2/comp2.norm();
//
//    auto it = ptMap.find(comp1);
//    if (it != ptMap.end()){
//        return it->first;
//    }
//    it = ptMap.find(comp2);
//    if (it != ptMap.end()){
//        return it->first;
//    }

    // not implemented

    return {0,0,0};
}

int cell::getNumPoints() {
    int sum = 0;
    for(const auto& pt : ptsMap){
        sum += pt.second->getNumPoints();
    }
    return sum;
}


Point cell::getPoint(int index) {
    int sum = 0;
    Point p;
    for (auto pt : ptsMap) {
        sum += pt.second->getNumPoints();
        if (index < sum){
            p = pt.second->getPoint(index - sum - pt.second->getNumPoints());
        }
    }
    return p;
}

const vector<pair<Point, shared_ptr<subCell>>> &cell::getPtsMap() const {
#ifdef debug
    c.push_back(ptsMap[DEBUG_SUB_CELL]);
    return c;
#endif
    return ptsMap;
}

int cell::getNumSubCell() {
    int size = (int) ptsMap.size();
    return size;
}

void cell::setBfield(Point p1) {
    B = p1;
}

Point cell::getB2A() {
    double area = this->getArea();
    Point B2 = Point(B.x*B.x,B.y*B.y,B.z*B.z);
    B2 = B2 * area;
    return B2;
}

