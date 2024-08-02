#include "MyRandom.h"

int32 GetRandom(int32 min, int32 max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

int32 GetRandomAge()
{
    return GetRandom(0, 60);
}

FLinearColor GetRandomColor()
{
    float R = GetRandom(0, 255) / 1000.f;
    float G = GetRandom(0, 255) / 1000.f;
    float B = GetRandom(0, 255) / 1000.f;
    return FLinearColor(R, G, B);
}
