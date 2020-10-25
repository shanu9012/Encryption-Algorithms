#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<sstream>
#include<vector>
#include<bitset>

using namespace std;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

int binaryToDecimal(int binaryNumber)
{
    int i = 1, rem, dec = 0;

    while(binaryNumber > 0)
    {
        rem = binaryNumber % 2;
        dec = dec + rem * i;
        i = i * 2;
        binaryNumber = binaryNumber/10;
    }

    return dec;
}

string decimalToHex(int dec)
{
    int i=0, j;
    int hex[1000];
    string hexdecnum;

    while(dec != 0) 
    {
        hex[i] = dec % 16;
        dec = dec / 16;
        i++;
    }

    for (j=i-1; j>=0; j--)
    {
        if(hex[j] > 9)
            hexdecnum += (char)(hex[j] + 55);
        else
            hexdecnum += (char)(hex[j] + 48);
    }

    return hexdecnum;
}

string binaryToHex(string messageBinary)
{
    if(messageBinary.length() % 4 != 0)
        cout << "Message lenght is not the multiple of 4.\n";

    
    int binaryNumber, count=0;
    char temps[4];
    string hexdecnum;
    
    while(count < messageBinary.length())
    {
        int y = 0;
        for(int z=count; z<count+4; z++)
            temps[y++] = messageBinary[z];

        binaryNumber = atoi(temps);
        if(binaryNumber == 0)
        {
            hexdecnum += '0';
            count += 4;
            continue;
        }

        int dec = binaryToDecimal(binaryNumber);
        
        string hex = decimalToHex(dec);
        hexdecnum += hex;

        count += 4;
    }

    return hexdecnum;    
}

string decimalToBinary(int n)
{
    int binaryNum[128];
 
    int i = 0;
    while(n>0)
    {
        binaryNum[i] = n%2;
        n = n/2;
        i++;
    }
 
    string binaryString;
    for (int j = i - 1; j >= 0; j--)
        binaryString += patch::to_string(binaryNum[j]);

    return binaryString;
}

string paddedMessage(string message)
{
    int ml = message.length() + 128;
    int templ = message.length();
    int paddedBits = 1024;
    if(ml % 1024 != 0)
    {
        int temp = ml % 1024;
        paddedBits = 1024 - temp;
    }

    for(int i=0; i<paddedBits; i++)
    {
        if(i==0)
            message += '1';
        else
            message += '0';
    }

    string length_in_binary = decimalToBinary(templ);

    for(int i = 128 - length_in_binary.length(); i>=1; i--)
        message += '0';

    return message + length_in_binary;
}

string circular_right_shift(string m, int s)
{
    int i,j;

    for(i=0; i<s; i++)
    {
        char temp = m[63];   // save LSB

        for(j=63; j>0; j--)
            m[j] = m[j-1];
        m[0] = temp;
    }

    return m;
}

string right_shift(string m, int s)
{
    for(int i=0; i<s; i++)
    {
        for(int j=63; j>0; j--)
            m[j] = m[j-1];
        m[0] = '0';
    }

    return m;
}

string take_xor(string binaryNumber1, string binaryNumber2)
{
    string xored;
    for(int i=0; i<binaryNumber1.length(); i++)
        xored += (binaryNumber1[i]-'0') ^ (binaryNumber2[i]-'0') + '0';

    return xored;
}

string addBinary(string a, string b)
{
    string result;
    int s = 0;
 
    int i = a.size() - 1;
    int j = b.size() - 1;

    while(i>=0 || j>=0)
    {
        // Comput sum of last digits and carry
        s += ((i >= 0)? a[i] - '0': 0);
        s += ((j >= 0)? b[j] - '0': 0);
 
        // If current digit sum is 1 or 3, add 1 to result
        result = char(s % 2 + '0') + result;
 
        // Compute carry
        s /= 2;
 
        // Move to next digits
        i--; j--;
    }
    //cout << result << endl << endl;
    return result;
}

