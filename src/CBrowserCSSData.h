#ifndef CBrowserCSSData_H
#define CBrowserCSSData_H

#include <CBrowserStyleData.h>

class CBrowserCSSData {
 public:
  typedef std::map<std::string, CBrowserStyleData> StyleDataMap;
  typedef std::map<std::string, StyleDataMap>      TypeStyleDataMap;

 public:
  CBrowserCSSData() { }

  const StyleDataMap     &globalStyle() const { return globalStyle_; }
  const StyleDataMap     &nameStyle() const { return nameStyle_; }
  const StyleDataMap     &typeStyle() const { return typeStyle_; }
  const StyleDataMap     &classStyle() const { return classStyle_; }
  const TypeStyleDataMap &typeClassStyle() const { return typeClassStyle_; }

  void clear() {
    globalStyle_   .clear();
    nameStyle_     .clear();
    typeStyle_     .clear();
    classStyle_    .clear();
    typeClassStyle_.clear();
  }

  bool hasGlobalStyleData() const {
    return (globalStyle_.find("*") != globalStyle_.end());
  }

  CBrowserStyleData &getGlobalStyleData() {
    auto p = globalStyle_.find("*");

    if (p == globalStyle_.end())
      p = globalStyle_.insert(p,
        StyleDataMap::value_type("*", CBrowserStyleData("*")));

    return (*p).second;
  }

  bool hasNameStyleData(const std::string &objName) const {
    return (nameStyle_.find(objName) != nameStyle_.end());
  }

  CBrowserStyleData &getNameStyleData(const std::string &objName) {
    auto p = nameStyle_.find(objName);

    if (p == nameStyle_.end())
      p = nameStyle_.insert(p,
        StyleDataMap::value_type(objName, CBrowserStyleData(objName)));

    return (*p).second;
  }

  bool hasTypeStyleData(const std::string &objType) const {
    return (typeStyle_.find(objType) != typeStyle_.end());
  }

  CBrowserStyleData &getTypeStyleData(const std::string &objType) {
    auto p = typeStyle_.find(objType);

    if (p == typeStyle_.end())
      p = typeStyle_.insert(p,
        StyleDataMap::value_type(objType, CBrowserStyleData(objType)));

    return (*p).second;
  }

  bool hasClassStyleData(const std::string &objClass) const {
    return (classStyle_.find(objClass) != classStyle_.end());
  }

  CBrowserStyleData &getClassStyleData(const std::string &objClass) {
    auto p = classStyle_.find(objClass);

    if (p == classStyle_.end())
      p = classStyle_.insert(p,
        StyleDataMap::value_type(objClass, CBrowserStyleData(objClass)));

    return (*p).second;
  }

  bool hasTypeClassStyleData(const std::string &objType, const std::string &objClass) const {
    auto p1 = typeClassStyle_.find(objType);

    if (p1 == typeClassStyle_.end())
      return false;

    const StyleDataMap &typeStyleDataMap = (*p1).second;

    return (typeStyleDataMap.find(objClass) != typeStyleDataMap.end());
  }

  CBrowserStyleData &getTypeClassStyleData(const std::string &objType,
                                           const std::string &objClass) {
    auto p1 = typeClassStyle_.find(objType);

    if (p1 == typeClassStyle_.end())
      p1 = typeClassStyle_.insert(p1,
        TypeStyleDataMap::value_type(objType, StyleDataMap()));

    StyleDataMap &typeStyleDataMap = (*p1).second;

    auto p2 = typeStyleDataMap.find(objClass);

    if (p2 == typeStyleDataMap.end())
      p2 = typeStyleDataMap.insert(p2,
        StyleDataMap::value_type(objClass, CBrowserStyleData(objClass)));

    return (*p2).second;
  }

 private:
  StyleDataMap     globalStyle_;
  StyleDataMap     nameStyle_;
  StyleDataMap     typeStyle_;
  StyleDataMap     classStyle_;
  TypeStyleDataMap typeClassStyle_;
};

#endif
