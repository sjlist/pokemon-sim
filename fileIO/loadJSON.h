//
// Created by slist on 12/15/18.
//
#ifndef POKEMON_SIM_LOADJSON_H
#define POKEMON_SIM_LOADJSON_H

#include <boost/property_tree/ptree.hpp>

boost::property_tree::ptree load_json_file(std::string file_path);

#endif /* POKEMON_SIM_LOADJSON_H */
