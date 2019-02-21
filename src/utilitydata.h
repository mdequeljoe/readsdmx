#ifndef UTILITYDATA_
#define UTILITYDATA_
#include "compactdata.h"
#include "datamessage.h"
#include "rapidxml.h"
#include <Rcpp.h>

class utility_msg : public compact_msg {
public:
  std::map<std::string, Rcpp::CharacterVector>
  read_msg(rapidxml::xml_node<> *root) {
    rapidxml::xml_node<> *dataset = root->first_node("DataSet");
    stopifnot_dataset(dataset);

    std::size_t n = cnt_obs_by_sibling(dataset);
    std::size_t m = 0;
    std::vector<std::map<std::string, std::string> > data_(n);
    std::map<std::string, std::string> obs_key, obs_val, sg_key, series_key,
        series_attr_val;
    // basic structure:
    // SiblingGroup
    //  Series
    //   Key
    //   Obs
    //    Obs dim node
    for (rapidxml::xml_node<> *sg = dataset->first_node("SiblingGroup"); sg;
         sg = sg->next_sibling()) {

      if (strcmp(sg->name(), "SiblingGroup") != 0)
        break;

      sg_key = get_node_value_(sg);

      // series
      for (rapidxml::xml_node<> *series = sg->first_node("Series"); series;
           series = series->next_sibling()) {

        if (strcmp(series->name(), "Series") != 0)
          break;

        series_key = sg_key;
        rapidxml::xml_node<> *key_node = series->first_node("Key");
        if (key_node == NULL)
          break;
        for (rapidxml::xml_node<> *key = key_node->first_node(); key;
             key = key->next_sibling()) {
          series_key[key->name()] = key->value();
        }
        // get any attributes from series
        series_attr_val = get_node_value_(series);
        series_key.insert(series_attr_val.begin(), series_attr_val.end());

        // series observations
        if (series->first_node("Obs") == NULL){
          data_[m] = series_key;
          ++m;
          continue;
        }

        for (rapidxml::xml_node<> *obs = series->first_node("Obs"); obs;
             obs = obs->next_sibling()) {

          obs_key = series_key;
          obs_val = get_node_value_(obs);
          obs_key.insert(obs_val.begin(), obs_val.end());

          for (rapidxml::xml_node<> *obsdata = obs->first_node(); obsdata;
               obsdata = obsdata->next_sibling()) {
            obs_key[obsdata->name()] = obsdata->value();
          }
          // combine information for sg, series, key and obs
          data_[m] = obs_key;
          ++m;
        }
      }
    }
    std::map<std::string, Rcpp::CharacterVector> out = as_list(data_, m);
    return out;
  }

  inline std::size_t cnt_obs_by_sibling(rapidxml::xml_node<> *ds) {
    std::size_t n = 0;
    for (rapidxml::xml_node<> *series = ds->first_node("SiblingGroup"); series;
         series = series->next_sibling()) {
      if (strcmp(series->name(), "SiblingGroup") != 0)
        break;
      n += cnt_obs(series);
    }
    return n;
  }
};

template <>
std::map<std::string, Rcpp::CharacterVector>
readsdmx<UTILITYDATA>(rapidxml::xml_node<> *root) {
  utility_msg d;
  return d.read_msg(root);
}

#endif
