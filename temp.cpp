#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int SIZE = 256;

typedef long long lng;
std::ifstream in("in.txt", std::ios::binary);
std::ofstream out("out.txt", std::ios::binary);

void GammaCode(int num, std::vector<bool>& vecprint);
void GammaCode(std::vector<bool>& vec, std::vector<bool>& vecprint);

void GammaCode(int num, std::vector<bool>& vecprint) {
  std::vector<bool> vec;
  while (num != 0) {
    vec.push_back(num & 1);
    num >>= 1;
  }
  GammaCode(vec, vecprint);
  return;
}

void GammaCode(std::vector<bool>& vec, std::vector<bool>& vecprint) {
  bool bit = 0;
  vec.push_back(1);
  for (int i = 0; i < vec.size() - 1; ++i) {
    vecprint.push_back(bit);
  }
  for (int i = vec.size() - 1; i >= 0; --i) {
    if (vec[i])
      bit = 1;
    else
      bit = 0;
    vecprint.push_back(bit);
  }
  return;
}

void CreateTable(std::string file_name) {}

void Encode(std::string file_name) {}

void Decode(std::string file_name) {}

int main() {
  int alph = 0;
  std::vector<std::pair<int, lng>> freq(SIZE, {0, 0});
  std::vector<int> text;
  std::vector<bool> vecprint;

  if (in.is_open()) {
    char symbol;
    while (in.read((char*)&symbol, sizeof(char))) {
      if (freq[symbol].second == 0) ++alph;
      ++freq[symbol].second;
      freq[symbol].first = symbol;
      text.push_back(symbol);
    }
  }

  std::sort(freq.begin(), freq.end());
  std::vector<std::vector<bool>> table(SIZE);
  std::vector<std::pair<std::vector<int>, lng>> temp_vec;
  int it1 = 0, it2 = 0;
  while (freq[it1].second == 0) ++it1;

  while ((freq.size() - it1) + (temp_vec.size() - it2) >= 2 &&
         (it1 < freq.size() || it2 < temp_vec.size())) {
    std::vector<std::pair<int, int>> cur;
    while (it1 < freq.size() && it2 < temp_vec.size() && cur.size() < 2) {
      if (freq[it1].second <= temp_vec[it2].second) {
        cur.push_back({it1, 1});
        ++it1;
      } else {
        cur.push_back({it2, 2});
        ++it2;
      }
    }
    while (it1 < freq.size() && cur.size() < 2) {
      cur.push_back({it1, 1});
      ++it1;
    }
    while (it2 < temp_vec.size() && cur.size() < 2) {
      cur.push_back({it2, 2});
      ++it2;
    }

    std::vector<int> new_vec;
    lng sum = 0;
    for (int i = 0; i < cur.size(); ++i) {
      int ind = cur[i].first;
      if (cur[i].second == 1) {
        new_vec.push_back(freq[ind].first);
        sum += freq[ind].second;
        if (i % 2 == 0)
          table[freq[ind].first].push_back(0);
        else
          table[freq[ind].first].push_back(1);
      } else {
        for (int j = 0; j < temp_vec[ind].first.size(); ++j) {
          new_vec.push_back(temp_vec[ind].first[j]);
          if (i % 2 == 0)
            table[temp_vec[ind].first[j]].push_back(0);
          else
            table[temp_vec[ind].first[j]].push_back(1);
        }
        sum += temp_vec[ind].second;
      }
    }
    temp_vec.push_back({new_vec, sum});
  }

  for (int i = 0; i < 8; ++i) {
    vecprint.push_back(0);
  }
  GammaCode(alph, vecprint);
  for (int i = 0; i < table.size(); ++i) {
    if (table[i].size() != 0) {
      std::vector<bool> temp;
      int symb = i;
      for (int j = 0; j < 8; ++j) {
        temp.push_back(symb & 1);
        symb >>= 1;
      }
      for (int j = temp.size() - 1; j >= 0; --j) {
        vecprint.push_back(temp[i]);
      }
      GammaCode(table[i], vecprint);
    }
  }
  int md = vecprint.size() % 8;
  for (int i = 7; i >= 0; --i) {
    vecprint[i] = md & 1;
    md >>= 1;
  }

  for (int i = 0; i < text.size(); ++i) {
    for (int j = 0; j < table[text[i]].size(); ++j) {
      bool bit = table[text[i]][j];
      vecprint.push_back(bit);
    }
  }

  uint8_t byte = 0;

  for (int i = 0; i < vecprint.size(); ++i) {
    if (i % 8 == 0 && i > 0) {
      out.write((char*)&byte, sizeof(byte));
      byte = 0;
    }
    if (vecprint[i]) byte |= (1 << (8 - i % 8 - 1));
    std::cout << (int)vecprint[i];
  }
  if (byte != 0) out.write((char*)&byte, 1);

  return 0;
}
