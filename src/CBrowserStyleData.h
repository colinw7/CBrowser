#ifndef CBrowserStyleData_H
#define CBrowserStyleData_H

#include <map>
#include <string>

class CBrowserStyleData {
 public:
  typedef std::map<std::string, std::string> NameValues;

 public:
  explicit CBrowserStyleData(const std::string &id="") :
   id_(id) {
  }

  const std::string &id() const { return id_; }
  void setId(const std::string &s) { id_ = s; }

  void setValue(const std::string &name, const std::string &value);

  const NameValues &nameValues() const { return nameValues_; }

 private:
  std::string id_;
  NameValues  nameValues_;
};

#endif
