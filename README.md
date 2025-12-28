# Geant4 showerlab

**This is a geant4 project for the simulation of an electron shower.**

## Building
Project was built using cmake on windows

## 0. Prerequisites
Make sure you have the geant4 libraries on your computer.
Project was built with geant4 v11.4.0 which can be found [here](https://geant4.web.cern.ch/download/11.4.0.html)

Use [this installation guide](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html)

Download CMake [here](https://cmake.org/download/) and install onto your computer

Download Visual Studio 2026 [here](https://visualstudio.microsoft.com/downloads/), make sure that you select the `Desktop development with C++` module in the Visual Studio Installer software.

### 1. Download project
Download project and extract files

### 2. CMake
Open CMake, drag and drop cmakelists.txt from the files extracted into the CMake GUI. 

Create a build folder and paste the path in the `Where to build the binaries:` path

Click `Configure` and confirm that the compiler is Visual Studio

Click `Finish`

Untick `Advanced` if it is ticked, then for options in red:

`CMAKE_CONFIGURATION_TYPES`	| **Delete everything other than _Release_**
`CMAKE_INSTALL_PREFIX`		| **Create a folder and copy and paste its directory**
`Geant4_DIR`				| ***Leave as is***
`PTL_DIR`					| ***Leave as is***

Click `Configure` again

Click `Generate`

Click `Open Project`

### 3. Visual Studio
In Visual studio, make sure that you have `showerlab` set as the startup project, then build it.

### 4. File Explorer
The executable can be found at the path specified by Visual Studio in a folder called `Release`

Copy the macro files from the build directory and paste them into the `Release` folder, or the executable will not run properly

### 5. Run the executable

### Macros
Use `/control/execute vis.mac` or `/control/execute run.mac`

You can tweak the macros vis.mac and run.mac in an IDE or just Notepad for desired results.

### Results
Results are written into the data folder in a .csv file
