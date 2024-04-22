#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <climits>
#include "Hash.h"
#include "Graph.h"

#define LOOP while(true)

struct Empty{};

HashMap<std::string, std::vector<std::string>> read_airport_csv(const std::string& filepath);

AirportGraph graph_from_data(const HashMap<std::string, std::vector<std::string>>& data);
std::vector<std::string> get_airport_names(const HashMap<std::string, std::vector<std::string>>& data);
HashMap<std::string, std::vector<std::string>> get_airports_in_states(const HashMap<std::string, std::vector<std::string>>& data);

void show_options(const std::vector<std::string>& options, int items_per_row=1);
void print_mst(const std::vector<Edge>& mst, int width=0);

int get_input_in_range(int min, int max, const std::string& prompt);

int main(){

    std::string infile;
    std::cout << "Name of input file: " << std::flush;
    getline(std::cin, infile);

    // a hashmap representing the data in the csv, access data with `data[column_n][row]`
    HashMap<std::string, std::vector<std::string>> data = read_airport_csv(infile);
    
    //string used when I don't care about result of getline
    std::string empty;

    AirportGraph airport_graph = graph_from_data(data);
    AirportGraph airport_graph_u = AirportGraph::make_undirected(airport_graph);

    std::vector<std::string> airport_names = get_airport_names(data);
    utils::sort(airport_names);

    HashMap<std::string, std::vector<std::string>> airports_in_states = get_airports_in_states(data);
    std::vector<std::string> state_names = airports_in_states.keys();
    utils::sort(state_names);
    
    LOOP{
        std::cout << "1. Shortest Path Between Airports" << std::endl;
        std::cout << "2. Shortest Paths to All Airports in a State" << std::endl;
        std::cout << "3. Shortest Path with a Certain Number of Stops" << std::endl;
        std::cout << "4. Display Number of Connections to Each Airport" << std::endl;
        std::cout << "5. MST with Prim's Algorithm" << std::endl;
        std::cout << "6. MST with Kruskal's Algorithm" << std::endl;
        std::cout << "7. Exit" << std::endl;
        int choice = get_input_in_range(1, 7, "Select an option: ");

        std::cout << std::endl;

        if(choice == 1){
            show_options(airport_names, 10);
            int start_airport_index = get_input_in_range(1, airport_names.size(), "Select starting airport number: ");
            int dest_airport_index = get_input_in_range(1, airport_names.size(), "Select destination airport number: ");

            std::string start_airport = airport_names[start_airport_index-1];
            std::string end_airport = airport_names[dest_airport_index-1];
            Path shortest = airport_graph.dijkstra(start_airport, end_airport);
            std::vector<std::string> path_vec = shortest.get_stops();

            std::cout << std::endl;
            std::cout << "Shortest route from " << start_airport << " to " << end_airport << ": " << std::flush;
            if(path_vec.size() == 0){
                std::cout << "None" << std::endl;
            }
            else{
                for(int i = path_vec.size()-1; i >= 1; i--){
                    std::cout << path_vec[i] << " -> ";
                }
                std::cout << path_vec[0] << ". " << std::flush;
                std::cout << "The length is " << shortest.get_distance() << ". ";
                std::cout << "The cost is " << shortest.get_cost() << ". " << std::endl;
            }
        }
        else if(choice == 2){
            show_options(airport_names, 10);
            int start_airport_index = get_input_in_range(1, airport_names.size(), "Select starting airport number: ");

            show_options(state_names, 10);
            int dest_state_index = get_input_in_range(1, state_names.size(), "Select destination state number: ");

            std::string start_airport = airport_names[start_airport_index-1];
            std::string dest_state = state_names[dest_state_index-1];

            std::cout << std::endl;
            std::cout << "Shortest paths from " << start_airport << " to " << dest_state << " state airports are: " << std::endl; 
            std::cout << std::left << std::setw(40) << "Path" << std::setw(40) << "Length" << std::setw(40) << "Cost" << std::endl;

            for(std::string airport : airports_in_states[dest_state]){

                Path shortest = airport_graph.dijkstra(start_airport, airport);
                std::vector<std::string> path_vec = shortest.get_stops();
                if(path_vec.size() == 0){
                    //std::cout << std::setw(50) << "N/A" << std::setw(50) << "N/A" << std::setw(50) << "N/A" << std::endl;
                    continue;
                }

                std::string path_str;
                for(int i = path_vec.size() - 1; i >= 1; i--){
                    path_str += path_vec[i] + " -> ";
                }
                path_str += path_vec[0];
                std::cout << std::setw(40) << path_str << std::setw(40) << shortest.get_distance() << std::setw(40) << shortest.get_cost();
                std::cout << std::endl;
            }
            std::cout << std::right;
        }
        else if(choice == 3){
            show_options(airport_names, 10);
            int start_airport_index = get_input_in_range(1, airport_names.size(), "Select starting airport number: ");
            int dest_airport_index = get_input_in_range(1, airport_names.size(), "Select destination airport number: ");
            int num_stops = get_input_in_range(0, INT_MAX, "Select number of stops: ");

            std::string start_airport = airport_names[start_airport_index-1];
            std::string dest_airport = airport_names[dest_airport_index-1];

            Path shortest_n_stops = airport_graph.dijkstra_n_stops(start_airport, dest_airport, num_stops);
            std::vector<std::string> path_vec = shortest_n_stops.get_stops();

            std::cout << std::endl;
            std::cout << "Shortest route from " << start_airport << " to " << dest_airport;
            std::cout << " with " << num_stops << " stops: " << std::flush;
            if(path_vec.size() == 0){
                std::cout << "None" << std::endl;
            }
            else{
                for(int i = path_vec.size()-1; i >= 1; i--){
                    std::cout << path_vec[i] << " -> ";
                }
                std::cout << path_vec[0] << ". " << std::flush;
                std::cout << "The length is " << shortest_n_stops.get_distance() << ". ";
                std::cout << "The cost is " << shortest_n_stops.get_cost() << ". " << std::endl;
            }
        }
        else if(choice == 4){
            std::cout << std::left << std::endl;
            std::cout << std::setw(10) << "Airport" << std::setw(10) << "Connections" << std::endl;
            for(std::string airport : airport_names){
                std::cout << std::setw(10) << airport << std::setw(10) << airport_graph.get_num_connections(airport) << std::endl;
            }
        }
        else if(choice == 5){
            std::vector<Edge> prim_mst = airport_graph_u.prim_mst();

            //std::cout << std::endl;
            if(prim_mst.size() == 0){
                std::cout << "No minimum spanning tree could be formed with Prim's algorithm" << std::endl;
            }
            else{
                std::cout << "Minimum Spanning Tree:" << std::endl;
                std::cout << std::left;
                std::cout << std::setw(15) << "Edge" << std::setw(15) << "Weight" << std::endl;
                print_mst(prim_mst, 15);
                std::cout << std::right;
            }
        }
        else if(choice == 6){
            std::vector<Edge> kruskal_mst = airport_graph_u.kruskal_mst();
            std::cout << "Minimum Spanning Tree:" << std::endl;
            std::cout << std::left;
            std::cout << std::setw(15) << "Edge" << std::setw(15) << "Weight" << std::endl;
            print_mst(kruskal_mst, 15);
            std::cout << std::right;
        }
        else if(choice == 7){
            break;
        }
        
        std::cout << std::endl;
        std::cout << "Hit enter to continue..." << std::endl;
        getline(std::cin, empty);
    }

    /*
    for(AirportNeighbor neighbor : airport_graph["IAD"]){
        std::cout << neighbor.get_neighbor() << ' ' << neighbor.get_distance() << ' ' << neighbor.get_cost() << std::endl;
    }
    
    std::cout << airport_graph.get_num_connections("YIP") << std::endl;
    //Path p = airport_graph.dijkstra("IAD", "MIA");
    Path p = airport_graph.dijkstra_n_stops("IAD", "MIA", 4);
    //Path p = airport_graph.dijkstra("ATL", "ORD");
    std::cout << "Printing path" << std::endl;
    for(auto iter = p.get_stops().rbegin(); iter != p.get_stops().rend(); ++iter){
        std::cout << *iter << ' ' << std::flush;
    }
    std::cout << std::endl;
    std::cout << p.get_distance() << ' ' << p.get_cost() << ' ' << p.get_num_stops() << std::endl;
    
    std::vector<Edge> mst = airport_graph_u.kruskal_mst();
    int total_cost = 0;
    for(auto e : mst){
        total_cost += e.get_weight();
    }
    std::cout << mst.size() << ' ' << total_cost << std::endl;
    std::vector<Edge> mst2 = airport_graph_u.prim_mst();
    total_cost = 0;
    for(auto e : mst2){
        total_cost += e.get_weight();
    }
    std::cout << mst2.size() << ' ' << total_cost << std::endl;
    //for(auto iter = mst.begin(); iter != mst.end(); ++iter){
    //    std::cout << iter->get_from() << " - " << iter->get_to() << ':' << iter->get_weight() << std::endl;
    //}
    */

    return 0;
}




