# Project 1
**Name:** 余卓阳 (Yu Zhuoyang)  
**SID:** 12112721

## Part 1 - Analysis

To avoid having too many 0s, we can store only significant digits, and use the number of significant digits before the decimal point to record the position of the decimal point. E.g. 12300: {1,2,3},5; 0.0123: {1,2,3},-1

0.12 * 3.4 = {1,2},0 * {3,4},1 = {0, 1\*3, 2\*3+1\*4, 2*4},0+1 = {0,3,10,8},1 = {0,4,0,8},1 = 0.408

## Part 2 - Code
#### Implementation of Multiplication
```C++
int index = n1.index + n2.index;
int size = n1.digits.size() + n2.digits.size();
vector<int> digits(size,0);
// add products
for (int i = 0; i < n1.digits.size(); i++) 
    for (int j = 0; j < n2.digits.size(); j++)
    {
        digits[i+j+1] += n1.digits[i] * n2.digits[j];
    }
// carry from low digits to high digits
for (int i = size-1; i > 0; i--) 
{
    digits[i-1] += digits[i] / 10;
    digits[i] = digits[i] % 10;
}
// remove trailing 0s
while (digits.back() == 0) 
{
    digits.pop_back();
}
// remove leading 0s
while (digits.size() != 0 & digits[0] == 0) 
{
    digits.erase(digits.begin());
    index--;
}
```

## Part 3 - Result & Verification

### Test cases #1
```C++
#define separator "," // used to separate large numbers into 3 by 3 digits
#define sep_min 5 // use separators when the highest digit >= 5
#define sci_big 25 // for |number| >= 1, use scientific notation when the highest digit >= 25
#define sci_small 10 /* for |number| < 1, use scientific notation when the number of 0s 
                        between decimal point and significant digits >= 10 */
#define sig_max 0 // keep all digits
#define round 0 // round half up
```
```C++
./mul 2 3
2 * 3 = 6
./mul 3.1416 2
3.1416 * 2 = 6.2832
./mul 3.1416 2.0e-2
3.1416 * 0.02 = 0.062832
./mul a 2
The input cannot be interpret as numbers!
./mul 1234567890 1234567890
1,234,567,890 * 1,234,567,890 = 1,524,157,875,019,052,100
./mul 1.0e200 1.0e200
1.0e200 * 1.0e200 = 1.0e400
```

#### identifying floating-point numbers
```C++
./mul 1 2.0
1 * 2.0 = 2.0
./ mul 1.2 5
1.2 * 5 = 6.0
```

#### big numbers
```C++
./mul 123456789987654321 999999
123,456,789,987,654,321 * 999,999 = 123,456,666,530,864,333,345,679
```

#### not two numbers
```C++
./mul
Please input numbers!
./mul 100
100 = 100
./mul 12 34 56 78 90
12 * 34 * 56 * 78 * 90 = 160,392,960
```

#### negative numbers
```C++
./mul 2 -3
2 * (-3) = -6
./mul 0 -0
0 * 0 = 0
./mul -2e-2 -3
(-0.02) * (-3) = 0.06
```

#### percentages
```C++
./mul 4% 25
0.04 * 25 = 1.0
```

#### default is 0 before or after "."
```C++
./mul .1 .2
0.1 * 0.2 = 0.02
./mul 2. 3
2.0 * 3 = 6.0
```

#### default is 1 before "e" and 0 after "e"
```C++
./mul 2e e2
2 * 100 = 200
./mul e 2
1 * 2 = 2
```

#### ignoring ","
```C++
./mul 1,234 12,345
1234 * 12,345 = 15,233,730
./mul 123,4, ,123,45
1234 * 12,345 = 15,233,730
```

#### illegal inputs
```C++
./mul 1.1.1 2
The input cannot be interpret as numbers!
./mul 1.0e2e2 2
The input cannot be interpret as numbers!
./mul --2 3
The input cannot be interpret as numbers!
./mul 2%% 3
The input cannot be interpret as numbers!
```

### Test cases #2 (separator and sep_min)
```C++
#define sci_big 20
#define sci_small 10
#define sig_max 0
#define round 0
```
#### cases #2-1
```C++
#define separator " " // used to separate large numbers into 3 by 3 digits
#define sep_min 5
```
```C++
./mul 1234 12345
1234 * 12 345 = 15 233 730
```
#### cases #2-2
```C++
#define separator ","
#define sep_min 4 // use separators when the highest digit >= 4
```
```C++
./mul 1234 12345
1,234 * 12,345 = 15,233,730
```
#### cases #2-3
```C++
#define separator ","
#define sep_min 0 // never use separators
```
&nbsp; &nbsp; &nbsp; or
```C++
#define separator ""
#define sep_min 5
```

```C++
./mul 1234 12345
1234 * 12345 = 15233730
```

