#pragma once
#include "Vector.h"

struct Point
{
    Vector index;

    int value;

    Point(): index(Vector()), value(-1)
    {
    }

    Point(const Vector& index): index(index), value(-1)
    {
    }

    Point(const Vector& index, const int value): index(index), value(value)
    {
    }

    Point(const Point& point): index(point.index), value(point.value) {}
};
