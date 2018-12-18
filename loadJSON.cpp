//
// Created by slist on 12/15/18.
//

#include "loadJSON.h"
#include "Config.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


boost::property_tree::ptree load_json_file(std::string file_path)
{
    boost::property_tree::ptree root;
    boost::property_tree::read_json(PROJECT_DIRECTORY + file_path, root);
    return root;
}
