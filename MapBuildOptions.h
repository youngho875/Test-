#pragma once
// Set in Project Properties > C/C++ > Preprocessor Definitions.
#ifndef C4MAP_ENABLE_LOCAL
#define C4MAP_ENABLE_LOCAL 1
#endif
#ifndef C4MAP_ENABLE_OGC
#define C4MAP_ENABLE_OGC 1
#endif
#if !C4MAP_ENABLE_LOCAL && !C4MAP_ENABLE_OGC
#error Enable at least one map source backend.
#endif
