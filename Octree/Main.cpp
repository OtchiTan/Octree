#include <chrono>
#include <iostream>
#include <thread>

#include "Public/World.h"
#include "Public/Model/Octree.h"

using namespace std;
using namespace chrono;

int octree_size = 67108864;
int render_distance = 2;
int chunk_size = 16;

int main()
{
    cout << "Start\n";

    const auto world = new World(octree_size,chunk_size);
    vector<thread> threads;
    
    cout << "Start Generation\n";

    time_point<system_clock> start = system_clock::now();

    for (int cx = -render_distance; cx < render_distance; ++cx)
    {
        for (int cy = -render_distance; cy < render_distance; ++cy)
        {
            for (int cz = -render_distance; cz < render_distance; ++cz)
            {
                if (Vector(cx,cy,cz) != Vector(0))
                {
                    world->compute_chunk({cx,cy,cz});
                }
            }
        }
    }

    time_point<system_clock> end = system_clock::now();

    cout << "Insert Duration " << duration_cast<seconds>(end - start).count() << " s \n";
    cout << "Insert Duration " << duration_cast<milliseconds>(end - start).count() << " ms \n";

    start = system_clock::now();
    for (int cx = 0; cx < render_distance; ++cx)
    {
        for (int cy = 0; cy < render_distance; ++cy)
        {
            for (int cz = 0; cz < render_distance; ++cz)
            {
                world->read_chunk({cx,cy,cz});
            }
        }
    }
    
    end = system_clock::now();

    cout << "Read Duration " << duration_cast<seconds>(end - start).count() << " s \n";
    cout << "Read Duration " << duration_cast<milliseconds>(end - start).count() << " ms \n";

    return 0;
}
