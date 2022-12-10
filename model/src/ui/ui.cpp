//#include <cstdio>
#include <chrono>
//#include <thread>
#include <bitset>

#include "ui.h"
#include "start.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <cctype>
#include <cstring>

#include "ImGuiFileDialog/ImGuiFileDialog.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static CommandLog commandLog;
bool stepTaken;
std::string intMessage = "";

int GUI::DrawUI()
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(836, 586, "RISC CPU Model", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 5.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGuiTextBuffer textBuffer;
    textBuffer.clear();
    stepTaken = false;

    //Run();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            //window_flags |= ImGuiWindowFlags_NoMove;
            //window_flags |= ImGuiWindowFlags_NoResize;
            //ImGui::ShowDemoWindow(&show_demo_window);

            SetupDockspace(nullptr);

            ImGui::Begin("RiscCpu", nullptr, window_flags);                          // Create a window called "Hello, world!" and append into it.


            ////my stuff
            // Child 1
            {
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                // ImGui::BeginChild("Commands", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), true, window_flags);
                ImGui::BeginChild("Commands", ImVec2(400, 260), true, window_flags);
                ImGui::Text("Commands");
                ImGui::Separator();
                GUI::ShowCommands();
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }

            ImGui::SameLine(0,10);

            // Child 2
            {
                ImGuiWindowFlags childFlags = ImGuiWindowFlags_None;
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::BeginChild("Regisers", ImVec2(200, 260), true, childFlags);
                ImGui::Text("Registers");
                ImGui::Separator();
                GUI::ShowRegisters();
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }

            ImGui::SameLine(0, 10);

            // Child 3
            {
                ImGuiWindowFlags childFlags = ImGuiWindowFlags_None;
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::BeginChild("Controls", ImVec2(200, 260), true, childFlags);
                ImGui::Text("Controls");
                ImGui::Separator();
                GUI::ShowControls();
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }
            
            // Child 4
            {
                ImGuiWindowFlags childFlags = ImGuiWindowFlags_None;
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::BeginChild("Flags", ImVec2(820, 150), true, childFlags);
                ImGui::Text("Flags");
                ImGui::Separator();
                GUI::ShowFlags();
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }

            // Child 5
            {
                ImGuiWindowFlags childFlags = ImGuiWindowFlags_None;
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::BeginChild("Operation", ImVec2(820, 150), true, childFlags);
                ImGui::Text("Current operation");
                ImGui::Separator();
                GUI::ShowOperationInfo();
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }

            int tempWidth, tempHeight;

            glfwGetWindowSize(window, &tempWidth, &tempHeight);
            //ImVec2 *size = new ImVec2(static_cast<float>(tempWidth), static_cast<float>(tempHeight));
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked



            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

CommandLog::CommandLog(){
    /*TextBuffer.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);*/

    ClearLog();
    memset(InputBuf, 0, sizeof(InputBuf));
    HistoryPos = -1;

    FormCommands();
    AutoScroll = true;
    ScrollToBottom = false;
    //AddLog("Welcome to Dear ImGui!");
}

CommandLog::~CommandLog()
{
    ClearLog();
    for (int i = 0; i < History.Size; i++)
        free(History[i]);
}

void CommandLog::ClearLog() {
    for (int i = 0; i < Items.Size; i++)
        free(Items[i]);
    Items.clear();
    CommandsToProcess.clear();
    currentCommandIndex = 0;
    isSudo = false;
}

void CommandLog::FormCommands() {
    Commands.push_back("ADD");
    Commands.push_back("ADDU");
    Commands.push_back("SUB");
    Commands.push_back("SUBU");
    Commands.push_back("ADDI");
    Commands.push_back("SLT");
    Commands.push_back("SLTI");
    Commands.push_back("SLTU");
    Commands.push_back("SLTIU");
    Commands.push_back("LUI");
    Commands.push_back("AUIP");
    Commands.push_back("MUL");
    Commands.push_back("MULU");
    Commands.push_back("DIV");
    Commands.push_back("DIVU");

    Commands.push_back("ADDF");
    Commands.push_back("SUBF");
    Commands.push_back("MULF");
    Commands.push_back("DIVF");
    Commands.push_back("TFIX");
    Commands.push_back("TFLT");

    Commands.push_back("AND");
    Commands.push_back("OR");
    Commands.push_back("XOR");
    Commands.push_back("SLL");
    Commands.push_back("SRL");
    Commands.push_back("SRA");

    Commands.push_back("LW");
    Commands.push_back("LH");
    Commands.push_back("LB");
    Commands.push_back("SW");
    Commands.push_back("SH");
    Commands.push_back("SB");
    Commands.push_back("MOV");
    Commands.push_back("SWP");
    Commands.push_back("IN");
    Commands.push_back("OUT");

    Commands.push_back("CLI");
    Commands.push_back("STI");
    Commands.push_back("INT");
    Commands.push_back("IRET");
    Commands.push_back("CALL");
    Commands.push_back("RET");
    Commands.push_back("JMP");
    Commands.push_back("JZ");
    Commands.push_back("JNZ");
    Commands.push_back("JO");
    Commands.push_back("JNO");
    Commands.push_back("JC");
    Commands.push_back("JNC");
    Commands.push_back("JS");
    Commands.push_back("JNS");

    Commands.push_back("NOP");
    Commands.push_back("RFL");
    Commands.push_back("WFL");
    Commands.push_back("RTLB");
    Commands.push_back("WTLB");
    Commands.push_back("RFE");
    Commands.push_back("SCALL");
}

void CommandLog::AddLog(const char *command) {
    char buf[1024];
    va_list args = nullptr;
    vsnprintf(buf, IM_ARRAYSIZE(buf), command, args);
    auto tmp = IM_ARRAYSIZE(buf)-1;
    buf[IM_ARRAYSIZE(buf)-1] = 0;
    Items.push_back(Strdup(buf));
}

void CommandLog::Draw() {
       // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (int i = 0; i < Items.Size; i++)
    {
        const char* item = Items[i];

        // Normally you would store more information in your item than just a string.
        // (e.g. make Items[] an array of structure, store color/type etc.)
        if (i == currentCommandIndex)
        {
            ImVec4 color;
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(item);
            ImGui::PopStyleColor();
        } else ImGui::TextUnformatted(item);
    }
    if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    ScrollToBottom = false;

    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
    {
        char* s = InputBuf;
        Strtrim(s);
        if (s[0])
            ExecCommand(s);
        strcpy(s, "");
        reclaim_focus = true;
    }
    ImGui::SameLine(0, 20);
    if (ImGui::Button("Clear"))           { ClearLog(); }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

}

void CommandLog::ExecCommand(const char* command_line)
{
    AddLog(command_line);

    // Insert into history. First find match and delete it so it can be pushed to the back.
    // This isn't trying to be smart or optimal.
    HistoryPos = -1;
    for (int i = History.Size - 1; i >= 0; i--)
        if (Stricmp(History[i], command_line) == 0)
        {
            free(History[i]);
            History.erase(History.begin() + i);
            break;
        }
    History.push_back(Strdup(command_line));

    // Process command
    CommandsToProcess.push_back(BuildCommand(command_line));

    // On command input, we scroll to bottom even if AutoScroll==false
    ScrollToBottom = true;
}

// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
int CommandLog::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
    CommandLog* console = (CommandLog*)data->UserData;
    return console->TextEditCallback(data);
}

