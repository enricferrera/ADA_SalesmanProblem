# ADA_SalesmanProblem
Repository containing the project realized in the subject ADA (Analysis and Design of Algorithms). It includes the `GraphApplication` project (MFC desktop app) and helper projects such as the `JPEGLib` static library (libjpeg source) used by the app for JPEG handling.

 
[Dijkstra.cpp](GraphApplication/Dijkstra.cpp)
[Greedy.cpp](GraphApplication/Greedy.cpp)
[Backtracking.cpp](GraphApplication/Backtracking.cpp)
[BranchAndBound.cpp](GraphApplication/BranchAndBound.cpp)
[Probabilistic.cpp](GraphApplication/Probabilistic.cpp)

Overview
--------
- `JPEGLib/` - libjpeg (libjpeg-turbo) source integrated as a Visual Studio project build (static library).
- `GraphApplication/` - the main MFC application that uses `JPEGLib` and provides algorithm visualizations.

Prerequisites
-------------
- Windows 10/11
- Visual Studio 2022 (recommended) with these components:
	- Desktop development with C++ workload
	- MFC/ATL support
	- Windows 10 SDK
	- MSVC Platform Toolset v143

Build instructions
------------------
Visual Studio:

1. Open `GraphApplication.sln` in Visual Studio.
2. Confirm both `JPEGLib` and `GraphApplication` are visible in Solution Explorer.
3. (Recommended) Add a project reference (GraphApplication → right-click → Add → Reference → Projects → check `JPEGLib`) so Visual Studio automatically builds the library and links to it.
4. Choose the configuration (e.g., Debug|x64 or Debug|Win32).
5. Build → Rebuild Solution.
 6. Run the application or locate the generated executable at a path similar to `x64/Debug/GraphApplication.exe`.

License & Credits
-----------------
Check the project files for licensing information. `libjpeg-turbo` is a third-party dependency; its code is included under its own license.