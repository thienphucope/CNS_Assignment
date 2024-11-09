#include "main.hpp"

ZZ InverseMod(ZZ a, ZZ m);
ZZ power_mod(ZZ base, ZZ exp, ZZ mod);
bool MillerRabinTest(ZZ to_test);
ZZ random_prime(int bit_length);
ZZ gcd(ZZ a, ZZ b);
void generate_keypair(ZZ& n, ZZ& e, ZZ& d);
ZZ encrypt(const ZZ& m, const ZZ& e, const ZZ& n);
ZZ decrypt(const ZZ& c, const ZZ& d, const ZZ& n);
