#include "Graph.h"


AirportGraph::AirportGraph() : adj_map(){};

std::vector<AirportNeighbor>& AirportGraph::operator[](const std::string& airport_name){
    return adj_map[airport_name];
}

void AirportGraph::add_edge(const std::string& from, const std::string& to, int distance, int cost){
    adj_map[from].push_back(AirportNeighbor(to, distance, cost));
}

Path AirportGraph::dijkstra(const std::string& from, const std::string& to) const{
    HashMap<std::string, bool> visited;
    HashMap<std::string, StopInfo> came_from;
    MinHeap<StopInfo> to_search;
    Path result;
    bool found_path = false;

    // from, distance, cost, num_stops
    to_search.push(StopInfo(from, 0, 0, -1));

    while(!to_search.is_empty()){
        StopInfo searching = to_search.pop();
        //std::cout << "Popped: " << searching.name << ' ' << searching.distance << ' ' << searching.cost << ' ' << searching.length << std::endl;
        if(visited[searching.name]){
            continue;
        }
        if(searching.name == to){
            found_path = true;
            break;
        }
        //std::cout << "Searching" << std::endl;
        //std::cout << searching.name << searching.distance << std::endl;
        if(!adj_map.has_key(searching.name)){
            continue;
        }
        std::vector<AirportNeighbor> neighbors = adj_map.get(searching.name);
        for(AirportNeighbor n : neighbors){
            int new_dist = searching.distance + n.distance;
            int new_cost = searching.cost + n.cost;
            int new_num_stops = searching.num_stops+1;

            if(came_from[n.neighbor].distance > new_dist){
                came_from[n.neighbor] = StopInfo(searching.name, new_dist, new_cost, new_num_stops);
            }

            to_search.push(StopInfo(n.neighbor, new_dist, new_cost, new_num_stops));
            //std::cout << "Added: " << n.get_neighbor() << ' ' << new_dist << ' ' << new_cost << ' ' << new_length << std::endl;
        }
        visited[searching.name] = true;
    }

    if(!found_path){
        return result;
    }

    //std::cout << "making path" << std::endl;
    std::string current = to;
    while(true){
        //std::cout << current << std::endl;
        result.stops.push_back(current);
        if(current == from){
            break;
        }
        current = came_from[current].name;
    }
    result.distance = came_from[to].distance;
    result.cost = came_from[to].cost;
    result.num_stops = came_from[to].num_stops;

    return result;
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

StopInfo::StopInfo() : StopInfo("", INT_MAX, INT_MAX, INT_MAX){};
StopInfo::StopInfo(const std::string& the_name, int the_distance, int the_cost, int the_num_stops){
    name = the_name;
    distance = the_distance;
    cost = the_cost;
    num_stops = the_num_stops;
};

bool StopInfo::operator>(const StopInfo& other) const{
    return this->distance > other.distance;
}
bool StopInfo::operator<(const StopInfo& other) const{
    return this->distance < other.distance;
}

const std::string& StopInfo::get_name() const{
    return name;
}
const int& StopInfo::get_distance() const{
    return distance;
};
const int& StopInfo::get_cost() const{
    return cost;
};
const int& StopInfo::get_num_stops() const{
    return num_stops;
};

Path::Path() : stops(), distance(), cost(), num_stops(){};

const std::vector<std::string>& Path::get_stops() const{
    return stops;
}
const int& Path::get_distance() const{
    return distance;
}
const int& Path::get_cost() const{
    return cost;
}
const int& Path::get_num_stops() const{
    return num_stops;
}
