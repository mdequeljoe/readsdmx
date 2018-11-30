#ifndef COMPACTDATA_
#define COMPACTDATA_

#include <Rcpp.h>
#include "rapidxml.hpp"
#include <rapidxml_utils.hpp>
#include "utils.h"
#include "datamessage.h"

using namespace Rcpp;

List compactdata_content_(rapidxml::xml_node<> *root){
  int n, nseries = 0, nobs = 0;
  CharacterVector series_attr, obs_attr, colnames;
  rapidxml::xml_node<> *dataset = root->first_node("DataSet");

  for (rapidxml::xml_node<> *series = dataset->first_node("Series");
       series; series = series->next_sibling())
  {
    if (strcmp(series -> name(), "Series") != 0)
      break;

    ++nseries;
    series_attr = get_node_attr(series, series_attr);

    for (rapidxml::xml_node<> *obs = series->first_node("Obs");
         obs; obs = obs->next_sibling())
    {
      ++nobs;
      obs_attr = get_node_attr(obs, obs_attr);
    }
  }

  //colnames is series_attributes + obs_attributes
  colnames = series_attr;

  for (n = 0; n < obs_attr.size(); n++){
    colnames.push_back(obs_attr[n]);
  }

  return List::create(Named("colnames") = colnames,
                      Named("series") = series_attr,
                      Named("obs") = obs_attr,
                      Named("n_series") = nseries,
                      Named("n_obs") = nobs);
}

template<>
List read_sdmxdata<COMPACTDATA>(rapidxml::xml_node<> *root)
{
  List content = compactdata_content_(root);
  CharacterVector series_names = content["series"];
  CharacterVector obs_names = content["obs"];
  int nobs = content["n_obs"];
  int nvars = series_names.size() + obs_names.size();
  int m = 0, n;

  //dataframe to return
  List out = init_dataframe(nvars, nobs);
  out.attr("names") = content["colnames"];

  //dataset
  rapidxml::xml_node<> *dataset = root->first_node("DataSet");

  //series
  for (rapidxml::xml_node<> *series = dataset->first_node("Series");
       series; series = series->next_sibling())
  {

    //stop after first sibling that isnt a series is reached
    if (strcmp(series -> name(), "Series") != 0)
      break;

    //series attributes
    out = get_node_value(series, out, m);
    n = m + 1;

    //series observations
    for (rapidxml::xml_node<> *obs = series->first_node("Obs");
         obs; obs = obs->next_sibling())
    {
      //series observation attributes
      out = get_node_value(obs, out, m);
      ++m;
    }

    // fill forward series attributes
    out = fill_series(out, series_names, n, m);

  }

  out.attr("row.names") = seq(1, nobs);
  out.attr("class") = "data.frame";
  return out;
}

#endif
