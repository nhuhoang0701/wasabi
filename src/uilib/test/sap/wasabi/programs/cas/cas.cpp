#include "peglib.h"
#include <assert.h>
#include <iostream>
#include "num.hpp"
#include "utils.h"
using namespace peg;
using namespace std;

int main(void) {
  parser parser(R"(
        # Grammar for Calculator...
        Expression  <- Additive / 'gcd(' Additive ',' Additive ')'
        Additive    <- Multitive '+' Additive / Multitive
        Multitive   <- Primary '*' Multitive / Primary
        Primary     <- '(' Additive ')' / Number
        Number      <- < [0-9]+ >
        %whitespace <- [ \t]*
    )");

  assert((bool)parser == true);

  parser["Expression"] = [](const SemanticValues& sv) {
    switch (sv.choice()) {
    case 0:  // "Additive"
      return any_cast<Num>(sv[0]);
    default: // "gcd"
      return any_cast<Num>(Num::gcd(any_cast<Num>(sv[0]),any_cast<Num>(sv[1])));
    }
  };
  parser["Additive"] = [](const SemanticValues& sv) {
    switch (sv.choice()) {
    case 0:  // "Multitive '+' Additive"
      return any_cast<Num>(sv[0]) + any_cast<Num>(sv[1]);
    default: // "Multitive"
      return any_cast<Num>(sv[0]);
    }
  };

  parser["Multitive"] = [](const SemanticValues& sv) {
    switch (sv.choice()) {
    case 0:  // "Primary '*' Multitive"
      return any_cast<Num>(sv[0]) * any_cast<Num>(sv[1]);
    default: // "Primary"
      return any_cast<Num>(sv[0]);
    }
  };
  parser["Number"] = [](const SemanticValues& sv) {
    return Num(string(sv.token()).c_str());
  };
  parser.enable_packrat_parsing(); // Enable packrat parsing.
  Num val;
  cout << ":>" << std::flush;
  string term = read_string();
  while(term!="quit"){
    val = 0;
    parser.parse(term.c_str(), val);
    cout << endl << ": " << val << endl;
    cout << ":>"<< std::flush;
    term = read_string();
  }
  ff_exit();
}
