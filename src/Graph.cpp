#include "Graph.h"


// defines hash function for StopInfo, only uses name and number of stops
// 
// StopInfo::operator== also only uses name and num_stops
template<>
inline unsigned long hash(const StopInfo& stop_info){
    return hash<>(stop_info.get_name()) + hash<>(stop_info.get_num_stops());
}

// defines hash function for Edge
//
// note that of from and to are switched the hash remains the same
template<>
inline unsigned long hash(const Edge& edge){
    return hash<>(edge.get_from()) + hash<>(edge.get_to());
}

// default constructor for airport graph
AirportGraph::AirportGraph() : adj_map(){};

// using [] with airport graph uses [] with underlying hashmap
std::vector<AirportNeighbor>& AirportGraph::operator[](const std::string& airport_name){
    return adj_map[airport_name];
}

// add the edge described by from, to, distance, and cost
void AirportGraph::add_edge(const std::string& from, const std::string& to, int distance, int cost){
    adj_map[from].push_back(AirportNeighbor(to, distance, cost));
    num_connections[from]++;
    num_connections[to]++;
}
// add the edge described by from, to, distance, and cost in both directions
void AirportGraph::add_edge_undirected(const std::string& from, const std::string& to, int distance, int cost){
    this->add_edge(from, to, distance, cost);
    this->add_edge(to, from, distance, cost);
}

// uses dijkstras algorithm to find the shortest distance path between from and to
//
// returns the path object that describes the complete path
Path AirportGraph::dijkstra(const std::string& from, const std::string& to) const{
    HashMap<std::string, bool> visited;
    HashMap<std::string, StopInfo> came_from;
    MinHeap<StopInfo> to_search;
    Path result;
    bool found_path = false;

    // StopInfo(from, distance, cost, num_stops);
    to_search.push(StopInfo(from, "", 0, 0, -1));

    // searches current shortest available path
    while(!to_search.is_empty()){
        StopInfo searching = to_search.pop();
        
        if(visited[searching.name]){
            continue;
        }
        if(searching.name == to){
            found_path = true;
            break;
        }
        
        // this had to be added since some airports have no outbound connections
        if(!adj_map.has_key(searching.name)){
            continue;
        }
        // discovers neighbors
        std::vector<AirportNeighbor> neighbors = adj_map.get(searching.name);
        for(AirportNeighbor n : neighbors){
            int new_dist = searching.distance + n.distance;
            int new_cost = searching.cost + n.cost;
            int new_num_stops = searching.num_stops+1;

            if(came_from[n.neighbor].distance > new_dist){
                came_from[n.neighbor] = StopInfo(searching.name, "", new_dist, new_cost, new_num_stops);
            }

            to_search.push(StopInfo(n.neighbor, searching.name, new_dist, new_cost, new_num_stops));
            
        }
        // mark as searched
        visited[searching.name] = true;
    }

    // if no path was foun return empty Path
    if(!found_path){
        return result;
    }

    // reconstruct path
    std::string current = to;
    while(true){
        result.stops.push_back(current);
        if(current == from){
            break;
        }
        current = came_from[current].name;
    }

    if(!came_from.has_key(to)){
        result.distance = 0;
        result.cost = 0;
        result.num_stops = 0;
    }
    else{
        result.distance = came_from[to].distance;
        result.cost = came_from[to].cost;
        result.num_stops = came_from[to].num_stops;
    }

    return result;
}


