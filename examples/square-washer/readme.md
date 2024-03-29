## Noise extraction module

### command line options
Usage: nex [--help] [--version] path current

A program for calculating the MSFN figure from simulation results obtained from InductEx.

Positional arguments:
    path           The path to the VTK file that contains the B-field calculations. Data type must be unstructured grid! [required]
    current        The current flowing around the loop [required]

Optional arguments:
    -h, --help     shows help message and exits 
    -v, --version  prints version information and exits 

### Example

#### Folder contents
  - loop.geo: This is the geometry file that generates a mesh file named loop.msh. The file describes the 3D volume of the SQUID washer.
  - loopFIELD.geo: Same as loop.geo but generates a loopFIELD.msh file. This is the mesh that defines the surface of the SQUID washer. It is used to specify where the B-feild must be calculated.
#### How to use:
    1) Start by running the following commands to generate the necessary mesh files:
    ```console
      $ gmsh loop.geo -0
      $ gmsh loopFIELD.geo -0
    ```
    2) Run tth:
    ```console
    $ tth settings_washer.json
    ```
    5) Get the value for the current in the washer by opening the output folder generated by tth. This folder contains a file called 'i.txt'. Open the file and copy the imaginary component (without the "j") of the current. For example: 
    ```
        # Current data [unit: amps] generated by TTH v1.0.50
        .freq 1.0000000000e+10
        fone fone 0.0000000000e+00 7.2023780176e-01j
    ```
    From this example i.txt file the following must be extracted:
    ```
    7.2023780176e-01
    ```
    4) Run nex replacing <current> with the value extracted in the previous step:
    ```console
    $ nex ./output/loop_test.vtk <current>
    ```
    5) The first value returned by the program is the MSFN value in units of the flux quantum squared per Hz. The second term is the total surface area of the squid washer in micro-meters squared. 

