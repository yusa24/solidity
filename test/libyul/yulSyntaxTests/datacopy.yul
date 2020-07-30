{
    datacopy(0, 1, 2)
    datasize("")
    datasize(0) // This should not be valid.
    dataoffset("")
    dataoffset(0) // This should not be valid.
}
// ----
// TypeError 3517: (28-36): Unknown data object "".
// TypeError 3517: (45-53): Unknown data object "0".
// TypeError 3517: (90-100): Unknown data object "".
// TypeError 3517: (109-119): Unknown data object "0".
