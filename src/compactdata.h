#ifndef COMPACTDATA_
#define COMPACTDATA_
#include "datamessage.h"
#include "rapidxml.h"
#include <Rcpp.h>

class compact_msg : public msg_base {
public:
  std::map<std::string, Rcpp::CharacterVector>
  read_msg(rapidxml::xml_node<> *root) {
    rapidxml::xml_node<> *dataset = root->first_node("DataSet");
    stopifnot_dataset(dataset);

    std::size_t n = cnt_obs(dataset);
    std::size_t m = 0;
    std::vector<std::map<std::string, std::string> > data_(n);
    std::map<std::string, std::string> obs_key, obs_val, series_key;

    // series
    for (rapidxml::xml_node<> *series = dataset->first_node("Series"); series;
         series = series->next_sibling()) {

      if (strcmp(series->name(), "Series") != 0)
        break;

      series_key = get_node_attributes(series);

      // series with no obs still returned to dataframe as NA
      if (series->first_node("Obs") == NULL) {
        data_[m] = series_key;
        ++m;
        continue;
      }

      // series observations
      for (rapidxml::xml_node<> *obs = series->first_node("Obs"); obs;
           obs = obs->next_sibling()) {
        obs_key = series_key;
        // series observation attributes
        obs_val = get_node_attributes(obs);
        obs_key.insert(obs_val.begin(), obs_val.end());
        data_[m] = obs_key;
        ++m;
      }
    }

    std::map<std::string, Rcpp::CharacterVector> out = as_list(data_, m);
    return out;
  }
  inline std::size_t cnt_obs(rapidxml::xml_node<> *ds) {
    std::size_t n = 0;
    for (rapidxml::xml_node<> *series = ds->first_node("Series"); series;
         series = series->next_sibling()) {
      if (strcmp(series->name(), "Series") != 0)
        break;
      // series with no obs still returned to dataframe as NA
      if (series->first_node("Obs") == NULL) {
        ++n;
        continue;
      }
      // series observations
      for (rapidxml::xml_node<> *obs = series->first_node("Obs"); obs;
           obs = obs->next_sibling()) {
        ++n;
      }
    }
    return n;
  }

};

template <>
std::map<std::string, Rcpp::CharacterVector>
readsdmx<COMPACTDATA>(rapidxml::xml_node<> *root) {
  compact_msg d;
  return d.read_msg(root);
}

#endif