vector<string> generateMessageSchedule(string messageBinary)
{
    int i,j;
    string temp;
    vector<string> W(80);

    j=0;
    for(i=0; i<1024; i+=64)
        W[j++] = messageBinary.substr(i, 64);

    for(i=16; i<80; i++)
    {
        string ROTR_1 = circular_right_shift(W[i-15], 1);
        string SHR_6 = right_shift(W[i-2], 6);
        string SHR_7 = right_shift(W[i-15], 7);
        string ROTR_8 = circular_right_shift(W[i-15], 8);
        string ROTR_19 = circular_right_shift(W[i-2], 19);
        string ROTR_61 = circular_right_shift(W[i-2], 61);

        string sigma_512_0 = take_xor(ROTR_1, take_xor(ROTR_8, SHR_7));
        string sigma_512_1 = take_xor(ROTR_19, take_xor(ROTR_61, SHR_6));

        W[i] = addBinary(sigma_512_1, addBinary(W[i-7], addBinary(sigma_512_0, W[i-16])));
    }

    return W;
}

string AND(string a, string b)
{
    string andValue;

    for(int i=0; i<a.length(); i++)
        andValue += ((a[i]-'0') & (b[i]-'0')) + '0';

    return andValue; 
}

string charToBinary(string m)
{
    string binary;

    for (size_t i=0; i<m.size(); i++)
        binary += patch::to_string(bitset<8>(m.c_str()[i]));

    return binary;
    
}

string hexToBinary(string a)
{
    string binary;
    for(int i=0; i!=a.length(); i++)
    {
        switch(toupper(a[i]))
        {
            case '0': binary += "0000"; break; 
            case '1': binary += "0001"; break;
            case '2': binary += "0010"; break;
            case '3': binary += "0011"; break;
            case '4': binary += "0100"; break;
            case '5': binary += "0101"; break;
            case '6': binary += "0110"; break;
            case '7': binary += "0111"; break;
            case '8': binary += "1000"; break;
            case '9': binary += "1001"; break;
            case 'A': binary += "1010"; break;
            case 'B': binary += "1011"; break;
            case 'C': binary += "1100"; break;
            case 'D': binary += "1101"; break;
            case 'E': binary += "1110"; break;
            case 'F': binary += "1111"; break;
        }
    }
    
    return binary;
}

