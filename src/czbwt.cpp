#include "czbwt.h"

using namespace std;
using std::string;

tuple<int, string> encode(string s) {
  // s is the BWT block data string (original data)
  int n = s.length();
  // Bucket stores the link headers of (3)
  int bucket_size = 256 * 256 * 256;
  vector<int> link(n);
  vector<int> bucket(bucket_size, -1);

  // Phase 1: build links of (4)
  // TODO: Instead of creating substrings just convert them to int by indexing as follows:
  int j;
  string _3mer;
  for (int i=-2; i < n-2; i++) {
    if (i == -2) {
      _3mer = s.substr(n-2, n) + s.substr(0, 1);
    }

    else if (i == -1) {
      _3mer = s.substr(n-1, n) + s.substr(0, 2);
    }

    else {
      _3mer = s.substr(i, 3);
    }

    // TODO: Probably there is a better way to calculate j, maybe shifting instead of multiplication?
    j = _3mer[0] + _3mer[1] * 256 + _3mer[2] * 256 * 256;
    // DEBUG: cout << "For '" << _3mer << "' j is: " << j << endl;
    link[i+2] = bucket[j];
    bucket[j] = i+2;
  }

  // Count traces the start position of the block
  int count = 0;
  int start = 0;
  // Phase 2: Output data
  int i;
  stringstream output;
  for (int j = 0; j < bucket_size; j++) {
    i = bucket[j];
    while (i != -1) {
      // output a character of s
      int index = (n + (i + 1) % n) % n;
      // DEBUG: cout << "i: " << i << " index: " << index << " val: " << s[index] << endl;
      output << s[index];

      // start stores the start position
      if (i == (n-1)) {
        start = count;
      }

      i = link[i];
      count = count + 1;
    }
  }

  // DEBUG: cout << "Start: " << start << endl;
  // DEBUG: cout << "Output: " << output.str() << endl;
  return make_tuple(start, output.str());
}

string decode(int start, string s) {
  // s is the BWT block data string (BWT encoded data)
  // link stores the Column [0,7,6,5] of the decoding matrix. N is the length of string s.
  int n = s.length();
  vector<int> link(n);
  // bucket_A stores the data counters
  // bucket_B stores the data counters
  int bucket_a_size = 256 * 256 * 256;
  int bucket_b_size = 256 * 256;
  vector<int> bucket_A(bucket_a_size, 0);
  vector<int> bucket_B(bucket_b_size, 0);

  // Phase 1: count Column 0
  int j;
  for (int i=0; i < n; i++) {
    j = s[i];
    link[i] = j;
    bucket_A[j] = bucket_A[j] + 1;
  }

  int p = 0;  // p traces the current position of link
  int m;
  for (int i=0; i < 256; i++) {
    // Initialize the data counters
    j = bucket_A[i];
    bucket_A[i] = 0;
    while (j > 0) {
      m = (link[p] << 8) | i; // m stores Column [0,7]
      link[p] = m;  // Phase 1: sort Column 7
      p = p + 1;
      bucket_B[m] = bucket_B[m] + 1;  // Phase 2: count Column [0,7]
      j = j - 1;
    }
  }

  // reset p
  p = 0;
  for (int i=0; i < bucket_b_size; i++) {
    j = bucket_B[i];
    while(j > 0) {
      m = (link[p] << 8) | i; // m stores Column [0,7,6]
      link[p] = m;  // Phase 2: sort column 6
      p = p + 1;
      bucket_A[m] = bucket_A[m] + 1;  // Phase 3: count column [0,7,6]
      j = j - 1;
    }
  }

  p = 0;  // reset p
  m = 0;  // m traces the link headers
  // Phase 4: calculate link headers
  for (int i = 0; i < bucket_a_size; i++) {
    // bucket_A stores the link headers of (7)
    m = m + bucket_A[i];
    bucket_A[i] = m;
  }

  j = start;  // j traces the position
  // Phase 4: output decoded data
  for (int i = 0; i < n; i++) {
    p = link[j];  // link keeps Column [0,7,6] after phase 2
    s[i] = (p >> 16) & 255; // s stores the decoded block string in Column 0
    j = bucket_A[p] - 1;  // seek the next position j with Column [0,7,6]: fetch & decrease
    bucket_A[p] = j;  // update the current link header of (8)
  }

  return s;
}

int main(int argc, char const *argv[])
{
  string s = "DALSARKARKARTALKALKARKARTALKALKARDALSARKAR";
  cout << "Input: " << s << endl;
  tuple <int, string> encoded = encode(s);
  cout << "Encoded: " <<  get<1>(encoded) << endl;
  string decoded = decode(get<0>(encoded), get<1>(encoded));
  cout << "Decoded: " << decoded << endl;
  return 0;
}
