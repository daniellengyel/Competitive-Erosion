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

    for(int x = 0; x < X; x++){
        setColor(x, Y - 1, 0); // top make 0
        setColor(x, 0, 1); // bottom make 1
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

std::vector<int> randomWalk(int startX, int startY, int startColor, CylinderGraph *graph){
    int x = startX;
    int y = startY;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, 1);

    while(graph->getColor(x, y) == startColor){
        std::vector<char> neighbors = {'L', 'R'};

        if(y < graph->Y - 2){
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

int main(){
    CylinderGraph graph(10, 10);
    graph.initializeGraph();
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, graph.X);

    int x;
    std::vector<int> dest;
    int top_y = graph.Y - 1;
    int bottom_y = 0;
    for(int i = 0; i < 1000; i++){

        if(i % 100 == 0){
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
        }


        // bottom
        x = dist(mt);
        dest = randomWalk(x, bottom_y, graph.getColor(x,bottom_y), &graph);
        graph.setColor(dest[0], dest[1], graph.getColor(x,bottom_y));

        // top
        x = dist(mt);
        dest = randomWalk(x, top_y, graph.getColor(x, top_y), &graph);
        graph.setColor(dest[0], dest[1], graph.getColor(x,top_y));



    }



    return 0;
}