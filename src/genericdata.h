#ifndef GENERICDATA_
#define GENERICDATA_
#include "datamessage.h"
#include "rapidxml.h"
#include <Rcpp.h>

class generic_msg : public msg_base {
public:
  std::map<std::string, Rcpp::CharacterVector>
  read_msg(rapidxml::xml_node<> *root) {
    rapidxml::xml_node<> *dataset = root->first_node("DataSet");
    stopifnot_dataset(dataset);

    std::size_t n = cnt_obs(dataset);
    std::size_t m = 0;
    std::vector<std::map<std::string, std::string> > data_(n);
    std::map<std::string, std::string> skey, okey, series_data;

    if (dataset->first_node("Series") == NULL) {

      // data not organised by series - only by observationi
      for (rapidxml::xml_node<> *obs = dataset->first_node("Obs"); obs;
           obs = obs->next_sibling()) {
        okey = obs_key_(obs);
        data_[m] = okey;
        ++m;
      }

    } else {

      for (rapidxml::xml_node<> *series = dataset->first_node("Series"); series;
           series = series->next_sibling()) {

        if (strcmp(series->name(), "Series") != 0)
          break;

        skey = series_key_(series);
        // series with no obs still returned to dataframe as NA
        if (series->first_node("Obs") == NULL) {
          data_[m] = skey;
          ++m;
          continue;
        }
        // collect observations
        for (rapidxml::xml_node<> *obs = series->first_node("Obs"); obs;
             obs = obs->next_sibling()) {
          series_data = skey;
          okey = obs_key_(obs);
          series_data.insert(okey.begin(), okey.end());
          data_[m] = series_data;
          ++m;
        }
      }
    }

    std::map<std::string, Rcpp::CharacterVector> out = as_list(data_, m);
    return out;
  }

  std::map<std::string, std::string> series_key_(rapidxml::xml_node<> *node) {
    std::map<std::string, std::string> series;

    for (rapidxml::xml_node<> *val = node->first_node(); val;
         val = val->next_sibling()) {
      if (strcmp(val->name(), "Obs") == 0)
        break;

      for (rapidxml::xml_node<> *val_key = val->first_node(); val_key;
           val_key = val_key->next_sibling()) {
        rapidxml::xml_attribute<> *key = val_key->first_attribute("id");
        if (key == NULL)
          key = val_key->first_attribute("concept");

        rapidxml::xml_attribute<> *val_ = val_key->first_attribute("value");
        if (val_ != NULL)
          series[key->value()] = val_->value();
        else
          series[key->value()] = key->next_attribute()->value();
      }
    }
    return series;
  }

  std::map<std::string, std::string> obs_key_(rapidxml::xml_node<> *node) {
    std::map<std::string, std::string> series_obs;

    for (rapidxml::xml_node<> *val = node->first_node(); val;
         val = val->next_sibling()) {
      if (val == NULL)
        continue;

      if (strcmp(val->name(), "Time") == 0) {
        series_obs[val->name()] = val->value();
      } else if (strcmp(val->name(), "Attributes") == 0 ||
                 strcmp(val->name(), "ObsKey") == 0) {
        // attributes and Obskey nodes are children
        for (rapidxml::xml_node<> *attr = val->first_node(); attr;
             attr = attr->next_sibling()) {
          // version 2.0:"concept", version 2.1:"id"
          rapidxml::xml_attribute<> *concept_attr = attr->first_attribute("id");
          if (concept_attr == NULL)
            concept_attr = attr->first_attribute("concept");

          rapidxml::xml_attribute<> *val_attr = attr->first_attribute("value");
          if (val_attr != NULL)
            series_obs[concept_attr->value()] = val_attr->value();
          else
            series_obs[concept_attr->value()] =
                concept_attr->next_attribute()->value();
        }
      } else // Obsvalue; Obsdimension (version 2.1)
      {
        series_obs[val->name()] = val->first_attribute("value")->value();
      }
    }
    return series_obs;
  }

  inline std::size_t cnt_obs(rapidxml::xml_node<> *ds) {
    std::size_t n = 0;
    if (ds->first_node("Series") == NULL) {
      // data not organised by series - only by observation
      for (rapidxml::xml_node<> *obs = ds->first_node("Obs"); obs;
           obs = obs->next_sibling()) {
        ++n;
      }
      return n;
    }
    for (rapidxml::xml_node<> *series = ds->first_node("Series"); series;
         series = series->next_sibling()) {

      if (strcmp(series->name(), "Series") != 0)
        break;

      // series with no obs still returned to dataframe as NA
      if (series->first_node("Obs") == NULL) {
        ++n;
        continue;
      }
      // collect observations
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
readsdmx<GENERICDATA>(rapidxml::xml_node<> *root) {
  generic_msg d;
  return d.read_msg(root);
}

#endif
