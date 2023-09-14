#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

typedef long long lng;
typedef unsigned char uchar;

const int SIZE = 256;
void GammaCode(std::vector<bool>& vec, std::vector<bool>& vecprint);

struct Vertex {
  bool value;
  Vertex *left = nullptr, *right = nullptr;  // left = 0, right = 1;
  bool terminal = 0;
  uchar symbol;
};

bool cmp(std::pair<int, lng> a, std::pair<int, lng> b) {
  if (a.second < b.second) return true;
  return false;
}

// int digit(lng num, int ind) {
//   std::string s = std::to_string(num);
//   if (ind > s.length()) return 0;
//   --ind;
//   char c = s[s.length() - ind - 1];
//   return c - '0';
// }

// void RadixSort(std::vector<std::pair<int, lng>>& vec) {
//   int m = 0;
//   for (int i = 0; i < vec.size(); ++i) {
//     std::string temp = std::to_string(vec[i].second);
//     int size = temp.length();
//     m = std::max(m, size);
//   }
//   int k = 10;
//   for (int i = 1; i <= m; ++i){
//     std::vector<int>c(k, 0);
//     for (int j = 0; j < vec.size(); ++j){
//       int d = digit(vec[j].second, i);
//       ++c[d];
//     }
//     int count = 0;
//     for (int j = 0; j < k; ++j){
//       int temp = c[j];
//       c[j] = count;
//       count += temp;
//     }
//     std::vector<std::pair<int, lng>>b(vec.size());
//     for (int j = 0; j < vec.size(); ++j){
//       int d = digit(vec[i].second, i);
//       b[c[d]] = vec[i];
//       ++c[d];
//     }
//     vec = b;
//   }
//   return;
// }

int GetNum(uchar byte) {
  int num = 0;
  for (int i = 0; i < 8; ++i) {
    if (byte & (1 << i)) {
      num |= 1 << i;
    }
  }
  return num;
}

void GammaCode(std::vector<bool>& vec, std::vector<bool>& vecprint) {
  bool bit = 0;
  int prevsize = vecprint.size();
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
  while ((vecprint.size() - prevsize) % 8 != 0) {
    vecprint.push_back(0);
  }
  return;
}

void Number(int num, std::vector<bool>& vecprint) {
  for (int i = 7; i >= 0; --i) {
    vecprint.push_back(num & (1 << i));
  }
  return;
}

std::vector<uchar> GetText(std::string file_in) {
  std::vector<uchar> text;
  std::ifstream in(file_in, std::ios::binary);
  if (in.is_open()) {
    char symbol;
    while (in.get(symbol)) {
      text.push_back((uchar)symbol);
    }
  }
  return text;
}

void Add(Vertex*& v, bool value) {
  if (!value) {
    if (!v->left) v->left = new Vertex();
    v->left->value = 0;
    v = v->left;
  } else {
    if (!v->right) v->right = new Vertex();
    v->right->value = 1;
    v = v->right;
  }
  return;
}

std::vector<std::vector<bool>> CreateTable(std::vector<uchar>& text) {
  std::vector<std::pair<int, lng>> freq(SIZE, {0, 0});

  for (int i = 0; i < text.size(); ++i) {
    freq[(int)text[i]].first = (int)text[i];
    ++freq[(int)text[i]].second;
  }

  // RadixSort(freq);
  std::sort(freq.begin(), freq.end(), cmp);
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

  return table;
}

void Encode(std::string file_in, std::string file_out) {
  std::ifstream in(file_in, std::ios::binary);
  std::ofstream out(file_out, std::ios::binary);

  std::vector<uchar> text = GetText(file_in);
  std::vector<std::vector<bool>> table = CreateTable(text);
  std::vector<bool> vecprint;
  int alph = 0;
  for (int i = 0; i < table.size(); ++i) {
    if (table[i].size() != 0) ++alph;
  }

  Number(0, vecprint);
  Number(alph, vecprint);
  for (int i = 0; i < table.size(); ++i) {
    if (table[i].size() != 0) {
      Number((int)i, vecprint);
      GammaCode(table[i], vecprint);
    }
  }

  for (int i = 0; i < text.size(); ++i) {
    for (int j = table[text[i]].size() - 2; j >= 0; --j) {
      bool bit = table[text[i]][j];
      vecprint.push_back(bit);
    }
  }
  int md = vecprint.size() % 8;
  for (int i = 7; i >= 0; --i) {
    vecprint[i] = md & 1;
    md >>= 1;
  }

  uchar byte = 0;
  for (int i = 0; i < vecprint.size(); ++i) {
    std::cout << (int)vecprint[i];
  }
  for (int i = 0; i < vecprint.size(); ++i) {
    if (i % 8 == 0 && i > 0) {
      out << (char)byte;
      byte = 0;
    }
    if (vecprint[i]) byte |= (1 << (8 - i % 8 - 1));
  }
  if (byte != 0) out << (char)byte;

  return;
}

