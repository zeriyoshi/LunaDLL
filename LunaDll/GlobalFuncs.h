#ifndef GlobalFuncs_hhhhhhh
#define GlobalFuncs_hhhhhhh

#include <string>
#include <vector>
#include <windows.h>
#include <wchar.h>
#include <string.h>

//String manupulation things
void splitStr(std::vector<std::string>& dest, const std::string& str, const char* separator);
void replaceSubStr(std::string& str, const std::string& from, const std::string& to);
void RemoveSubStr(std::string& sInput, const std::string& sub);
std::vector<std::string> splitCmdArgs( std::string str);

std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);
std::string wstr2str(const std::wstring &wstr);

std::string i2str(int source);
bool is_number(const std::string& s);
bool file_existsX(const std::string& name);
bool isAbsolutePath(const std::wstring& path);
bool isAbsolutePath(const std::string& path);

/// Functions ///
void ResetLunaModule();
void InitGlobals();
void ResetFreeGlob();
void CleanUp();

/// HELPER FUNCTIONS ///
std::vector<std::wstring> wsplit( std::wstring str, wchar_t delimiter);
std::vector<std::string> split( std::string str, char delimiter);
bool vecStrFind(const std::vector<std::string>& vecStr, const std::string& find);
// Module Helper Funcs
HMODULE getModule(std::string moduleName);

// File/Path Helper Funcs
bool readFile(std::wstring &content, std::wstring path, std::wstring errMsg = std::wstring());
bool readFile(std::string &content, std::string path, std::string errMsg = std::string());
bool writeFile(const std::string &content, const std::string &path);
std::vector<std::string> listFilesOfDir(const std::string& path);
std::vector<std::string> listOfDir(const std::string& path, DWORD fileAttributes);
std::wstring getModulePath();
std::wstring getCustomFolderPath();

// Time Helper Funcs
std::string generateTimestamp(std::string format);
std::string generateTimestampForFilename();
std::string generateTimestamp();

// Network Helper Funcs
void sendPUTRequest(const std::string& server, const std::string& data);

// Debug stuff
void RedirectIOToConsole();
void dumpTypeLibrary(IDispatch* dispatchToDump, std::wostream& toOutput);

#endif
