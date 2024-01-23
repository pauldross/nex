/*
 * Author: Paul Rossouw
 * Date Created: 02/09/2023
 * Class description: Higher level abstraction to simplify the interface
 */

#ifndef GLYPH3D_NOISEEX_H
#define GLYPH3D_NOISEEX_H


#define B_PT(v) {v[0], v[1], v[3]}
// Temporary current data for loop
//#define curr 0.79190019755 Amps


// general includes
#include "cell.h"
#include "Point.h"
#include <cmath>
#include <string>
#include <vector>
//

// vtk imports
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkStructuredGrid.h>
//

using namespace std;

class noiseEx {
    public:
        /**
         * Class constructor
         * @param fileName: String, the file path to the desired vtk file.
         */

        noiseEx(string fileName, bool init_now = true, bool run_now = true);

        ~noiseEx();

        /**
         * Get file name of vtk file
         * @warning: Do not modify return value
         * @return return file name
         */
        [[nodiscard]] const string &getFileName() const;

        /**
         * Set file path
         * @param name : Path to vtk file.
         */
        void setFileName(const string &name);


        void init();

        /**
         * Runs the algorithm
         * @warning: Init must be run before this can be run
         */
        void run();


        /**
         * @brief Draws the object to a window. It only really works with 2D structures so the flat surfaces will look
         * fine but be warned: Anything on the edges look very funky. At least this is the theory at the time of writing
         * adding a TODO for further investigation at a later stage. It could be that the actual algorithm is messed up
         * and not just the way i am drawing the points. I need a clever way to test this.
         */
        void debugDraw();


        /**
         * @brief So this is currently the only way to call the lower level methods to perform point ordering. Consider
         * a cleaner implementation or just renaming this to something like computeAreas or something. Adding
         * TODO read brief future Paul!
         * @return surface area of the object
         */
        double getArea();

        double  getMSFN(double current);

    private:
        void _init();
        string fileName;
        vector<shared_ptr<cell>> Cells;

        static Point vtkPtsToPoint(vtkPoints *PointArray, int pid);
        vtkNew<vtkGenericDataObjectReader> reader;
        vtkUnstructuredGrid *output;
        bool isInit = false;

        vtkPoints *PointArray;
        vtkDoubleArray *D_array;
        vtkIdType num;
        bool hasRun = false;
};


#endif //GLYPH3D_NOISEEX_H
