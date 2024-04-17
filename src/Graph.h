#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include "Hash.cpp"

class AirportNeighbor;

class AirportGraph{
    private:
        HashMap<std::string, std::vector<AirportNeighbor>> adj_map;

    public:
        AirportGraph();
        std::vector<AirportNeighbor>& operator[](const std::string& airport_name);

        void add_edge(const std::string& from, const std::string& to, int distance, int cost);
};

class AirportNeighbor{
    private:
        std::string neighbor;
        int distance;
        int cost;
        AirportNeighbor(const std::string& neighbor_name, int the_distance, int the_cost);

    public:
        const std::string& get_neighbor() const;
        const int& get_distance() const;
        const int& get_cost() const;
    
    friend class AirportGraph;
};

#endif
