/*  Src code to read contents of an image,
 *           to flip contents of an image,
 *           to write contents of an image.
 * */

#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>
#include <climits>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
#include <bitset>

const uint16_t ENDIAN_LITTLE = 0x4949;
const uint16_t ENDIAN_BIG    = 0x4d4d;

#endif /* __PROGTEST__ */

/*  Program has class Image to handle an image.
 *  test case are handled in main function
 * */

class Image{
private:   // declaration of private members of the Image class
    std::ifstream inFile;               //  input  stream
    std::ofstream outFile;              //  output stream
    std::uint16_t endianInput;          //  handling endianity of data {ENDIAN_BIG || ENDIAN_LITTLE}
    std::uint16_t width;                //  width  = number of columns for pixels
    std::uint16_t height;               //  height = number of rows    for pixels
    std::uint16_t format;               //  format = pixel format
    int channels;                       //  channels = number of channels :: {1 || 3 || 4} <=> {(B&W | grey) || RGB || RGBA}
    int bits_per_channel;               //  bits_per_channel = to handle bits per pixel per channel :: {1 bit || 8 bits || 16 bits}
    int bytes;                          //  combination of channels and bits_per_channel, an argument to read from a stream or to write to a stream
    bool flipHorizontal;                //  flag to flip the image horizontally
    bool flipVertical;                  //  flag to flip the image vertically
    std::vector<std::vector<std::uint64_t>> data; // 2-d vector to store data amd process it
    void countBytes();                  //  function to handle combinations for number of bytes required, return type void
    void flipHorizontally();            //  function flips the image  horizontally, return type void
    void flipVertically();              //  function flips the image  vertically, return type void
public:
    Image (const char *srcFileName, const char *dstFileName, bool flipHorizontal, bool flipVertical);   //constructor to open files and get flags for flipping
    ~Image ();                          //  destructor closes files
    bool loadHeader ();                 //  function to load header  of the image, return type boolean
    bool loadData();                    //  function to load data    of the image, return type boolean
    bool writeHeader();                 //  function to write header of the image, return type boolean
    bool writeData();                   //  function to write data   of the image, return type boolean
    void flip();                        //  function to manipulate other 2 flipping functions according to flags
};


Image::Image (const char *srcFileName, const char *dstFileName, bool flipHorizontal, bool flipVertical)
{
    inFile.open(srcFileName, std::ios::binary | std::ios::in);          //  input  stream
    outFile.open(dstFileName, std::ios::binary | std::ios::out);        //  output stream
    this->flipHorizontal = flipHorizontal;                              //  flag to flip the image horizontally
    this->flipVertical = flipVertical;                                  //  flag to flip the image vertically
}

Image::~Image ()
{
    inFile.close();                                                     // close file
    outFile.close();                                                    // close file
}

bool Image::loadHeader()
{
    if (!inFile)                                                        // check if stream is ok, return false if not
    {
        return false;
    }

    inFile.read((char *) &endianInput, 2);                              // read first 2 bytes for endianity of the image

    if (inFile.fail())                                                  // check if bytes are read, stream does not return failure flag
    {
        return false;
    }

    if((endianInput != ENDIAN_LITTLE) && (endianInput != ENDIAN_BIG))   // only 2 options  are available: ENDIAN_LITTLE or ENDIAN_BIG
    {
        return false;
    }


    inFile.read((char *) &width, 2);                                    // read next 2 bytes for width of the image

    if (inFile.fail() || width == 0)                                    // check if bytes are read, stream does not return failure flag, or width is not equal to zero
    {
        return false;
    }

    inFile.read((char *) &height, 2);                                   // read next 2 bytes for height of the image

    if (inFile.fail() || height == 0)                                   // check if bytes are read, stream does not return failure flag, or height is not equal to zero
    {
        return false;
    }

    inFile.read((char *) &format, 2);                                   // read next 2 bytes for format of pixels of the image

    if (inFile.fail())                                                  // check if stream does not return failure flag,
    {
        return false;
    }

    if(endianInput == ENDIAN_BIG)                                       // convert from Big Endian to Little Endian
    {
        width =  (std::uint16_t) ((width & 0xFF00) >> 8)  + ((width & 0x00FF) << 8);
        height = (std::uint16_t) ((height & 0xFF00) >> 8) + ((height & 0x00FF) << 8);
        format = (std::uint16_t) ((format & 0xFF00) >> 8) + ((format & 0x00FF) << 8);
    }

    if((format & 0xFFE0) != 0)                                          // for the format of pixels: bits from 15 to 5 must be 0
    {
        return  false;
    }


    switch(format)                                                      // check format combinations
    {
        case 0x0000: bits_per_channel = 1; channels = 1;
            break;
        case 0x0002: bits_per_channel = 1; channels = 3;
            break;
        case 0x0003: bits_per_channel = 1; channels = 4;
            break;
        case 0x000c: bits_per_channel = 8; channels = 1;
            break;
        case 0x000e: bits_per_channel = 8; channels = 3;
            break;
        case 0x000f: bits_per_channel = 8; channels = 4;
            break;
        case 0x0010: bits_per_channel = 16; channels = 1;
            break;
        case 0x0012: bits_per_channel = 16; channels = 3;
            break;
        case 0x0013: bits_per_channel = 16; channels = 4;
            break;
        default:
            return false;
    }

    return true;
}

