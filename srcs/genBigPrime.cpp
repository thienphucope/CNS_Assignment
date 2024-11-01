// 64 bits is maximum you can get in c++ so it's implemented to do so
// you can edit constexpr var in getRandom64() to get lower amount of bits
#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <bitset>

uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m) {
	int64_t res = 0;

	while (a != 0) {
		if (a & 1) {

			res = (res + b) % m;
		}
		a >>= 1;
		b = (b << 1) % m;
	}
	return res;
}

uint64_t powMod(uint64_t a, uint64_t b, uint64_t n) {
	uint64_t x = 1;
	
	a %= n;

	while (b > 0) {
		if (b % 2 == 1) {
			x = mulmod(x, a, n); // multiplying with base
		}
		a = mulmod(a, a, n); // squaring the base
		b >>= 1;
	}
	return x % n;
}

std::vector<int> first_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
									31, 37, 41, 43, 47, 53, 59, 61, 67,
									71, 73, 79, 83, 89, 97, 101, 103,
									107, 109, 113, 127, 131, 137, 139,
									149, 151, 157, 163, 167, 173, 179,
									181, 191, 193, 197, 199, 211, 223,
									227, 229, 233, 239, 241, 251, 257,
									263, 269, 271, 277, 281, 283, 293,
									307, 311, 313, 317, 331, 337, 347, 349 };

// going through all 64 bits and placing randomly 0s and 1s
// setting first and last bit to 1 to get 64 odd number 
uint64_t getRandom64() {
	// the value need to be 63 bits because you can not using 64 bit values do a^2 which is needed
	constexpr int bits = 5; 
	std::bitset<bits> a;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<short> distr(0, 1);

	for (int i = 0; i < bits; i++) {
		a[i] = distr(gen);
	}

	a[0] = 1;
	a[bits - 1] = 1;

	return a.to_ullong();
}

uint64_t getLowLevelPrime() {
	while (true) {
		uint64_t candidate = getRandom64();
		bool is_prime = true;
		for (int i = 0; i < first_primes.size(); i++) {
			if (candidate == first_primes[i])
				return candidate;

			if (candidate % first_primes[i] == 0) {
				is_prime = false;
				break;
			}
		}
		if (is_prime)
			return candidate;
	}
}

bool trialComposite(uint64_t a, uint64_t evenC, uint64_t to_test, int max_div_2) {
	if (powMod(a, evenC, to_test) == 1)
		return false;

	for (int i = 0; i < max_div_2; i++) {
		uint64_t temp = static_cast<uint64_t>(1) << i;
		if (powMod(a, temp * evenC, to_test) == to_test - 1)
			return false;
	}

	

	return true;
}

bool MillerRabinTest(uint64_t to_test) {
	constexpr int accuracy = 20;

	int max_div_2 = 0;
	uint64_t evenC = to_test - 1;
	while (evenC % 2 == 0) {
		evenC >>= 1;
		max_div_2++;
	}

	// random numbers init
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint64_t> distr(2, to_test);

	for (int i = 0; i < accuracy; i++) {
		uint64_t a = distr(gen);

		if (trialComposite(a, evenC, to_test, max_div_2)) {
			return false;
		}
	}

	return true;
}

uint64_t getBigPrime() {
	while (true) {
		uint64_t candidate = getLowLevelPrime();
		if (MillerRabinTest(candidate))
			return candidate;
	}
}

int main(){
	std::cout<<getBigPrime()<<std::endl; 
}
