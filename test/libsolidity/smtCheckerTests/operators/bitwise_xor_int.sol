pragma experimental SMTChecker;

contract C {
	function f() public pure {
		int8 x = 1;
		int16 y = 0;
		assert(x ^ y == 1);
		x = 7; y = 3;
		assert(x ^ y < 5);
		assert(x ^ y > 5);
	}
}
// ----
// Warning 6328: (164-181): Assertion violation happens here
