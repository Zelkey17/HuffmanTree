//
// Created by ash on 09.09.23.
//

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <utility>

#ifndef GETSMALER_READER_H
#define GETSMALER_READER_H


class Reader {
    int fd = -1;
    std::string file_name;
    char buf = 0;
    int buf_size = 0;


public:
    [[nodiscard]] std::string get_file_name() const noexcept;

    [[nodiscard]] bool is_open() const noexcept;

    void open(std::string file);

    bool get_bit(bool &ans);

    template<int N>
    requires (N < 8)
    bool get_N_bit(){

    }

    bool get_byte(char &ans);

    ~Reader();

    Reader() = default;

    explicit Reader(std::string file);
};


#endif //GETSMALER_READER_H
