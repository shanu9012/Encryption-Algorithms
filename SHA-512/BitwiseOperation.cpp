using namespace std;

class BitwiseOperation
{
    public:

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

    string AND(string a, string b)
    {
        string andValue;

        for(int i=0; i<a.length(); i++)
            andValue += ((a[i]-'0') & (b[i]-'0')) + '0';

        return andValue; 
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
};