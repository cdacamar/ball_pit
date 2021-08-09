module;
#include <string>
export module Util;

export import Util.BasicTypes;
export import Util.EnumUtils;
export import Util.RandomGenerator;
export import Util.Stopwatch;

export
using std::to_string;

export
using std::uint8_t;

// Expose the allocation and deallocation functions (since they would be type dependent in template bodies and not bound).
export
using ::operator new;
using ::operator new[];
using ::operator delete;
using ::operator delete[];