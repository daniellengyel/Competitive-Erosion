//
// Created by Daniel Lengyel on 2019-03-31.
//

#include "Graph.h"
#include <random>
#include <vector>
#include <iostream>

CylinderGraph::CylinderGraph(int X, int Y){
    CylinderGraph::X = X;
    CylinderGraph::Y = Y;
}

void CylinderGraph::initializeGraph(){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    for(int i = 0; i < X*Y; i++){
        graph.push_back(dist(mt));
    }
}

int CylinderGraph::getColor(int x, int y){
    if((y < 0) or (y >= Y)){
        return -1;
    }
    return graph[y*(x % X)];
}

std::vector<int> randomWalk(int startX, int startY, int startColor, CylinderGraph *graph){
    int x = startX;
    int y = startY;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, 1);

    while(graph->getColor(x, y) == startColor){
        std::vector<char> neighbors = {'L', 'R'};

        if(y < graph->Y - 1){
            neighbors.push_back('U');
        }
        if(y > 0){
            neighbors.push_back('D');
        }

        double rand_neighbor = dist(mt);
        char next_neighbor = 'N';


        for(int n = 0; n < neighbors.size(); n++){
            if(rand_neighbor <= 1.0*(1 + n)/neighbors.size()){
                next_neighbor = neighbors[n];
                break;
            }
        }
        if(next_neighbor == 'N') {return {};}
        if(next_neighbor == 'U'){ y++;}
        if(next_neighbor == 'D'){y--;}
        if(next_neighbor == 'R'){x++;}
        if(next_neighbor == 'L'){x--;}

    }
    return {x, y};
}

int main(){
    CylinderGraph graph(10, 10);
    graph.initializeGraph();

    std::vector<int> dest = randomWalk(0, 0, graph.getColor(0,0), &graph);

    std::cout << graph.getColor(0,0) << std::endl;
    if(dest.empty()){
        std::cout << "shit" << std::endl;
    }
    std::cout << graph.getColor(dest[0], dest[1]) << std::endl;

    return 0;
}