void Decode(std::string file_in, std::string file_out) {
  std::ifstream in(file_in, std::ios::binary);
  std::ofstream out(file_out);

  char byte;
  in.get(byte);
  int bitrem = GetNum((uchar)byte);
  in.get(byte);
  int alph = GetNum((uchar)byte);

  Vertex* root = new Vertex();
  for (int i = 0; i < alph; ++i) {
    in.get(byte);
    int symb = GetNum((uchar)byte);
    bool one = false;
    int zero = 0;
    int count = 0;
    int ind = 0;
    uchar cur = (uchar)byte;
    while (!one) {
      in.get(byte);
      cur = (uchar)byte;
      ++count;
      for (int i = 7; i >= 0; --i) {
        if (!(cur & (1 << i)))
          ++zero;
        else {
          one = true;
          ind = i;
          break;
        }
      }
    }
    // 0001110
    int size = ceil((zero * 2 + 1) / 8.0);
    auto v = root;
    --ind;
    while (ind != -1 && zero > 0) {
      Add(v, cur & (1 << ind));
      --ind;
      --zero;
    }
    while (count < size) {
      ++count;
      in.get(byte);
      cur = (uchar)byte;
      for (int i = 7; i >= 0 && zero > 0; --i) {
        Add(v, cur & (1 << i));
        --zero;
      }
    }
    v->terminal = true;
    v->symbol = symb;
  }

  std::cout << '\n';
  Vertex* v = root;
  Vertex* prev = nullptr;
  std::queue<bool> q;
  while (in.get(byte)) {
    uchar cur = (uchar)byte;
    while (!q.empty()) {
      bool val = q.front();
      q.pop();
      if (v->terminal) {
        std::cout << (uchar)v->symbol;
        out << (uchar)v->symbol;
        prev = v;
        v = root;
      }
      if (!val)
        v = v->left;
      else
        v = v->right;
    }
    for (int i = 0; i < bitrem; ++i) {
      bool val = cur & (1 << (7 - i));
      if (v->terminal) {
        std::cout << (uchar)v->symbol;
        out << (uchar)v->symbol;
        prev = v;
        v = root;
      }
      if (!val)
        v = v->left;
      else
        v = v->right;
    }
    for (int i = bitrem; i < 8; ++i) {
      q.push(cur & (1 << (7 - i)));
    }
  }
  if (prev == nullptr || (v != nullptr && prev != v && v->terminal)) {
    std::cout << (uchar)v->symbol;
    out << (uchar)v->symbol;
    v = root;
  }
  std::cout << '\n';
  return;
}

void PrintTable(std::vector<std::vector<bool>>& table) {
  int size = 0;
  for (int i = 0; i < table.size(); ++i) {
    if (table[i].size() != 0) {
      ++size;
      std::cout << (uchar)i << "(" << i << "): ";
      for (int j = table[i].size() - 1; j >= 0; --j) {
        std::cout << (int)table[i][j];
      }
      std::cout << '\n';
    }
  }
  std::cout << "SIZE = " << size << '\n';
  return;
}

int main() {
  std::cout
      << "\033[1;33mCommands:\n1) Create an encoding table\n2) Encode the "
         "text\n3) Decode the text\n\033[0m\n";
  int command;
  std::cout << "command: ";
  while (std::cin >> command) {
    std::string file_out, file_in;
    std::cout << "\n\033[1;33mPlease write name of the file to getting "
                 "data:\n\033[0m\n";
    std::cin >> file_in;
    if (command == 2 || command == 3) {
      std::cout << "\n\033[1;33mPlease write name of the file to outputting "
                   "data:\n\033[0m\n";
      std::cin >> file_out;
    }
    std::cout << "\033[1;32mDone!\033[0m";
    if (command == 1) {
      std::cout << '\n';
      std::vector<uchar> text = GetText(file_in);
      std::vector<std::vector<bool>> table = CreateTable(text);
      PrintTable(table);
    } else if (command == 2) {
      Encode(file_in, file_out);
    } else if (command == 3) {
      Decode(file_in, file_out);
    }
    std::cout << "\ncomand: ";
  }
  return 0;
}
