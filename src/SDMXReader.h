#ifndef READSDMX_
#define READSDMX_

#include <Rcpp.h>
#include "rapidxml.hpp"
#include <rapidxml_utils.hpp>
#include <datamessage.h>
#include <compactdata.h>
#include <genericdata.h>

class SDMXReader
{
  rapidxml::xml_document<> sdmxdoc_;
  rapidxml::xml_node<> *root_;
  DataMessage data_type;

public:
  SDMXReader(Rcpp::List bdata)
  {
    std::string data = read_binlist(bdata);
    sdmxdoc_.parse<rapidxml::parse_strip_xml_namespaces>(&data[0]);
    root_ = sdmxdoc_.first_node();
    data_type = get_data_message();
  }
  SDMXReader(std::string fname)
  {
    rapidxml::file<> sdmx_file_(fname.c_str());
    sdmxdoc_.parse<rapidxml::parse_strip_xml_namespaces>(sdmx_file_.data());
    root_ = sdmxdoc_.first_node();
    data_type = get_data_message();
  }
  std::map<std::string, CharacterVector> read_sdmx()
  {
    switch (data_type)
    {
    case GENERICDATA:
      return readsdmx<GENERICDATA>(root_);
    case COMPACTDATA:
      return readsdmx<COMPACTDATA>(root_);
    default:
      Rcpp::stop("sdmx data message not detected/supported");
    }
  }
private:
  DataMessage get_data_message()
  {
    return data_message_(root_);
  }
  std::string raw_to_string(RawVector x)
  {
    std::string s(RAW(x), RAW(x) + x.size());
    return s;
  }
  std::string read_binlist(List r)
  {
    std::string m;
    for (int i = 0; i < r.size(); i++)
    {
      m += raw_to_string(as<RawVector>(r[i]));
    }
    m.push_back('\0');
    return m;
  }
};

#endif
