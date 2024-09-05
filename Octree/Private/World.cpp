#include "../Public/World.h"

void World::compute_chunk(const Vector& chunk_index) const
{
    const Vector origin_indices = chunk_index * chunk_size_ * 2 + chunk_size_;
    for (int x = origin_indices.x - chunk_size_; x < origin_indices.x + chunk_size_; ++x)
    {
        for (int y = origin_indices.y - chunk_size_; y < origin_indices.y + chunk_size_; ++y)
        {
            for (int z = origin_indices.z - chunk_size_; z < origin_indices.z + chunk_size_; ++z)
            {
                const auto index = Vector(x, y, z);
                octree_->insert(index, 0);
            }
        }
    }
}

void World::read_chunk(const Vector& chunk_index) const
{
    const Vector origin_indices = chunk_index * (chunk_size_ * 2);
    for (int x = origin_indices.x - chunk_size_; x < origin_indices.x + chunk_size_; ++x)
    {
        for (int y = origin_indices.y - chunk_size_; y < origin_indices.y + chunk_size_; ++y)
        {
            for (int z = origin_indices.z - chunk_size_; z < origin_indices.z + chunk_size_; ++z)
            {
                const auto index = Vector(x, y, z);
                octree_->find(index);
            }
        }
    }
}
