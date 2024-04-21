#include "Graph.h"

template<>
inline unsigned long hash(const StopInfo& stop_info){
    return hash<>(stop_info.get_name()) + hash<>(stop_info.get_num_stops());
}

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
    to_search.push(StopInfo(from, "", 0, 0, -1));

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
                came_from[n.neighbor] = StopInfo(searching.name, "", new_dist, new_cost, new_num_stops);
            }

            to_search.push(StopInfo(n.neighbor, searching.name, new_dist, new_cost, new_num_stops));
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

Path AirportGraph::dijkstra_n_stops(const std::string& from, const std::string& to, int required_stops){
    Path result;
    HashDSU<std::string> visited_dsu;
    HashMap<StopInfo, bool> visited;
    MinHeap<StopInfo> to_search;
    HashMap<StopInfo, StopInfo> came_from;
    bool found_path = false;

    if(!adj_map.has_key(from)){
        return result;
    }
    
    for(AirportNeighbor initial_neighbor : adj_map[from]){
        int new_dist = initial_neighbor.distance;
        int new_cost = initial_neighbor.cost;
        int new_num_stops = 0;
        // cost and distance aren't used in hash<StopInfo> or StopInfo::operator== :), oh boy
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

        std::vector<AirportNeighbor> neighbors = adj_map[searching.name];
        for(AirportNeighbor n : neighbors){
            //if adding this connection would create a loop, don't even bother
            if(visited_dsu.find_set(n.get_neighbor()) == visited_dsu.find_set(searching.from)){
                continue;
            }
            int new_dist = searching.distance + n.distance;
            int new_cost = searching.cost + n.cost;
            int new_num_stops = searching.num_stops+1;

            if(came_from[StopInfo(n.neighbor, searching.name, 0, 0, new_num_stops)].distance > new_dist){
                //came_from[n.neighbor] = StopInfo(searching.name, "", new_dist, new_cost, new_num_stops);
                came_from[StopInfo(n.neighbor, searching.name, 0, 0, new_num_stops)] = StopInfo(searching.name, "", new_dist, new_cost, new_num_stops);
            }

            //std::cout << "Came from: " << n.neighbor << ' ' << searching.name << ' ' << new_num_stops << std::endl;
            to_search.push(StopInfo(n.neighbor, searching.name, new_dist, new_cost, new_num_stops));
        }
        visited[searching] = true;
        //???
        visited_dsu.union_set(searching.name, searching.from);
    }
    std::cout << found_path << std::endl;

    if(!found_path){
        return result;
    }
    //std::cout << "making path" << std::endl;
    std::string current = to;
    int stops = required_stops;
    while(true){
        //std::cout << current << ' ' << stops << std::endl;
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

StopInfo::StopInfo() : StopInfo("", "", INT_MAX, INT_MAX, INT_MAX){};
StopInfo::StopInfo(const std::string& the_name, const std::string& came_from, int the_distance, int the_cost, int the_num_stops){
    name = the_name;
    from = came_from;
    distance = the_distance;
    cost = the_cost;
    num_stops = the_num_stops;
};

bool StopInfo::operator==(const StopInfo& other) const{
    return this->name == other.name && this->num_stops == other.num_stops;
}
bool StopInfo::operator>(const StopInfo& other) const{
    return this->distance > other.distance;
}
bool StopInfo::operator<(const StopInfo& other) const{
    return this->distance < other.distance;
}

const std::string& StopInfo::get_name() const{
    return name;
}
const std::string& StopInfo::get_from() const{
    return from;
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