// uses a slightly modified version of dijkstras algorithm to find the shortest path
// with a certain number of stops
Path AirportGraph::dijkstra_n_stops(const std::string& from, const std::string& to, int required_stops) const{
    // similar to dijksrta, but vertices can be searched multiple and a dsu is used to prevent searching in loops
    // forever
    Path result;
    HashDSU<std::string> visited_dsu;
    HashMap<StopInfo, bool> visited;
    MinHeap<StopInfo> to_search;
    HashMap<StopInfo, StopInfo> came_from;
    bool found_path = false;

    if(!adj_map.has_key(from)){
        return result;
    }
    
    for(AirportNeighbor initial_neighbor : adj_map.get(from)){
        int new_dist = initial_neighbor.distance;
        int new_cost = initial_neighbor.cost;
        int new_num_stops = 0;
        // cost, distance, and from aren't used in hash<StopInfo> or StopInfo::operator== :), oh boy
        came_from[StopInfo(initial_neighbor.neighbor, from, 0, 0, new_num_stops)] = StopInfo(from, "", new_dist, new_cost, new_num_stops);
        to_search.push(StopInfo(initial_neighbor.neighbor, from, new_dist, new_cost, new_num_stops));
    }
    visited_dsu.make_set(from);

    while(!to_search.is_empty()){
        StopInfo searching = to_search.pop();

        if(visited[searching]){
            continue;
        }
        if(searching.name == to){
            if(searching.num_stops == required_stops){
                found_path = true;
                break;
            }
            else{
                // don't search past destination
                continue;
            }
        }

        if(!adj_map.has_key(searching.name)){
            continue;
        }

        std::vector<AirportNeighbor> neighbors = adj_map.get(searching.name);
        for(AirportNeighbor n : neighbors){
            //if adding this connection would create a loop, don't even bother
            if(visited_dsu.find_set(n.get_neighbor()) == visited_dsu.find_set(searching.from)){
                continue;
            }
            int new_dist = searching.distance + n.distance;
            int new_cost = searching.cost + n.cost;
            int new_num_stops = searching.num_stops+1;

            if(came_from[StopInfo(n.neighbor, searching.name, 0, 0, new_num_stops)].distance > new_dist){
                came_from[StopInfo(n.neighbor, searching.name, 0, 0, new_num_stops)] = StopInfo(searching.name, "", new_dist, new_cost, new_num_stops);
            }

            to_search.push(StopInfo(n.neighbor, searching.name, new_dist, new_cost, new_num_stops));
        }
        visited[searching] = true;
        //???
        visited_dsu.union_set(searching.name, searching.from);
    }

    if(!found_path){
        return result;
    }
    std::string current = to;
    int stops = required_stops;
    while(true){
        result.stops.push_back(current);
        if(current == from){
            break;
        }
        current = came_from.get(StopInfo(current, "", 0, 0, stops)).name;
        stops--;
    }
    
    result.distance = came_from.get(StopInfo(to, "", 0, 0, required_stops)).distance;
    result.cost = came_from.get(StopInfo(to, "", 0, 0, required_stops)).cost;
    result.num_stops = came_from.get(StopInfo(to, "", 0, 0, required_stops)).num_stops;

    return result;
}

unsigned AirportGraph::get_num_connections(const std::string& airport_name) const{
    if(!num_connections.has_key(airport_name)){
        return 0;
    }
    else{
        return num_connections.get(airport_name);
    }
}

AirportGraph AirportGraph::make_undirected(const AirportGraph& graph){
    HashMap<Edge, bool> added;
    AirportGraph result;
    std::vector<std::string> keys = graph.adj_map.keys();

    for(std::string k : keys){

        for(AirportNeighbor n : graph.adj_map.get(k)){
            if(added[Edge(k, n.neighbor, 0)]){
                continue;
            }

            //have to look for connection from other direction 
            if(!graph.adj_map.has_key(n.neighbor))
                goto no_returning;
            for(AirportNeighbor nn : graph.adj_map.get(n.neighbor)){
                if(nn.neighbor == k){
                    //found connection from other direction
                    int min_cost = n.cost < nn.cost ? n.cost : nn.cost;
                    result.add_edge_undirected(k, n.neighbor, -1, min_cost);
                    goto done;
                }
            }
            //if no connection from other direction was found
            no_returning:;
            result.add_edge_undirected(k, n.neighbor, -1, n.cost);
            done:;
            added[Edge(k, n.neighbor, 0)] = true;
        }

    }

    return result;
}

std::vector<Edge> AirportGraph::kruskal_mst() const{
    std::vector<Edge> mst;

    HashDSU<std::string> visited_dsu;
    HashMap<Edge, bool> added;
    MinHeap<Edge> to_search;

    // create minheap of all unique edges
    for(std::string key : adj_map.keys()){
        for(AirportNeighbor n : adj_map.get(key)){
            if(added[Edge(key, n.neighbor, n.cost)]){
                continue;
            }
            to_search.push(Edge(key, n.neighbor, n.cost));
            added[Edge(key, n.neighbor, n.cost)] = true;
        }
    }

    // while minheap isnt empty
    while(!to_search.is_empty()){
        Edge searching = to_search.pop();
        // if popped edge doesn't form a loop add it to mst
        if(visited_dsu.find_set(searching.to) != visited_dsu.find_set(searching.from)){
            visited_dsu.union_set(searching.to, searching.from);
            mst.push_back(searching);
        }
    }

    return mst;
}