int CommandLog::TextEditCallback(ImGuiInputTextCallbackData* data)
{
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag)
    {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char*> candidates;
            for (int i = 0; i < Commands.Size; i++)
                if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                    candidates.push_back(Commands[i]);

            if (candidates.Size == 0)
            {
                // No match
                //AddLog("No match");
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                /*AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);*/
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = HistoryPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (HistoryPos == -1)
                    HistoryPos = History.Size - 1;
                else if (HistoryPos > 0)
                    HistoryPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (HistoryPos != -1)
                    if (++HistoryPos >= History.Size)
                        HistoryPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != HistoryPos)
            {
                const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
    }
    return 0;
}

void GUI::ShowCommands() {
    commandLog.Draw();
}

void GUI::SetupDockspace(bool* p_open)
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}

void GUI::ShowRegisters()
{
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

    ImVec2 outer_size = ImVec2(0.0f, 220);
    if (ImGui::BeginTable("RegistersTable", 2, flags, outer_size))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow();

        // Demonstrate using clipper for large vertical lists
        ImGuiListClipper clipper;
        clipper.Begin(64);
        while (clipper.Step())
        {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 2; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (column == 0) {
                        ImGui::Text("%d", row);
                    }
                    else {
                        ImGui::Text("%d", GUI::MemoryPointer->registers[row]);
                    }
                }
            }
        }
        ImGui::EndTable();
    }
}

void GUI::RunProgram()
{
    bool processed = false;
    int initialSize = commandLog.CommandsToProcess.size();
    int i = 0;

    while (i < initialSize)
    {
    	if (!processed || stepTaken) {
    		intMessage = CpuPointer->Execute(commandLog.CommandsToProcess[0], *GUI::MemoryPointer);
            CpuPointer->PC++;
            commandLog.currentCommandIndex++;
            commandLog.CommandsToProcess.erase(commandLog.CommandsToProcess.begin());
    		i++;
                
    		if (CpuPointer->FLAGS & (1 << 3))
    		{
    			processed = true;
                stepTaken = false;
    		}
    	} else
    	{
            i = initialSize + 1;
            break;
    	}
    }
}

