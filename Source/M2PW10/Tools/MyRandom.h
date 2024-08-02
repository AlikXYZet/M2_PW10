#pragma once

#include "CoreMinimal.h"
#include <random>

// Получение рандомного значения [min, max]
int32 GetRandom(int32 min, int32 max);

// Получение рандомного значения [0, 60]
int32 GetRandomAge();

// Получение рандомного цвета FLinearColor(rand, rand, rand, 1.f)
FLinearColor GetRandomColor();