// reads the data from airports.csv at the given filepath
// assumes the formatting used for airports.csv and returns the data as a hashmap where
// hashmap[column_name][row_num] to get individual data entries
// note thate for any column_name used the number of rows will be the same
HashMap<std::string, std::vector<std::string>> read_airport_csv(const std::string& filepath){
    std::ifstream infile(filepath);
    std::string line;
    
    std::vector<std::string> col_names = {"Origin", "Dest", "Origin_City", "Dest_City", "Dist", "Cost"};
    HashMap<std::string, std::vector<std::string>> result;

    getline(infile, line);
    
    while(true){
        if(infile.eof())
            break;
        getline(infile, line);
        int start = 0, len = 0, col = 0;
        bool in_quotes = false;

        for(char c : line){
            if(c == '"')
                in_quotes = in_quotes ? false : true;
            else if(in_quotes);
            else if(c == ','){
                result[col_names[col]].push_back(line.substr(start, len));
                start += len+1;
                len = -1;
                col++;
            }
            len++;
        }

        // redimentary way to hand CRLF and LF line endings
        // I spent hours debugging just to find out that line endings were
        // causing all my problems...
        if(line[line.size()-1] == '\r')
            len--;

        result[col_names[col]].push_back(line.substr(start, len));
    }

    return result;
}

