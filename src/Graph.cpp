#include "Graph.h"


AirportGraph::AirportGraph() : adj_map(){};

std::vector<AirportNeighbor>& AirportGraph::operator[](const std::string& airport_name){
    return adj_map[airport_name];
}

void AirportGraph::add_edge(const std::string& from, const std::string& to, int distance, int cost){
    adj_map[from].push_back(AirportNeighbor(to, distance, cost));
}


AirportNeighbor::AirportNeighbor(const std::string& neighbor_name, int the_distance, int the_cost){
    neighbor = neighbor_name;
    distance = the_distance;
    cost = the_cost;
}

const std::string& AirportNeighbor::get_neighbor() const{
    return neighbor;
}
const int& AirportNeighbor::get_distance() const{
    return distance;
}
const int& AirportNeighbor::get_cost() const{
    return cost;
}
