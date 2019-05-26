#ifndef __M19_SEMANTICS_SYMBOL_H__
#define __M19_SEMANTICS_SYMBOL_H__

#include <string>
#include <vector>
#include <cdk/basic_type.h>

namespace m19 {

  class symbol {
    basic_type *_type;
    std::string _name;
    long _value; // hack!

    int _offset = 0;
    
    //function symbol
    bool _function;
    bool _defined_function = false;
    std::vector<basic_type*> _argument_types;

  public:
    symbol(basic_type *type, const std::string &name, long value) :
        _type(type), _name(name), _value(value), _function(false) {
    }
    symbol(basic_type *type, const std::string &name, long value, std::vector<basic_type*> types) :
        _type(type), _name(name), _value(value), _function(true), _argument_types(types) {
    }

    virtual ~symbol() {
      delete _type;
    }

    basic_type *type() const {
      return _type;
    }
    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    bool global() const {
      return _offset == 0;
    }
    int offset() {
      return _offset;
    }
    void set_offset(int offset) {
      _offset = offset;
    }
    bool is_function() {
      return _function;
    }
    bool defined_function() {
      return _defined_function;
    }
    void defined_function(bool def) {
      _defined_function = def;
    }
    std::vector<basic_type*> arguments() {
      return _argument_types;
    }
  };

} // m19

#endif
