#include "../../Public/Model/Octree.h"

position Octree::get_position(const Vector& index) const
{
    if (index.x < center_.x)
    {
        if (index.y < center_.y)
        {
            if (index.z < center_.z)
                return back_left_bottom;
            return back_left_top;
        }
        if (index.z < center_.z)
            return back_right_bottom;
        return back_right_top;
    }
    if (index.y < center_.y)
    {
        if (index.z < center_.z)
            return front_left_bottom;
        return front_left_top;
    }
    if (index.z < center_.z)
        return front_right_bottom;
    return front_right_top;
}

bool Octree::insert(const Vector& index, const int value)
{
    if (index.x >= center_.x + ray_ ||
        index.x < center_.x - ray_ ||
        index.y >= center_.y + ray_ ||
        index.y < center_.y - ray_ ||
        index.z >= center_.z + ray_ ||
        index.z < center_.z - ray_
    )
    {
        std::cout << "Out of octree\n";
        return false;
    }

    const position position = get_position(index);

    if (children_[position] == nullptr)
    {
        Octree* old_node = nullptr;
        Octree* new_node = new Octree(index, ray_ >> 1, value);

        if (children_[position].compare_exchange_weak(old_node,new_node))
        {
            return true;
        }
        delete new_node;
        return old_node->insert(index, value);
    }
    
    if (children_[position].load()->point_ == nullptr)
    {
        return children_[position].load()->insert(index, value);
    }
    
    if (children_[position].load()->point_->index == index)
    {
        children_[position].load()->point_->value = value;
        return true;
    }

    Octree* old_node = children_[position];
    const Point old_stored_point = {*old_node->point_};
    
    const int new_ray = ray_ >> 1;
    
    Octree* new_node = new Octree(
        new_ray,
        PointPosition[position] * new_ray + center_
    );

    if (children_[position].compare_exchange_weak(old_node, new_node))
    {
        new_node->insert(old_stored_point.index, old_stored_point.value);
        return new_node->insert(index, value);
    }
    delete new_node;
    
    old_node->insert(old_stored_point.index, old_stored_point.value);
    return old_node->insert(index, value);
}

int Octree::find(const Vector& index)
{
    if (index.x >= center_.x + ray_ ||
       index.x < center_.x - ray_ ||
       index.y >= center_.y + ray_ ||
       index.y < center_.y - ray_ ||
       index.z >= center_.z + ray_ ||
       index.z < center_.z - ray_
   )
    {
        std::cout << "Out of octree\n";
        return -1;
    }
    const position position = get_position(index);

    if (children_[position] == nullptr)
    {
        return -1;
    }
    
    if (children_[position].load()->point_ == nullptr)
    {
        return children_[position].load()->find(index);
    }

    if (index == children_[position].load()->point_->index)
    {
        return children_[position].load()->point_->value;
    }

    return -1;
}

Octree* Octree::find_octree(const Vector& index, int chunk_size)
{
    if (index.x >= center_.x + ray_ ||
       index.x < center_.x - ray_ ||
       index.y >= center_.y + ray_ ||
       index.y < center_.y - ray_ ||
       index.z >= center_.z + ray_ ||
       index.z < center_.z - ray_
   )
    {
        std::cout << "Out of octree\n";
        return nullptr;
    }

    if (ray_ == chunk_size)
        return this;
    
    const position position = get_position(index);

    if (children_[position].load()->point_ == nullptr)
    {
        return children_[position].load()->find_octree(index, chunk_size);
    }

    return nullptr;
}

bool Octree::insert_octree(Octree* octree)
{    
    if (octree->center_.x >= center_.x + ray_ ||
         octree->center_.x < center_.x - ray_ ||
         octree->center_.y >= center_.y + ray_ ||
         octree->center_.y < center_.y - ray_ ||
         octree->center_.z >= center_.z + ray_ ||
         octree->center_.z < center_.z - ray_
     )
    {
        std::cout << "Out of octree\n";
        return false;
    }
    
    const position position = get_position(octree->center_);

    if (ray_ >> 1 == octree->ray_)
    {
        delete children_[position];
        children_[position] = octree;
        return true;
    }
    
    if (children_[position] == nullptr)
    {
        Octree* old_node = nullptr;
        const int new_ray = ray_ >> 1;
    
        Octree* new_node = new Octree(
            new_ray,
            PointPosition[position] * new_ray + center_
        );

        if (children_[position].compare_exchange_weak(old_node,new_node))
        {
            return new_node->insert_octree(octree);
        }
        delete new_node;
        return old_node->insert_octree(octree);
    }
    
    return children_[position].load()->insert_octree(octree);
}
