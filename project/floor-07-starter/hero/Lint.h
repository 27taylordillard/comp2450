// COMP 2450 — Floor 6 starter
// hero/Lint.h — bracket balancer (Wednesday's exercise).
//
// One free function: isBalanced(s) returns true iff every opener in
// `s` has a matching closer in the right order. Recognized pairs:
//
//     (  )
//     [  ]
//     {  }
//
// Non-bracket characters are ignored.  Examples:
//
//     isBalanced("({a} (b) [c])")   == true
//     isBalanced("({)}")            == false
//     isBalanced(")")               == false
//     isBalanced("(")               == false
//     isBalanced("")                == true
//
// You write the body in hero/Lint.cpp using your Stack<char>. This is
// the classic stack-use exercise; reading it should feel like the
// definition of "what is a stack for, actually."

#pragma once

#include <string>

namespace dungeon {

bool isBalanced(const std::string& s);

}  // namespace dungeon
