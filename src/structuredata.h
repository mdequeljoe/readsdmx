#ifndef STRUCTUREDATA_
#define STRUCTUREDATA_
#include <Rcpp.h>
#include "rapidxml.h"
#include "datamessage.h"


std::map<std::string, std::string> cl_key_(rapidxml::xml_node<> *cl_node, const char* attr_name, const char* node_name){

  std::map<std::string, std::string> cl;
  cl[attr_name] = cl_node -> first_attribute(attr_name) -> value();
  for (rapidxml::xml_node<> *val = cl_node->first_node(node_name);
       val; val = val->next_sibling())
  {
    if (strcmp(val->name(), node_name) != 0)
      break;
    rapidxml::xml_attribute<> *cl_lang = val->first_attribute("lang");
    cl[cl_lang->value()] = val->value();
  }
  return cl;
}

template <>
std::map<std::string, Rcpp::CharacterVector> readsdmx<STRUCTUREDATA>(rapidxml::xml_node<> *root)
{
  // rapidxml::xml_node<> *codelists = root->first_node("CodeLists");
  // if (codelists == NULL){
  //   codelists = root->first_node("Structures")->first_node("CodeLists");
  // }
  // if (codelists == NULL)
  //   Rcpp::stop("codelists not found");
  rapidxml::xml_node<> *codelists = (root->first_node("CodeLists") == NULL) ?
    root->first_node("Structures")->first_node("Codelists") : root->first_node("CodeLists");
  if (codelists == NULL)
    Rcpp::stop("codelists not found");
  std::vector<std::map<std::string, std::string> > data_;
  std::map<std::string, std::string> code_list_key, code_key, cl_data;
  int m = 0;

  for (rapidxml::xml_node<> *cl = codelists->first_node();
       cl; cl = cl->next_sibling())
  {
    if (strcmp(cl->name(), "CodeList") != 0)
      break;

    code_list_key = cl_key_(cl, "id", "Name");
    for (rapidxml::xml_node<> *code = cl->first_node("Code");
         code; code = code->next_sibling())
    {
      if (strcmp(code->name(), "Code") != 0)
        break;
      code_key = cl_key_(code, "value", "Description");
      cl_data = code_list_key;

      for (std::map<std::string, std::string>::iterator it_ = code_key.begin();
           it_ != code_key.end(); ++it_)
      {
        std::string nm = (it_->first.compare("value") == 0) ? it_->first : it_->first + "_description";
        cl_data[nm] = it_ -> second;
      }
      data_.push_back(cl_data);
      ++m;
    }
  }
  std::map<std::string, Rcpp::CharacterVector> out = as_list(data_, m);
  return out;
}

#endif
