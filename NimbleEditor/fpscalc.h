#ifndef FPSCALC
#define FPSCALC
#include <cstdio>
#include <ctime>

/* exposes interface for calculating FPS */
class FPSCalc
{
public:
    FPSCalc();
    FPSCalc(float duration_threshold);
    // Increment current_duration, handle overflows
    float Tick();

    // Set reference point
    void Tock();

    // Return a sampling of the FPS.
    int GetFPS();
private:
    // How long a duration must pass before we poll FPS data?
    // example: 0.25f == 4 times per second.
    float duration_threshold;

    // Currently, how long have we been counting frames?
    float current_duration;

    // Based on total_duration, we multiply  by this
    // to calculate frames/s
    // example: if total_duration == 0.25, frame_multiplier == 4 (1.0/0.25).
    int frame_multiplier;

    // Holds historical information about FPS
    int * fps_data;

    // How many slots to allocate for historical information?
    const int FPS_DATA_LENGTH = 10;

    // Current index into fps_data
    int fps_data_index;

    std::clock_t start;

    // Tracks the  number of frames we complete until next storage point
    int frames;
};

#endif // FPSCALC

