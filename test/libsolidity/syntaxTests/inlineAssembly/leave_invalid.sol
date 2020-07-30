contract C {
  function f() public pure {
    assembly {
      leave
    }
  }
}
// ----
// SyntaxError 8149: (73-74): Keyword "leave" can only be used inside a function.
