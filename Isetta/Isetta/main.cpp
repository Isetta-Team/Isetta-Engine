#include <iostream>
#include <string>
#include <chrono>
#include "Core/Time.h"

#include "Core/Debug.h"

using namespace Isetta;

void StartUp() {
    std::cout << "Initializing game" << std::endl;
    Debug::StartUp();
}

void Update() {
    // cout << Time::time << endl;

}

void ShutDown() {
    std::cout << "Shutting down game" << std::endl;
    Debug::ShutDown();
}

int main() {
    using clock = std::chrono::high_resolution_clock;
    using second = chrono::duration<float>;

    StartUp();

    Debug::PrintF(Debug::Memory, Debug::Info, "Hi %s, you are %d", "Yidi", 10);
    Debug::PrintF("Test\n");

    const float gameMaxDuration = 10.0;

    Time::startTime = clock::now();
    auto lastFrameStartTime = clock::now();

    while (true) {
        Time::deltaTime = second(clock::now() - lastFrameStartTime).count();
        Time::time = second(clock::now() - Time::startTime).count();
        lastFrameStartTime = clock::now();

        Update();
        Time::frameCount++;

        if (Time::time > gameMaxDuration) {
            break;
        }
    }

    ShutDown();
    system("pause");
    return 0;
}
