#pragma once

#define GB_BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)