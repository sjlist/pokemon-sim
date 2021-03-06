//
// Created by slist on 12/15/18.
//

#include <Config.h>
#include <fileIO/loadJSON.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace std;

boost::property_tree::ptree load_json_file(string file_path)
{
    boost::property_tree::ptree root;
    boost::property_tree::read_json(PROJECT_DIRECTORY + file_path, root);
    return root;
}
