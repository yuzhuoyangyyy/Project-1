 #include <iostream>
#include <vector>
using namespace std;

#define separator "," // used to separate large numbers into 3 by 3 digits
#define sep_min 5     // >0: use separators when the highest digit >= sep_min
                      // =0: never use separators
#define sci_big 25    // for |number| >= 1
                      // >0: use scientific notation when the highest digit >= sci_big
                      // =0: never use scientific notation
                      // =-1: the type of output depends on input
#define sci_small 10  // for |number| < 1
                      // >0: use scientific notation when the number of 0s between decimal point and significant digits >= sci_small
                      // =0: never use scientific notation
                      // =-1: the type of output depends on input
                    /* the type of output depends on input means
                        output a factor in scientific notation when the factor is input in big floating-point number,
                        output the product in scientific notation when one of the factors is input in big floating-point number */
#define sig_max 0     // when using scientific notation
                      // >0: keep at most sig_max significant digits
                      // =0: keep all digits
#define round 0       // when approximating
                      // =0: round half up
                      // =1: round up
                      // =-1: round down

class Number
{
    vector<int> digits;
    bool positive;
    int index;  // of the decimal point in digits, i.e. the number of digits in the integer part, i.e. the highest digit
    bool scientific;  // print with scientific notation or not
    bool point;  // print as a floating-point number

public:
    Number(const vector<int> &digits, bool positive, int index, bool scientific, bool point) :
            digits(digits), positive(positive), index(index), scientific(scientific), point(point) {};

    static Number mul(Number n1, Number n2)
    {
        if (n1.digits.size() == 0 || n2.digits.size() == 0)
            return Number({}, true, 2, false, n1.point || n2.point);
        else
        {
            bool positive = (n1.positive + n2.positive + 1) % 2;   // true when same, false when different
            int index = n1.index + n2.index;
            int size = n1.digits.size() + n2.digits.size();
            vector<int> digits(size, 0);
            // add products
            for (int i = 0; i < n1.digits.size(); i++)
                for (int j = 0; j < n2.digits.size(); j++)
                    digits[i + j + 1] += n1.digits[i] * n2.digits[j];
            // carry from low digits to high digits
            for (int i = size - 1; i > 0; i--)
            {
                digits[i - 1] += digits[i] / 10;
                digits[i] = digits[i] % 10;
            }
            // remove trailing 0s
            while (digits.size() != 0 & digits.back() == 0)
                digits.pop_back();
            // remove leading 0s
            while (digits.size() != 0 & digits[0] == 0)
            {
                digits.erase(digits.begin());
                index--;
            }
            bool scientific = false;
            if (index > 0)
            {
                if (sci_big < 0)
                    scientific = n1.scientific || n2.scientific;
                else if (sci_big == 0)
                    scientific = false;
                else if (index >= sci_big)
                    scientific = true;
            }
            else
            {
                if (sci_small < 0)
                    scientific = n1.scientific || n2.scientific;
                else if (sci_small == 0)
                    scientific = false;
                else if (index <= -sci_small)
                    scientific = true;
            }
            bool point = n1.point || n2.point;
            return Number(digits, positive, index, scientific, point);
        }
    }

