#pragma once

#include <cmath>
#include <Unigine.h>
#include "Structures.h"

Unigine::Math::quat ToQuaternion(EulerAngles ea);

EulerAngles ToEulerAngles(Unigine::Math::quat q);
