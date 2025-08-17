// Created by cubevlmu on 2025/8/13.
// Copyright (c) 2025 Flybird Games. All rights reserved.

#pragma once

namespace neo {

    typedef void(*stack_trace_func)(const char*);
    void getStackTrace(const stack_trace_func);

    void assertIt(const char* msg, const char* func, int line, const char* file);

}

#ifdef NE_DEBUG
#define NE_ASSERTATION 1
#define NE_ASSERT(COND) do { if (!(COND)) { ::neo::assertIt(#COND, __func__, __LINE__, __FILE__); } } while(false)
#else
#define NE_ASSERTATION 0
#define NE_ASSERT(COND) ((void)(COND))
#endif