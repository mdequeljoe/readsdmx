#ifndef STRUCTUREDATA_
#define STRUCTUREDATA_
#include <Rcpp.h>
#include "rapidxml.h"
#include "datamessage.h"

//<CodeList id="CL_QNA_LOCATION" agencyID="OECD">
// <Name xml:lang="en">Country</Name>
// <Name xml:lang="fr">Pays</Name>
//
// id en fr
// cl_qna, country, pays// <Code value="AUS">
// <Description xml:lang="en">Australia</Description>
// <Description xml:lang="fr">Australie</Description>
// </Code>

// <message:CodeLists>
//   <CodeList id="CL_THE_CODELIST" agencyID="ORG">
//   <Name xml:lang="en">the codelist</Name>
//   <Name xml:lang="fr">la liste de codes</Name>
//   <Code value="A">
//   <Description xml:lang="en">A letter</Description>
//   <Description xml:lang="fr">lettre A</Description>
//   </Code>

// id               country   pays
// CL_QNA_LOCATION Australia Australie


std::map<std::string, std::string> cl_key_(rapidxml::xml_node<> *cl_node, const char* attr_name, const char* node_name){

  std::map<std::string, std::string> cl;
  cl["id"] = cl_node -> first_attribute(attr_name) -> value();
  for (rapidxml::xml_node<> *val = cl_node->first_node(node_name);
       val; val = val->next_sibling())
  {
    if (strcmp(val->name(), node_name) != 0)
      break;

    rapidxml::xml_attribute<> *cl_lang = val->first_attribute("lang");
    Rcpp::Rcout << val->value() << std::endl;
    Rcpp::Rcout << cl_lang->value() << std::endl;
    cl[cl_lang->value()] = val->value();
  }
  return cl;
}

template <>
std::map<std::string, Rcpp::CharacterVector> readsdmx<STRUCTUREDATA>(rapidxml::xml_node<> *root)
{
  rapidxml::xml_node<> *codelists = root->first_node("CodeLists");
  if (codelists == NULL)
    Rcpp::stop("codelists not found");
  std::map<std::string, Rcpp::CharacterVector> out;
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
      cl_data.insert(code_key.begin(), code_key.end());
      data_.push_back(cl_data);
      ++m;
    }
  }
  for (std::size_t i = 0; i < data_.size(); i++)
  {
    std::map<std::string, std::string> d = data_[i];
    for (std::map<std::string, std::string>::iterator it_ = d.begin(); it_ != d.end(); ++it_)
    {
      std::string nm = it_->first;
      if (out.find(nm) == out.end())
      {
        out[nm] = Rcpp::CharacterVector(m, NA_STRING);
      }
      out[nm][i] = it_->second;
    }
  }

  return out;
}

#endif
