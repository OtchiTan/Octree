#include <chrono>
#include <iostream>
#include <thread>

#include "Public/World.h"
#include "Public/Model/Octree.h"

using namespace std;
using namespace chrono;

int render_distance = 4;
int chunk_size = 16;
int chunks_to_compute = 10;

int main()
{
    time_point<system_clock> start, end;
    cout << "Start\n";

    vector<World*> worlds;
    worlds.assign(chunks_to_compute,nullptr);

    for (int i = 0; i < chunks_to_compute; ++i)
    {
        worlds[i] = new World(chunk_size, render_distance);
    }
    
    start = system_clock::now();

    for (int i = 0; i < chunks_to_compute; ++i)
    {
        worlds[i]->compute_chunk({0});
    }

    end = system_clock::now();

    cout << "Mono thread Insert Duration " << duration_cast<milliseconds>(end - start).count() << " ms \n";
    
    for (int i = 0; i < chunks_to_compute; ++i)
    {
        worlds[i] = new World(chunk_size, render_distance);
    }

    vector<thread> threads;
    threads.resize(chunks_to_compute);
    
    for (int i = 0; i < chunks_to_compute; ++i)
    {
        threads[i] = thread(&World::compute_chunk,worlds[i],Vector(0));
    }

    start = system_clock::now();
    
    for (int i = 0; i < chunks_to_compute; ++i)
    {
        threads[i].join();
    }

    end = system_clock::now();
    
    cout << "Multi thread Insert Duration " << duration_cast<milliseconds>(end - start).count() << " ms \n";
    return 0;
}
