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

  // Phase 1: build links  of (4)
  int j;
  string _3mer;
  for (int i=0; i < n; i++) {
    if (i < 2) {
      _3mer = s.substr(n+i-2) + s.substr(0, i+1);
      // cout << "i: " << i << ", n+i-2: " << n+i-2 << endl;
      // cout << "First part: '" << s.substr(n+i-2) << "'" << endl;
      // cout << "Second part: '" << s.substr(0, i+1) << "'" << endl;
    }
    else {
      _3mer = s.substr(i-2, 3);
    }
    // Reverse string
    // std::swap(_3mer[0], _3mer[2]);
    cout << "_3mer: " << _3mer << endl;
    // j = _3mer[0] + _3mer[1] * 256 + _3mer[2] * 256 * 256;
    j = _3mer[0] * 256 * 256 + _3mer[1] * 256 + _3mer[2];
    cout << "For '" << _3mer << "' j is: " << j << endl;
    link[i] = bucket[j];
    // stringstream sa;
    // for (int z=0; z < n; z++) {
    //   sa << link[z];
    //   sa << ", ";
    // }
    // cout << sa.str() << endl;
    bucket[j] = i;
  }

  // Count traces the start position of the block
  int count = 0;
  int start = 0;
  // Phase 2: Output data
  int tmp;
  int index;
  int orig_index;
  stringstream output;
  for (int j = 0; j < bucket_size; j++) {
    tmp = bucket[j];
    while (tmp != -1) {
      // start stores the start position
      if (tmp == (n-1)) {
        cout << "TMP: " << tmp << " COUNT: " << count << endl;
        start = count;
      }
      cout << "j: " << j << " TMP: " << tmp << endl;
      // output a character of s
      orig_index = (tmp + 1) % n;
      index = (n + ((tmp - 3) % n)) % n;
      output << s[index];
      tmp = link[tmp];
      count += 1;
    }
  }
  // start = (n + ((start - 3)%n)) % n;
  cout << "Output: " << output.str() << endl;
  cout << "Start: " << start << endl;
  // cout << "My start: " << n - start << endl;

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

  // p traces the current position of link
  int p = 0;
  int m;
  for (int i=0; i < 256; i++) {
    // Initialize the data counters
    j = bucket_A[i];
    bucket_A[i] = 0;
    while (j > 0) {
      // m stores Column [0,7]
      m = (link[p] << 8) | i;
      // Phase 1: sort Column 7
      link[p] = m;
      p = p + 1;
      // Phase 2: count Column [0,7]
      bucket_B[m] = bucket_B[m] + 1;
      j = j - 1;
    }
  }

  // reset p
  p = 0;
  for (int i=0; i < bucket_b_size; i++) {
    j = bucket_B[i];
    while(j > 0) {
      // m stores Column [0,7,6]
      m = (link[p] << 8) | i;
      // Phase 2: sort column 6
      link[p] = m;
      p = p + 1;
      // Phase 3: count column [0,7,6]
      bucket_A[m] = bucket_A[m] + 1;
      j = j - 1;
    }
  }

  // reset p
  p = 0;
  // m traces the link headers
  m = 0;
  // Phase 4: calculate link headers
  for (int i = 0; i < bucket_a_size; i++) {
    // bucket_A stores the link headers of (7)
    m = m + bucket_A[i];
    bucket_A[i] = m;
  }

  // j traces the position
  j = start;
  // Phase 4: output decoded data
  for (int i = 0; i < n; i++) {
    // link keeps Column [0,7,6] after phase 2
    p = link[j];
    // s stores the decoded block string in Column 0
    s[i] = (p >> 16) & 255;
    // seek the next position j with Column [0,7,6]: fetch & decrease
    j = bucket_A[p] - 1;
    // update the current link header of (8)
    bucket_A[p] = j;
  }

  return s;
}

int main(int argc, char const *argv[])
{
  string s = "XYXYXCOL";
  cout << "Input: " << s << endl;
  tuple <int, string> encoded = encode(s);
  string decoded = decode(get<0>(encoded), get<1>(encoded));
  cout << "Decoded: " << decoded << endl;
  return 0;
}
