contract A {
  int immutable a;
  constructor() { a = 5; }
  function f() public { --a; }
}

// ----
// TypeError 1581: (85-86): Immutable variables can only be initialized inline or assigned directly in the constructor.
// TypeError 1574: (85-86): Immutable state variable already initialized.