bool Image::loadData()                                                         //  function to load data    of the image, return type boolean
{

    countBytes();                                                              // call to function, which  processes, how many bytes to read from the stream or write to the stream
    data.resize(height, std::vector<std::uint64_t>(width));                    // 2d allocation of memory for vector

    for(auto y = 0; y <  height; ++y)
    {
        for(auto x = 0; x <  width; ++x)
        {
            inFile.read((char *) &data[y][x], bytes);                          // looping through the matrix and reading x bytes, byte by byte

            if(inFile.fail())
            {
                return false;
            }

        }
    }

    inFile.get();
    if(!inFile.eof())                                                          // since all the data is loaded, check if it's end of file
    {
        return false;
    }

    return true;
}

bool Image::writeHeader()                                                    //  function to write header of the image, return type boolean
{
    if (!outFile)                                                             // check if stream is ok, return false if not
    {
        return false;
    }

    std::uint16_t width_out = width;                                        // header components to new output file
    std::uint16_t height_out = height;
    std::uint16_t format_out = format;

    if(endianInput == ENDIAN_BIG)                                           // convert from Little Endian to Big Endian
    {
        width_out =  (std::uint16_t) ((width_out & 0xFF00) >> 8)  + ((width_out & 0x00FF) << 8);
        height_out = (std::uint16_t) ((height_out & 0xFF00) >> 8) + ((height_out & 0x00FF) << 8);
        format_out = (std::uint16_t) ((format_out & 0xFF00) >> 8) + ((format_out & 0x00FF) << 8);
    }

    outFile.write((char *) &endianInput, 2);                                // write 2 byte by 2 byte, till header is fully written


    if (outFile.fail())
    {
        return false;
    }

    outFile.write((char *) &width_out, 2);


    if (outFile.fail())
    {
        return false;
    }

    outFile.write((char *) &height_out, 2);


    if (outFile.fail())
    {
        return false;
    }

    outFile.write((char *) &format_out, 2);


    if (outFile.fail())
    {
        return false;
    }

    return true;

}

bool Image::writeData()                                                                          //  function to write data   of the image, return type boolean
{

    if(!outFile)
    {
        return false;
    }

    for(auto y = 0; y < height; ++y)
    {
        for(auto x = 0; x < width; ++x)
        {
            outFile.write((char *) &data[y][x], bytes);                                         // write pixel by pixel to the output file

            if(!outFile)
            {
                return false;
            }
        }
    }
    return true;
}

void Image::countBytes()                                                                      //  function to handle combinations for number of bytes required, return type void
{
    if((channels == 1) && (bits_per_channel == 1 || bits_per_channel == 8))
    {
        bytes = 1;
    }else if((channels == 3) && (bits_per_channel == 1 || bits_per_channel == 8))
    {
        bytes = 3;
    }else if((channels == 4) && (bits_per_channel == 1 || bits_per_channel == 8))
    {
        bytes = 4;
    }

    if(channels == 1 && bits_per_channel == 16)
    {
        bytes = 2;
    }else if(channels == 3 && bits_per_channel == 16)
    {
        bytes = 6;
    }else if(channels == 4 && bits_per_channel == 16)
    {
        bytes = 8;
    }
}

void Image::flipVertically(){                                                               // function flips pixels vertically

    for(int x = 0; x < (int) width; ++x)
    {
        int start = 0;
        int end = (int) (height - 1);
        while (start < end)
        {
            std::uint64_t temp = data[start][x];
            data[start][x] = data[end][x];
            data[end][x] = temp;
            start++;
            end--;
        }
    }

}

void Image::flipHorizontally()                                                              // function flips pixels horizontally
{

    for(int y = 0; y < (int)(height); ++y)
    {
        int start = 0;
        int end = (int)(width - 1);
        while (start < end)
        {
            std::uint64_t temp = data[y][start];
            data[y][start] = data[y][end];
            data[y][end] = temp;
            start++;
            end--;
        }
    }

}

