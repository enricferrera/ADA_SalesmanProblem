// AssertsMaximFluxe.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include "Graph.h"
#include "Logs.h"
#include <iomanip> 
#include <Windows.h>


// =============================================================================
// UTILITATS ===================================================================
// =============================================================================

extern CString NombreAlumno1;
extern CString ApellidosAlumno1;
extern CString NIUAlumno1;

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

static char ProgramFileNameBuffer[_MAX_PATH+1] = "";

const char* ProgramFileName()
{
	if (!*ProgramFileNameBuffer) {
		HMODULE mod = GetModuleHandle(NULL);
		ProgramFileNameBuffer[_MAX_PATH] = '\0';
		GetModuleFileName(mod, ProgramFileNameBuffer, _MAX_PATH);
	}
	return ProgramFileNameBuffer;
}

// =============================================================================
// LOGS ========================================================================
// =============================================================================

ofstream LogOut;

// OpenLog =====================================================================

void OpenLog()
{
	// Buscar el directorio LOGS
	string dirName;
	dirName = "LOGS";
	if (!dirExists(dirName)) {
		dirName = "..\\LOGS";
		if (!dirExists(dirName)) {
			dirName = "..\\..\\LOGS";
			if (!dirExists(dirName)) {
				dirName = "..\\..\\..\\LOGS";
				if (!dirExists(dirName)) {
					// Attempt to create LOGS in the executable directory
					const char* exePath = ProgramFileName();
					std::string exeDir(exePath);
					size_t pos = exeDir.find_last_of("\\/");
					if (pos != std::string::npos) exeDir = exeDir.substr(0, pos);
					std::string exeLogs = exeDir + "\\LOGS";
					if (createDirectories(exeLogs)) {
						dirName = exeLogs;
					} else {
						AfxMessageBox("LOGS directory not found and could not be created. Close Application.");
						exit(1);
					}
				}
			}
		}
	}
	// Buscar fichero de LOGS no creado
	string fileName;
	for (int i = 1; i < 10000; ++i) {
		fileName = StrPrint("%s\\%04d_GraphApplicationProf.log", dirName.c_str(), i);
		ifstream tmp(fileName);
		if (!tmp.is_open()) {
			tmp.close();
			goto fileNameOk;
		}
	}
	AfxMessageBox("Error opening Log file. Close Application.");
	exit(1);
fileNameOk:
	LogOut.open(fileName);
	// Escribir cabecera del fichero log
	PrintLog("Open executable: %s", ProgramFileName());
	PrintLog("Command line: %s", GetCommandLineA());
	PrintLog("Nom Alumne 1: %s", NombreAlumno1);
	PrintLog("Cognoms Alumne : %s", ApellidosAlumno1);
	PrintLog("NIU Alumne 1: %s", NIUAlumno1);
}

// CloseLog =====================================================================

void CloseLog()
{
	PrintLog("Close executable: %s", ProgramFileName());
	LogOut.close();
}

// PrintLog ====================================================================

void PrintLog(const char* Format, ...)
{
	va_list al;
	va_start(al, Format);
	char buf[1024];
	_vsnprintf_s(buf, 1024, Format, al);
	buf[1023] = 0;
	va_end(al);
	SYSTEMTIME time;
	GetLocalTime(&time);
	LogOut << setw(2) << setfill('0') << time.wDay << "/" << setw(2) << setfill('0') << time.wMonth << "/" << time.wYear;
	LogOut<< " " << setw(2) << setfill('0') << time.wHour << ":" << setw(2) << setfill('0') << time.wMinute << ":" << setw(2) << setfill('0') << time.wSecond << ": ";
	LogOut << buf << endl;
	LogOut.flush();
}

// Create directories recursively (returns true on success or if already exists)
bool createDirectories(const std::string& dirPath)
{
	if (dirExists(dirPath)) return true;
	std::string path = dirPath;
	// Normalize separators
	for (char &c : path) if (c=='/') c='\\';
	std::string cur;
	size_t i = 0;
	// If path starts with drive letter (like C:\), include that
	if (path.size() >= 2 && path[1] == ':') {
		cur = path.substr(0, 2);
		i = 2;
	}
	for (; i < path.size(); ++i) {
		cur.push_back(path[i]);
		if (path[i] == '\\') {
			// create the cur directory
			if (!dirExists(cur)) {
				if (!CreateDirectoryA(cur.c_str(), NULL)) {
					DWORD err = GetLastError();
					if (err != ERROR_ALREADY_EXISTS) return false;
				}
			}
		}
	}
	// last segment
	if (!dirExists(cur)) {
		if (!CreateDirectoryA(cur.c_str(), NULL)) {
			DWORD err = GetLastError();
			if (err != ERROR_ALREADY_EXISTS) return false;
		}
	}
	return dirExists(dirPath);
}