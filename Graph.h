//
// Created by Daniel Lengyel on 2019-03-31.
//

#ifndef COMPETITIVE_EROSION_GRAPH_H
#define COMPETITIVE_EROSION_GRAPH_H

#include <vector>
#include <random>

using namespace std;

class CylinderGraph{
public:


    vector<int> graph;
    int X;
    int Y;

    int RED = 1;
    int BLUE = 0;

    // top = RED
    // bottom = BLUE

    int topBlueY; // The highest y coordinate of blue
    int bottomRedY; // the lowest y coordinate of red

    vector<int> numRed; // the number of Red for every y.
    vector<int> numBlue; // the number of Blue for every y.

    vector<int> fluctuations; // the y coordinate of x = 0 as a timeseries. The first non Blue coordinate from bottom.

    CylinderGraph(int N, int M);

    void initializeGraph();

    int getColor(int x, int y);
    int setColor(int x, int y, int color);

    void setTopBlueY();
    void setBottomRedY();

    void MarkovChain(int n, int interval);

    void printGraph();

    vector<int> randomWalk(int startX, int startY, int startColor, std::uniform_real_distribution<double> & dist, std::mt19937 & mt);
};


#endif //COMPETITIVE_EROSION_GRAPH_H
