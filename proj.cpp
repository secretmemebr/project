#include <iostream>
#include <chrono>
#include <thread>
using namespace std;


// Function to calculate power under modulo
long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}


long long gcd(long long a, long long b) {
  if (b == 0) {
    return a;
  }
  return gcd(b, a % b);
}


long long d_calculation(int d,int z,int e) {

    while(d>0) {
        
        if((d*e)%z == 1) {
        
        return d;
        }

        else {

            d++;
        }
    }
    
}

long long e_calculation(int e, int z) {

    while(e>1 && e<z) {

        if(gcd(e,z) == 1) {

            return e;
        }

        else {
            e++;
        }
    }
}

// Caesar Cipher.


string encrypt(const string& plaintext, int shiftKey) {
    string encryptedText = "";

    for (char c : plaintext) {
        if (isalpha(c)) {
            char shifted = islower(c) ? 'a' + (c - 'a' + shiftKey) % 26 : 'A' + (c - 'A' + shiftKey) % 26;
            encryptedText += shifted;
        } else {
            encryptedText += c; // Keep non-alphabetic characters unchanged
        }
    }

    return encryptedText;
}

string decrypt(const string& ciphertext, int shiftKey) {
    string decryptedText = "";

    for (char c : ciphertext) {
        if (isalpha(c)) {
            char shifted = islower(c) ? 'a' + (c - 'a' - shiftKey + 26) % 26 : 'A' + (c - 'A' - shiftKey + 26) % 26;
            decryptedText += shifted;
        } else {
            decryptedText += c; // Keep non-alphabetic characters unchanged
        }
    }

    return decryptedText;
}

// Hill Cipher

// Function to calculate the determinant of a 2x2 matrix
int determinant(int key[2][2]) {
    return key[0][0] * key[1][1] - key[0][1] * key[1][0];
}

// Function to find the modular multiplicative inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Modular inverse doesn't exist
}

// Function to perform matrix multiplication
void matrixMultiply(int key[2][2], int text[], int result[], int textSize) {
    for (int i = 0; i < 2; ++i) {
        int sum = 0;
        for (int j = 0; j < textSize; ++j) {
            sum += key[i][j] * text[j];
        }
        result[i] = sum % 26; // Modulo 26 for the English alphabet
    }
}

// Function to encrypt plaintext using Hill Cipher
string hillCipherEncrypt(string plaintext, int keyMatrix[2][2]) {
    string cleanedText = "";
    for (char &c : plaintext) {
        if (isalpha(c)) {
            cleanedText += toupper(c);
        }
    }

    // Pad plaintext if its length is not a multiple of 2
    while (cleanedText.length() % 2 != 0) {
        cleanedText += 'X'; // Adding 'X' as padding
    }

    string ciphertext = "";
    int plainArray[cleanedText.length()]; // Array to hold numerical representations of plaintext characters

    // Map characters to numbers
    for (size_t i = 0; i < cleanedText.length(); ++i) {
        plainArray[i] = cleanedText[i] - 'A'; // Assuming A=0, B=1, ..., Z=25
    }

    // Perform matrix multiplication with the key matrix
    for (size_t i = 0; i < cleanedText.length(); i += 2) {
        int subset[2] = {plainArray[i], plainArray[i + 1]};
        int encryptedSubset[2];
        matrixMultiply(keyMatrix, subset, encryptedSubset, 2);

        // Convert the resulting numbers back to characters
        for (int num : encryptedSubset) {
            ciphertext += char(num + 'A'); // Convert back to characters
        }
    }

    return ciphertext;
}

// Function to decrypt ciphertext using Hill Cipher
string hillCipherDecrypt(string ciphertext, int keyMatrix[2][2]) {
    // Calculate the determinant of the key matrix
    int det = determinant(keyMatrix);
    det = (det % 26 + 26) % 26; // Ensure positive value (mod 26)

    // Find the modular multiplicative inverse of the determinant
    int detInverse = modInverse(det, 26);
    if (detInverse == -1) {
        return "Invalid key"; // Modular inverse doesn't exist
    }

    // Calculate the adjugate of the key matrix
    int temp = keyMatrix[0][0];
    keyMatrix[0][0] = keyMatrix[1][1];
    keyMatrix[1][1] = temp;
    keyMatrix[0][1] = -keyMatrix[0][1];
    keyMatrix[1][0] = -keyMatrix[1][0];

    // Apply modulo 26 to the adjugate matrix
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            keyMatrix[i][j] = (keyMatrix[i][j] % 26 + 26) % 26;
        }
    }

    // Multiply by the modular multiplicative inverse of the determinant
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            keyMatrix[i][j] = (keyMatrix[i][j] * detInverse) % 26;
        }
    }

    string decryptedText = "";
    int cipherArray[ciphertext.length()]; // Array to hold numerical representations of ciphertext characters

    // Map characters to numbers
    for (size_t i = 0; i < ciphertext.length(); ++i) {
        cipherArray[i] = ciphertext[i] - 'A'; // Assuming A=0, B=1, ..., Z=25
    }

    // Perform matrix multiplication with the inverse of the key matrix
    for (size_t i = 0; i < ciphertext.length(); i += 2) {
        int subset[2] = {cipherArray[i], cipherArray[i + 1]};
        int decryptedSubset[2];
        matrixMultiply(keyMatrix, subset, decryptedSubset, 2);

        // Convert the resulting numbers back to characters
        for (int num : decryptedSubset) {
            decryptedText += char(num + 'A'); // Convert back to characters
        }
    }

    // Find the index of the last non-'X' character in the decrypted text
    size_t lastNonX = decryptedText.find_last_not_of("X");
    if (lastNonX != string::npos) {
        decryptedText = decryptedText.substr(0, lastNonX + 1); // Trim excess 'X'
    }

    return decryptedText;
}





