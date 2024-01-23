/**
 * @Author Paul Rossouw
 * @brief This is a pretty poor implementation. Maybe some polymorphism should be sprinkled in here. This is rough but
 * does the job (for now).
 */

#ifndef GLYPH3D_POINT_H
#define GLYPH3D_POINT_H

#include <iostream>
#include <cmath>

typedef struct simplePoint {
    double x;
    double y;
    double z;
    [[nodiscard]] double norm() const { return std::sqrt(x*x + y*y + z*z); }
    simplePoint() : x(0), y(0), z(0) {};
    simplePoint(double x, double y, double z) : x(x), y(y), z(z) {};
} simplePoint;

typedef struct Point{
    double x = 0, y = 0, z = 0;
    explicit operator simplePoint() { return simplePoint(x, y, z); }
    int pid = 0;
    [[nodiscard]] double norm() const { return std::sqrt(x*x + y*y + z*z); }
    simplePoint planeNormal = {0,0,0};
    // Default constructor
    Point() : x(0), y(0), z(0), pid(0), planeNormal{ 0, 0, 0 } {}
    Point(double x, double y, double z) : x(x), y(y), z(z) {};
    Point(double x, double y, double z,int pid) : x(x), y(y), z(z), pid(pid) {};
} Point;


// hashing function courtesy of chatgpt
struct point_hash {
    size_t operator()(const Point& p) const {
        // You can use a combination of the hash values of x, y, and z as the key
        size_t seed = 0;
        seed ^= std::hash<double>()(p.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<double>()(p.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<double>()(p.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};


bool operator==(Point p1, Point p2);
Point cross(Point p1, Point p2);
simplePoint cross(simplePoint p1, simplePoint p2);
Point operator+(Point a, Point b);
Point operator-(Point a, Point b);
double operator*(Point a, Point b);
Point operator/(Point a, double b);
std::ostream &operator<<(std::ostream &s, Point pt);
simplePoint operator+(simplePoint a, simplePoint b);
simplePoint operator-(simplePoint a, simplePoint b);
double operator*(simplePoint a, simplePoint b);
simplePoint operator/(simplePoint a, double b);
std::ostream &operator<<(std::ostream &s, simplePoint pt);
Point operator*(Point p1, double a);
Point operator*(double a, Point p1);
simplePoint operator*(simplePoint p1, double a);
simplePoint operator*(double a, simplePoint p1);

#endif //GLYPH3D_POINT_H