void Image::flip()                                                                          // according to flag flip pixels horzontally, vertically, both or none
{
    if(flipHorizontal) { flipHorizontally(); }
    if(flipVertical)   { flipVertically();   }
}


bool flipImage ( const char  * srcFileName,                                                 // tester function, gets name of the image, output file and flip flags, if everything file, returns true
                 const char  * dstFileName,
                 bool          flipHorizontal,
                 bool          flipVertical )
{
    Image image(srcFileName, dstFileName, flipHorizontal, flipVertical);

    if(!image.loadHeader())
    {
        return false;
    }
    if(!image.loadData())
    {
        return false;
    }
    image.flip();
    if(!image.writeHeader())
    {
        return false;
    }
    if(!image.writeData())
    {
        return false;
    }

    return true;
}

#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1,
                      const char * fileName2 )
{
}

/*main function
 * tests implememnted porgram above
 * */

int main ( void )
{

    assert ( flipImage ( "input_00.img", "output_00.img", true, false )
             && identicalFiles ( "output_00.img", "ref_00.img" ) );

    assert ( flipImage ( "input_01.img", "output_01.img", false, true )
             && identicalFiles ( "output_01.img", "ref_01.img" ) );

    assert ( flipImage ( "input_02.img", "output_02.img", true, true )
             && identicalFiles ( "output_02.img", "ref_02.img" ) );

    assert ( flipImage ( "input_03.img", "output_03.img", false, false )
             && identicalFiles ( "output_03.img", "ref_03.img" ) );

    assert ( flipImage ( "input_04.img", "output_04.img", true, false )
             && identicalFiles ( "output_04.img", "ref_04.img" ) );

    assert ( flipImage ( "input_05.img", "output_05.img", true, true )
             && identicalFiles ( "output_05.img", "ref_05.img" ) );

    assert ( flipImage ( "input_06.img", "output_06.img", false, true )
             && identicalFiles ( "output_06.img", "ref_06.img" ) );

    assert ( flipImage ( "input_07.img", "output_07.img", true, false )
             && identicalFiles ( "output_07.img", "ref_07.img" ) );

    assert ( flipImage ( "input_08.img", "output_08.img", true, true )
             && identicalFiles ( "output_08.img", "ref_08.img" ) );

    assert ( ! flipImage ( "input_09.img", "output_09.img", true, false ) );

    // extra inputs (optional & bonus tests)
    assert ( flipImage ( "extra_input_00.img", "extra_out_00.img", true, false )
             && identicalFiles ( "extra_out_00.img", "extra_ref_00.img" ) );
    assert ( flipImage ( "extra_input_01.img", "extra_out_01.img", false, true )
             && identicalFiles ( "extra_out_01.img", "extra_ref_01.img" ) );
    assert ( flipImage ( "extra_input_02.img", "extra_out_02.img", true, false )
             && identicalFiles ( "extra_out_02.img", "extra_ref_02.img" ) );
    assert ( flipImage ( "extra_input_03.img", "extra_out_03.img", false, true )
             && identicalFiles ( "extra_out_03.img", "extra_ref_03.img" ) );
    assert ( flipImage ( "extra_input_04.img", "extra_out_04.img", true, false )
             && identicalFiles ( "extra_out_04.img", "extra_ref_04.img" ) );
    assert ( flipImage ( "extra_input_05.img", "extra_out_05.img", false, true )
             && identicalFiles ( "extra_out_05.img", "extra_ref_05.img" ) );
    assert ( flipImage ( "extra_input_06.img", "extra_out_06.img", true, false )
             && identicalFiles ( "extra_out_06.img", "extra_ref_06.img" ) );
    assert ( flipImage ( "extra_input_07.img", "extra_out_07.img", false, true )
             && identicalFiles ( "extra_out_07.img", "extra_ref_07.img" ) );

    /*----------------------------------------------------------------------------*/
    assert ( flipImage ( "extra_input_08.img", "extra_out_08.img", true, false )
             && identicalFiles ( "extra_out_08.img", "extra_ref_08.img" ) );
    assert ( flipImage ( "extra_input_09.img", "extra_out_09.img", false, true )
             && identicalFiles ( "extra_out_09.img", "extra_ref_09.img" ) );
    assert ( flipImage ( "extra_input_10.img", "extra_out_10.img", true, false )
             && identicalFiles ( "extra_out_10.img", "extra_ref_10.img" ) );
    assert ( flipImage ( "extra_input_11.img", "extra_out_11.img", false, true )
             && identicalFiles ( "extra_out_11.img", "extra_ref_11.img" ) );

    return 0;
}
#endif /* __PROGTEST__ */
