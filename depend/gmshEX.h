/**
 * @author Paul
 * @brief A simple api for extracting the shell of a GMSH volume. The api also implements methods for running tth and
 * writing to the settings_file
 */


#ifndef GMSHEX_GMSHEX_H
#define GMSHEX_GMSHEX_H

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <argparse/argparse.hpp>
#include <cstdlib>
#include <regex>
#include <filesystem>


using json = nlohmann::json;

namespace gmshEX {

    int FieldCount;
    bool isInit = false;
    std::string path;
    /**
     * @brief Just here for clarity. This function must always be run before any other function
     */
    void init(){
        path = "";
        isInit = true;
        FieldCount = 0;
    }

    /**
     * @brief writes the updated .geo file to only contain the surfaces bounded by a volume in the .geo file passed via
     * the fileName parameter
     * @param fileName The file name of the target .geo file
     * @return The name of the object as obtained from the file name
     */
    std::string extractSurfaces(const std::string& fileName){
        using namespace std;
        if (!isInit) {throw runtime_error("init() must be run first.");}

        size_t lastSlashPos = fileName.find_last_of('/');
        std::string fl = "";
        std::string Name = fileName.substr(0,fileName.length() - 4);

        // Check if an absolute path is specified
        if (lastSlashPos != std::string::npos && lastSlashPos > 0) {
            path = fileName.substr(0, lastSlashPos+1);

            // Extract the filename
            Name = fileName.substr(lastSlashPos + 1);
            fl = Name;
            Name = Name.substr(0,Name.length() - 4);

            std::cout << "Absolute Path: " << path << std::endl;
            std::cout << "Filename: " << Name << std::endl;
        } else {
            std::cout << "Absolute Path: (Not specified)" << std::endl;
            std::cout << "Filename: " << Name << std::endl;
            fl = Name;
        }

        if(!path.empty()) {
            filesystem::current_path(path);
        }



        // find file
        ifstream inputFile(fl);
        // Check if the input file is open
        if (!inputFile.is_open()) {
            throw runtime_error("Failed to open input file");
        }

        // Open the output file for writing
        std::ofstream outputFile(Name+"FIELD.geo");

        // Check if the output file is open
        if (!outputFile.is_open()) {
            throw runtime_error("Failed to open output file");
        }
        // Read and process each line in the input file
        std::string line;
        while (std::getline(inputFile, line)) {
            // Check if the line contains "physical" or "volume"
            if (line.find("Physical") == std::string::npos && line.find("Volume") == std::string::npos && line.find("Save") == string::npos) {
                // If the line does not contain these words, write it to the output file
                outputFile << line << std::endl;
            }
            if (line.find("Plane Surface") != std::string::npos) {

                FieldCount++;
                size_t pos1 = line.find('(');
                size_t pos2 = line.find(')');

                if (pos1 != std::string::npos && pos2 != std::string::npos) {
                    std::string number = line.substr(pos1 + 1, pos2 - pos1 - 1);

                    // Create the new line in the desired format and write it to the output file
                    outputFile << "Physical Surface(\"Fieldplane"<< stoi(number) - 1 << "\") = {" << number << "};" << std::endl;
                }
            }
        }
        outputFile << "Save \""<< Name << "FIELD.msh" <<"\";" << endl;
        // Close both input and output files
        inputFile.close();
        outputFile.close();

        return Name;
    }

    /**
     * @brief runs gmsh on the newly created .geo file.
     * @warning you really should run extractSurfaces first
     * @param Name The name as obtained by the extractSurfaces method
     * @param fileName The file name of the target .geo file to mesh with gmsh (optional)
     */
    void runGMSH(const std::string& Name, std::string fileName = ""){
        using namespace std;
        if (fileName.empty()) {fileName = Name+"FIELD.geo";}
        if (!isInit) {throw runtime_error("init() must be run first.");}
        cout << "Running gmsh on " << fileName+" -0 -v 1" << endl;
        string run_gmsh = "gmsh "+fileName+" -0 -v 1";
        if(system(run_gmsh.c_str())){
            throw runtime_error("Something went wrong when running gmsh!");
        }
        cout << "Done!" << endl;
    }

    /**
     * @brief Write the updated json file. This file has the correct field planes extracted by the extractSurfaces function
     * as well as references to the updated .msh files
     * @param Name The name as obtained by the extractSurfaces method
     * @param fileName The file name of the target .geo file to mesh with gmsh (optional)
     */
    void updateJSON(const std::string& Name, std::string fileName = "") {
        using namespace std;
        cout << "Updating JSON" << endl;
        if (!isInit) {throw runtime_error("init() must be run first.");}
        if (fileName.empty()) {fileName = "settings_"+Name+".json";}
        ifstream f(fileName);
        json curr = json::parse(f);
        f.close();

        //UPDATE json
        json adjusted = curr;
        adjusted["MESHES"]["IXMeshField0"]["ENTITIES"].clear();
        adjusted["OUTPUT"]["PLOT"]["B"]["ENTITIES"].clear();
        for (int i = 0; i < FieldCount; i++){
            adjusted["MESHES"]["IXMeshField0"]["ENTITIES"].push_back("Fieldplane"+to_string(i));
            adjusted["OUTPUT"]["PLOT"]["B"]["ENTITIES"].push_back("IXMeshField0::Fieldplane" + to_string(i));
        }
        adjusted["MESHES"]["IXMeshField0"]["FILE"] = Name+"FIELD.msh";
        adjusted["OUTPUT"]["PLOT"]["B"]["FILE"] = "B"+Name+".vtk";

        //write to new JSON file:
        ofstream newJson(fileName);
        newJson << regex_replace(adjusted.dump(4),regex(R"(PLOT\d+)"),"PLOT");
        newJson.close();
        cout << "Done!" << endl;
    }

    /**
     * @brief Runs tth on the updated settings.json file
     * @param Name The name as obtained by the extractSurfaces method
     * @param fileName The file name of the target .geo file to mesh with gmsh (optional)
     */
    void runTTH(const std::string& Name, std::string fileName = ""){
        if (!isInit) {throw std::runtime_error("init() must be run first.");}
        if (fileName.empty()) {fileName = "tth settings_"+Name+".json";}
        system((fileName).c_str());
    }

    /**
     * @brief This is for completeness and for future proofing this api. Should be run when you are done using this api.
     */
    void close(){
        FieldCount = 0;
        isInit = false;
    }

    /**
     * @brief Runs it all!
     */
    void runEveryThing(const std::string& fileName){
        init();
        auto Name = extractSurfaces(fileName);
        runGMSH(Name);
        updateJSON(Name);
        runTTH(Name);
        close();
    }

}

#endif //GMSHEX_GMSHEX_H
