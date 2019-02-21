#ifndef STRUCTUREDATA_
#define STRUCTUREDATA_
#include <Rcpp.h>
#include "rapidxml.h"
#include "datamessage.h"

class structure_msg : public msg_base {
public:
  std::map<std::string, Rcpp::CharacterVector> read_msg(rapidxml::xml_node<> *root){
    bool version_21 = false;
    rapidxml::xml_node<> *codelists = root->first_node("CodeLists");
    if (codelists == NULL || codelists == 0)
    {
      // have to first check that structures node exists before codelists
      codelists = root->first_node("Structures");
      codelist_exists(codelists);
      codelists = codelists -> first_node("Codelists");
      //check again
      codelist_exists(codelists);
      version_21 = true;
    }

    std::vector<std::map<std::string, std::string> > data_;
    std::map<std::string, std::string> code_list_key, code_key, cl_data;
    std::size_t m = 0;

    for (rapidxml::xml_node<> *cl = codelists->first_node();
         cl; cl = cl->next_sibling())
    {
      if (strcmp(cl->name(), "CodeList") != 0 &&
          strcmp(cl->name(), "Codelist") != 0)
        break;

      code_list_key = cl_key_(cl, "id", "Name");
      for (rapidxml::xml_node<> *code = cl->first_node("Code");
           code; code = code->next_sibling())
      {
        if (strcmp(code->name(), "Code") != 0)
          break;
        code_key = version_21 ? cl_key_(code, "id", "Name") : cl_key_(code, "value", "Description");
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
private:
  void codelist_exists(rapidxml::xml_node<> *codelists){
    if (codelists == NULL || codelists == 0)
      Rcpp::stop("codelists not found");
  }
};

template <>
std::map<std::string, Rcpp::CharacterVector> readsdmx<STRUCTUREDATA>(rapidxml::xml_node<> *root)
{
  structure_msg d;
  return d.read_msg(root);
}

#endif
