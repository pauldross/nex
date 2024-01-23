//
// Created by paul on 9/11/23.
//

//#define debug
#include "subCell.h"




subCell::subCell(vector<simplePoint> pts, simplePoint center, simplePoint norm): points(std::move(pts)), mid(center), normal(norm) {
    this->_translate();
    areaComputed = false;
#ifdef  debug
//    for (auto pt : points){
//        cout << 180/PI * this->_angleUV(pt) << "\t" << pt << endl;
//    }
#endif
    this->_orderPts();
#ifdef debug
//    for (auto pt : points){
//        cout << 180/PI * this->_angleUV(pt) << "\t" << pt << endl;
//    }
#endif
    cout << this->_computeArea();
    this->_translateBack();
}

void subCell::_translate() {
    for (auto p : points) {
        // little trick to solve numerical precision problems
        mid = mid + (p*1000.0);
    }
    mid = mid / 1000.0;
    mid = mid / (double) points.size();
#ifdef debug
    cout << "Mid: " << mid << endl;
#endif
    for(auto & point : points){
        point = point - mid;
    }
    v = points[0];
    v = v / v.norm();
    u = cross(v,normal);
    u = u / u.norm();
}

void subCell::_orderPts() {
#ifdef debug
    cout << "U: " << u << " V:" << v << endl;
#endif
    int n = (int) points.size();
    for (int i = 1; i < n; i++) {
        simplePoint key = points[i];
        int j = i - 1;
        while (j >= 0 && this->comparePt(points[j],key)) {
            points[j + 1] = points[j];
            j--;
        }
        points[j + 1] = key;
    }
}

double subCell::_computeArea() {

    areaComputed = true;

    simplePoint pt0 = points[0];
    simplePoint pt_s = {0,0,0};
    for (int i = 0; i < points.size(); i++){
        pt_s = pt_s + (cross(points[i] - pt0, points[(i+1) % points.size()] - pt0));
    }
    area = pt_s.norm()/2;
    return area;
}

void subCell::_translateBack() {
    for(auto &pt: points){
        pt = pt + mid;
    }

//#ifdef debug
//    cout << "u " << u << endl;
//    cout << "v " << v << endl;
//#endif
}

double subCell::_angleUV(simplePoint p1) {

#ifdef debug
//    cout << "u " << u << endl;
//    cout << "v " << v << endl;
#endif
    double u_component = p1 * u;
    double v_component = p1 * v;
    double n = p1.norm();
    double angle = acos(v_component/n);
    // fix when the angle is zero. For some reason acos here returns nan if v_component and n is close enough.
    // CPP please explain?
    if (isnan(angle)) angle = 0;
    if (angle == 0 && v_component < 0){
        angle = PI;
    }
    if (u_component < 0){
        double diff = PI-angle;
        angle = diff + PI;
    }
    return angle;
}


bool subCell::comparePt(simplePoint p1, simplePoint p2) {
    return this->_angleUV(p1) > this->_angleUV(p2);
}

double subCell::getArea() {
    if (areaComputed) {return area;};
    this->_translate();
#ifdef debug
//    for (auto pt : points){
//        cout << 180/PI * this->_angleUV(pt) << "\t" << pt << endl;
//    }
//    cout << "------" << endl;
cout << "*****************" << endl;
cout << "Subcell with normal: " << this->normal << endl;
#endif
    this->_orderPts();
#ifdef debug
    for (auto pt : points){
        cout << 180/PI * this->_angleUV(pt) << "\t" << pt << endl;
    }
    cout << "*****************" << endl;
#endif
    double A =  this->_computeArea();
    this->_translateBack();
    return A;
}

void subCell::pushPoint(simplePoint pt) {
    points.push_back(pt);
}

subCell::subCell(simplePoint norm) : normal(norm){
    areaComputed = false;
    uv_comp = false;
}

void subCell::printPoints() {
    for(auto it : points){
        cout << it << endl;
    }
}

int subCell::getNumPoints() {
    return (int) points.size();
}

Point subCell::getPoint(int index) {
    simplePoint s1 = points[index];
    return Point(s1.x,s1.y,s1.z);
}

const vector<simplePoint> &subCell::getPoints() const {
    return points;
}
