//
// Created by Daniel Lengyel on 2019-03-31.
//

#include "Graph.h"

#include <chrono>
#include <iostream>
#include <fstream>


using namespace std;

// x is expected to be within 0 and X-1
// y is expect to be within 0 and Y-1

// assumes X is power of 2
// assumes Y is power of 2

CylinderGraph::CylinderGraph(int X, int Y) {
  CylinderGraph::X = X;
  CylinderGraph::Y = Y;
  CylinderGraph::topBlueY = Y - 1;
  CylinderGraph::bottomRedY = 0;
}

void CylinderGraph::initializeGraph() {
  graph = vector<int>(X * Y, BLUE);
  numRed = vector<int>(Y, 0);
  numBlue = vector<int>(Y, X);
   correlations = vector<double>(X,0);

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, 1);
  CylinderGraph::topBlueY = Y - 1;
  CylinderGraph::bottomRedY = 0;


//  for (int y = 0; y < Y; y++) {
//    for (int x = 0; x < X; x++) {
//      setColor(x, y, (x + y) % 2);
//    }
//  }


    // start with limit shape
  for(int y = 0; y < Y; y++) {
      for (int x = 0; x < X; x++) {
          if(y >= Y/2){ setColor(x, y, RED); }
      }
  }


  setTopBlueY();
  setBottomRedY();


}

int CylinderGraph::getColor(int x, int y) {
    return graph[y * X + ((x % X) + X) % X];
}

void CylinderGraph::setTopBlueY() {
  if ((topBlueY < Y - 1) and (numBlue[topBlueY + 1] != 0)) {
    topBlueY++;
  }  // assumes that topBlueY can only increase by one.
  while ((topBlueY > 0) and (numBlue[topBlueY] == 0)) {
    topBlueY--;
  }
}

void CylinderGraph::setBottomRedY() {
  if ((bottomRedY > 0) and (numRed[bottomRedY - 1] != 0)) {
    bottomRedY--;
  }  // assumes that bottomRedY can only decrease by one.
  while ((bottomRedY < Y - 1) and (numRed[bottomRedY] == 0)) {
    bottomRedY++;
  }
}

void CylinderGraph::setColor(int x, int y, int color) {

  if (color == BLUE) {
    numBlue[y]++;
    numRed[y]--;
  } else {
    numBlue[y]--;
    numRed[y]++;
  }

  graph[y * X + x] = color;
}

std::vector<int> CylinderGraph::randomWalk(
    int startX, int startY, int startColor,
    std::uniform_real_distribution<double>& dist, std::mt19937& mt) {
  int x = startX;
  int y = startY;

  char next_neighbor;
  double rand_neighbor;
  while (graph[y * X + x] == startColor) {
    rand_neighbor = dist(mt);
    if (y == Y-1) {
      if (rand_neighbor <= 0.3333) {
        next_neighbor = 'L';
      } else if (rand_neighbor <= 0.6666) {
        next_neighbor = 'D';
      } else {
        next_neighbor = 'R';
      }
    } else if (y == 0) {
      if (rand_neighbor <= 0.3333) {
        next_neighbor = 'L';
      } else if (rand_neighbor <= 0.6666) {
        next_neighbor = 'U';
      } else {
        next_neighbor = 'R';
      }
    } else {
      if (rand_neighbor <= 0.25) {
        next_neighbor = 'L';
      } else if (rand_neighbor <= 0.5) {
        next_neighbor = 'U';
      } else if (rand_neighbor <= 0.75) {
        next_neighbor = 'D';
      } else {
        next_neighbor = 'R';
      }
    }

    if (next_neighbor == 'U') {
      y++;
    }
    if (next_neighbor == 'D') {
      y--;
    }
    if (next_neighbor == 'R') {
      x = (x + 1) % X;
    }
    if (next_neighbor == 'L') {
      x = (x - 1 + X) % X;
    }
  }
  return {x, y};
}

