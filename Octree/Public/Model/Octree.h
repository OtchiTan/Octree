#pragma once
#include <iostream>
#include <vector>

#include "Point.h"
#include "Position.h"

const std::vector<Vector> PointPosition{
    {1,1,1},
    {1,1,-1},
    {1,-1,1},
    {1,-1,-1},
    {-1,1,1},
    {-1,1,-1},
    {-1,-1,1},
    {-1,-1,-1},
};

class Octree
{
    Point* point_;

    Vector center_;
    int ray_;
    std::vector<Octree*> children_;

    Octree()
    {
        point_ = new Point();
        ray_ = -1;
        center_ = {};
    }
    
    Octree(const Vector& center, const int ray, const int value): center_(center),ray_(ray)
    {
        point_ = new Point(center, value);
    }

    inline position get_position(const Vector& index) const;
    
public:
    explicit Octree(const int& size, const Vector& center = Vector(0)): center_(center)
    {
        if ((size & size - 1) != 0)
        {
            std::cout << "Size is not power of 2\n";
            return;
        }
        
        ray_ = size;
        point_ = nullptr;

        children_.assign(8, nullptr);
    }

    bool insert(const Vector& index, int value);
    int find(const Vector& index) const;

    Octree* find_octree(const Vector& index, int chunk_size);
    bool insert_octree(Octree* octree);
};
