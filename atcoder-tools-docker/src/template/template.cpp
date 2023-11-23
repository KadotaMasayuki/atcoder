#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;

// debug print
//   dprint(); dprint(a); dprint(a, b); dprint(a, b, c); dprint(std::vector);
#define DEBUGPRINT 1
#ifdef DEBUGPRINT
#define dprint(...) debug_print(__VA_ARGS__)
void debug_print() { cout << endl; }
template <class Head, class... Body>
void debug_print(Head head, Body... body)
{
    cout << head;
    if (sizeof...(body) > 0) cout << ", ";
    debug_print(body...);
}
template <class T>
void debug_print(vector<T> vec)
{
  for (long long i = 0; i < vec.size() - 1; i ++) cout << vec.at(i) << ", ";
  cout << vec.at(vec.size() - 1) << endl;
}
#else
#define debug(...) ;
#endif

int main(){
  // set precision (10 digit)
  cout << setprecision(10);
  
  cout << "Yes" << endl;
}