void CylinderGraph::MarkovChain(int num_samples, int interval) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, X - 1);

  std::random_device rdRW;
  std::mt19937 mtRW(rd());
  std::uniform_real_distribution<double> distRW(0, 1);

  int x;
  std::vector<int> dest;  // TODO make point

  for (int sample = 0; sample < num_samples; sample++) {
    std::cout<<"sample "<<sample<<"..."<<std::endl;
    for (int i = 0; i < interval; i++) {
      // bottom
      x = dist(mt);
      dest = randomWalk(x, max(bottomRedY - 1, 0), BLUE, distRW, mtRW);
      setColor(dest[0], dest[1], BLUE);

      // top
      x = dist(mt);
      dest = randomWalk(x, min(topBlueY + 1, Y - 1), RED, distRW, mtRW);
      setColor(dest[0], dest[1], RED);

      setTopBlueY();
      setBottomRedY();
    }
    std::cout<<"done."<<std::endl;

   // printDensityGraph(32);

    fluctuations.push_back(h(0));

    for (int x = 0; x < X; x++) {
      correlations[x] += (h(x) - Y / 2.0) * (h(0) - Y / 2.0) / num_samples;
    }
  }
}

int CylinderGraph::h(int x) {
  int red_interface = bottomRedY;
  while ((red_interface < Y - 1) and (graph[X * red_interface + x] == BLUE)) {
    red_interface++;
  }

  return red_interface;
}

void CylinderGraph::printDensityGraph(int scale) {
  for (int yb = 0; yb < Y / scale; yb++) {
    for (int xb = 0; xb < X / scale; xb++) {
      double s = 0;
      for (int y = 0; y < scale; y++) {
        for (int x = 0; x < scale; x++) {
          s += getColor(xb * scale + x, yb * scale + y);
        }
      }
      s /= scale * scale;

      if (s < .1)
        std::cout << 1 << " ";
      else if (s > .9)
        std::cout << 0 << " ";
      else
        std::cout << "."
                  << " ";
    }
    std::cout << std::endl;
  }
}

