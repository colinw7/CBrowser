#ifndef CBrowserStyleData_H
#define CBrowserStyleData_H

#include <map>

class CBrowserStyleData {
 public:
  typedef std::map<std::string, std::string> NameValues;

 public:
  explicit CBrowserStyleData(const std::string &id) :
   id_(id) {
  }

  void setValue(const std::string &name, const std::string &value);

  const NameValues &nameValues() const { return nameValues_; }

 private:
  std::string id_;
  NameValues  nameValues_;
};

#endif
