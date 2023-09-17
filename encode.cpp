//
// Created by ash on 14.09.23.
//

#include "encode.h"

#include<iostream>

void encode(Reader &rd, Writer &wr) {
    // Массив для подсчета частоты встречаемости символов
    unsigned long long characterFrequencies[256] = {0};
    char currentChar;
    std::vector<char> allCharacters;

    // Считываем символы и увеличиваем соответствующий счетчик в массиве characterFrequencies
    while (rd.get_byte(currentChar)) {
        characterFrequencies[static_cast<uint8_t>(currentChar)]++;
        allCharacters.push_back(currentChar);
    }

    // Множество для хранения пар (частота, символы и их коды)
    std::set<std::pair<int, std::vector<std::pair<char, std::vector<bool>>>>> symbolSet;

    // Заполняем начальное множество с отдельными символами и частотами
    for (int i = 0; i < 256; i++) {
        if (characterFrequencies[i] > 0) {
            symbolSet.insert({characterFrequencies[i], {{static_cast<char>(i), {}}}});
        }
    }

    // Удаляем элементы с частотой 0
    while (symbolSet.begin()->first == 0) {
        symbolSet.erase(symbolSet.begin());
    }

    // Карта для хранения символов и соответствующих им битовых последовательностей
    std::map<char, std::vector<bool>> symbolCodeMap;

    if (symbolSet.size() == 1) {
        // Если есть всего один символ, устанавливаем для него код {1}
        symbolCodeMap[symbolSet.begin()->second[0].first] = {1};
    } else {
        // Построение дерева Хаффмана для кодирования символов
        while (symbolSet.size() > 1) {
            auto lowestFreqSymbol0 = *symbolSet.begin();
            symbolSet.erase(symbolSet.begin());
            auto lowestFreqSymbol1 = *symbolSet.begin();
            symbolSet.erase(symbolSet.begin());
            decltype(lowestFreqSymbol0) mergedSymbol;
            mergedSymbol.first = lowestFreqSymbol0.first + lowestFreqSymbol1.first;
            for (auto symbol : lowestFreqSymbol0.second) {
                symbol.second.push_back(0);
                mergedSymbol.second.push_back(symbol);
            }
            for (auto symbol : lowestFreqSymbol1.second) {
                symbol.second.push_back(1);
                mergedSymbol.second.push_back(symbol);
            }
            symbolSet.insert(mergedSymbol);
        }

        // Заполняем карту символов и их кодов
        for (auto symbol : symbolSet.begin()->second) {
            std::reverse(symbol.second.begin(), symbol.second.end());
            symbolCodeMap[symbol.first] = symbol.second;
        }
    }

    std::vector<bool> encodedData;
    int bitCount = 0;

    // Записываем символы и их коды в выходной поток
    for (auto [character, code] : symbolCodeMap) {
        std::cout<<std::hex<<character<<" ";
        // Записываем символ
        for (int i = 7; i >= 0; i--) {
            encodedData.push_back(character & (1 << i));
            bitCount++;
        }

        // Записываем длину кода символа
        for (int i = 7; i >= 0; i--) {
            encodedData.push_back(code.size() & (1 << i));
            bitCount++;
        }

        // Записываем код символа
        for (bool bit : code) {
            std::cout<<bit;
            encodedData.push_back(bit);
            bitCount++;
        }
        std::cout<<std::endl;
    }

    // Завершающие биты для выравнивания по байту
    encodedData.push_back(0);
    encodedData.push_back(0);
    encodedData.push_back(0);

    // Записываем закодированные символы
    for (char character : allCharacters) {
        for (bool bit : symbolCodeMap[character]) {
            encodedData.push_back(bit);
        }
    }

    // Рассчитываем и записываем режим
    int paddingBits = (8 - encodedData.size() % 8) % 8;
    char numUniqueSymbols = symbolCodeMap.size();
    wr.set_byte(numUniqueSymbols);

    encodedData[bitCount++] = (paddingBits & 4);
    encodedData[bitCount++] = (paddingBits & 2);
    encodedData[bitCount++] = (paddingBits & 1);

    // Выводим данные
    for (bool bit : encodedData) {
        wr.set_bit(bit);
    }
}