void CylinderGraph::printGraph() {
  for (int y = Y - 1; y >= 0; y--) {
    for (int x = 0; x < X; x++) {
      int c = getColor(x, y);
      std::cout << c;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int oldmain() {
    std::cout << "starting...." << std::endl;
    auto start = std::chrono::steady_clock::now();

    CylinderGraph graph(1024, 1024);
    graph.initializeGraph();
    graph.MarkovChain(100, 300000);
    ofstream myfile;
    myfile.open ("example.txt");

    for (auto x : graph.fluctuations) {
        cout << x << ", ";
    }
    std::cout << std::endl;

    for (auto c : graph.correlations) {
        cout << c;
        cout << ", ";
    }

    myfile.close();
    std::cout << std::endl;
    graph.printDensityGraph(2);

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    typedef std::chrono::duration<float> float_seconds;
    auto secs = std::chrono::duration_cast<float_seconds>(diff);
    std::cout << "Time elapsed: " << secs.count() << " s." << std::endl;

    return 0;
}


ArbitraryGraph::ArbitraryGraph(int N, int num_colors, vector<vector<vector<double>>> domain_starting_regions,
                               function<int(double, double)> domain_function) {
    ArbitraryGraph::N = N;
    ArbitraryGraph::num_colors = num_colors;
    ArbitraryGraph::domain_function = domain_function;
    vector<vector<vector<int>>> starting_regions;
    for(int color = 0; color < num_colors; color++){
        vector<vector<int>> color_region;
        for(int region = 0; region < domain_starting_regions[color].size(); region++){
            vector<int> p;
            p.push_back(int(domain_starting_regions[color][region][0]*(N-1)));
            p.push_back(int(domain_starting_regions[color][region][1]*(N-1)));
            cout << p[0];
            cout << p[1];
            cout << '\n';
            color_region.push_back(p);
        }

        starting_regions.push_back(color_region);
    }
    ArbitraryGraph::starting_regions = starting_regions;
}

void ArbitraryGraph::initializeGraph() {
    graph = vector<int>(N * N, NO_COLOR);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, num_colors - 1);

  for (int y = 0; y < N; y++) {
    for (int x = 0; x < N; x++) {
        if(domain_function(x*1.0/(N -1), y*1.0/(N - 1))){
            int c = dist(mt);
            setColor(x, y, c);
        }
    }
  }
}

int ArbitraryGraph::getColor(int x, int y) {
    if((x < 0) or (x >= N)){
        return NO_COLOR;
    }
    if((y < 0)  or (y >= N)){
        return NO_COLOR;
    }
    return graph[y * N  + x];
}



void ArbitraryGraph::setColor(int x, int y, int color) {
    if((x < 0) or (x >= N)){
        return;
    }
    if((y < 0)  or (y >= N)){
        return;
    }
    graph[y * N + x] = color;
}

std::vector<int> ArbitraryGraph::randomWalk(
        int startX, int startY, int startColor,
        std::uniform_real_distribution<double>& dist, std::mt19937& mt) {
    int x = startX;
    int y = startY;

    char next_neighbor;
    double rand_neighbor;
    while (getColor(x, y) == startColor) {
        rand_neighbor = dist(mt);
        vector<char> valid_neighbors;
        if(getColor(x + 1, y) != NO_COLOR){
            valid_neighbors.push_back('R');
        }
        if(getColor(x - 1, y) != NO_COLOR){
            valid_neighbors.push_back('L');
        }
        if(getColor(x, y+1) != NO_COLOR){
            valid_neighbors.push_back('U');
        }
        if(getColor(x, y-1) != NO_COLOR){
            valid_neighbors.push_back('D');
        }

        for(int i = 1; i < (valid_neighbors.size() + 1); i++){
            if(rand_neighbor < i/(1.0*valid_neighbors.size()) ){
                next_neighbor = valid_neighbors[i-1];
                break;
            }
        }
        // TODO what if no valid neighbor found?

        if (next_neighbor == 'U') {
            y++;
        }
        if (next_neighbor == 'D') {
            y--;
        }
        if (next_neighbor == 'R') {
            x++;
        }
        if (next_neighbor == 'L') {
            x--;
        }
    }
    return {x, y};
}

void ArbitraryGraph::MarkovChain(int num_samples) {

    std::random_device rdRW;
    std::mt19937 mtRW(rdRW());
    std::uniform_real_distribution<double> distRW(0, 1);

    int k;
    vector<int> p;
    vector<int> dest;  // TODO make point

    for (int sample = 0; sample < num_samples; sample++) {

        for (int color = 0; color < num_colors; color++) {

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(0, starting_regions[color].size() - 1);

            k =  dist(mt);
            p = starting_regions[color][k];
            dest = randomWalk(p[0], p[1], color, distRW, mtRW);
            setColor(dest[0], dest[1], color);
        }

        if(sample % 100 == 0){
            printGraph();
        }

    }
}


void ArbitraryGraph::printGraph() {
    for (int y = N - 1; y >= 0; y--) {
        for (int x = 0; x < N; x++) {
            int c = getColor(x, y);
            if(c == 9){
                std::cout << ' ';
            } else{
                std::cout << c;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int always_true(double x, double y){
    return 1;
}

int circle(double x, double y){
    return (x - 0.5)*(x - 0.5) + (y - 0.5)*(y - 0.5) < 0.25;
}

int main() {
    std::cout << "starting...." << std::endl;
    auto start = std::chrono::steady_clock::now();

    vector<vector<vector<double>>> starting_pos  = {{{ 0.5 - 0.86 * 0.5, 0.05 + 0.25}}, {{0.5, 0.95}}, {{0.5 + 0.86 * 0.5, 0.25}}}; // is starting pos on domain or lattice? Should be domain.  Fix it!

    ArbitraryGraph graph(100, 3, starting_pos, circle);
    graph.initializeGraph();
    graph.printGraph();


    graph.MarkovChain(1000000);
//    ofstream myfile;
//    myfile.open ("example.txt");
//
//    for (auto x : graph.fluctuations) {
//        cout << x << ", ";
//    }
//    std::cout << std::endl;
//
//    for (auto c : graph.correlations) {
//        cout << c;
//        cout << ", ";
//    }
//
//    myfile.close();
//    std::cout << std::endl;
//    graph.printDensityGraph(2);
//
//    auto end = std::chrono::steady_clock::now();
//    auto diff = end - start;
//    typedef std::chrono::duration<float> float_seconds;
//    auto secs = std::chrono::duration_cast<float_seconds>(diff);
//    std::cout << "Time elapsed: " << secs.count() << " s." << std::endl;

    return 0;
}

