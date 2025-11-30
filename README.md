# ADA_SalesmanProblem
Repository containing the project realized in the subject ADA (Analysis and Design of Algorithms). It includes the `GraphApplication` project (MFC desktop app) and helper projects such as the `JPEGLib` static library (libjpeg source) used by the app for JPEG handling.

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
Recommended (Visual Studio):

1. Open `GraphApplication.sln` in Visual Studio.
2. Confirm both `JPEGLib` and `GraphApplication` are visible in Solution Explorer.
3. (Recommended) Add a project reference (GraphApplication → right-click → Add → Reference → Projects → check `JPEGLib`) so Visual Studio automatically builds the library and links to it.
4. Choose the configuration (e.g., Debug|x64 or Debug|Win32).
5. Build → Rebuild Solution.
6. Run the application or locate the generated executable at a path similar to `x64\Debug\GraphApplication.exe`.

Build from PowerShell (scripted):

1. Open *Developer Command Prompt for VS* (or set proper environment via `vcvarsall.bat`) and navigate to the repo root.
2. Use the script:

```powershell
.\build.ps1 -Configuration Debug -Platform x64
```

The script will try to build `JPEGLib` first, and then the solution.

Common issues & fixes
---------------------
- Cannot find `jpeglib.h`: Ensure the `GraphApplication` project has `libjpeg-turbo` included in C/C++ → Additional Include Directories. (Suggested value: `$(ProjectDir)libjpeg-turbo;$(ProjectDir)JPEGLib`)
- Linker can't find `JPEGLib.lib`: Build `JPEGLib` first or add a project reference (preferred). You can also add `$(SolutionDir)JPEGLib\$(Platform)\$(Configuration)` to Linker → Additional Library Directories.
- Missing MFC headers like `afxcontrolbars.h`: Install the MFC/ATL optional component in Visual Studio.
- Toolset mismatch (v143 vs v142): Either change the Platform Toolset to match your installed toolset (Project → Properties → General → Platform Toolset) or install the matching toolset.

Releases / Pre-built binaries
----------------------------
If you want to provide an executable to users without them building the solution, create a `releases/` or `bin/` folder in this repo and add built binaries there (or use GitHub Releases). Avoid committing Visual Studio build outputs under `x64/` (prefer a separate `releases/`).

If you want me to:
- Add a GitHub Action to build and publish the release automatically, or
- Add an automatic `run.ps1` script to find and execute the built executable,
tell me and I’ll add them.

Troubleshooting
---------------
- If `msbuild` is not found in PATH, open a Developer Command Prompt for Visual Studio and run the previously mentioned PowerShell script from the Developer Prompt.
- If you hit a specific compiler error, please paste the first 10-20 error lines and I’ll suggest a targeted fix.
 - Note: the application logs to a `LOGS` folder. If `LOGS` is not present, the application will try to create a `LOGS` folder in the executable directory automatically; if that fails it will display an error and exit.

License & Credits
-----------------
Check the project files for licensing information. `libjpeg-turbo` is a third-party dependency; its code is included under its own license.

Happy building! If you want, I can also add CI config or a sample prebuilt release.
