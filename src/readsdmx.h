#ifndef READSDMX_
#define READSDMX_

#include <Rcpp.h>
#include "rapidxml.hpp"
#include <rapidxml_utils.hpp>
#include <compactdata.h>
using namespace Rcpp;

//to do: add other datamessages
enum DataMessage {
  COMPACTDATA,
  GENERICDATA
};

class ReadSDMX {

  rapidxml::file<> sdmx_file_;
  rapidxml::xml_document<> sdmxdoc_;
  rapidxml::xml_node<> *root_;
  DataMessage data_type;

public:

  ReadSDMX(const char* path):sdmx_file_(path)
  {

    sdmxdoc_.parse<rapidxml::parse_strip_xml_namespaces>(sdmx_file_.data());
    root_ = sdmxdoc_.first_node();
    data_type = get_data_message();
  }

  List read_sdmx(){

    if (data_type == COMPACTDATA)
      return read_compactdata();
    return List(1); /*add in generic format later */
  }

private:

  DataMessage get_data_message(){
    //data type information can also be stored in dataset node...
    if (strcmp(root_ -> name(), "CompactData") == 0)
      return COMPACTDATA;
    else return GENERICDATA;
  }

  List read_compactdata(){
    return read_compactdata_(root_);
  }

};

#endif