string round_function(string messageBinary)
{
    string K[80] = {"428a2f98d728ae22", "7137449123ef65cd", "b5c0fbcfec4d3b2f", "e9b5dba58189dbbc",
                    "3956c25bf348b538", "59f111f1b605d019", "923f82a4af194f9b", "ab1c5ed5da6d8118",
                    "d807aa98a3030242", "12835b0145706fbe", "243185be4ee4b28c", "550c7dc3d5ffb4e2",
                    "72be5d74f27b896f", "80deb1fe3b1696b1", "9bdc06a725c71235", "c19bf174cf692694",
                    "e49b69c19ef14ad2", "efbe4786384f25e3", "0fc19dc68b8cd5b5", "240ca1cc77ac9c65",
                    "2de92c6f592b0275", "4a7484aa6ea6e483", "5cb0a9dcbd41fbd4", "76f988da831153b5",
                    "983e5152ee66dfab", "a831c66d2db43210", "b00327c898fb213f", "bf597fc7beef0ee4",
                    "c6e00bf33da88fc2", "d5a79147930aa725", "06ca6351e003826f", "142929670a0e6e70",
                    "27b70a8546d22ffc", "2e1b21385c26c926", "4d2c6dfc5ac42aed", "53380d139d95b3df",
                    "650a73548baf63de", "766a0abb3c77b2a8", "81c2c92e47edaee6", "92722c851482353b",
                    "a2bfe8a14cf10364", "a81a664bbc423001", "c24b8b70d0f89791", "c76c51a30654be30",
                    "d192e819d6ef5218", "d69906245565a910", "f40e35855771202a", "106aa07032bbd1b8",
                    "19a4c116b8d2d0c8", "1e376c085141ab53", "2748774cdf8eeb99", "34b0bcb5e19b48a8",
                    "391c0cb3c5c95a63", "4ed8aa4ae3418acb", "5b9cca4f7763e373", "682e6ff3d6b2b8a3",
                    "748f82ee5defb2fc", "78a5636f43172f60", "84c87814a1f0ab72", "8cc702081a6439ec",
                    "90befffa23631e28", "a4506cebde82bde9", "bef9a3f7b2c67915", "c67178f2e372532b",
                    "ca273eceea26619c", "d186b8c721c0c207", "eada7dd6cde0eb1e", "f57d4f7fee6ed178",
                    "06f067aa72176fba", "0a637dc5a2c898a6", "113f9804bef90dae", "1b710b35131c471b",
                    "28db77f523047d84", "32caab7b40c72493", "3c9ebe0a15c9bebc", "431d67c49c100d4c",
                    "4cc5d4becb3e42b6", "597f299cfc657e2a", "5fcb6fab3ad6faec", "6c44198c4a475817" };

    string a, b, c, d, e, f, g, h;
    string h0, h1, h2, h3, h4, h5, h6, h7;

    h0 = hexToBinary("6a09e667f3bcc908"), h1 = hexToBinary("bb67ae8584caa73b");
    h2 = hexToBinary("3c6ef372fe94f82b"), h3 = hexToBinary("a54ff53a5f1d36f1");
    h4 = hexToBinary("510e527fade682d1"), h5 = hexToBinary("9b05688c2b3e6c1f");
    h6 = hexToBinary("1f83d9abfb41bd6b"), h7 = hexToBinary("5be0cd19137e2179");

    for(int i=0; i<80; i++)
        K[i] = hexToBinary(K[i]);

    string e_AND_f, NOT_e_AND_g, ch_e_f_g, a_AND_b, a_AND_c, b_AND_c, maj_a_b_c, T1, T2;
    string ROTR_28, ROTR_34, ROTR_39, ROTR_14, ROTR_18, ROTR_41, sigma_512_0, sigma_512_1;

    int messageBlocks = 0;
    string currentMessage;
    vector<string> W(80);

    while(messageBlocks < messageBinary.length())
    {
        currentMessage = messageBinary.substr(messageBlocks, 1024);
        
        W = generateMessageSchedule(currentMessage);
        
        // Initialize working variables to current hash value:
        a = h0, b = h1, c = h2, d = h3, e = h4, f = h5, g = h6, h = h7;

        for(int i=0; i<80; i++)
        {
            e_AND_f = AND(e, f);
            
            string NOT_e;
            for(int j=0; j<e.length(); j++)
                NOT_e += !(e[j]-'0') + '0';

            NOT_e_AND_g = AND(NOT_e, g);
            ch_e_f_g = take_xor(e_AND_f, NOT_e_AND_g);

            a_AND_b = AND(a, b);
            a_AND_c = AND(a, c);
            b_AND_c = AND(b, c);
            maj_a_b_c = take_xor(take_xor(a_AND_b, a_AND_c), b_AND_c);

            ROTR_14 = circular_right_shift(e, 14);
            ROTR_18 = circular_right_shift(e, 18);
            ROTR_28 = circular_right_shift(a, 28);
            ROTR_34 = circular_right_shift(a, 34);
            ROTR_39 = circular_right_shift(a, 39);
            ROTR_41 = circular_right_shift(e, 41);

            sigma_512_0 = take_xor(take_xor(ROTR_28, ROTR_34), ROTR_39);
            sigma_512_1 = take_xor(take_xor(ROTR_14, ROTR_18), ROTR_41);

            T1 = addBinary(addBinary(addBinary(addBinary(h, ch_e_f_g), sigma_512_1), W[i]), K[i]);
            T2 = addBinary(sigma_512_0, maj_a_b_c);

            h = g;
            g = f;
            f = e;
            e = addBinary(d, T1);
            d = c;
            c = b;
            b = a;
            a = addBinary(T1, T2);
        }

        h0 = addBinary(h0, a), h4 = addBinary(h4, e);
        h1 = addBinary(h1, b), h5 = addBinary(h5, f);
        h2 = addBinary(h2, c), h6 = addBinary(h6, g);
        h3 = addBinary(h3, d), h7 = addBinary(h7, h);

        messageBlocks += 1024;
    }

    return h0 + h1 + h2 + h3 + h4 + h5 + h6 + h7;
}


int main()
{
    ifstream inFile;

    string message;
    char x;

    inFile.open("message.txt");
    if (!inFile)
    {
        cout << "Unable to open file";
        exit(1);
    }

    while(inFile >> x)
        message += x;

    inFile.close();

    string messageBinary = charToBinary(message);

    string messageBinaryPadded = paddedMessage(messageBinary);
    //cout << messageBinaryPadded.length() << endl;

    string messageHex = binaryToHex(messageBinaryPadded);
   
    string message_digest_binary = round_function(messageBinaryPadded);

    cout << "Message Hash : \n" << binaryToHex(message_digest_binary) << endl;

    return 0;
}
