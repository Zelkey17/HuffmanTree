//
// Created by ash on 13.09.23.
//

#include "Writer.h"

Writer::Writer(std::string file) {
    open(std::move(file));
}

bool Writer::set_byte(char ans) {
    for (int i = 7; i >= 0; i--) {
        set_bit(ans & (1 << i));
    }
}

Writer::~Writer() {
    buf = buf << (8 - buf_size);
    write(fd, &buf, 1);
    ::close(fd);
}

bool Writer::set_bit(bool b) {
    if (buf_size < 8) {
        buf <<= 1;
        buf += b;
        buf_size++;
        return true;
    } else {
        if (auto ss = write(fd, &buf, 1); ss == -1) {
            ::perror(("Write::set_bit (" + file_name + ")").c_str());
            exit(1);
        } else if (ss == 0) {
            return false;
        }
        buf_size = 0;
        buf <<= 1;
        buf += b;
        buf_size++;
        return true;
    }
}

void Writer::open(std::string file) {
    close(fd);
    file_name = std::move(file);
    if (int new_fd = ::open(file_name.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR); new_fd > 0) {
        buf_size = 0;
        fd = new_fd;
    } else {
        ::perror(("Writer::open(\"" + file_name + "\")").c_str());
        exit(1);
    }
}

bool Writer::is_open() const noexcept {
    return fcntl(fd, F_GETFL) > 0;
}

std::string Writer::get_file_name() const noexcept {
    return file_name;
}
