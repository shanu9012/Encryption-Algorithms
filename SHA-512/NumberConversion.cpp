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

class NumberConversion
{
    public:

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

    string charToBinary(string m)
    {
        string binary;

        for (size_t i=0; i<m.size(); i++)
            binary += patch::to_string(bitset<8>(m.c_str()[i]));

        return binary;
        
    }
};