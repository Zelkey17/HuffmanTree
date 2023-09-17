//
// Created by ash on 09.09.23.
//

#include "Reader.h"

[[nodiscard]] std::string Reader::get_file_name() const noexcept {
    return file_name;
}

[[nodiscard]] bool Reader::is_open() const noexcept {
    return fcntl(fd, F_GETFL) > 0;
}

void Reader::open(std::string file) {
    close(fd);
    file_name = std::move(file);
    if (int new_fd = ::open(file_name.c_str(), O_RDONLY); new_fd > 0) {
        buf_size = 0;
        fd = new_fd;
    } else {
        ::perror(("Reader::open(\"" + file_name + "\")").c_str());
        exit(1);
    }
}

bool Reader::get_bit(bool &ans) {
    if (buf_size > 0) {
        ans = (buf >> --buf_size) & 1;
        return true;
    } else {
        if (auto ss = read(fd, &buf, 1);ss == -1) {
            ::perror(("Reader::get_bit (" + file_name + ")").c_str());
            exit(1);
        } else if (ss == 0) {
            return false;
        }
        buf_size = 8;
        ans = (buf >> --buf_size) & 1;
        return true;
    }
}

Reader::~Reader() {
    ::close(fd);
}

Reader::Reader(std::string file) {
    open(std::move(file));
}

bool Reader::get_byte(char &ans) {
    ans = 0;
    for (int i = 0; i < 8; i++) {
        bool q;
        if (!get_bit(q))return false;
        ans <<= 1;
        ans += q;
    }
}

