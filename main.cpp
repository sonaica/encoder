#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int SIZE = 256;

// color text

struct Vertex {
  bool value;
  Vertex *left = nullptr, *right = nullptr;  // left = 0, right = 1;
  bool terminal = 0;
  char symbol;
};

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

int GetNumConst(int& ind, std::vector<bool>& bits) {
  int num = 0;
  for (int i = 0; i < 8; ++i, ++ind) {
    num |= (bits[ind] << (8 - i - 1));
  }
  return num;
}

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
    for (int i = 0; i < 8; ++i) {
      bits.push_back(byte & (1 << (8 - i - 1)));
    }
  }
  int ind = 0;

  int bitrem = GetNumConst(ind, bits);
  int alph = GetNum(ind, bits);

  Vertex* root = new Vertex();
  for (int it = 0; it < alph; ++it) {
    int symb = GetNumConst(ind, bits);

    int count = 0;
    while (ind < bits.size() && !bits[ind]) {
      ++count;
      ++ind;
    }
    ++ind;
    auto v = root;
    for (int i = 0; i < count && ind < bits.size(); ++i, ++ind) {
      if (!bits[ind]) {
        if (!v->left) v->left = new Vertex();
        v->left->value = 0;
        v = v->left;
      } else {
        if (!v->right) v->right = new Vertex();
        v->right->value = 1;
        v = v->right;
      }
    }
    v->terminal = true;
    v->symbol = symb;
  }
  Vertex* v = root;
  // 312 115 1011
  // 316 100 11111
  // 321 102 0001
  // 325 103 1001
  // 329 104 1001
  // 333 106 0101
  // 337 107 1101
  // 341 108 1101
  // 345 32 11111
  // 350 101 0001
  // 354 114 1011
  // 358 116 0111
  // 362 121 1111
  // 366 117 0111
  // 370 105 0101
  // 374 111 0011
  // 378 112 0011
  while (ind < bits.size() - (8 - bitrem) % 8) {
    while (!v->terminal) {
      if (!bits[ind])
        v = v->left;
      else
        v = v->right;
      ++ind;
    }
    std::cout << (char)v->symbol;
    v = root;
  }
  return;
}

int main() {
  std::cout << "\033[1;33mComands:\n1) Create an encoding table\n2) Encode the "
               "text\n3) "
               "Encode the text with table\n4) Decode the text\n\033[0m\n";
  int comand;
  while (std::cin >> comand) {
    std::string file_out, file_in;
    std::cout << "\n\033[1;33mPlease write name of the file to getting "
                 "data:\n\033[0m\n";
    std::cin >> file_in;
    std::cout << "\n\033[1;33mPlease write name of the file to outputting "
                 "data:\n\033[0m\n";
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
// 32 00000111110
// 100 00000111111
// 101 000010000
// 102 000010001
// 103 000010010
// 104 000010011
// 105 000010100
// 106 000010101
// 107 000010110
// 108 000010111
// 111 000011000
// 112 000011001
// 114 000011010
// 115 000011011
// 116 000011100
// 117 000011101
// 121 000011110
