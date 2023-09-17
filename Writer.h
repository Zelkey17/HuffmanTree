//
// Created by ash on 13.09.23.
//

#ifndef GETSMALER_WRITER_H
#define GETSMALER_WRITER_H

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <utility>


class Writer {
    int fd = -1;
    std::string file_name;
    char buf = 0;
    int buf_size = 0;


public:
    [[nodiscard]] std::string get_file_name() const noexcept;

    [[nodiscard]] bool is_open() const noexcept;

    void open(std::string file);

    bool set_bit(bool b);

    template<int N>
    requires (N <= 8)
    bool set_N_bit(){

    }

    bool set_byte(char ans);

    ~Writer();

    Writer() = default;

    explicit Writer(std::string file);
};

#endif //GETSMALER_WRITER_H