AirportGraph graph_from_data(const HashMap<std::string, std::vector<std::string>>& data){
    AirportGraph result;

    int rows = data.get("Origin").size();

    for(int i = 0; i < rows; i++){
        std::string from = data.get("Origin")[i];
        std::string to = data.get("Dest")[i];

        int dist = std::stoi(data.get("Dist")[i]);
        int cost = std::stoi(data.get("Cost")[i]);

        result.add_edge(from, to, dist, cost);
    }

    return result;
}

std::vector<std::string> get_airport_names(const HashMap<std::string, std::vector<std::string>>& data){
    int rows = data.get("Origin").size();

    HashMap<std::string, Empty> airport_set;
    for(int i = 0; i < rows; i++){
        std::string from = data.get("Origin")[i];
        std::string to = data.get("Dest")[i];

        airport_set[from];
        airport_set[to];
    }

    return airport_set.keys();
}

HashMap<std::string, std::vector<std::string>> get_airports_in_states(const HashMap<std::string, std::vector<std::string>>& data){
    HashMap<std::string, std::vector<std::string>> result;
    HashMap<std::string, int> state_indices;
    int counter = 0;
    std::vector<HashMap<std::string, Empty>> state_airports;

    //std::cout << "GAIS" << std::endl;

    state_airports.push_back(HashMap<std::string, Empty>());
    //state_sets.put("FL", HashMap<std::string, Empty>());

    int rows = data.get("Origin").size();

    for(int i = 0; i < rows; i++){
        std::string from = data.get("Origin")[i];
        std::string to = data.get("Dest")[i];

        std::string from_city = data.get("Origin_City")[i];
        std::string to_city = data.get("Dest_City")[i];

        std::string from_state = from_city.substr(from_city.size()-3, 2);
        std::string to_state = to_city.substr(to_city.size()-3, 2);
        //std::cout << from_state << std::endl;
        //state_sets[from_state];
        //state_sets[to_state];
        if(!state_indices.has_key(from_state)){
            state_indices[from_state] = counter;
            counter++;
            state_airports.push_back(HashMap<std::string, Empty>());
        }
        state_airports[state_indices[from_state]][from];

        if(!state_indices.has_key(to_state)){
            state_indices[to_state] = counter;
            counter++;
            state_airports.push_back(HashMap<std::string, Empty>());
        }
        state_airports[state_indices[to_state]][to];
    }

    for(std::string key : state_indices.keys()){
        //std::cout << key << std::endl;
        for(std::string airport : state_airports[state_indices[key]].keys()){
            result[key].push_back(airport);
        }
    }

    return result;
}

void show_options(const std::vector<std::string>& options, int items_per_row){

    for(long unsigned i = 0; i < options.size(); i++){

        std::cout << (i+1) << '.' << options[i] << "   " << std::flush;

        if((i+1)%items_per_row == 0){
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void print_mst(const std::vector<Edge>& mst, int width){
    //std::cout << std::left;
    for(Edge e : mst){
        std::string edge_str;

        edge_str += e.get_from() + " - " + e.get_to();
        std::cout << std::setw(width) << edge_str << std::setw(width) << e.get_weight() << std::endl;
    }
}

int get_input_in_range(int min, int max, const std::string& prompt){
    while(true){
        std::cout << prompt << std::flush;
        
        std::string input;
        int num_input;
        getline(std::cin, input);

        try{
            num_input = stoi(input);
        }
        catch(const std::exception& _e){
            continue;
        }
        if(min <= num_input && num_input <= max){
            return num_input;
        }
    }
}
