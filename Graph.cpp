//
// Created by Daniel Lengyel on 2019-03-31.
//

#include "Graph.h"
#include <random>
#include <vector>
#include <iostream>

using namespace std;

CylinderGraph::CylinderGraph(int X, int Y){
    CylinderGraph::X = X;
    CylinderGraph::Y = Y;
    CylinderGraph::topBlueY = Y-1;
    CylinderGraph::bottomRedY = 0;

}

void CylinderGraph::initializeGraph(){

    graph = vector<int>(X * Y, 0);
    numRed = vector<int>(Y, 0);
    numBlue = vector<int>(Y, 0);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    CylinderGraph::topBlueY = Y-1;
    CylinderGraph::bottomRedY = 0;

    for(int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            int color = dist(mt);
            if (color == RED) {
                numRed[y]++;
            } else {
                numBlue[y]++;
            }
            setColor(x, y, color);
        }
    }

    setTopBlueY();
    setBottomRedY();
}

void CylinderGraph::setTopBlueY(){
    if((topBlueY + 1 <= Y - 1) and (numBlue[topBlueY + 1] != 0)) {topBlueY++;} // assumes that topBlueY can only increase by one.
    while((topBlueY > 0) and (numBlue[topBlueY] == 0)) {topBlueY--;}
}

void CylinderGraph::setBottomRedY() {
    if((bottomRedY - 1 >= 0) and (numRed[bottomRedY - 1] != 0)) {bottomRedY--;}// assumes that bottomRedY can only decrease by one.
    while((bottomRedY < Y - 1) and (numRed[bottomRedY] == 0)) {bottomRedY++;}
}

int CylinderGraph::getColor(int x, int y){
    if((y < 0) or (y >= Y)){
        return -1;
    }
    return graph[y*X + ((x % X) + X) % X];
}

int CylinderGraph::setColor(int x, int y, int color){
    if((y < 0) or (y >= Y)){
        return -1;
    }

    graph[y*X + ((x % X) + X) % X] = color;
    return 1;
}

std::vector<int> CylinderGraph::randomWalk(int startX, int startY, int startColor){
    int x = startX;
    int y = startY;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, 1);

    char next_neighbor;
    double rand_neighbor;
    while((getColor(x, y) != -1) and (getColor(x, y) == startColor)){
        rand_neighbor = dist(mt);
        if(y == Y - 1) {
            if (rand_neighbor <= 0.3333) {
                next_neighbor = 'L';
            } else if (rand_neighbor <= 0.6666) {
                next_neighbor = 'D';
            } else {
                next_neighbor = 'R';
            }
        } else if(y == 0){
            if(rand_neighbor <= 0.3333){
                next_neighbor = 'L';
            } else if(rand_neighbor <= 0.6666){
                next_neighbor = 'U';
            } else {
                next_neighbor = 'R';
            }
        } else {
            if(rand_neighbor <= 0.25){
                next_neighbor = 'L';
            } else if(rand_neighbor <= 0.5){
                next_neighbor = 'U';
            } else if(rand_neighbor <= 0.75) {
                next_neighbor = 'D';
            } else {
                next_neighbor = 'R';
            }
        }

        if(next_neighbor == 'U'){y++;}
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
    std::vector<int> dest; // TODO make point
    int err;
    for(int i = 0; i < n; i++){

        // bottom
        x = dist(mt);
        dest = randomWalk(x, max(bottomRedY - 1, 0), BLUE);
        err = setColor(dest[0], dest[1], BLUE);
        if(err == -1){ break; }

        // top
        x = dist(mt);
        dest = randomWalk(x, min(topBlueY + 1, Y - 1), RED);
        err = setColor(dest[0], dest[1], RED);
        if(err == -1){ break; }

        // resetting top/bottom pointers.
        setTopBlueY();
        setBottomRedY();

        if(i % 1000 == 0){
            cout << i;
            cout << std::endl;
            printGraph();

        }


    }
}

void CylinderGraph::printGraph() {

    for(int y = Y-1; y >= 0; y--){
        for(int x = 0; x < X; x++){
            int c = getColor(x, y);
            std::cout << c;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(){
    CylinderGraph graph(512, 512);
    graph.initializeGraph();
    graph.MarkovChain(100000);

    return 0;
}