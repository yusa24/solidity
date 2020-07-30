contract C {
  function f() public pure {
    assembly {
      function g() -> a,b, c {}
      let a, sub, mov := g()
    }
  }
}
// ----
// ParserError 5568: (102-105): Cannot use reserved name "sub" as identifier name.
