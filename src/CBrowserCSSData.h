#ifndef CBrowserCSSData_H
#define CBrowserCSSData_H

#include <CBrowserStyleData.h>
#include <CUrl.h>

class CBrowserCSSData {
 public:
  typedef std::map<std::string, CBrowserStyleData> StyleDataMap;
  typedef std::map<std::string, StyleDataMap>      TypeStyleDataMap;

 public:
  CBrowserCSSData() { }

  const CUrl &url() const { return url_; }
  void setUrl(const CUrl &v) { url_ = v; }

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

  // global
  bool hasGlobalStyleData() const {
    return (globalStyle_.find("*") != globalStyle_.end());
  }

  const CBrowserStyleData &getGlobalStyleData() const {
    auto p = globalStyle_.find("*");

    assert(p != globalStyle_.end());

    return (*p).second;
  }

  CBrowserStyleData &getGlobalStyleData() {
    auto p = globalStyle_.find("*");

    if (p == globalStyle_.end())
      p = globalStyle_.insert(p,
        StyleDataMap::value_type("*", CBrowserStyleData("*")));

    return (*p).second;
  }

  // name
  bool hasNameStyleData(const std::string &objName) const {
    return (nameStyle_.find(objName) != nameStyle_.end());
  }

  const CBrowserStyleData &getNameStyleData(const std::string &objName) const {
    auto p = nameStyle_.find(objName);

    assert(p != nameStyle_.end());

    return (*p).second;
  }

  CBrowserStyleData &getNameStyleData(const std::string &objName) {
    auto p = nameStyle_.find(objName);

    if (p == nameStyle_.end())
      p = nameStyle_.insert(p,
        StyleDataMap::value_type(objName, CBrowserStyleData(objName)));

    return (*p).second;
  }

  // type
  bool hasTypeStyleData(const std::string &objType) const {
    return (typeStyle_.find(objType) != typeStyle_.end());
  }

  const CBrowserStyleData &getTypeStyleData(const std::string &objType) const {
    auto p = typeStyle_.find(objType);

    assert(p != typeStyle_.end());

    return (*p).second;
  }

  CBrowserStyleData &getTypeStyleData(const std::string &objType) {
    auto p = typeStyle_.find(objType);

    if (p == typeStyle_.end())
      p = typeStyle_.insert(p,
        StyleDataMap::value_type(objType, CBrowserStyleData(objType)));

    return (*p).second;
  }

  // class
  bool hasClassStyleData(const std::string &objClass) const {
    return (classStyle_.find(objClass) != classStyle_.end());
  }

  const CBrowserStyleData &getClassStyleData(const std::string &objClass) const {
    auto p = classStyle_.find(objClass);

    assert(p != classStyle_.end());

    return (*p).second;
  }

  CBrowserStyleData &getClassStyleData(const std::string &objClass) {
    auto p = classStyle_.find(objClass);

    if (p == classStyle_.end())
      p = classStyle_.insert(p,
        StyleDataMap::value_type(objClass, CBrowserStyleData(objClass)));

    return (*p).second;
  }

  // type/class
  bool hasTypeClassStyleData(const std::string &objType, const std::string &objClass) const {
    auto p1 = typeClassStyle_.find(objType);

    if (p1 == typeClassStyle_.end())
      return false;

    const StyleDataMap &typeStyleDataMap = (*p1).second;

    return (typeStyleDataMap.find(objClass) != typeStyleDataMap.end());
  }

  const CBrowserStyleData &getTypeClassStyleData(const std::string &objType,
                                                 const std::string &objClass) const {
    auto p1 = typeClassStyle_.find(objType);

    assert(p1 != typeClassStyle_.end());

    const StyleDataMap &typeStyleDataMap = (*p1).second;

    auto p2 = typeStyleDataMap.find(objClass);

    assert(p2 != typeStyleDataMap.end());

    return (*p2).second;
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

  static const CBrowserStyleData &indexStyleData(const StyleDataMap &map, int ind) {
    static CBrowserStyleData s_styleData;

    int ind1 = 0;

    for (const auto &styleData : map) {
      if (ind == ind1)
        return styleData.second;

      ++ind1;
    }

    assert(false);

    return s_styleData;
  }

  static const StyleDataMap &indexTypeStyleData(const TypeStyleDataMap &map, int ind) {
    static StyleDataMap s_styleDataMap;

    int ind1 = 0;

    for (const auto &styleDataMap : map) {
      if (ind == ind1)
        return styleDataMap.second;

      ++ind1;
    }

    assert(false);

    return s_styleDataMap;
  }

 private:
  CUrl             url_;
  StyleDataMap     globalStyle_;
  StyleDataMap     nameStyle_;
  StyleDataMap     typeStyle_;
  StyleDataMap     classStyle_;
  TypeStyleDataMap typeClassStyle_;
};

#endif
