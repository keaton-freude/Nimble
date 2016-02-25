#include "fpscalc.h"
#include <iostream>
using namespace std;

FPSCalc::FPSCalc()
{

}

FPSCalc::FPSCalc(float duration_threshold)
{
    this->duration_threshold = duration_threshold;

    // Calculate helper values
    this->frame_multiplier = (int)(1.0f / duration_threshold);
    current_duration = 0.0f;
    fps_data = new int[FPS_DATA_LENGTH];
    for(int i = 0; i < FPS_DATA_LENGTH; ++i)
        fps_data[i] = 0;
    fps_data_index = 0;
    start = -1;
    frames = 0;
    fps_data_index = 0;
}

float FPSCalc::Tick()
{
    // early out if first Tick
    if (start == -1)
        return 0.0f;

    // How long has it been since we last got updated by QT?
    float duration = (std::clock() - start) / (float) CLOCKS_PER_SEC;

    // We've completed 1 frame since then, add it up!
    frames++;

    // How close are we to completing 1 second?
    current_duration += duration;

    if (current_duration >= duration_threshold)
    {
        //Second has passed
        // record the FPS in the current fps_data_pointer
        fps_data[fps_data_index] = frames * this->frame_multiplier;

        fps_data_index++;
        if (fps_data_index > this->FPS_DATA_LENGTH - 1)
            fps_data_index = 0;
        frames = 0;
        current_duration = 0;
    }

    return duration;
}

void FPSCalc::Tock()
{
    start = std::clock();
}

int FPSCalc::GetFPS()
{
    int average_fps = 0;

    for(int i = 0; i < this->FPS_DATA_LENGTH; ++i)
    {
        average_fps += fps_data[i];
    }

    return average_fps / FPS_DATA_LENGTH;
}