std::vector<Edge> AirportGraph::prim_mst() const{
    std::vector<Edge> mst;
    MinHeap<Edge> to_search;
    HashMap<std::string, bool> visited;
    std::vector<std::string> keys = adj_map.keys();
    unsigned num_airports = keys.size();
    std::string start = keys[0];

    for(AirportNeighbor n : adj_map.get(start)){
        to_search.push(Edge(start, n.neighbor, n.cost));
    }
    visited[start] = true;
    while(!to_search.is_empty()){
        Edge searching = to_search.pop();

        // if this connection doesn't add a loop...
        if(visited[searching.to]){
            continue;
        }

        // ...added to mst and marked searching.to as visited
        mst.push_back(searching);
        for(AirportNeighbor n : adj_map.get(searching.to)){
            to_search.push(Edge(searching.to, n.neighbor, n.cost));
        }
        //visited[searching.from] = true;
        visited[searching.to] = true;

    }

    // return empty mst if prims could not connect all nodes in mst
    // this occurs when graph is disconnected
    if(mst.size() != (num_airports - 1)){
        return {};
    }
    

    return mst;
}

// costructor for AirportNeighbor
AirportNeighbor::AirportNeighbor(const std::string& neighbor_name, int the_distance, int the_cost){
    neighbor = neighbor_name;
    distance = the_distance;
    cost = the_cost;
}

// returns neighbor field of AirportNeighbor
const std::string& AirportNeighbor::get_neighbor() const{
    return neighbor;
}
// returns distance field of AirportNeighbor
const int& AirportNeighbor::get_distance() const{
    return distance;
}
// returns cost field of AirportNeighbor
const int& AirportNeighbor::get_cost() const{
    return cost;
}

// default constructor for edge
//
// empty string for from and to with a weight of 0
Edge::Edge() : from(""), to(""), weight(0){};

// construct edge with given from, to, and weight
Edge::Edge(const std::string& came_from, const std::string& go_to, int the_weight){
    from = came_from;
    to = go_to;
    weight = the_weight;
}
// overloading == for Edge
bool Edge::operator==(const Edge& other) const{
    // edges are equal if they have the same endpoints
    return ((this->from == other.from) && (this->to == other.to)) || ((this->from == other.to) && (this->to == other.from));
}
// overloading > for edge, uses weight field
bool Edge::operator>(const Edge& other) const{
    return this->weight > other.weight;
}
// overloading < for edge, uses weight field
bool Edge::operator<(const Edge& other) const{
    return this->weight < other.weight;
}
// getter for from value
const std::string& Edge::get_from() const{
    return from;
}
// getter for to value
const std::string& Edge::get_to() const{
    return to;
}
// getter for edge weight
const int& Edge::get_weight() const{
    return weight;
}

// default constructor for stopinfo, sets distance, cost, and num_stops to INT_MAX
// acts as infinite distance, cost, and stops
StopInfo::StopInfo() : StopInfo("", "", INT_MAX, INT_MAX, INT_MAX){};
// construct StopInfo instance with given name, came_from, distance, cost, and num_stops
StopInfo::StopInfo(const std::string& the_name, const std::string& came_from, int the_distance, int the_cost, int the_num_stops){
    name = the_name;
    from = came_from;
    distance = the_distance;
    cost = the_cost;
    num_stops = the_num_stops;
};
// overload == for StopInfo, uses name and num_stops
bool StopInfo::operator==(const StopInfo& other) const{
    return this->name == other.name && this->num_stops == other.num_stops;
}
// overload > for StopInfo, uses distance
bool StopInfo::operator>(const StopInfo& other) const{
    return this->distance > other.distance;
}
// overload < for StopInfo, uses distance
bool StopInfo::operator<(const StopInfo& other) const{
    return this->distance < other.distance;
}

// return name from StopInfo
const std::string& StopInfo::get_name() const{
    return name;
}
// return `from` from StopInfo
const std::string& StopInfo::get_from() const{
    return from;
}
// return distance from StopInfo
const int& StopInfo::get_distance() const{
    return distance;
};
// returns cost from StopInfo
const int& StopInfo::get_cost() const{
    return cost;
};
// returns num_stops from StopInfo
const int& StopInfo::get_num_stops() const{
    return num_stops;
};

// default constructor for path uses default constructor for all fields
Path::Path() : stops(), distance(), cost(), num_stops(){};

// returns stops vector from Path
const std::vector<std::string>& Path::get_stops() const{
    return stops;
}
// returns total distnce from Path
const int& Path::get_distance() const{
    return distance;
}
// returns total cost from Path
const int& Path::get_cost() const{
    return cost;
}
// returns total num_stops from Path
const int& Path::get_num_stops() const{
    return num_stops;
}
