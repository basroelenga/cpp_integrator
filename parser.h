#ifndef PARSER_H
#define PARSER_H

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "parameters.h"
#include "core.h"

std::tuple<std::shared_ptr<std::vector<Sol_object>>, std::shared_ptr<std::vector<Sol_object>>> parse_file(std::shared_ptr<Parameters> param);
std::shared_ptr<std::vector<Sol_object>> get_object_data(std::shared_ptr<std::vector<Sol_object>> object_data_all, std::shared_ptr<std::vector<Sol_object>> target_data, std::shared_ptr<Parameters> param);

#endif
