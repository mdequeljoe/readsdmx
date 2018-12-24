#ifndef READSDMX_
#define READSDMX_
#include <Rcpp.h>
#include "rapidxml.h"
#include "rapidxml_utils.h"
#include "datamessage.h"
#include "compactdata.h"
#include "genericdata.h"
#include "structuredata.h"

class SDMXReader
{
  rapidxml::xml_node<> *root_;
  rapidxml::xml_document<> sdmxdoc_;
public:
  SDMXReader(Rcpp::List bdata)
  {
    std::string data = read_binlist(bdata);
    sdmxdoc_.parse<rapidxml::parse_strip_xml_namespaces>(&data[0]);
    root_ = sdmxdoc_.first_node();
    data_type = get_data_message();
    data_ = read_sdmx();
  }
  SDMXReader(std::string fname)
  {
    rapidxml::file<> sdmx_file_(fname.c_str());
    sdmxdoc_.parse<rapidxml::parse_strip_xml_namespaces>(sdmx_file_.data());
    root_ = sdmxdoc_.first_node();
    data_type = get_data_message();
    data_ = read_sdmx();
  }
  std::map<std::string, Rcpp::CharacterVector> data(){
    return data_;
  }
private:
  std::map<std::string, Rcpp::CharacterVector> data_;
  DataMessage data_type;
  DataMessage get_data_message()
  {
    return data_message_(root_);
  }
  std::map<std::string, Rcpp::CharacterVector> read_sdmx()
  {
    switch (data_type)
    {
    case GENERICDATA:
      return readsdmx<GENERICDATA>(root_);
    case COMPACTDATA:
      return readsdmx<COMPACTDATA>(root_);
    case STRUCTUREDATA:
      return readsdmx<STRUCTUREDATA>(root_);
    default:
      Rcpp::stop("sdmx data message not detected/supported");
    }
  }
  std::string raw_to_string(Rcpp::RawVector x)
  {
    std::string s(RAW(x), RAW(x) + x.size());
    return s;
  }
  std::string read_binlist(Rcpp::List r)
  {
    std::string m;
    for (int i = 0; i < r.size(); i++)
    {
      m += raw_to_string(Rcpp::as<Rcpp::RawVector>(r[i]));
    }
    m.push_back('\0');
    return m;
  }
};

#endif
