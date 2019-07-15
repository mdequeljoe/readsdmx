#ifndef CROSS_SECTIONALDATA_
#define CROSS_SECTIONALDATA_
#include "compactdata.h"
#include "datamessage.h"
#include "rapidxml.h"
#include <Rcpp.h>

class cross_sectional_msg : public compact_msg {
public:
  std::map<std::string, Rcpp::CharacterVector>
  read_msg(rapidxml::xml_node<> *root) {
    rapidxml::xml_node<> *dataset = root->first_node("DataSet");
    stopifnot_dataset(dataset);

    std::size_t n = cnt_obs(dataset);
    std::size_t m = 0;
    std::vector<std::map<std::string, std::string> > data_(n);
    std::map<std::string, std::string> obs_key, obs_val, group_key, section_key,
        section_attr;
    // group
    // section
    //  nodes..
    for (rapidxml::xml_node<> *group = dataset->first_node("Group"); group;
         group = group->next_sibling()) {

      if (strcmp(group->name(), "Group") != 0)
        break;

      group_key = get_node_attributes(group);

      for (rapidxml::xml_node<> *section = group->first_node("Section");
           section; section = section->next_sibling()) {
        section_key = group_key;
        if (section == NULL)
          break;
        section_attr = get_node_attributes(section);
        section_key.insert(section_attr.begin(), section_attr.end());

        if (section->first_node() == NULL) {
          data_[m] = section_key;
          ++m;
          continue;
        }
        for (rapidxml::xml_node<> *s_dim = section->first_node(); s_dim;
             s_dim = s_dim->next_sibling()) {
          obs_key = section_key;
          obs_key["OBS_KEY"] = s_dim->name();
          obs_val = get_node_attributes(s_dim);
          obs_key.insert(obs_val.begin(), obs_val.end());
          data_[m] = obs_key;
          ++m;
        }
      }
    }

    std::map<std::string, Rcpp::CharacterVector> out = as_list(data_, m);
    return out;
  }
  inline std::size_t cnt_obs(rapidxml::xml_node<> *ds) {
    std::size_t n = 0;
    for (rapidxml::xml_node<> *group = ds->first_node("Group"); group;
         group = group->next_sibling()) {
      if (strcmp(group->name(), "Group") != 0)
        break;
      for (rapidxml::xml_node<> *section = group->first_node("Section");
           section; section = section->next_sibling()) {
        for (rapidxml::xml_node<> *obs = section->first_node(); obs;
             obs = obs->next_sibling()) {
          ++n;
        }
      }
    }
    return n;
  }
};

template <>
std::map<std::string, Rcpp::CharacterVector>
readsdmx<CROSS_SECTIONALDATA>(rapidxml::xml_node<> *root) {
  cross_sectional_msg d;
  return d.read_msg(root);
}

#endif
