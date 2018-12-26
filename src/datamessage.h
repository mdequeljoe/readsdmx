#ifndef DATAMESSAGE_
#define DATAMESSAGE_
#include "rapidxml.h"
#include <Rcpp.h>

enum DataMessage
{
  COMPACTDATA,
  GENERICDATA,
  STRUCTUREDATA,
  SCHEMADATA,
  NOMSG
};

template<DataMessage d> std::map<std::string, Rcpp::CharacterVector> readsdmx(rapidxml::xml_node<> *root);

DataMessage inline data_message_type(std::string msg)
{
  if (msg == "CompactData" || msg == "compact" || msg == "StructureSpecificData")
  {
    return COMPACTDATA;
  }
  else if (msg == "GenericData" || msg == "generic")
  {
    return GENERICDATA;
  }
  else if (msg == "Structure" || msg == "structure")
  {
    return STRUCTUREDATA;
  }
  else if (msg == "Schema" || msg == "schema")
  {
    return SCHEMADATA;
  }
  else
  {
    return NOMSG;
  }
}

std::string msg_suffix(std::string s, char x){
  return s.substr(s.find_last_of(x) + 1);
}

DataMessage
  find_data_message(rapidxml::xml_node<> *node, char sep)
  {
    if (node->first_attribute("xmlns") == 0)
      return NOMSG;
    rapidxml::xml_attribute<> *xml_ns = node -> first_attribute("xmlns");
    std::string ns_msg = xml_ns -> value();
    std::string msg = msg_suffix(ns_msg, sep);
    return data_message_type(msg);
  }

DataMessage
  data_message_(rapidxml::xml_node<> *node)
  {
    //first check node name for msg
    std::string msg = node->name();
    DataMessage data_msg = data_message_type(msg);
    if (data_msg != NOMSG)
    {
      return data_msg;
    }
    //if not there try its xmlns attribute  xmlns = .../generic
    DataMessage group_msg = find_data_message(node, char('/'));
    if (group_msg != NOMSG)
    {
      return group_msg;
    }
    //if not there then lastly try the dataset node xmlns = ...:generic
    rapidxml::xml_node<> *dataset = node->first_node("DataSet");
    return find_data_message(dataset, char(':'));
  }

std::map<std::string, Rcpp::CharacterVector> as_list(std::vector<std::map<std::string, std::string> > data, int len)
{
  std::map<std::string, Rcpp::CharacterVector> out;
  for (std::size_t i = 0; i < data.size(); i++)
  {
    std::map<std::string, std::string> d = data[i];
    for (std::map<std::string, std::string>::iterator it_ = d.begin(); it_ != d.end(); ++it_)
    {
      std::string nm = it_->first;
      if (out.find(nm) == out.end())
      {
        out[nm] = Rcpp::CharacterVector(len, NA_STRING);
      }
      out[nm][i] = it_->second;
    }
  }
  return out;
}


#endif
