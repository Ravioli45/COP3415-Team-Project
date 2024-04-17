#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Hash.h"
#include "Graph.h"

HashMap<std::string, std::vector<std::string>> read_airport_csv(const std::string& filepath);

AirportGraph graph_from_data(const HashMap<std::string, std::vector<std::string>>& data);

int main(){

    // a hashmap representing the data in the csv, access data with `data[column_n][row]`
    HashMap<std::string, std::vector<std::string>> data = read_airport_csv("airports.csv");

    AirportGraph airport_graph = graph_from_data(data);

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