### Test cases #3 (sci_big and sci_small)
```C++
#define separator ","
#define sep_min 5
#define sig_max 0
#define round 0
```
#### cases #3-1
```C++
#define sci_big 5 // for |number| >= 1, use scientific notation when the highest digit >= 5
#define sci_small 5 /* for |number| < 1, use scientific notation when the number of 0s 
                        between decimal point and significant digits >= 5 */
```
```C++
./mul 1234 12345
1234 * 1.2345e4 = 1.523373e7
./mul 0.000012 0.00000123
0.000012 * 1.23e-6 = 1.476e-11
./mul 12000 0.00000123
1.2e4 * 1.23e-6 = 0.01476
```
#### cases #3-2
```C++
#define sci_big 0 // for |number| >= 1, never use scientific notation
#define sci_small 0 // for |number| < 1, never use scientific notation
```
```C++
./mul 1234 12345
1234 * 12,345 = 15,233,730
./mul 0.000012 0.00000123
0.000012 * 0.00000123 = 0.00000000001476
./mul 12000 0.00000123
12,000 * 0.00000123 = 0.01476
```
#### cases #3-3
```C++
#define sci_big -1 // for |number| >= 1, the type of output depends on input
#define sci_small -1 // for |number| < 1, the type of output depends on input
 /* depending on input means
    output a factor in scientific notation when the input of the factor contains "e", 
    output the product in scientific notation when one of the factors is input in "e" */
```
```C++
./mul 1234 12345
1234 * 12,345 = 15,233,730
./mul 12.34e2 12345
1.234e3 * 12,345 = 1.523373e7
./mul 0.000012 0.00000123
0.000012 * 0.00000123 = 0.00000000001476
./mul 12e-6 0.00000123
1.2e-5 * 0.00000123 = 1.476e-11
./mul 12000 0.00000123
12,000 * 0.00000123 = 0.01476
./mul 12e3 0.00000123
1.2e4 * 0.00000123 = 1.476e-2
./mul 12e3 12.3e-7
1.2e4 * 1.23e-6 = 1.476e-2
```
#### cases #3-4
```C++
#define sci_big -1 // for |number| >= 1, the type of output depends on input
#define sci_small 0 // for |number| < 1, never use scientific notation
```
```C++
./mul 1234 0.000123
1234 * 0.000123 = 0.151782
./mul 1234 12.3e-5
1234 * 0.000123 = 0.151782
./mul 12.34e2 0.000123
1.234e3 * 0.000123 = 0.151782
```

### Test cases #4 (sig_max and round)
```C++
#define separator ","
#define sep_min 5
#define sci_big 6
#define sci_small 4
```
#### cases #4-1
```C++
#define sig_max 0
#define round 0 // useless
```
```C++
./mul 12345 123456
12,345 * 1.23456e5 = 1.52406432e9
./mul 0.00012345 0.0000123456
0.00012345 * 1.23456e-5 = 1.52406432e-9
./mul 12345 12345
12,345 * 12,345 = 1.52399025e8
./mul 0.00012345 0.000012345
0.00012345 * 1.2345e-5 = 1.52399025e-9
```
#### cases #4-2
```C++
#define sig_max 8 // when using scientific notation, keep at most 8 significant digits
#define round 0 // round half up
```
```C++
./mul 12345 123456
12,345 * 1.23456e5 = 1.5240643e9
./mul 0.00012345 0.0000123456
0.00012345 * 1.23456e-5 = 1.5240643e-9
./mul 12345 12345
12,345 * 12,345 = 1.5239903e8
./mul 0.00012345 0.000012345
0.00012345 * 1.2345e-5 = 1.5239903e-9
```
#### cases #4-3
```C++
#define sig_max 8
#define round 1 // round up
```
```C++
./mul 12345 123456
12,345 * 1.23456e5 = 1.5240644e9
./mul 0.00012345 0.0000123456
0.00012345 * 1.23456e-5 = 1.5240644e-9
./mul 12345 12345
12,345 * 12,345 = 1.5239903e8
./mul 0.00012345 0.000012345
0.00012345 * 1.2345e-5 = 1.5239903e-9
```
#### cases #4-4
```C++
#define sig_max 8
#define round -1 // round down
```
```C++
./mul 12345 123456
12,345 * 1.23456e5 = 1.5240643e9
./mul 0.00012345 0.0000123456
0.00012345 * 1.23456e-5 = 1.5240643e-9
./mul 12345 12345
12,345 * 12,345 = 1.5239902e8
./mul 0.00012345 0.000012345
0.00012345 * 1.2345e-5 = 1.5239902e-9
```

## Part 4 - Difficulties & Solutions
For big integers, using the long long type may also overflow, but using arrays (or vectors) to store digits and rewriting multiplications is a solution. 
It is hard to determine a single standard about the output type well, but setting the output type by defining the macros in the code is a solution. 
