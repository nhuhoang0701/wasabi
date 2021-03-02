#pragma once
#include <emscripten.h>
#include <string>
using std::string;
EM_JS(char, get_character, (), {
    return Module.inputAsync();
  })
EM_JS(void, ff_exit, (), {
    return Module.ff_exit();
  })
string read_string(){
  string aString;
  char aCharacter;
  while((aCharacter = get_character())!=13){
    aString += aCharacter;
  }
  return aString;
}
