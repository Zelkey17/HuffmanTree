//
// Created by ash on 14.09.23.
//


#include "decode.h"

// Декодирование данных с использованием дерева Хаффмана
void decode(Reader& rd, Writer& wr) {
    // Чтение количества байт для описания кодирования
    char numBytes;
    rd.get_byte(numBytes);
    auto n = static_cast<uint16_t>(static_cast<unsigned char>(numBytes));
    if (n == 0) {
        n = 256;
    }

    // Создание корня дерева Хаффмана
    Node* root = new Node;

    // Декодирование
    for (int i = 0; i < n; i++) {
        char character;
        rd.get_byte(character);

        char numBits;
        rd.get_byte(numBits);
        auto k = static_cast<uint8_t>(numBits);
        Node* currentNode = root;

        for (int j = 0; j < k; j++) {
            bool bit;
            rd.get_bit(bit);

            if (*(currentNode->children[bit])) {
                currentNode = *(currentNode->children[bit]);
            } else {
                *(currentNode->children[bit]) = new Node();
                currentNode = *(currentNode->children[bit]);
            }
        }
        currentNode->character = character;
    }

    // Чтение режима декодирования
    bool m1 = false, m2 = false, m3 = false;
    rd.get_bit(m1);
    rd.get_bit(m2);
    rd.get_bit(m3);
    uint8_t mode = 4 * m1 + 2 * m2 + m3;

    // Чтение входных битов
    std::vector<bool> inputBits;
    bool bit;
    while (rd.get_bit(bit)) {
        inputBits.push_back(bit);
    }

    // Убирание лишних битов в соответствии с режимом
    for (int i = 0; i < mode; i++) {
        inputBits.pop_back();
    }

    // Декодирование входных битов и запись результата в выходной поток
    Node* currentNode = root;
    for (bool el : inputBits) {
        currentNode = *(currentNode->children[el]);
        if (currentNode->is_leaf()) {
            wr.set_byte(currentNode->character);
            currentNode = root;
        }
    }

    // Освобождение памяти, выделенной для дерева Хаффмана
    delete root;
}

// Проверка, является ли узел листовым
bool Node::is_leaf() const {
    return !(left || right);
}

Node::~Node() {
    delete left;
    delete right;
}
