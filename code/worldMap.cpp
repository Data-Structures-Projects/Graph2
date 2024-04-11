#include "worldMap.hpp"
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

WorldMap::WorldMap() {}

void WorldMap::createWorldMap(string fileName)
{
    ifstream input;
    input.open(fileName);
    if(!input.is_open()) {
        cout <<"File does not exist / unable to open. Exiting.";
        return;
    }
    string data;
    string line;
    int initalize = -1;
    int row = -1;
    int col = 0;
    while(getline(input, line)){
        stringstream str(line);
        if(initalize == -1){
            while(getline(str, data, ' ')){
                if(initalize == -1){
                    rows = stoi(data);
                    initalize = 0;
                }else{
                    cols = stoi(data);
                }
            }
           worldMap = (int**)malloc(sizeof(int**)*rows);
           row = 0;
        }else{
            worldMap[row] = (int*)malloc(sizeof(int*)*cols);
            while(getline(str,data,' ')){
                worldMap[row][col] = stoi(data);
                col = col + 1;
            }
            row = row + 1;
            col = 0;
        }
    } 
    // TODO END
}

void WorldMap::printWorldMap()
{
    for(int i = 0; i< rows; i++){
        for(int j = 0; j<cols; j++){
            cout<<"| "<< worldMap[i][j] << " ";
        }
            cout<< "|" << endl;
    }
}

void WorldMap::addRegion(int x, int y)
{
    for(int i = 0; i < regions.size(); i++){
        if(regions.at(i)->x == x && regions.at(i)->y == y){
            return;
        }
    }
    Region* temp = new Region;
    temp->x = x;
    temp->y = y;
    regions.push_back(temp);
}

bool checkIfEdgeExists(Region *r, int x2, int y2)
{
    for(int i = 0; i < r->neighbors.size(); i++){
        if(r->neighbors.at(i).region->x == x2 && r->neighbors.at(i).region->y == y2){
            return true;
        }
    }
    return false;
}

void WorldMap::addEdgeBetweenRegions(int x1, int y1, int x2, int y2)
{
    Region* one = nullptr;
    Region* two = nullptr;
    for(int i = 0; i < regions.size(); i++){
        if(regions.at(i)->x == x1 && regions.at(i)->y == y1){
           one = regions.at(i);
        }
        if(regions.at(i)->x == x2 && regions.at(i)->y == y2){
           two = regions.at(i);
        }
    }
    if(one == nullptr || two == nullptr){
        return;
    }
    if(checkIfEdgeExists(one, x2, y2)){
        return;
    }else{
        NeighboringRegion* n_one = new NeighboringRegion;
        n_one->region = one;
        NeighboringRegion* n_two = new NeighboringRegion;
        n_two->region = two;
        one->neighbors.push_back(*n_two);
        two->neighbors.push_back(*n_one);
    }
}

vector<vector<int>> WorldMap::findAdjacentLandRegions(int x, int y)
{
    // TODO START
    vector<vector<int>> adjacent;
    vector<int> location;
    Region* one;
    int row;
    int col;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j<2; j++){
            row = i + x;
            col = j + y;
            if(0 <= row && row < rows && 0 <= col  && col < cols){
                if(row != x || col != y){
                    if(worldMap[row][col] == 1){
                        location.push_back(row);
                        location.push_back(col);
                        adjacent.push_back(location);
                        location.clear();
                    }
                }
            }
        }
    }
    return adjacent;
    // TODO END
}

void WorldMap::convertWorldMapToAdjacencyListGraph()
{
    vector<vector<int>> adjacent;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(worldMap[i][j] == 1){
                addRegion(i,j);
            }
        }
    }
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(worldMap[i][j] == 1){
                adjacent = findAdjacentLandRegions(i,j);
                while(!adjacent.empty()){
                    addEdgeBetweenRegions(i, j, adjacent.front().front(), adjacent.front().back());
                    adjacent.erase(adjacent.begin());
                }
            }
        }
    }
}

// helper function to check if v2 is a neighbor of verter v1
bool isNeighbor(int x1, int y1, int x2, int y2, const vector<Region *> regions)
{
    return false;
}

WorldMap::~WorldMap()
{
    // TODO START

    // TODO END
}

void WorldMap::displayEdges()
{
    for(int i = 0; i < regions.size(); i++){
        cout<<"("<<regions[i]->x<<','<<regions[i]->y<<")"<<" --> ";
        for(int j = 0; j < regions[i]->neighbors.size(); j++){
            cout<<"("<<regions[i]->neighbors[j].region->x<<','<<regions[i]->neighbors[j].region->y<<")"<<" ";
        }
        cout<<endl;
    }
}

// HELPER FOR findNumOfIslands
void findNumOfIslandsHelper(Region *r)
{
    if(r->visited){
        return;
    }
    r->visited = true;
    cout << "Reached vertex: "<<"("<<r->x<<','<<r->y<<")"<<endl; 
    for(int i = 0; i < r->neighbors.size(); i++){
        if(!r->neighbors.at(i).region->visited){
            findNumOfIslandsHelper(r->neighbors.at(i).region);
        }
    }
    return;
}

int WorldMap::findNumOfIslands()
{
    int count = 0;
    for(int i = 0; i < regions.size(); i++){
        if(!regions[i]->visited){
            count++;
            findNumOfIslandsHelper(regions[i]);
        }
    }
    return count;
    
}

vector<Region *> &WorldMap::getRegions()
{
    return regions;
}
