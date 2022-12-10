#pragma once
#include <vector>

#include "cpu.h"
#include "imgui.h"

struct CommandLog {
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;   // Keep scrolling if already at the bottom.
    bool                  ScrollToBottom;

    bool isSudo;
    int currentCommandIndex = 0;
    std::vector<Command> CommandsToProcess;


    CommandLog();
    ~CommandLog();

    // Portable helpers
    static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static char* Strdup(const char* s)                           { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
    static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void ClearLog();
    void FormCommands();
    void AddLog(const char* command);
    void ExecCommand(const char* command_line);
    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
    int TextEditCallback(ImGuiInputTextCallbackData* data);

    void Draw();
};

class GUI
{
    public:
    int DrawUI();

    void SetCpuPointer(CPU& p);
    void SetMemoryPointer(Mem& p);

private:
    CPU* CpuPointer;
    Mem* MemoryPointer;

    void SetupDockspace(bool* p_open);
    static void ShowCommands();
    void ShowRegisters();
    void ShowControls();
    void ShowFlags();
    void ShowOperationInfo();
    void RunProgram();

    static void PushStyleCompact();
    static void PopStyleCompact();
    int Pause();
};
