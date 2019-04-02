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
    return graph[y*X + ((x % X) + X) % X];
}

void CylinderGraph::setColor(int x, int y, int color){
    if((y < 0) or (y >= Y)){
        return;
    }

    graph[y*X + ((x % X) + X) % X] = color;
}

std::vector<int> CylinderGraph::randomWalk(int startX, int startY, int startColor){
    int x = startX;
    int y = startY;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, 1);

    while(getColor(x, y) == startColor){
        std::vector<char> neighbors = {'L', 'R'};

        if(y < Y - 2){
            neighbors.push_back('U');
        }
        if(y > 1){
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

void CylinderGraph::MarkovChain(int n) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, X);

    int x;
    std::vector<int> dest;
    int top_y = Y - 1;
    int bottom_y = 0;

    for(int i = 0; i < n; i++){

        // bottom
        x = dist(mt);
        dest = randomWalk(x, bottom_y, 0);
        setColor(dest[0], dest[1], 0);

        // top
        x = dist(mt);
        dest = randomWalk(x, top_y, 1);
        setColor(dest[0], dest[1], 1);

    }
}
int main(){
    CylinderGraph graph(100, 100);
    graph.initializeGraph();
    graph.MarkovChain(10000);

    int count_ones = 0;
    for(int row = 0; row < graph.Y; row++){
        for(int col = 0; col < graph.X; col++){
            int c = graph.getColor(col, row);
            std::cout << c;
            if(c == 1){count_ones++;}
        }
        std::cout << std::endl;
    }
    std::cout << count_ones;
    std::cout << std::endl;

    return 0;
}