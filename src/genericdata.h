#ifndef GENERICDATA_
#define GENERICDATA_

#include <Rcpp.h>
#include "rapidxml.h"
#include "rapidxml_utils.h"
#include "datamessage.h"

std::map<std::string, std::string> series_key_(rapidxml::xml_node<> *node)
{
  std::map<std::string, std::string> series;

  for (rapidxml::xml_node<> *val = node->first_node();
       val; val = val->next_sibling())
  {
    if (strcmp(val->name(), "Obs") == 0)
      break;

    for (rapidxml::xml_node<> *val_key = val->first_node(); val_key;
    val_key = val_key->next_sibling())
    {
      rapidxml::xml_attribute<> *key = val_key->first_attribute("id");
      if (key == NULL)
      {
        key = val_key->first_attribute("concept");
      }
      series[key->value()] = key->next_attribute()->value();
    }
  }

  return series;
}

std::map<std::string, std::string> series_obs_key_(rapidxml::xml_node<> *node)
{
  std::map<std::string, std::string> series_obs;

  for (rapidxml::xml_node<> *val = node->first_node(); val; val = val->next_sibling())
  {
    if (val == NULL)
      continue;

    if (strcmp(val->name(), "Time") == 0)
    {
      series_obs[val->name()] = val->value();
    }
    else if (strcmp(val->name(), "Attributes") == 0 || strcmp(val->name(), "ObsKey") == 0)
    {
      //attributes and Obskey nodes are children
      for (rapidxml::xml_node<> *attr = val->first_node(); attr; attr = attr->next_sibling())
      {
        //version 2.0:"concept", version 2.1:"id"
        rapidxml::xml_attribute<> *concept_attr = attr->first_attribute("id");
        concept_attr = (concept_attr == NULL) ? attr->first_attribute("concept") : concept_attr;
        series_obs[concept_attr->value()] = concept_attr->next_attribute()->value();
      }
    }
    else // Obsvalue; Obsdimension (version 2.1)
    {
      series_obs[val->name()] = val->first_attribute("value")->value();
    }
  }

  return series_obs;
}

template<>
std::map<std::string, Rcpp::CharacterVector> readsdmx<GENERICDATA>(rapidxml::xml_node<> *root)
{
  rapidxml::xml_node<> *dataset = root->first_node("DataSet");
  std::vector<std::map<std::string, std::string> > data_;
  std::map<std::string, std::string> skey, okey, sdata;
  int m = 0;
  for (rapidxml::xml_node<> *series = dataset->first_node("Series");
       series; series = series->next_sibling())
  {

    if (strcmp(series->name(), "Series") != 0)
      break;

    skey = series_key_(series);
    // series with no obs still returned to dataframe as NA
    if (series->first_node("Obs") == NULL)
    {
      data_.push_back(skey);
      ++m;
      continue;
    }
    // collect observations
    for (rapidxml::xml_node<> *obs = series->first_node("Obs");
         obs; obs = obs->next_sibling())
    {
      sdata = skey;
      okey = series_obs_key_(obs);
      sdata.insert(okey.begin(), okey.end());
      data_.push_back(sdata);
      ++m;
    }
  }

  std::map<std::string, Rcpp::CharacterVector> out;
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
