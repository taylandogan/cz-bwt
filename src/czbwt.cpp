#include "czbwt.h"

using namespace std;
using std::string;

int encode(string s) {
  // s is the BWT block data string (original data)
  // n is the length of string s
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

  return 0;
}


int main(int argc, char const *argv[])
{
  string s = "XYZAACOL";
  encode(s);
  return 0;
}
