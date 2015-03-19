#include "ErrorReporter.h"
#include <windows.h>
#include <dbghelp.h>
#include "../GlobalFuncs.h"
#include "../Defines.h"

std::string ErrorReport::generateStackTrace()
{
    unsigned int i;
    void * stack[100];
    unsigned short frames;
    SYMBOL_INFO * symbol;
    HANDLE process;
    process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    frames = CaptureStackBackTrace(0, 100, stack, NULL);
    symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    std::string bkTrace = "";
    for (i = 0; i < frames; i++)
    {
        char buf[500];
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        sprintf(buf, "%i: %s - 0x%0llx\n", frames - i - 1, symbol->Name, symbol->Address);
        bkTrace += buf;
    }
    return bkTrace;
}

void ErrorReport::writeErrorLog(const std::string &text)
{
    std::string smbxPath = utf8_encode(getModulePath());
    smbxPath += "\\";
    smbxPath += "ERROR_";
    smbxPath += generateTimestamp();
    smbxPath += ".txt";

    writeFile(text, smbxPath);
}

void ErrorReport::sendErrorReport(const std::string &url)
{

}



std::string ErrorReport::getCustomVB6ErrorDescription(VB6ErrorCode errCode)
{
    std::string errDesc = "";
    errDesc += "VB6 Error Code: " + std::to_string(static_cast<int>(errCode)) + "\n";

    switch (errCode)
    {
    case ErrorReport::VB6ERR_OVERFLOW:
        errDesc += "VB6 Error Name: Overflow\n";
        errDesc += "This happens when a number gets too big in the memory\n";
        break;
    case ErrorReport::VB6ERR_OUTOFMEMORY:
        errDesc += "VB6 Error Name: Out of Memory\n";
        errDesc += "This happens when you run out of memory.\n";
        break;
    case ErrorReport::VB6ERR_OUTOFRANGE:
        errDesc += "VB6 Error Name: Subscript out of range\n";
        errDesc += "This happens when too many items are in the scenery.\n";
        break;
    case ErrorReport::VB6ERR_DIVBYZERO:
        errDesc += "VB6 Error Name: Division by zero\n";
        break;
    case ErrorReport::VB6ERR_TYPEMISMATCH:
        errDesc += "VB6 Error Name: Type mismatch\n";
        break;
    default:
        break;
    }
    return errDesc;
}


void ErrorReport::ReportVB6Error(VB6ErrorCode errCode)
{
    std::string fullErrorDescription = "";
    fullErrorDescription += "**************************************************\n";
    fullErrorDescription += "*                  Summary                       *\n";
    fullErrorDescription += "**************************************************\n";
    fullErrorDescription += "SMBX has crashed due an error. See the description for more information!";
    fullErrorDescription += "LunaLua Version: " + std::string(LUALUA_VERSION);
    fullErrorDescription += std::string("Time/Date: ") + generateTimestamp();
    fullErrorDescription += "**************************************************\n";
    fullErrorDescription += "*              Description                       *\n";
    fullErrorDescription += "**************************************************\n";
    fullErrorDescription += getCustomVB6ErrorDescription(errCode);
    fullErrorDescription += "\n";
    fullErrorDescription += "If you like to help us finding the error then please post this log at:\n";
    fullErrorDescription += "* http://engine.wohlnet.ru/forum/ or\n";
    fullErrorDescription += "* http://www.supermariobrosx.org/forums/viewforum.php?f=35\n or";
    fullErrorDescription += "* http://talkhaus.raocow.com/viewforum.php?f=36\n";
    fullErrorDescription += "\n";
    fullErrorDescription += "**************************************************\n";
    fullErrorDescription += "*              Stacktrace                        *\n";
    fullErrorDescription += "**************************************************\n";
    fullErrorDescription += generateStackTrace();
    
    writeErrorLog(fullErrorDescription);
}