    static Number trans(string str)
    {
        bool positive = true;
        int index = 0;
        bool scientific = false;
        bool point = false;

        while (str.find(',') != str.npos)
            str.erase(str.begin() + str.find(','));
        if (str[0] == '-')
        {
            positive = false;
            str.erase(str.begin());
        }
        if (str.back() == '%')
        {
            index -= 2;
            str.pop_back();
            point = true;
        }
        if (str.find('e') != str.npos)   // input is in scientific notation
        {
            if (str.size() > str.find('e') + 1)  // transform the string following 'e' into int
            {
                if ((str[str.find('e') + 1] < '0' || str[str.find('e') + 1] > '9') && str[str.find('e') + 1] != '-')
                    throw 0;
                for (int i = str.find('e') + 2; i < str.size(); i++)
                    if (str[i] < '0' || str[i] > '9')
                        throw 0;
                index += atoi(str.substr(str.find('e') + 1).c_str());
            }
            // remove 'e' and the string following it
            str.erase(str.begin() + str.find('e'), str.begin() + str.size());
            if (str.size() == 0)  // no numbers are in front of 'e'
                str = "1";
            scientific = true;
        }
        if (str.find('.') != str.npos)
        {  // input is decimal
            index += str.find('.');
            point = true;
            str.erase(str.begin() + str.find('.'));  // remove '.'
        }
        else  // input is integer
            index += str.size();

        vector<int> digits;
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] >= '0' && str[i] <= '9')
                digits.push_back(str[i] - '0');
            else
                throw 0;
        }
        while (digits.size() != 0 & digits.back() == 0)  // remove trailing 0s
            digits.pop_back();
        while (digits.size() != 0 & digits[0] == 0)  // remove leading 0s
        {
            digits.erase(digits.begin());
            index--;
        }

        if (index > 0)
        {
            if (sci_big > 0)
            {  // require to use scientific notation when the highest digit >= sci_big
                if (index >= sci_big)
                    scientific = true;
                else
                    scientific = false;
            }
            else if (sci_big == 0)  // require never to use scientific notation
                scientific = false;
            // else, whether to use scientific notation in the output is same as the input
        }
        else
        {
            if (sci_small > 0)
            {
                if (index <= -sci_small)
                    scientific = true;
                else
                    scientific = false;
            }
            else if (sci_small == 0)
                scientific = false;
        }
        return Number(digits, positive, index, scientific, point);
    }

    void print(bool in)
    {
        if (digits.size() == 0)
        {
            cout << "0";
            if (point)
                cout << ".0";
        }
        else
        {
            if (!positive)
            {
                if (in)
                    cout << "(";
                cout << "-";
            }
            if (scientific)
            {  // keep at most sig_max significant digits when sig_max > 0
                for (int i = 0; (i < sig_max || sig_max <= 0) && i < digits.size(); i++)
                {
                    if (i == sig_max-1)
                    {
                        if (round > 0)
                        {
                            if (digits.size() >= i+2 & digits[i+1] != 0)
                                cout << digits[i]+1;
                            else
                                cout << digits[i];
                        }
                        else if (round == 0)
                        {
                            if (digits.size() >= i+2 & digits[i+1] >= 5)
                                cout << digits[i]+1;
                            else
                                cout << digits[i];
                        }
                        else
                            cout << digits[i];
                    }
                    else
                        cout << digits[i];
                    if (i == 0)
                        cout << ".";
                }
                if (digits.size() == 1)
                {  // require "0" after "%d."
                    cout << "0";
                }
                cout << "e" << index-1;
            }
            else
            {
                if (index <= 0)
                    cout << "0." << string(-index,'0');
                for (int i = 0; i < digits.size(); i++)  // use separator
                {
                    if (sep_min > 0 && index >= sep_min && i > 0 && i < index && (i-index) % 3 == 0)
                        cout << separator;
                    if (i == index && i != 0)
                        cout << ".";
                    cout << digits[i];
                }
                for (int i = digits.size(); i < index; i++)
                {
                    if (sep_min > 0 && index >= sep_min && (i-index) % 3 == 0)
                        cout << separator;
                    cout << "0";
                }
                if (index >= 0 && index >= digits.size() && point)
                    cout << ".0";
            }
            if (!positive && in)
                cout << ")";
        }
    }
};

int main(int argc, char *argv[])
{
    try
    {
        if (argc == 1)
            cout << "Please input numbers!";
        else
        {
            Number n({1}, true, 1, false, false);
            for (int i = 1; i < argc; i++)
            {
                Number ni = Number::trans(argv[i]);
                n = Number::mul(n, ni);
                ni.print(true);
                if (i < argc - 1)
                    cout << " * ";
                else
                    cout << " = ";
            }
            n.print(false);
        }
        cout << endl;
    }
    catch(int e)
    {
        if (e == 0)
            cout << "The input cannot be interpret as numbers!" << endl;
    }
    return 0;
}
