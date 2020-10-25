#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<sstream>
#include<vector>
#include<bitset>

#include"NumberConversion.cpp"
#include"BitwiseOperation.cpp"

using namespace std;

class SHA_512
{
    private:

    string messageChar, messageBinary, messageBinaryPadded;
    string messageHex, message_digest_binary;
   
    public:

    SHA_512(string m)
    {
        NumberConversion nc;
        messageChar = m;
        messageBinary = nc.charToBinary(messageChar);
        messageBinaryPadded = padMessage();
        messageHex = nc.binaryToHex(messageBinaryPadded);
    }

    string padMessage()
    {
        string message = messageBinary;
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

        NumberConversion nc;
        string length_in_binary = nc.decimalToBinary(templ);

        for(int i = 128 - length_in_binary.length(); i>=1; i--)
            message += '0';

        return message + length_in_binary;
    }

    vector<string> generateMessageSchedule(string messageBinary)
    {
        int i,j;
        string temp;
        vector<string> W(80);
        BitwiseOperation bw;

        j=0;
        for(i=0; i<1024; i+=64)
            W[j++] = messageBinary.substr(i, 64);

        for(i=16; i<80; i++)
        {
            string ROTR_1 = bw.circular_right_shift(W[i-15], 1);
            string SHR_6 = bw.right_shift(W[i-2], 6);
            string SHR_7 = bw.right_shift(W[i-15], 7);
            string ROTR_8 = bw.circular_right_shift(W[i-15], 8);
            string ROTR_19 = bw.circular_right_shift(W[i-2], 19);
            string ROTR_61 = bw.circular_right_shift(W[i-2], 61);

            string sigma_512_0 = bw.take_xor(ROTR_1, bw.take_xor(ROTR_8, SHR_7));
            string sigma_512_1 = bw.take_xor(ROTR_19, bw.take_xor(ROTR_61, SHR_6));

            W[i] = bw.addBinary(sigma_512_1, bw.addBinary(W[i-7], bw.addBinary(sigma_512_0, W[i-16])));
        }

        return W;
    }

    void round_function()
    {
        string K[80];
        ifstream inFile;
        inFile.open("Additive Constants.txt");
        if(!inFile)
        {
            cout << "Unable to open file";
            exit(1);
        }

        int t = 0;
        string x;
        while(inFile >> x)
            K[t++] = x;

        inFile.close();

        NumberConversion nc;
        BitwiseOperation bw;

        string a, b, c, d, e, f, g, h;
        string h0, h1, h2, h3, h4, h5, h6, h7;

        h0 = nc.hexToBinary("6a09e667f3bcc908"), h1 = nc.hexToBinary("bb67ae8584caa73b");
        h2 = nc.hexToBinary("3c6ef372fe94f82b"), h3 = nc.hexToBinary("a54ff53a5f1d36f1");
        h4 = nc.hexToBinary("510e527fade682d1"), h5 = nc.hexToBinary("9b05688c2b3e6c1f");
        h6 = nc.hexToBinary("1f83d9abfb41bd6b"), h7 = nc.hexToBinary("5be0cd19137e2179");

        for(int i=0; i<80; i++)
            K[i] = nc.hexToBinary(K[i]);

        string e_AND_f, NOT_e_AND_g, ch_e_f_g, a_AND_b, a_AND_c, b_AND_c, maj_a_b_c, T1, T2;
        string ROTR_28, ROTR_34, ROTR_39, ROTR_14, ROTR_18, ROTR_41, sigma_512_0, sigma_512_1;

        int messageBlocks = 0;
        string currentMessage;
        vector<string> W(80);

        while(messageBlocks < messageBinaryPadded.length())
        {
            currentMessage = messageBinaryPadded.substr(messageBlocks, 1024);
            
            W = generateMessageSchedule(currentMessage);
           
            // Initialize working variables to current hash value:
            a = h0, b = h1, c = h2, d = h3, e = h4, f = h5, g = h6, h = h7;

            for(int i=0; i<80; i++)
            {
                e_AND_f = bw.AND(e, f);
                
                string NOT_e;
                for(int j=0; j<e.length(); j++)
                    NOT_e += !(e[j]-'0') + '0';

                NOT_e_AND_g = bw.AND(NOT_e, g);
                ch_e_f_g = bw.take_xor(e_AND_f, NOT_e_AND_g);

                a_AND_b = bw.AND(a, b);
                a_AND_c = bw.AND(a, c);
                b_AND_c = bw.AND(b, c);
                maj_a_b_c = bw.take_xor(bw.take_xor(a_AND_b, a_AND_c), b_AND_c);

                ROTR_14 = bw.circular_right_shift(e, 14);
                ROTR_18 = bw.circular_right_shift(e, 18);
                ROTR_28 = bw.circular_right_shift(a, 28);
                ROTR_34 = bw.circular_right_shift(a, 34);
                ROTR_39 = bw.circular_right_shift(a, 39);
                ROTR_41 = bw.circular_right_shift(e, 41);

                sigma_512_0 = bw.take_xor(bw.take_xor(ROTR_28, ROTR_34), ROTR_39);
                sigma_512_1 = bw.take_xor(bw.take_xor(ROTR_14, ROTR_18), ROTR_41);

                T1 = bw.addBinary(bw.addBinary(bw.addBinary(bw.addBinary(h, ch_e_f_g), sigma_512_1), W[i]), K[i]);
                T2 = bw.addBinary(sigma_512_0, maj_a_b_c);

                h = g;
                g = f;
                f = e;
                e = bw.addBinary(d, T1);
                d = c;
                c = b;
                b = a;
                a = bw.addBinary(T1, T2);
            }

            h0 = bw.addBinary(h0, a), h4 = bw.addBinary(h4, e);
            h1 = bw.addBinary(h1, b), h5 = bw.addBinary(h5, f);
            h2 = bw.addBinary(h2, c), h6 = bw.addBinary(h6, g);
            h3 = bw.addBinary(h3, d), h7 = bw.addBinary(h7, h);

            messageBlocks += 1024;
        }

        message_digest_binary = h0 + h1 + h2 + h3 + h4 + h5 + h6 + h7;
    }

    void display()
    {
        NumberConversion nc;
        cout << "SHA-512 Hash of Message is :\n";
        cout << nc.binaryToHex(message_digest_binary) << endl;
    }
};


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

    SHA_512 sha(message);
    sha.round_function();
    sha.display();
    
    return 0;
}
