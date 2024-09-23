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

    mutex_.lock();
    if (children_[position] == nullptr)
    {
        delete children_[position];
        children_[position] = new Octree(index, ray_ >> 1, value);
        mutex_.unlock();
        return true;
    }
    
    if (children_[position]->point_ == nullptr)
    {
        mutex_.unlock();
        return children_[position]->insert(index, value);
    }
    
    if (children_[position]->point_->index == index)
    {
        children_[position]->point_->value = value;
        mutex_.unlock();
        return true;
    }

    const Point old_stored_point = {*children_[position]->point_};
    delete children_[position];
    children_[position] = nullptr;

    const int new_ray = ray_ >> 1;

    children_[position] = new Octree(
        new_ray,
        PointPosition[position] * new_ray + center_
    );

    mutex_.unlock();
    
    children_[position]->insert(old_stored_point.index, old_stored_point.value);
    return children_[position]->insert(index, value);
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

    mutex_.lock();
    if (children_[position] == nullptr)
    {
        mutex_.unlock();
        return -1;
    }
    
    if (children_[position]->point_ == nullptr)
    {
        mutex_.unlock();
        return children_[position]->find(index);
    }

    if (index == children_[position]->point_->index)
    {
        mutex_.unlock();
        return children_[position]->point_->value;
    }

    mutex_.unlock();
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

    mutex_.lock();
    if (children_[position]->point_ == nullptr)
    {
        mutex_.unlock();
        return children_[position]->find_octree(index, chunk_size);
    }

    mutex_.unlock();
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

    mutex_.lock();
    if (children_[position] == nullptr)
    {
        delete children_[position];
        children_[position] = new Octree(ray_ >> 1, octree->center_);
        mutex_.unlock();
        return true;
    }
    
    if (children_[position]->point_ == nullptr)
    {
        mutex_.unlock();
        return children_[position]->insert_octree(octree);
    }
    
    if (ray_ >> 1 == octree->ray_)
    {
        delete children_[position];
        children_[position] = octree;
        mutex_.unlock();
        return true;
    }
    
    mutex_.unlock();
    return children_[position]->insert_octree(octree);
}
