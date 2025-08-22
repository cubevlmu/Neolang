// Created by cubevlmu on 2025/8/23.
// Copyright (c) 2025 Flybird Games. All rights reserved.

#pragma once

#include <neo/common.hpp>
#include <format>

namespace neo {

    template <typename... Args>
    NE_FORCE_INLINE std::string format(std::string_view fmtStr, const Args&... args) {
        return std::vformat(fmtStr, std::make_format_args(args...));
    }

}