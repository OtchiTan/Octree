#pragma once

#include "Model/Octree.h"

class World
{
    Octree* octree_;
    int chunk_size_;
    int render_distance_;
    std::mutex mutex_;

public:
    World(const int chunk_size, const int render_distance): chunk_size_(chunk_size), render_distance_(render_distance)
    {
        octree_ = new Octree(67108864);
    }

    ~World()
    {
        delete octree_;
    }
    
    void compute_chunk(const Vector& chunk_index);
    void read_chunk(const Vector& chunk_index) const;

    void compute_world(const Vector& chunk_index);
    void read_world(const Vector& chunk_index) const;

    int get_block(const Vector& block_index) const;
};
