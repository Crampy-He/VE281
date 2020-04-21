#include <iostream>
#include <getopt.h>
#include "priority_queue.h"
#include "binary_heap.h"
#include "unsorted_heap.h"
#include "fib_heap.h"
#include <vector>

using namespace std;
class point{
public:
    int x,y;
    int weight,cost;
    bool reach=false;
    point * predecessor= nullptr;
    struct point_compare{
        bool operator()(const point &a, const point &b) const{
            if(a.cost!=b.cost){
                return a.cost < b.cost;
            }else if(a.x!=b.x){
                return a.x < b.x;
            }else{
                return a.y <b.y;
            }
        }
    };

};
void trace_back(point *p);
int main(int argc, char*  argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    bool verbose = false;
    string heap_type;
    while (true) {
        const option long_options[] = {
                {"verbose",        no_argument,       NULL, 'v'},
                {"implementation", required_argument, NULL, 'i'},
                {0,                0, 0,                    0}
        };
        int result = getopt_long(argc, argv, "vi:", long_options, NULL);
        if (result == -1)break;
        if (result == '?')break;
        if (result == 'v') {
            verbose = true;
        } else if (result == 'i') {
            heap_type = optarg;
        }
    }
    priority_queue<point, point::point_compare> *queue;
    if (heap_type == "BINARY") {
        queue = new binary_heap<point, point::point_compare>();
    } else if (heap_type == "UNSORTED") {
        queue = new unsorted_heap<point, point::point_compare>();
    } else if (heap_type == "FIBONACCI") {
        queue = new fib_heap<point, point::point_compare>();
    }
    int width, height = 0;
    cin >> width;
    cin >> height;
    int start_x, start_y, end_x, end_y;
    cin >> start_x >> start_y >> end_x >> end_y;
    vector<vector<point*>> grid;
    grid.resize(width);
    for(int i=0; i<width;  i++){
        grid[i].resize(height);
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = new point;
            cin >> grid[i][j]->weight;
            grid[i][j]->x = j;
            grid[i][j]->y = i;
            grid[i][j]->cost = grid[i][j]->weight;
        }
    }
    grid[start_y][start_x]->reach = true;
    queue->enqueue(*grid[start_x][start_y]);
    int step = 0;
    while (!queue->empty()) {
        auto temp = queue->dequeue_min();
        if (verbose) {
            cout << "Step " << step << endl;
            cout << "Choose cell (" << temp.x << ", " << temp.y
                 << ") with accumulated length " << temp.cost << "." << endl;
        }
        step++;
        int add_x[4] = {1, 0, -1, 0};
        int add_y[4] = {0, 1, 0, -1};
        for (int i = 0; i < 4; i++) {
            int neigh_x, neigh_y;
            neigh_x = temp.x + add_x[i];
            neigh_y = temp.y + add_y[i];
            if (neigh_x < 0 || neigh_y < 0 ||  neigh_x > width - 1 ||neigh_y > height - 1) {
                continue;
            } else if(grid[neigh_y][neigh_x]->reach ){
                continue;
            }
            else {
                grid[neigh_y][neigh_x]->cost = grid[temp.y][temp.x]->cost + grid[neigh_y][neigh_x]->weight;
                grid[neigh_y][neigh_x]->reach = true;
                grid[neigh_y][neigh_x]->predecessor = grid[temp.y][temp.x];
                if (end_x == neigh_x && neigh_y == end_y) {
                    if (verbose) {
                        cout << "Cell (" << end_x << ", " << end_y << ") with accumulated length "
                             << grid[neigh_y][neigh_x]->cost << " is the ending point." << endl;
                    }
                    cout << "The shortest path from (" << start_x << ", " << start_y << ") to (" << end_x << ", "
                         << end_y << ") is " << grid[neigh_y][neigh_x]->cost << "." << endl;
                    cout << "Path:" << endl;
                    trace_back(grid[end_y][end_x]);
                    delete queue;
                    for (i = 0; i < height; i++) {
                        for (int j = 0; j < width; j++) {
                            delete (grid[i][j]);
                        }
                    }
                    return 0;
                } else {
                    queue->enqueue(*grid[neigh_y][neigh_x]);
                    if (verbose) {
                        cout << "Cell (" << neigh_x << ", " << neigh_y << ") with accumulated length "
                             << grid[neigh_y][neigh_x]->cost << " is added into the queue." << endl;
                    }
                }
            }
        }
    }
    delete queue;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            delete (grid[i][j]);
        }
    }
}

void trace_back(point *p){
    if(p!= nullptr)
    {
        trace_back(p->predecessor);
        cout<<"("<<p->x<<", "<<p->y<<")"<<endl;
    }
    else
    {
        return;
    }
}



