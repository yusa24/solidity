contract A {
  int immutable a;
  constructor() { a = 5; }
  function f() public { a += 7; }
}

// ----
// TypeError 1581: (83-84): Immutable variables can only be initialized inline or assigned directly in the constructor.
// TypeError 1574: (83-84): Immutable state variable already initialized.
