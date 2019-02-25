#ifndef DATAMESSAGE_
#define DATAMESSAGE_
#include "rapidxml.h"
#include <Rcpp.h>

enum DataMessage {
  COMPACTDATA,
  GENERICDATA,
  UTILITYDATA,
  CROSS_SECTIONALDATA,
  STRUCTUREDATA,
  SCHEMADATA,
  NOMSG
};

template <DataMessage d>
std::map<std::string, Rcpp::CharacterVector>
readsdmx(rapidxml::xml_node<> *root);

class data_msg {
public:
  data_msg(rapidxml::xml_node<> *root) { message_ = data_message_(root); }
  DataMessage message() { return message_; }
  std::map<std::string, DataMessage> msg_map() {
    std::map<std::string, DataMessage> m;

    m["CompactData"] = COMPACTDATA;
    m["compact"] = COMPACTDATA;
    m["StructureSpecificData"] = COMPACTDATA;

    m["UtilityData"] = UTILITYDATA;

    m["generic"] = GENERICDATA;
    m["GenericData"] = GENERICDATA;
    m["GenericTimeSeriesData"] = GENERICDATA;

    m["CrossSectionalData"] = CROSS_SECTIONALDATA;

    // to do: check if case sensitivity matters? e.g. is STRUCTURE ok?
    m["structure"] = STRUCTUREDATA;
    m["Structure"] = STRUCTUREDATA;

    m["schema"] = SCHEMADATA;
    return m;
  }

private:
  DataMessage message_;
  DataMessage msg_type(std::string s) {
    std::map<std::string, DataMessage> d = msg_map();
    std::map<std::string, DataMessage>::iterator it = d.find(s);
    return it == d.end() ? NOMSG : it->second;
  }
  DataMessage find_msg(rapidxml::xml_node<> *node, char sep) {
    if (node == NULL)
      return NOMSG;
    if (node->first_attribute("xmlns") == 0)
      return NOMSG;
    rapidxml::xml_attribute<> *xml_ns = node->first_attribute("xmlns");
    std::string ns_msg = xml_ns->value();
    std::string msg = msg_suffix(ns_msg, sep);
    return msg_type(msg);
  }
  DataMessage data_message_(rapidxml::xml_node<> *node) {
    // first check node name for msg <CompactData ...
    std::string msg = node->name();
    DataMessage data_msg = msg_type(msg);
    if (data_msg != NOMSG)
      return data_msg;

    // if not there try its xmlns attribute  xmlns = .../generic
    DataMessage group_msg = find_msg(node, char('/'));
    if (group_msg != NOMSG)
      return group_msg;

    // if not there then lastly try the dataset node xmlns = ...:generic
    rapidxml::xml_node<> *dataset = node->first_node("DataSet");
    return find_msg(dataset, char(':'));
  }
  std::string msg_suffix(std::string s, char x) {
    return s.substr(s.find_last_of(x) + 1);
  }
};

class msg_base {
public:
  std::map<std::string, Rcpp::CharacterVector>
  as_list(std::vector<std::map<std::string, std::string> > data,
          std::size_t len) {
    std::map<std::string, Rcpp::CharacterVector> out;
    for (std::size_t i = 0; i < data.size(); i++) {
      std::map<std::string, std::string> *d = &data[i];
      for (std::map<std::string, std::string>::iterator it_ = d->begin();
           it_ != d->end(); ++it_) {
        std::string nm = it_->first;
        if (out.find(nm) == out.end()) {
          out[nm] = Rcpp::CharacterVector(len, NA_STRING);
        }
        out[nm][i] = Rcpp::String(it_->second);
      }
    }
    return out;
  }
  void stopifnot_dataset(rapidxml::xml_node<> *ds) {
    if (ds == NULL)
      Rcpp::stop("dataset node not detected");
  }
};

#endif
