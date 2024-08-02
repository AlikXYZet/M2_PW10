#pragma once

#include "CoreMinimal.h"
#include <random>

// ��������� ���������� �������� [min, max]
int32 GetRandom(int32 min, int32 max);

// ��������� ���������� �������� [0, 60]
int32 GetRandomAge();

// ��������� ���������� ����� FLinearColor(rand, rand, rand, 1.f)
FLinearColor GetRandomColor();
