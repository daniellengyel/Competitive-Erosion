//
// Created by Daniel Lengyel on 2019-03-31.
//


#include <random>
#include <vector>

#ifndef COMPETITIVE_EROSION_GRAPH_H
#define COMPETITIVE_EROSION_GRAPH_H


using namespace std;

class CylinderGraph {
public:
    vector<int> graph;
    int X;
    int Y;

    int RED = 1;
    int BLUE = 0;

    // top = RED
    // bottom = BLUE

    int topBlueY;    // The highest y coordinate of blue
    int bottomRedY;  // the lowest y coordinate of red

    vector<int> numRed;   // the number of Red for every y.
    vector<int> numBlue;  // the number of Blue for every y.

    vector<int> fluctuations;  // the y coordinate of x = 0 as a timeseries. The
    // first non Blue coordinate from bottom.

    vector<double> correlations;  // correlations[i] gives E[(h[0] -
    // E[h[0]])*(h[i] - E[h[i]])], where h[i] is the
    // height of the interface at x = i.

    CylinderGraph(int N, int M);

    void initializeGraph();

    int h(int x);  // height of the interface at x.
    int getColor(int x, int y);
    void setColor(int x, int y, int color);

    void setTopBlueY();
    void setBottomRedY();

    void MarkovChain(int num_samples, int interval);

    void printGraph();

    // This function is for easier visualization. It computes a new cylinder of dimension X/scale x Y/scale
    // where each matrix element corresponds to a scale*scale sized block of the original cylinder, and is
    // equal to either "1", "." or "0" depending on the proportion of 1 and 0 in the block.
    void printDensityGraph(int scale);

    vector<int> randomWalk(int startX, int startY, int startColor,
                           std::uniform_real_distribution<double>& dist,
                           std::mt19937& mt);
};



class ArbitraryGraph {
public:
    int N;
    vector<int> graph; // (x,y) ->  y*N + x
    int NO_COLOR = -1;

    int num_colors; // number of colors
    vector<vector<vector<int>>> starting_regions;

    // boundries are [0, 1]^2
    function<int(double, double)> domain_function; // (x, y) -> bool

    ArbitraryGraph(int N, int num_colors, vector<vector<vector<int>>> starting_regions, function<int(double, double)> domain_function);

    void initializeGraph();

    int getColor(int x, int y);
    void setColor(int x, int y, int color);

    void MarkovChain(int num_samples);

    void printGraph();


    vector<int> randomWalk(int startX, int startY, int startColor,
                           std::uniform_real_distribution<double>& dist,
                           std::mt19937& mt);
};

#endif //COMPETITIVE_EROSION_GRAPH_H
