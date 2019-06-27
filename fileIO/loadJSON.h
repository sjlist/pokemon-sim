//
// Created by slist on 12/15/18.
//
#ifndef POKEMON_SIM_LOADJSON_H
#define POKEMON_SIM_LOADJSON_H

#include <boost/property_tree/ptree.hpp>
using namespace std;

boost::property_tree::ptree load_json_file(string file_path);

#endif /* POKEMON_SIM_LOADJSON_H */
