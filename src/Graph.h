#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <climits>
#include "Hash.cpp"
#include "Minheap.cpp"

class AirportNeighbor;
class Path;
class Edge;

class AirportGraph{
    private:
        HashMap<std::string, std::vector<AirportNeighbor>> adj_map;
        HashMap<std::string, unsigned> num_connections;

    public:
        AirportGraph();
        std::vector<AirportNeighbor>& operator[](const std::string& airport_name);

        void add_edge(const std::string& from, const std::string& to, int distance, int cost);
        void add_edge_undirected(const std::string& from, const std::string& to, int distance, int cost);
        Path dijkstra(const std::string& from, const std::string& to) const;
        Path dijkstra_n_stops(const std::string& from, const std::string& to, int required_stops) const;
        unsigned get_num_connections(const std::string& airport_name) const;

        static AirportGraph make_undirected(const AirportGraph& graph);
        std::vector<Edge> kruskal_mst() const;
        std::vector<Edge> prim_mst() const;
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

class Edge{
    private:
        std::string from;
        std::string to;
        int weight;

        Edge(const std::string& came_from, const std::string& go_to, int the_weight);
    public:
        Edge();
        bool operator==(const Edge& other) const;
        bool operator>(const Edge& other) const;
        bool operator<(const Edge& other) const;

        const std::string& get_from() const;
        const std::string& get_to() const;
        const int& get_weight() const;

    friend class AirportGraph;
};

class StopInfo{
    private:
        std::string name;
        std::string from;
        int distance;
        int cost;
        int num_stops;
        StopInfo(const std::string& the_name, const std::string& from,int the_distance, int the_cost, int num_stops);

    public:
        StopInfo();
        bool operator==(const StopInfo& other) const;
        bool operator>(const StopInfo& other) const;
        bool operator<(const StopInfo& other) const;

        const std::string& get_name() const;
        const std::string& get_from() const;
        const int& get_distance() const;
        const int& get_cost() const;
        const int& get_num_stops() const;

    friend class AirportGraph;
};

class Path{
    private:
        std::vector<std::string> stops;
        int distance;
        int cost;
        int num_stops;
        Path();

    public:
        const std::vector<std::string>& get_stops() const;
        const int& get_distance() const;
        const int& get_cost() const;
        const int& get_num_stops() const;

    friend class AirportGraph;
};

#endif
