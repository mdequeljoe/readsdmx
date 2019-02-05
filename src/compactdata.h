#ifndef COMPACTDATA_
#define COMPACTDATA_
#include <Rcpp.h>
#include "rapidxml.h"
#include "datamessage.h"

inline std::size_t cnt_compact_obs(rapidxml::xml_node<> *ds)
{
  std::size_t n = 0;
  for (rapidxml::xml_node<> *series = ds->first_node("Series");
       series; series = series->next_sibling())
  {
    if (strcmp(series->name(), "Series") != 0)
      break;
    // series with no obs still returned to dataframe as NA
    if (series->first_node("Obs") == NULL)
    {
      ++n;
      continue;
    }
    //series observations
    for (rapidxml::xml_node<> *obs = series->first_node("Obs");
         obs; obs = obs->next_sibling())
    {
      ++n;
    }
  }
  return n;
}


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
std::map<std::string, Rcpp::CharacterVector> readsdmx<COMPACTDATA>(rapidxml::xml_node<> *root)
{
  rapidxml::xml_node<> *dataset = root->first_node("DataSet");
  if (dataset == NULL)
    Rcpp::stop("dataset node not detected");
  
  std::size_t n = cnt_compact_obs(dataset);
  std::size_t m = 0;
  std::vector<std::map<std::string, std::string> > data_(n);
  std::map<std::string, std::string> obs_key, obs_val, series_key;

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
      data_[m] = series_key;
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
      data_[m] = obs_key;
      ++m;
    }
  }

  std::map<std::string, Rcpp::CharacterVector> out = as_list(data_, m);
  return out;
}

#endif