void GUI::ShowControls()
{
    std::string sudoLabelTrue = "Enter Superuser mode###modeButton";
    std::string sudoLabelFalse = "Exit Superuser mode###modeButton";

	bool openFile = ImGui::Button("Open", ImVec2(185, 30));
    ImGui::Spacing();
	bool startProgram = ImGui::Button("Start", ImVec2(185, 30));
    ImGui::Spacing();
    bool stop = ImGui::Button("Stop", ImVec2(185, 30));
    ImGui::Spacing();
	bool doStep = ImGui::Button("Step", ImVec2(185, 30));
    ImGui::Spacing();
	bool doReset = ImGui::Button("Reset", ImVec2(185, 30));
    ImGui::Spacing();
	bool enterSudo = ImGui::Button((commandLog.isSudo ? sudoLabelFalse.c_str() : sudoLabelTrue.c_str()), ImVec2(185, 30));


    //open file
    if (openFile)
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".ass", ".");
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
            std::ifstream input(filePathName);
            std::string temp;
            while (std::getline(input,temp))
            {
                commandLog.ExecCommand(temp.c_str());
            }
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }

    //start program
    if (startProgram)
    {
        std::thread commandRunner(&GUI::RunProgram, this);
        commandRunner.join();
    }

    if (stop)
    {
        commandLog.CommandsToProcess.clear();
    }

    if(doReset)
    {
	    CpuPointer->Reset(*MemoryPointer);
        commandLog.CommandsToProcess.clear();
        commandLog.ClearLog();
    }


	if (doStep)
	{
		stepTaken = true;
        std::thread commandRunner(&GUI::RunProgram, this);
        commandRunner.join();
	}

    if (enterSudo)
    {
        if (!commandLog.isSudo) {
	        CpuPointer->SetUFlag();
        } else CpuPointer->ResetUFlag();

        commandLog.isSudo = !commandLog.isSudo;
    }
}

void GUI::ShowFlags()
{
    ImGuiTableFlags flags = flags = !ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | !ImGuiTableFlags_Resizable | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersV;
    

    ImVec2 outer_size = ImVec2(0.0f, 110.0f);
    if (ImGui::BeginTable("FlagsTable", 7, flags, outer_size))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("Z");
        ImGui::TableSetupColumn("C");
        ImGui::TableSetupColumn("S");
        ImGui::TableSetupColumn("O");
        ImGui::TableSetupColumn("I");
        ImGui::TableSetupColumn("T");
        ImGui::TableSetupColumn("U");
        ImGui::TableHeadersRow();

        // Demonstrate using clipper for large vertical lists
    	ImGui::TableNextRow();
    	for (int column = 0; column < 7; column++)
    	{
    		ImGui::TableSetColumnIndex(column);
    		bool value = 0;
            auto tmp = std::bitset<8>(CpuPointer->FLAGS);
    		if (CpuPointer->FLAGS & (1 << (7 - column))) value = 1;
    		ImGui::Text("%d", value);
    	}
        ImGui::EndTable();
    }
}

void GUI::ShowOperationInfo()
{
    ImGuiTableFlags flags = flags = !ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | !ImGuiTableFlags_Resizable | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersV;


    ImVec2 outer_size = ImVec2(0.0f, 110.0f);
    if (ImGui::BeginTable("CurrCommandTable", 5, flags, outer_size))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("PC");
        ImGui::TableSetupColumn("OpCode");
        ImGui::TableSetupColumn("Arg1");
        ImGui::TableSetupColumn("Arg2");
        ImGui::TableSetupColumn("Offset");
        ImGui::TableHeadersRow();

        // Demonstrate using clipper for large vertical lists
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%d", CpuPointer->PC);
        if (!commandLog.CommandsToProcess.empty()) {
            auto command = commandLog.CommandsToProcess[0];
            for (int column = 0; column < 5; column++)
            {
                ImGui::TableSetColumnIndex(column);

                if (column == 1) ImGui::Text("%d", command.opcode);
                if (column == 2) ImGui::Text("%d", command.param1);
                if (column == 3) ImGui::Text("%d", command.param2);
                if (column == 4) ImGui::Text("%d", command.offset);
            }
        }
        ImGui::EndTable();
    }
}

void GUI::SetCpuPointer(CPU& p)
{
	CpuPointer = &p;
}

void GUI::SetMemoryPointer(Mem& p)
{
	MemoryPointer = &p;
}

int GUI::Pause()
{
	while (!stepTaken) {}

    return 0;
}

// Make the UI compact because there are so many fields
void GUI::PushStyleCompact()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
}

void GUI::PopStyleCompact()
{
    ImGui::PopStyleVar(2);
}
