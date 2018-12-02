#ifndef READSDMX_UTILS_
#define READSDMX_UTILS_

#include <Rcpp.h>
#include "rapidxml.hpp"
#include <rapidxml_utils.hpp>

using namespace Rcpp;

bool has_element(CharacterVector x, const char *s)
{
  return std::find(x.begin(), x.end(), s) != x.end();
}

std::string raw_to_string(RawVector x)
{
  std::string s(RAW(x), RAW(x) + x.size());
  return s;
}

std::string read_binlist(List r)
{
  std::string m;
  for (int i = 0; i < r.size(); i++)
  {
    m += raw_to_string(as<RawVector>(r[i]));
  }
  m.push_back('\0');
  return m;
}

CharacterVector get_node_attr(rapidxml::xml_node<> *node, CharacterVector names)
{

  if (node->first_attribute() == 0)
    return names;

  for (rapidxml::xml_attribute<> *attr = node->first_attribute();
       attr; attr = attr->next_attribute())
  {
    if (!has_element(names, attr->name()))
      names.push_back(attr->name());
  }
  return names;
}

List get_node_value(rapidxml::xml_node<> *node, List data, int m)
{
  if (node->first_attribute() == 0)
    return data;

  for (rapidxml::xml_attribute<> *series_attr = node->first_attribute();
       series_attr; series_attr = series_attr->next_attribute())
  {
    as<CharacterVector>(data[series_attr->name()])[m] = series_attr->value();
  }
  return data;
}

List fill_series(List data, CharacterVector fill_names, int n, int m)
{
  std::string name;
  int i, j;
  for (i = 0; i < fill_names.size(); i++)
  {
    name = fill_names[i];
    for (j = n; j < m; j++)
    {
      as<CharacterVector>(data[name])[j] = as<CharacterVector>(data[name])[j - 1];
    }
  }
  return data;
}

List init_dataframe(int n, int v)
{
  List out(n);
  for (int i = 0; i < n; i++)
  {
    out[i] = CharacterVector(v, NA_STRING);
  }
  return out;
}

#endif
