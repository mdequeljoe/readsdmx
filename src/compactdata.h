#ifndef COMPACTDATA_
#define COMPACTDATA_

#include <Rcpp.h>
#include "rapidxml.hpp"
#include "datamessage.h"
using namespace Rcpp;

std::map<std::string, std::string> get_node_value_(rapidxml::xml_node<> *node)
{
  std::map<std::string, std::string> out;
  if (node->first_attribute() == 0)
    return out;

  for (rapidxml::xml_attribute<> *series_attr = node->first_attribute();
       series_attr; series_attr = series_attr->next_attribute())
  {
    out[series_attr->name()] = series_attr->value();
  }
  return out;
}

template<>
std::map<std::string, CharacterVector> readsdmx<COMPACTDATA>(rapidxml::xml_node<> *root)
{
  int m = 0;
  std::vector<std::map<std::string, std::string> > data_;
  rapidxml::xml_node<> *dataset = root->first_node("DataSet");
  std::map<std::string, std::string> obs_key, obs_val, series_key;
  std::map<std::string, CharacterVector> out;

  //series
  for (rapidxml::xml_node<> *series = dataset->first_node("Series");
       series; series = series->next_sibling())
  {

    if (strcmp(series->name(), "Series") != 0)
      break;

    series_key = get_node_value_(series);

    // series with no obs still returned to dataframe as NA
    if (series->first_node("Obs") == NULL)
    {
      data_.push_back(series_key);
      ++m;
      continue;
    }

    //series observations
    for (rapidxml::xml_node<> *obs = series->first_node("Obs");
         obs; obs = obs->next_sibling())
    {
      obs_key = series_key;
      //series observation attributes
      obs_val = get_node_value_(obs);
      obs_key.insert(obs_val.begin(), obs_val.end());
      data_.push_back(obs_key);
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
        out[nm] = CharacterVector(m, NA_STRING);
      }
      out[nm][i] = it_->second;
    }
  }

  return out;
}

#endif
