/**
 * Copyright 2018 Ted Percival <ted@tedp.id.au>
 * https://github.com/tedjp/commune/blob/b5a21b8/lib/safefd.h
 *
 * License: GPL
 * https://github.com/tedjp/commune/blob/b5a21b8/LICENSE
 */
#pragma once

#include <algorithm>
#include <unistd.h>
#include <utility>

namespace commune {

// Basically unique_ptr but for file descriptors

class SafeFD {
public:
    explicit SafeFD(int fd = -1):
        fd_(fd)
    {}

    SafeFD(const SafeFD&) = delete;
    SafeFD(SafeFD&& other) /*noexcept(std::is_nothrow_swappable<int>::value)*/ {
        ::std::swap(fd_, other.fd_);
    }
    SafeFD& operator=(const SafeFD&) = delete;
    SafeFD& operator=(SafeFD&& other) /*noexcept(std::is_nothrow_swappable<int>::value)*/ {
        ::std::swap(fd_, other.fd_);
        return *this;
    }

    // Implicit conversion to int for convenience
    operator int() const {
        return fd_;
    }

    ~SafeFD() {
        if (fd_ != -1) {
            close(fd_);
        }
    }

    int release() {
        int fd = fd_;
        fd_ = -1;
        return fd;
    }

    void reset() {
        if (fd_ != -1)
            close(fd_);
        fd_ = -1;
    }

    void swap(SafeFD& other) {
        std::swap(fd_, other.fd_);
    }

    operator bool() const {
        return fd_ != -1;
    }

    int get() const {
        return fd_;
    }

    int operator*() const {
        return fd_;
    }

private:
    int fd_ = -1;
};

} // namespace commune
