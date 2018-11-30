#include <rapidxml.hpp>
#include <Rcpp.h>

enum DataMessage
{
  COMPACTDATA,
  GENERICDATA,
  STRUCTURE,
  SCHEMA,
  NOMSG
};

template<DataMessage d> Rcpp::List read_sdmxdata(rapidxml::xml_node<> *root);

DataMessage inline data_message_type(std::string msg)
{
  if (msg == "CompactData" || msg == "compact")
  {
    return COMPACTDATA;
  }
  else if (msg == "GenericData" || msg == "generic")
  {
    return GENERICDATA;
  }
  else if (msg == "Structure" || msg == "structure")
  {
    return STRUCTURE;
  }
  else if (msg == "Schema" || msg == "schema")
  {
    return SCHEMA;
  }
  else
  {
    return NOMSG;
  }
}

DataMessage
guess_data_message(rapidxml::xml_node<> *node)
{
  rapidxml::xml_node<> *child = node->first_node("DataSet");
  if (node->first_attribute("xmlns") == 0)
    return NOMSG;
  rapidxml::xml_attribute<> *xml_ns = child -> first_attribute("xmlns");
  std::string ns_type = xml_ns -> value();
  std::string guess = ns_type.substr(ns_type.find_last_of(":") + 1);
  return data_message_type(guess);
}

DataMessage
data_message_(rapidxml::xml_node<> *node)
{
  std::string msg = node->name();
  DataMessage data_msg = data_message_type(msg);
  if (data_msg != NOMSG)
  {
    return data_msg;
  }
  // try to obtain msg information if first child has a xmlns attribute
  return guess_data_message(node);
}
