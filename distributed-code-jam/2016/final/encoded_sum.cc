#include <message.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using ll = long long;

void receive(int);
void send(int);
void put_ll(int, ll);
ll get_ll(int);
void put_int(int, int);
int get_int(int);
void put_char(int, char);
char get_char(int);
void put_vll(int, const std::vector<ll>&);
std::vector<ll> get_vll(int);
void put_vint(int, const std::vector<int>&);
std::vector<int> get_vint(int);
void put_str(int, const std::string &s);
std::string get_str(int);
void put_pll(int, const std::pair<ll, ll>&);
std::pair<ll, ll> get_pll(int);
void put_pii(int, const std::pair<int, int>&);
std::pair<int, int> get_pii(int);

// Algorithm Begin
#include "encoded_sum.h"

const int mod = 1e9 + 7;
int rk[10], cnt[10], as[10];

ll pow_mod(ll a, ll n) {
  ll r = 1;
  for (; n; n >>= 1) {
    if (n & 1) r = r * a % mod;
    a = a * a % mod;
  }
  return r;
}

void up(int x) {
  if (cnt[rk[x]] == 1) return;
  int t = rk[x];
  for (int i = 0; i < 10; ++i) {
    if (rk[i] >= t) ++rk[i];
  }
  memset(cnt, 0, sizeof(cnt));
  rk[x] = t;
  for (int i = 0; i < 10; ++i) cnt[rk[i]]++;
}

void up(int x, int y) {
  if (cnt[rk[x]] == 2) return;
  int t = rk[x];
  for (int i = 0; i < 10; ++i) {
    if (rk[i] >= t) ++rk[i];
  }
  memset(cnt, 0, sizeof(cnt));
  rk[x] = rk[y] = t;
  for (int i = 0; i < 10; ++i) cnt[rk[i]]++;
}

int main() {
  srand(23333);
  int nodes = NumberOfNodes();
  int me = MyNodeId();
  int n = GetLength();
  int L = 1ll * me * n / nodes, R = 1ll * (me + 1) * n / nodes;
  for (int i = 0; i < 10; ++i) {
    rk[i] = 0;
    cnt[i] = 0;
  }
  cnt[0] = 10;
  for (int i = L; i < R; ++i) {
    int a = GetScrollOne(i) - 'A';
    int b = GetScrollTwo(i) - 'A';
    if (a != b) {
      if (rk[a] != rk[b]) {
        up(a);
        up(b);
      } else {
        up(a, b);
      }
    } else {
      up(a);
    }
  }
  //printf("%d %d\n", L, R);
  for (int i = 0; i < 10; ++i) {
    put_int(0, rk[i]);
    //printf("%d ", rk[i]);
  }
  //puts("");
  send(0);
  if (!me) {
    std::vector<int> code[10];
    for (int node = 0; node < nodes; ++node) {
      receive(node);
      for (int i = 0; i < 10; ++i) {
        code[i].push_back(get_int(node));
      }
    }
    std::vector<int> id = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::sort(id.begin(), id.end(), [&](int x, int y) {
      return code[x] > code[y];
    });
    for (int i = 0; i < 10; ++i) as[id[i]] = i;
    for (int node = 1; node < nodes; ++node) {
      for (int i = 0; i < 10; ++i) {
        put_int(node, as[i]);
      }
      send(node);
    }
  } else {
    receive(0);
    for (int i = 0; i < 10; ++i) {
      as[i] = get_int(0);
    }
  }
  ll ret = 0, pw = pow_mod(10, n - R);
  for (int i = R - 1; i >= L; --i) {
    int a = GetScrollOne(i) - 'A';
    int b = GetScrollTwo(i) - 'A';
    ret += pw * (as[a] + as[b]) % mod;
    pw = pw * 10 % mod;
  }
  ret %= mod;
  if (me) {
    put_ll(0, ret);
    send(0);
  } else {
    for (int node = 1; node < nodes; ++node) {
      receive(node);
      ret += get_ll(node);
    }
    printf("%lld\n", ret % mod);
  }
  return 0;
}

// Algorithm End

void receive(int node) {
  Receive(node);
}

void send(int node) {
  Send(node);
}

void put_ll(int node, ll value) {
  PutLL(node, value);
}

ll get_ll(int node) {
  return GetLL(node);
}

void put_int(int node, int value) {
  PutInt(node, value);
}

int get_int(int node) {
  return GetInt(node);
}

void put_char(int node, char value) {
  PutChar(node, value);
}

char get_char(int node) {
  return GetChar(node);
}

void put_vll(int node, const std::vector<ll> &v) {
  put_int(node, v.size());
  for (auto &&e: v) put_ll(node, e);
}

std::vector<ll> get_vll(int node) {
  int n = get_int(node);
  std::vector<ll> res(n);
  for (int i = 0; i < n; ++i) res[i] = get_ll(node);
  return res;
}

void put_vint(int node, const std::vector<int> &v) {
  put_int(node, v.size());
  for (auto &&e: v) put_int(node, e);
}

std::vector<int> get_vint(int node) {
  int n = get_int(node);
  std::vector<int> res(n);
  for (int i = 0; i < n; ++i) res[i] = get_int(node);
  return res;
}

void put_str(int node, const std::string &s) {
  put_int(node, s.size());
  for (auto &&c: s) put_char(node, c);
}

std::string get_str(int node) {
  int n = get_int(node);
  std::string res(n, ' ');
  for (int i = 0; i < n; ++i) res[i] = get_char(node);
  return res;
}

void put_pll(int node, const std::pair<ll, ll> &p) {
  put_ll(node, p.first);
  put_ll(node, p.second);
}

std::pair<ll, ll> get_pll(int node) {
  std::pair<ll, ll> res;
  res.first = get_ll(node);
  res.second = get_ll(node);
  return res;
}

void put_pii(int node, const std::pair<int, int> &p) {
  put_int(node, p.first);
  put_int(node, p.second);
}

std::pair<int, int> get_pii(int node) {
  std::pair<int, int> res;
  res.first = get_int(node);
  res.second = get_int(node);
  return res;
}
