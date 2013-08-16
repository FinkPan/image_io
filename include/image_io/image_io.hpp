#ifndef _IMAGE_IO_HPP_
#define _IMAGE_IO_HPP_

#include "image_base.hpp"
#include "pbm.hpp"
#include "pgm.hpp"
#include "ppm.hpp"

std::string ImageLoad(const std::string&,std::ifstream&);

#endif