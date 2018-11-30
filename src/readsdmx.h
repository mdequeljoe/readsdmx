#ifndef READSDMX_
#define READSDMX_

#include <Rcpp.h>
#include "rapidxml.hpp"
#include <rapidxml_utils.hpp>
#include <compactdata.h>

using namespace Rcpp;

class ReadSDMX
{
  rapidxml::xml_document<> sdmxdoc_;
  rapidxml::xml_node<> *root_;
  DataMessage data_type;

public:
  ReadSDMX(std::string filetext)
  {
    sdmxdoc_.parse<rapidxml::parse_strip_xml_namespaces>(&filetext[0]);
    root_ = sdmxdoc_.first_node();
    data_type = get_data_message();
  }
  ReadSDMX(const char *path)
  {
    rapidxml::file<> sdmx_file_(path);
    sdmxdoc_.parse<rapidxml::parse_strip_xml_namespaces>(sdmx_file_.data());
    root_ = sdmxdoc_.first_node();
    data_type = get_data_message();
  }

  List read_sdmx()
  {

    if (data_type == COMPACTDATA)
      return read_sdmxdata<COMPACTDATA>(root_);
    return List(1); /*add in generic format later */
  }

private:
  DataMessage get_data_message()
  {
    return data_message_(root_);
  }
};

#endif
