#pragma once

#include "Model/Octree.h"

class World
{
    Octree* octree_;
    int chunk_size_;

public:
    World(const int world_size, const int chunk_size): chunk_size_(chunk_size)
    {
        octree_ = new Octree(world_size);
    }

    ~World()
    {
        delete octree_;
    }
    
    void compute_chunk(const Vector& chunk_index) const;
    void read_chunk(const Vector& chunk_index) const;
};
