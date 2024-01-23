//
// Created by paul on 9/11/23.
//

#include "Point.h"


Point operator+(Point a, Point b){
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
Point operator-(Point a, Point b){
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
double operator*(Point a, Point b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
Point operator/(Point a,double b){
    return {a.x/b, a.y/b, a.z/b};
}
std::ostream& operator<<(std::ostream &s, Point pt) {
    return s << "(x: " << pt.x << "\t y: " << pt.y << "\t z:" << pt.z<< ")";
}

bool operator==(Point p1, Point p2){
    Point comp1 = p2 - p1;
    Point comp2 = p1 + p2;

    if (comp1.x == 0 && comp1.y == 0 && comp1.z == 0){return true;}
    if (comp2.x == 0 && comp2.y == 0 && comp2.z == 0){return true;}
    return false;
}


simplePoint operator+(simplePoint a, simplePoint b){
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
simplePoint operator-(simplePoint a, simplePoint b){
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
double operator*(simplePoint a, simplePoint b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
simplePoint operator/(simplePoint a,double b){
    return {a.x/b, a.y/b, a.z/b};
}
std::ostream& operator<<(std::ostream &s, simplePoint pt) {
    return s << "(x: " << pt.x << "\t y: " << pt.y << "\t z:" << pt.z<< ")";
}


/*
 * [(p1.y * b3 – a3 * b2), (a3 * b1 – a1 * b3), (a1 * b2 – p1.y * b1)]
 */

Point cross(Point p1, Point p2){
    return Point(p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z, p1.x * p2.y - p1.y * p2.x);
}

simplePoint cross(simplePoint p1, simplePoint p2){
    return simplePoint(p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z, p1.x * p2.y - p1.y * p2.x);
}

Point operator*(Point p1, double a){
    return Point(a*p1.x, a*p1.y, a*p1.z);
}
Point operator*(double a, Point p1){
    return Point(a*p1.x, a*p1.y, a*p1.z);
}

simplePoint operator*(simplePoint p1, double a) {
    return simplePoint(a*p1.x, a*p1.y, a*p1.z);
}

simplePoint operator*(double a, simplePoint p1) {
    return simplePoint(a*p1.x, a*p1.y, a*p1.z);
}
