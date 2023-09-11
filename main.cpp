#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int SIZE = 256;

// color text
// могут ли давать таблицу кодирования для кодировки
// расширение файла
// номер битов % 8, единица всегда

int GetNum(int& ind, std::vector<bool>& bits) {
  int num = 0;
  int count = 0;
  while (!bits[ind]) {
    ++count;
    ++ind;
  }
  ++ind;
  for (int i = 0; i < count; ++i, ++ind) {
    num |= (bits[ind] << (count - i - 1));
  }
  return num;
}

int GetNumConst(int& ind, std::vector<bool>& bits) {}

void CreateTable(std::string file_in, std::string file_out) {
  std::ifstream in(file_in, std::ios::binary);
  std::ofstream out(file_out, std::ios::binary);
  return;
}

void Encode(std::string file_in, std::string file_out) {
  std::ifstream in(file_in, std::ios::binary);
  std::ofstream out(file_out, std::ios::binary);
  return;
}

void Decode(std::string file_in, std::string file_out) {
  std::ifstream in(file_in, std::ios::binary);
  std::ofstream out(file_out, std::ios::binary);

  // 0000011000100000011000000000011111000000001110010000000111111000000011100101000010000000000011100110000010001000000011100111000010010000000011101000000010011000000011101001000010100000000011101010000010101000000011101011000010110000000011101100000010111000000011101111000011000000000011110000000011001000000011110010000011010000000011110011000011011000000011110100000011100000000011110101000011101000000011111001000011110110111111111000101001110011010101101111010111110000101011001110111110111001010001110011

  std::vector<bool> bits;
  uint8_t byte;
  while (in.read((char*)&byte, sizeof(byte))) {
    std::vector<bool> temp_bits;
    for (int i = 0; i < 8; ++i) {
      temp_bits.push_back(byte & 1);
      byte >>= 1;
    }
    for (int i = temp_bits.size() - 1; i >= 0; --i) {
      bits.push_back(temp_bits[i]);
    }
  }
  int ind = 0;

  int alph = GetNum(ind, bits);
  std::vector<bool> code;
  std::vector<std::vector<bool>> table(SIZE);
  for (int it = 0; it < alph; ++it) {
    int symb;
    for (int i = 0; i < 8 && ind < bits.size(); ++i, ++ind) {
      symb |= (bits[ind] << (8 - 1 - i));
    }
    int count = 0;
    while (ind < bits.size() && !bits[ind]) {
      ++count;
      ++ind;
    }
    ++ind;
    for (int i = 0; i < count && ind < bits.size(); ++i, ++ind) {
      table[symb].push_back(bits[ind]);
    }
  }
  while (ind < bits.size()) {
    code.push_back(bits[ind]);
    ++ind;
  }
  return;
}

int main() {
  std::cout << "Comands:\n1) Create an encoding table\n2) Encode the text\n3) "
               "Encode the text with table\n4) Decode the text\n";
  int comand;
  while (std::cin >> comand) {
    std::string file_out, file_in;
    std::cout << "Please write name of the file to getting data:\n";
    std::cin >> file_in;
    std::cout << "Please write name of the file to outputting data:\n";
    std::cin >> file_out;
    if (comand == 1) {
    } else if (comand == 2) {
    } else if (comand == 3) {
    } else if (comand == 4) {
      Decode(file_in, file_out);
    }
  }
  return 0;
}

// 0000011000100000011000000000011111000000001110010000000111111000000011100101000010000000000011100110000010001000000011100111000010010000000011101000000010011000000011101001000010100000000011101010000010101000000011101011000010110000000011101100000010111000000011101111000011000000000011110000000011001000000011110010000011010000000011110011000011011000000011110100000011100000000011110101000011101000000011111001000011110110111111111000101001110011010101101111010111110000101011001110111110111001010001110011
