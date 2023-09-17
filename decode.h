//
// Created by ash on 14.09.23.
//

#ifndef GETSMALER_DECODE_H
#define GETSMALER_DECODE_H

#include "Reader.h"
#include "Writer.h"
#include<vector>
#include<map>

// Декодирование данных с использованием дерева Хаффмана
void decode(Reader& rd, Writer& wr);


// Структура для узла дерева Хаффмана
struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    char character = ' ';
    Node** children[2] = {&left, &right};

    // Проверка, является ли узел листовым
    [[nodiscard]] bool is_leaf() const;

    ~Node();
};

#endif //GETSMALER_DECODE_H
