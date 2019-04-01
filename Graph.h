//
// Created by Daniel Lengyel on 2019-03-31.
//

#ifndef COMPETITIVE_EROSION_GRAPH_H
#define COMPETITIVE_EROSION_GRAPH_H

#include <vector>

class CylinderGraph{
public:
    std::vector<int> graph;
    int X;
    int Y;

    CylinderGraph(int N, int M);

    void initializeGraph();

    int getColor(int x, int y);
};


#endif //COMPETITIVE_EROSION_GRAPH_H
