//
// Created by paul on 9/14/23.
//


// spin density in units m^-2
#define SPIN_DENS 5e17

// Bohr magneton in units JT^-1
#define MU_B 9.2740100783e-24

//flux quantumn
#define PHI_0 2.067833848e-15

#define m2toum2 1e12

#define B2toPHI2 3.35237503e-12

#define B_PT(v) {v[0], v[1], v[3]}
#define PT(pt) pt.x , pt.y, pt.z

#include "noiseEx.h"
#include "vtkUnstructuredGrid.h"

noiseEx::noiseEx(string fileName, bool init_now, bool run_now) :
fileName(std::move(fileName))
{
    //init vtk generic data object
    reader->SetFileName(this->fileName.c_str());
    reader->Update();
    if (init_now){
        this->init();
        if (run_now) {
            this->run();
        }
    }

}

double noiseEx::getArea(){
    double sum = 0;
    for(const auto& c : Cells){
        sum += c->getArea();
    }
    return sum;
}

const string &noiseEx::getFileName() const {
    return fileName;
}

void noiseEx::setFileName(const string &name) {
    fileName = name;
}

void noiseEx::init() {
    if (reader->IsFileUnstructuredGrid())
        this->_init();
    else {
        throw std::invalid_argument("Could not read input file. Make sure it is an unstructured grid.");
    }
}

void noiseEx::_init() {
//    std::cout << "Reading file" << std::endl;
    output = reader->GetUnstructuredGridOutput();
    PointArray = output->GetPoints();
    // get data array
    D_array = vtkDoubleArray::SafeDownCast(output->GetPointData()->GetArray("10.000GHz_fone_imag"));
    //vector to store cell data
    // compute cell neighbors
    output->BuildLinks();
    // loop through all points  output->GetNumberOfPoints()
    num = output->GetNumberOfPoints();
    isInit = true;
    hasRun = false;
}


// create cell for each point. Create new subcell for each plane.
void noiseEx::run() {
    if(!isInit){
        throw std::runtime_error("Object has not been initialized");
    }
    int n = 400;
    for (int i = 0; i < num; i++ ){
//        make new cell for each point
//        for(int i = n; i < n+1; i++){
        Cells.emplace_back(make_shared<cell>());
        // get data from D_array:
        double* B_Field = D_array->GetTuple3(i);
        Point B_f = Point(B_Field[0],B_Field[1],B_Field[2]);
        Cells.back()->setBfield(B_f);
        // This gets each cell connected to this point (each triangle)
        //reused variables
        vtkIdType ncells;
        vtkIdType * tcells;
        output->GetPointCells(i, ncells, tcells);
        // point vector that hopefully gets cleared lol
        vector<Point> point_vec;
        for (int k = 0; k < ncells; k++) {
            vtkIdType npts;
            vtkIdType const * pts;
            // Then use cell ids to get points
            output->GetCellPoints(tcells[k],npts,pts);
            Point cellCentrePoint = noiseEx::vtkPtsToPoint(PointArray, i);
            for (int j = 0; j< npts; j++){
                if(pts[j] != i){
                    // where all my points are
                    point_vec.push_back(noiseEx::vtkPtsToPoint(PointArray, (int) pts[j]));
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
            Point p1 = noiseEx::vtkPtsToPoint(PointArray,(int)pts[0]);
            Point p2 = noiseEx::vtkPtsToPoint(PointArray,(int)pts[1]);
            Point p3 = noiseEx::vtkPtsToPoint(PointArray,(int)pts[2]);

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
                Cells.back()->pushPoints(noiseEx::vtkPtsToPoint(PointArray, i),c.first);
            }
        }

    }
    hasRun = true;
}

void noiseEx::debugDraw() {
    vtkSmartPointer<vtkUnsignedCharArray> col = vtkSmartPointer<vtkUnsignedCharArray>::New();
    col->SetName("Colors");
    col->SetNumberOfComponents(3);

    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polygon = vtkSmartPointer<vtkCellArray>::New();
    int increment = 0;
    auto n = (double) Cells.size();
    int count = 0;
    double inc = 0.f;
    for (auto &c : Cells){
        for( auto sb : c->getPtsMap()){
            for (auto pt : sb.second->getPoints()){
                points->InsertNextPoint(PT(pt));
            }
            polygon->InsertNextCell((int)sb.second->getNumPoints()+1);
            for (int j = 0; j < sb.second->getNumPoints(); j++){
                polygon->InsertCellPoint(j+increment);
            }
            polygon->InsertCellPoint(increment);
            increment += (int) sb.second->getNumPoints();
            inc++;
        }
    }

    polyData->SetPoints(points);
    polyData->SetLines(polygon);

    polyData->GetPointData()->SetScalars(col);

    // plotting stuff
    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkArrowSource> arrowSource;
    vtkNew<vtkGlyph3D> Glyph3D;
    Glyph3D->SetSourceConnection(arrowSource->GetOutputPort());
//    Glyph3D->SetVectorModeToUseNormal();
    Glyph3D->SetInputData(output);
    Glyph3D->SetScaleFactor(.000002);
    Glyph3D->Update();

    vtkNew<vtkPolyDataMapper> g_mapper;
    g_mapper->SetInputConnection(Glyph3D->GetOutputPort());

    vtkNew<vtkPolyDataMapper> c_mapper;
    c_mapper->SetInputData(polyData);
    c_mapper->ScalarVisibilityOff();


    vtkNew<vtkActor> c_actor;
    c_actor->SetMapper(c_mapper);
//    c_actor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());
    c_actor->GetProperty()->SetLineWidth(3);

    vtkNew<vtkActor> g_actor;
    g_actor->SetMapper(g_mapper);
    g_actor->GetProperty()->SetColor(colors->GetColor3d("Gold").GetData());

    vtkNew<vtkRenderer> ren;
    ren->AddActor(c_actor);
//    ren->AddActor(g_actor);
    ren->SetBackground(colors->GetColor3d("SlateGray").GetData());
    vtkNew<vtkRenderWindow> renwin;
    renwin->AddRenderer(ren);
    renwin->SetSize(640, 480);
    renwin->SetWindowName("DecimatePolyline");
    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renwin);
    renwin->Render();
    iren->Start();
}

Point noiseEx :: vtkPtsToPoint(vtkPoints *PointArray, int pid){
    double *ps = PointArray->GetPoint(pid);
    return {1E6*ps[0],1E6*ps[1],1E6*ps[2],pid};
}


//frmla: sigma * area * mu_b^2/(3*I^2) * <B(r)^2>
//where <B(r)^2> = 1/A * ∫(Bx^2+By^2+Bz^2)dS
//getB2A returns ∫(Bx^2+By^2+Bz^2)dS in the shape:  {Bx^2, By^2, Bz^2} * Area or <B(r)^2>*Area
//so grouping terms sigma*mu_b/(3*I^2) * totB
//where totB is sum(getB2a, all cells)
//then convert to units of flux quantumn squared
// T^2*um^2 (Tot B) * 5e17m^-2 (sigma) (bohr magneton)^2/ (3*A^2 (I) * flux quantumn^2)
double noiseEx :: getMSFN(double current){
    if(!hasRun) {throw runtime_error("Please call the run method of noiseEx first");}
    Point totB= {0,0,0};
    for (const auto& c : Cells){
        totB = totB + c->getB2A();
    }
    totB = totB/m2toum2  * (SPIN_DENS * MU_B*MU_B / (3*current*current*PHI_0*PHI_0));
    return totB.x + totB.y + totB.z;
}

noiseEx::~noiseEx() = default;
