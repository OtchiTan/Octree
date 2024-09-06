#include "../Public/World.h"

void World::compute_chunk(const Vector& chunk_index)
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
    const Vector origin_indices = chunk_index * chunk_size_ * 2 + chunk_size_;
    for (int x = origin_indices.x - chunk_size_; x < origin_indices.x + chunk_size_; ++x)
    {
        for (int y = origin_indices.y - chunk_size_; y < origin_indices.y + chunk_size_; ++y)
        {
            for (int z = origin_indices.z - chunk_size_; z < origin_indices.z + chunk_size_; ++z)
            {
                const auto index = Vector(x, y, z);
                const auto data = octree_->find(index);
                if (data == -1)
                {
                    std::cout << "Not Found\n";
                }
            }
        }
    }
}

void World::compute_world(const Vector& chunk_index)
{
    for (int cx = chunk_index.x - render_distance_; cx < chunk_index.x + render_distance_; ++cx)
    {
        for (int cy = chunk_index.y - render_distance_; cy < chunk_index.y + render_distance_; ++cy)
        {
            for (int cz = chunk_index.z - render_distance_; cz < chunk_index.z + render_distance_; ++cz)
            {
                compute_chunk({cx,cy,cz});
            }
        }
    }
}

void World::read_world(const Vector& chunk_index) const
{
    for (int cx = chunk_index.x - render_distance_; cx < chunk_index.x + render_distance_; ++cx)
    {
        for (int cy = chunk_index.y - render_distance_; cy < chunk_index.y + render_distance_; ++cy)
        {
            for (int cz = chunk_index.z - render_distance_; cz < chunk_index.z + render_distance_; ++cz)
            {
                read_chunk({cx,cy,cz});
            }
        }
    }
}

int World::get_block(const Vector& block_index) const
{
    return octree_->find(block_index);
}
