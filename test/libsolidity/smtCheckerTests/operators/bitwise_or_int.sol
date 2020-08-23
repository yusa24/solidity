pragma experimental SMTChecker;

contract C {
	function f() public pure {
		int16 x = 1;
		int16 y = 0;
		assert(x | y == 1);
		x = 0; y = 0;
		assert(x | y != 0);
		y = 240;
		x = 15;
		int16 z = x | y;
		assert(z == 255);
	}
}
// ----
// Warning 6328: (144-162): Assertion violation happens here
