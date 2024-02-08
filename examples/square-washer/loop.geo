// Gmsh geometry file (for TetraHenry calculation model), generated by InductEx v7.0.03

Mesh.MeshSizeFromPoints = 0;
Mesh.MeshSizeExtendFromBoundary = 0;
Mesh.MeshSizeFromCurvature = 0;
Mesh.MshFileVersion = 2.2;    //  Fix the mesh version to 2.2 - this works best for TTH
Mesh.Algorithm = 5;
Mesh.Algorithm3D = 1;
Mesh.OptimizeNetgen = 1;  
Mesh.MeshSizeMax = 4E-7;
Mesh.MeshSizeMin = 4E-7;

// Variables - characteristic lengths
_lc1 = 5E-7;
_lcT0ybco = 5E-7;
_lcT0term = 0;
_lcT0text = 0;
_lcObject1 = 0.0015;
R = 5E-6;
//W = %W;
W = 8E-6;

Point(1) = {-R, -R, -0.075E-6};
Point(2) = {-R, R, -0.075E-6};
Point(3) = {R, R, -0.075E-6};
Point(4) = {R, -R, -0.075E-6};
Point(5) = {(R+W), (R+W), -0.075E-6};
Point(6) = {-(R+W), (R+W), -0.075E-6};
Point(7) = {-(R+W), -(R+W), -0.075E-6};
Point(8) = {(R+W), -(R+W), -0.075E-6};
Point(9) = {(R+W), (R+W), 0.075E-6};
Point(10) = {-(R+W), (R+W), 0.075E-6};
Point(11) = {-(R+W), -(R+W), 0.075E-6};
Point(12) = {(R+W), -(R+W), 0.075E-6};
Point(13) = {-R, -R, 0.075E-6};
Point(14) = {-R, R, 0.075E-6};
Point(15) = {R, R, 0.075E-6};
Point(16) = {R, -R, 0.075E-6};
Point(17) = {0, 0, -1E-5, 1.5};
Point(18) = {0, 0, 1E-5, 1.5};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 5};
Line(9) = {9, 10};
Line(10) = {10, 11};
Line(11) = {11, 12};
Line(12) = {12, 9};
Line(13) = {13, 14};
Line(14) = {14, 15};
Line(15) = {15, 16};
Line(16) = {16, 13};
Line(17) = {5, 9};
Line(18) = {6, 10};
Line(19) = {7, 11};
Line(20) = {8, 12};
Line(21) = {1, 13};
Line(22) = {2, 14};
Line(23) = {3, 15};
Line(24) = {4, 16};
Line(25) = {17, 18};

Line Loop(1) = {1, 2, 3, 4};
Line Loop(2) = {5, 6, 7, 8};
Line Loop(3) = {9, 10, 11, 12};
Line Loop(4) = {13, 14, 15, 16};
Line Loop(5) = {5, 18, -9, -17};
Line Loop(6) = {6, 19, -10, -18};
Line Loop(7) = {7, 20, -11, -19};
Line Loop(8) = {8, 17, -12, -20};
Line Loop(9) = {1, 22, -13, -21};
Line Loop(10) = {2, 23, -14, -22};
Line Loop(11) = {3, 24, -15, -23};
Line Loop(12) = {4, 21, -16, -24};

Plane Surface(1) = {2, 1};
Plane Surface(2) = {3, 4};
Plane Surface(3) = {5};
Plane Surface(4) = {6};
Plane Surface(5) = {7};
Plane Surface(6) = {8};
Plane Surface(7) = {9};
Plane Surface(8) = {10};
Plane Surface(9) = {11};
Plane Surface(10) = {12};

Surface Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

Volume(1) = {1};



Physical Volume("T0_ybco") = {1};
Physical Line("fone") = {25};

Coherence;
Mesh 3;
Save "loop.msh";
