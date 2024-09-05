#pragma once

struct Vector
{
    int x;
    int y;
    int z;

    Vector(): x(-1), y(-1), z(-1)
    {
    }

    Vector(const int value): x(value), y(value), z(value)
    {
    }

    Vector(const int x, const int y, const int z): x(x), y(y), z(z)
    {
    }

    Vector(const Vector& index): x(index.x), y(index.y), z(index.z)
    {
    }

    int operator[](const int axis) const
    {
        switch (axis % 3)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: return -1;
        }
    }

    bool operator==(const Vector& index) const
    {
        return x == index.x && y == index.y && z == index.z;
    }
    
    bool operator!=(const Vector& index) const
    {
        return x != index.x || y != index.y || z != index.z;
    }

    Vector operator*(const int& multiplier) const
    {
        return {x * multiplier, y * multiplier, z * multiplier};
    }

    Vector operator+(const int& addition) const
    {
        return {x + addition, y + addition, z + addition};
    }

    Vector operator+(const Vector& addition) const
    {
        return {x + addition.x, y + addition.y, z + addition.z};
    }
};
