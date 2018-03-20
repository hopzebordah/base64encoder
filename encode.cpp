/*
Alexander Peters
encode.cpp
A program that encodes a file into a base64 string
usage: 
./encode <filepath>
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>

#define BASE64_TABLE "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

using namespace std;

// this program encodes a file into base64

string encode_base64(unsigned char input [])
{
    // strings passed to this function must be 3 chars in length
    struct int24 { unsigned int val : 24; } field;
    field.val = 0;
    
    // pack all three bytes into bitfield
    for (int i=0; i<3; i++)
        field.val = field.val | ((int)input[i] << (16 - i*8));

    // pull each 6 bits out of bitfield and put into split array
    int split [4];
    for (int i=0; i<4; i++)
        split[3-i] = (field.val >> (i*6)) & 0x3F;
    
    // get base64 chars with ints
    string result(4, 0);
    for (int i=0; i<4; i++)
        result[i] = BASE64_TABLE[split[i]];

    // return result string
    return result;
}

int get_filesize(string filename)
{
    ifstream file(filename.c_str(), ios::ate | ios::binary);
    long int i = file.tellg();
    file.close();
    return i;
}

string getbase64_file(string filename)
{
    // gather information about file
    int filesize = get_filesize(filename);

    // determine if padding is needed
    int padding = 0;
    int arrsize = filesize;
    if (arrsize % 3) // if there is a remainder, then
    {
        padding = 3 - (arrsize % 3); // the padding is (3 - the remainder)
        arrsize += padding; // add the padding to the array length
    }
    
    int pad = (filesize % 3) ? 4 : 0;
    int numthrees = filesize / 3;
    int resultlen = filesize + numthrees;

    // declare temp array for data sent to encode_base64()
    unsigned char chunk [3];

    // result string for output
    string result = "";

    // open specified file
    ifstream file(filename.c_str(), ios::in | ios::binary);

    // outer loop : 'rows', or arrsize/3
    for (int row=0; row<arrsize/3; row++)
    {
        memset(chunk, 0, sizeof chunk); // set memory to 0 in case exactly 3 bytes are not read from file
        file.read((char*)chunk, sizeof chunk); // read 3 bytes from file
        result.append( encode_base64(chunk)); // append result from encode_base64() to result string
    }    
    file.close();

    // change trailing zeroes (A)s to '='  
    for (int i=resultlen+1; i<resultlen+pad; i++)
        result[i] = '=';        

    return result;
}

int main(int argc, char **argv)
{
    // filename is cli arg1
    string filename = argv[1];

    // encode file by filename
    string test = getbase64_file(filename);

    // print every 76 chars for readability
    for (int i=0; i<test.size(); i+=76)
        cout << test.substr(i, 76) << endl;

    return 0;
}
