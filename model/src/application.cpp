#include <thread>
#include <memory>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "ui/ui.h"
#include "start.h"

int main() {

    CPU _cpu {};
    Mem _mem {};
    std::shared_ptr<CPU> cpu = std::make_shared<CPU>(_cpu);
    std::shared_ptr<Mem> mem = std::make_shared<Mem>(_mem);

    std::thread cpuWorker(Init, cpu, mem);
    cpuWorker.join();
	SetCpuPointer(*cpu);
    SetMemoryPointer(*mem);

    DrawUI();

    return 0;
}

