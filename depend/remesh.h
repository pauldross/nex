//
// Created by paul on 9/20/23.
// input: the field file and the .geo file of the structure
// outputs: the remeshed structure

#ifndef GLYPH3D_REMESH_H
#define GLYPH3D_REMESH_H

#include <vtkNew.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkInformation.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include "Point.h"
#include <functional>
#include <regex>
#include <utility>
#include <filesystem>
#include <vtkIndent.h>
#include <vtkInformationIterator.h>
#include <vtkInformationKey.h>

namespace fs = std::filesystem;

#define B_PT(v) {v[0], v[1], v[3]}

namespace remesh {
    std::string meshFileName;
    void generateBgMesh(const std::string& fileName, const std::string& geoFileName = "washerFIELD.geo", std::string mFileName = "post"){
        using namespace std;
        meshFileName = std::move(mFileName);
        //read in file
        vtkNew<vtkGenericDataObjectReader> reader;
        reader->SetFileName(fileName.c_str());
        reader->Update();

        if (reader->IsFileUnstructuredGrid()){
            ofstream POS(meshFileName+".geo");
            std::cout << "Reading file" << std::endl;
            auto output = reader->GetUnstructuredGridOutput();
            vtkPoints *PointArray = output->GetPoints();
            // get data array
            vtkDoubleArray *D_array = vtkDoubleArray::SafeDownCast(output->GetPointData()->GetArray("10.000GHz_fone_imag"));


            POS << "View \"background mesh\" {" << endl;
            const double max_norm =  D_array->GetMaxNorm() * 0.2;
            const double min_norm = D_array->GetMaxNorm() * 0.01;

            // loop through each cell
            auto num = output->GetNumberOfCells();
            for(vtkIdType i = 0; i < num; i++){

                vtkIdType npts;
                vtkIdType const * pts;
                // Then use cell ids to get points
                output->GetCellPoints(i,npts,pts);
                POS << "ST(";
                for (vtkIdType k = 0; k < npts-1; k++){
                    double * pt = PointArray->GetPoint(pts[k]);
                    Point p = (Point) {pt[0], pt[1], pt[2]};
//                    p.z = std::round(p.z * 1e-50) / 1e-50;

                    POS << p.x << "," << p.y << "," << p.z << ',';
                }
                double * pt = PointArray->GetPoint(pts[npts-1]);
                Point p = (Point) {pt[0], pt[1], pt[2]};
                POS << p.x << "," << p.y << "," << p.z;

                double p1 = ((Point) B_PT(D_array->GetTuple3(pts[0]))).norm();
                double p2 = ((Point) B_PT(D_array->GetTuple3(pts[1]))).norm();
                double p3 = ((Point) B_PT(D_array->GetTuple3(pts[2]))).norm();

                // TODO make this not hard code. Perhaps the user must specify?
                const long  double min_len = 0.9E-7f;
                const long double max_len = 8E-6f;


                function<long double(double)> map = [&max_norm, &min_norm, &max_len, &min_len](double x) ->long  double {
                    if (x < min_norm) {
                        return max_len;
                    } else if (x > max_norm) {
                        return min_len;
                    } else {
                        return ((max_len - min_len)/(min_norm - max_norm))*(x - max_norm) + min_len;
                    }
                };

//            cout << 1/(p1*1E8) << endl;
                POS << "){" << map(p1) << "," << map(p2) << "," << map(p3) << "};" << endl;
            }



            POS << "};" << endl << endl;
            POS << "Background Mesh View[0];" << endl;
            POS << "Mesh.MeshSizeExtendFromBoundary = 0;" << endl << "Mesh.MeshSizeFromPoints = 0;" << endl << "Mesh.MeshSizeFromCurvature = 0;" << endl;
            //open the geometry file
            ifstream GEO(geoFileName);
            regex pattern("(Save +\").+(\";)");
            string line;
            while(std::getline(GEO,line)){
                if(line.find("Save") != string::npos){
                    string replaced = regex_replace(line, pattern, "$1"+meshFileName+".msh"+"$2");
                    line = replaced;
                }
                POS << line << endl;
            }
            GEO.close();
            POS.close();
            cout << "Finished writing file with background mesh" << endl;
        } else {
            cout << "Only unstructured grids are supported" << endl;
        }

    }
    void cleanUp(){
//        remove((meshFileName+".msh").c_str());
        remove((meshFileName+".geo").c_str());
        try {
            fs::path currentDir = fs::current_path();

            for (const auto& entry : fs::directory_iterator(currentDir)) {
                if (entry.path().extension() == ".geo_unrolled") {
                    fs::remove(entry.path());
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    void runGMSH(){
        using namespace std;
        cout << "starting gmsh" << endl;
//        string run = "gmsh "+meshFileName+".geo -0 -v 1";
        string run = "gmsh "+meshFileName+".geo ";
        system(run.c_str());
        cout << "gmsh finished" << endl;
    }
}


#endif //GLYPH3D_REMESH_H