void loadingAnimation() {
  const int totalIterations = 22; 
  for (int i = 0; i < totalIterations; ++i) {
    cout << "\rLoading [";
    for (int j = 0; j <= i; ++j) {
      cout << "=";
    }
    
    cout << "] " << ((i + 1) * 100) / totalIterations << "%"; // Adjust percentage calculation
    cout.flush();
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  cout << endl;
}

int main() {

   int choice;
    cout << "Choose encryption method:" << endl;
    cout << "1. Hill Cipher" << endl;
    cout << "2. RSA" << endl;
    cout << "3. Caesar Cipher" << endl;
    cout << "4. Diffie-Hellman" << endl;
    cin >> choice;
    cin.ignore(); // Ignore newline character from the input buffer

    switch (choice) {
        case 1: {
            // Hill Cipher
            
          int keyMatrix[2][2];
    cout << "Enter the 2x2 key matrix for Hill cipher (each element separated by space):" << endl;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            cin >> keyMatrix[i][j];

    // Get the plaintext input from the user
    cin.ignore(); // Clear the newline character from the input buffer
    string plaintext;
    cout << "Enter the plaintext (upper and lower case letters and spaces only): ";
    getline(cin, plaintext);

    // Encrypt the plaintext using Hill cipher
    string ciphertext = hillCipherEncrypt(plaintext, keyMatrix);

    // Display the encrypted ciphertext
    cout << "Encrypted text: " << ciphertext << endl;

    // Decrypt the ciphertext using Hill cipher
    string decrypted = hillCipherDecrypt(ciphertext, keyMatrix);

    // Display the decrypted text
    cout << "Decrypted text: " << decrypted << endl;

            

            break;
        }
        case 2: {
            // RSA
           

  // Prime numbers
  long long p = 61; 
  long long q = 53; 

  // Modulus
  long long n = p * q;

  // Totient
  long long z = (p - 1) * (q - 1);

  // Public exponent
  long long e = 2;
  e = e_calculation(e,z);

  // Private exponent
  long long d = 1;
   d = d_calculation(d, z,e);

  // Message
  string text;
  cout << "Enter text to encrypt: ";
  getline(cin, text);

  cout << "Public key is (e,n) = (" << e << "," << n << ")" << endl;
  cout << "Private key is (d,n) = (" << d << "," << n << ")" << endl;

  cout << "\nEncryption and Decryption:" << endl;

  // Initialize arrays
  long long encryptedMsg[text.length()], decryptedMsg[text.length()];

  cout << "Message: " << text << endl;

  cout << "Processing...";
  loadingAnimation();
  cout << "Processing complete!" << endl;

  cout << "Encrypted Message: ";
  for (int i = 0; i < text.length(); ++i) {
    // Convert character to numerical value (A = 1, B = 2, ...)
    long long msg = text[i] - 'A' + 1;

    // Encrypt message
    encryptedMsg[i] = power(msg, e, n);
    
    // Print encrypted message
    cout << encryptedMsg[i] << " ";
  }
  cout << endl;

  cout << "Decrypted Message: ";
  for (int i = 0; i < text.length(); ++i) {
    // Decrypt message
    decryptedMsg[i] = power(encryptedMsg[i], d, n);

    // Convert numerical value back to character
    cout << char(decryptedMsg[i] - 1 + 'A');
  }
  cout << endl;

  break;

}
            
        
     case 3: {
            // Caesar Cipher

    int key;
    cout << "Enter the key for shifting characters: ";
    cin >> key;

    cin.ignore(); // Clear the newline character from the input buffer

    string inputText;
    cout << "Enter the text to encrypt: ";
    getline(cin, inputText);

    cout << "Processing...";
    loadingAnimation();
    cout << "Processing complete!" << endl;

    string encrypted = encrypt(inputText, key);

     
    cout << "Encrypted text: " << encrypted << endl;

    string decrypted = decrypt(encrypted, key);
    cout << "Decrypted text: " << decrypted << endl;
        
        break;
     }

    case 4: {

           long long Ps, Gs, p, g, q, h, K_A, K_B;

    // Accept prime number Ps and primitive root Gs from users
    cout << "Enter a prime number Ps: ";
    cin >> Ps;
    cout << "Enter a primitive root Gs: ";
    cin >> Gs;

    // Alen's private key g
    cout << "Enter Alen's private key g: ";
    cin >> g;

    // Generate the public key p
    p = power(Gs, g, Ps);

    // Roy's private key h
    cout << "Enter Roy's private key h: ";
    cin >> h;

    // Generate the public key q
    q = power(Gs, h, Ps);

    // Generating the secret keys after key exchange
    K_A = power(q, g, Ps); // Alen's Secret key
    K_B = power(p, h, Ps); // Roy's Secret key

    cout << "Processing...";
    loadingAnimation();
    cout << "Processing complete!" << endl;
    
    cout << "Alen's Secret key is: " << K_A << endl;
    cout << "Roy's Secret key is: " << K_B << endl;

        break;
    }
    } 

    return 0;

}
