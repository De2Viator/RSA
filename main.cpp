#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cmath>
#include <cstring>
#include <random>

typedef std::map<unsigned long long, unsigned long long> PrimeNumbers;
struct ErtasophenInfo {
    unsigned long long * numbers;
    unsigned long long length;
};
unsigned long long euclide_algroythm(unsigned long long bigger_number, unsigned long long smaller_number) {
    while (bigger_number%smaller_number !=0) {
        unsigned long long remainder = smaller_number;
        smaller_number = bigger_number % smaller_number;
        bigger_number = remainder;
    }
    return smaller_number;
}
PrimeNumbers defactor_number(unsigned long long number) {
    PrimeNumbers prime_numbers;
    for(unsigned long long divisor = 2; divisor<=number; ++divisor) {
        while (number % divisor == 0) {
            number /= divisor;
            if(prime_numbers[divisor]) {
                ++prime_numbers[divisor];
            } else {
                prime_numbers[divisor] = 1;
            }
        }
    }
    return prime_numbers;
}

unsigned long long euler_function(const PrimeNumbers& prime_numbers) {
    unsigned long long prime_numbers_amount = 1;
    for (auto & prime_number : prime_numbers) {
        unsigned long long default_number = prime_number.first;
        unsigned long long exponent = prime_number.second;
        prime_numbers_amount *= pow(default_number, exponent)  - pow(default_number, exponent-1);
    }
    return prime_numbers_amount;
}

ErtasophenInfo eratosphen_sieve(unsigned long long number) {
    auto  * is_prime = new unsigned long long[number + 1];

    for (unsigned long long i = 0; i <= number; i++) {
        is_prime[i] = 1;
    }

    is_prime[0] = is_prime[1] = 1;
    for (unsigned long long i = 2; i * i <= number; i++) {
        if (is_prime[i]) {
            for (unsigned long long j = i * i; j <= number; j += i) {
                is_prime[j] = 0;
            }
            if(number % i == 0) {
                is_prime[i] = 0;
            }
        }
    }

    unsigned long long array_length = 0;

    for (unsigned long long i = 2; i <= number; i++) {
        if (is_prime[i]) {
            is_prime[array_length] = i;
            array_length++;
        }
    }
    return {is_prime, array_length};
}

unsigned long long mod_pow(unsigned long long base, unsigned long long exponent, unsigned long long modulus) {
    if (exponent == 0) {
        return 1;
    }
    if (exponent % 2 == 0) {
        unsigned long long temp = mod_pow(base, exponent / 2, modulus);
        return (temp * temp) % modulus;
    } else {
        return (base * mod_pow(base, exponent - 1, modulus)) % modulus;
    }
}
unsigned long long generate_private(unsigned long long public_number, unsigned long long module) {
    unsigned long long exponent = euler_function(defactor_number(module)) - 1;
    unsigned long long d = mod_pow(public_number, exponent, module);
    return d;
}

unsigned long long encrypt(unsigned long long public_key, unsigned long long message, unsigned long long module) {
    unsigned long long encrypted_message = mod_pow(message,public_key,module);
    return encrypted_message;
}

unsigned long long decrypt(unsigned long long cypher, unsigned long long private_key, unsigned long long module) {
    unsigned long long decrypted = mod_pow(cypher, private_key, module);
    return decrypted;
}

unsigned long long generate_random_number() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(3, 71);
        int random_number = distribution(gen);
        return random_number;
};

int main() {
    srand(time(NULL));
    unsigned long long p;
    unsigned long long q;
    while (euclide_algroythm(p,q) != 1) {
        p = generate_random_number();
        q = generate_random_number();
    }
    if(euclide_algroythm(p,q) != 1) {
        return -1;
    }
    unsigned long long n = p * q;
    unsigned long long coprime_amount = euler_function(defactor_number(n));
    ErtasophenInfo coprime_to_both = eratosphen_sieve(coprime_amount);
    unsigned long long e = coprime_to_both.numbers[rand() % coprime_to_both.length];
    unsigned long long d = generate_private(e,coprime_amount);
    const unsigned short MAX_SIZE = 100;
    char message_string[MAX_SIZE] = "";
    char encrypted_string[MAX_SIZE * 3] = "";
    char decrypted_string[MAX_SIZE] = "";
    std::cout << "Enter a message:";
    std::cin.getline(message_string, MAX_SIZE);
    encrypted_string[0] = '\0';

    for (unsigned int i = 0; message_string[i] != '\0'; ++i) {
        unsigned long long el = static_cast<unsigned long long>(message_string[i]);
        char asciiCode[4];
        unsigned long long encrypted_char = encrypt(e,el,n);
        snprintf(asciiCode, sizeof(asciiCode), "%llu", encrypted_char);
        strcat(encrypted_string, asciiCode);
        strcat(encrypted_string, " ");
    }

    std::cout<<"Encryption: "<<encrypted_string<<std::endl;
    char * token = strtok(encrypted_string, " ");
    while (token != NULL) {
        unsigned long long decrypted_value = decrypt(atoll(token), d, n);
        char character = static_cast<char>(decrypted_value);
        strncat(decrypted_string, &character, 1);
        token = strtok(NULL, " ");
    }
    std::cout<<"Decryption: "<<decrypted_string<<std::endl;
    return 0;
}
