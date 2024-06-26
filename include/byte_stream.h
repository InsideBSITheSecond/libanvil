/*
 * byte_stream.h
 * Copyright (C) 2012 - 2019 David Jolly
 * ----------------------
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BYTE_STREAM_H_
#define BYTE_STREAM_H_

#include <cstdlib>
#include <string>
#include <vector>
#include <stdint.h>

class byte_stream {
private:

    /*
     * Stream buffer
     */
    std::vector<char> buff;

    /*
     * Stream buffer length/position
     */
    size_t pos;

    /*
     * Swap endian
     */
    bool swap;

    /*
     * Number of entries in buff
     */
    size_t numberOfEntries;

    /*
     * Read byte stream into variable
     * (char, short, int, long)
     */
    template<class T>
    unsigned int read_stream(T& var) {
        size_t remainingValues = available();
        if (remainingValues < sizeof(T))
            return END_OF_STREAM;

        // assign type T from stream
        var = 0;
        unsigned int width = sizeof(T);
        for (unsigned int i = 0; i < width; ++i) {
            T value = buff.at(pos++) & 0xFF; // Required to ensure the type sizes match when shifting

            uint8_t numberOfShifts = (8 * ((width - 1) - i));
            if (swap) {
                numberOfShifts = 8 * width - (numberOfShifts + 8);
            }

            var |= (value << numberOfShifts);
        }
        return SUCCESS;
    }

    /*
     * Read byte stream into variable
     * (float, double)
     */
    template<class T>
    bool read_stream_float(T& var) {
        std::vector<unsigned char> data;

        size_t remainingValues = available();
        if (remainingValues < sizeof(T))
            return END_OF_STREAM;

        // assign type T from stream
        for (unsigned int i = 0; i < sizeof(T); ++i) {
            data.push_back(buff.at(pos++));
        }
        if (swap)
            swap_endian(data);
        var = static_cast<T>(atof((char*) data.data()));
        return SUCCESS;
    }

    /*
     * Convert between endian types
     */
    static void swap_endian(std::vector<unsigned char>& data);

    /*
     * Write variable into byte stream
     * (char, short, int, long, float, double)
     */
    template<class T>
    unsigned int write_stream(T var) {
        /*char *parts = NULL;
        std::vector<unsigned char> data;

        // convert to char array
        parts = reinterpret_cast<char *>(&var);
        data.insert(data.end(), parts, parts + sizeof(T));
        if(swap)
            swap_endian(data);
        buff.insert(buff.begin() + pos, data.begin(), data.end());
        pos += data.size(); */

        if (swap) {
            char byte;
            //std::cout<<sizeof(T)<<"\n";
            for (int i = sizeof(T) - 1; i >= 0; i--) {
                //std::cout<<i<<"\n";
                byte = *((char*) (&var) + i); //get 1-byte chunks of memory at the var address
                buff.push_back(byte);
            }
        } else {
            char byte;
            for (int i = 0; i < sizeof(T); i++) {
                byte = *((char*) (&var) + i); //get 1-byte chunks of memory at the var address
                buff.push_back(byte);
            }
        }
        pos += sizeof(T);
        numberOfEntries = buff.size();
        return SUCCESS;
    }

public:

    /*
     * Status indicators
     */
    static const unsigned int END_OF_STREAM = 0;
    static const unsigned int SUCCESS = 1;

    /*
     * Swap flags
     */
    static const int NO_SWAP_ENDIAN = 0x0;
    static const int SWAP_ENDIAN = 0x1;

    /*
     * Byte stream constructor
     */
    byte_stream(void) : pos(0), swap(NO_SWAP_ENDIAN) { return; }

    /*
     * Byte stream constructor
     */
    byte_stream(unsigned int swap) : pos(0), swap(swap) { return; }

    /*
     * Byte stream constructor
     */
    byte_stream(const byte_stream& other) : buff(other.buff), pos(other.pos), swap(other.swap) { return; }

    /*
     * Byte stream constructor
     */
    byte_stream(const std::string& buff);

    /*
     * Byte stream constructor
     */
    byte_stream(std::vector<char>& buff);

    /*
     * Byte stream destructor
     */
    virtual ~byte_stream(void) { return; }

    /*
     * Byte stream assignment
     */
    byte_stream& operator=(const byte_stream& other);

    /*
     * Byte stream equals
     */
    bool operator==(const byte_stream& other);

    /*
     * Byte stream not equals
     */
    bool operator!=(const byte_stream& other) { return !(*this == other); }

    /*
     * Byte stream input
     */
    bool operator<<(std::vector<char>& input);

    /*
     * Byte stream input
     */
    bool operator<<(std::vector<char>&& input);

    /*
     * Byte stream input
     */
    bool operator<<(const std::string& input);

    /*
     * Byte stream input
     */
    bool operator<<(char input) { return write_stream<char>(input); }

    /*
     * Byte stream input
     */
    bool operator<<(short input) { return write_stream<short>(input); }

    /*
     * Byte stream input
     */
    bool operator<<(int input) { return write_stream<int>(input); }

    /*
     * Byte stream input
     */
    bool operator<<(long input) { return write_stream<long>(input); }

    /*
     * Byte stream input
     */
    bool operator<<(long long input) { return write_stream<long long>(input); }

    /*
     * Byte stream input
     */
    bool operator<<(float input) { return write_stream<float>(input); }

    /*
     * Byte stream input
     */
    bool operator<<(double input) { return write_stream<double>(input); }

    /*
     * Byte stream output
     */
    bool operator>>(char& output);

    /*
     * Byte stream output
     */
    bool operator>>(short& output);

    /*
     * Byte stream output
     */
    bool operator>>(int& output);

    /*
     * Byte stream output
     */
    bool operator>>(long& output);

    /*
     * Byte stream output
     */
    bool operator>>(long long& output);

    /*
     * Byte stream output
     */
    bool operator>>(float& output);

    /*
     * Byte stream output
     */
    bool operator>>(double& output);

    /*
     * Returns the available bytes left in the stream
     */
    size_t available(void);

    /*
     * Clear the stream
     */
    void clear(void);

    /*
     * Returns the current position of the stream
     */
    size_t get_position(void) { return pos; }

    /*
     * Returns the status of the stream
     */
    bool good(void) { return available() != END_OF_STREAM; }

    /*
     * Returns the endian swap status of the stream
     */
    bool is_swap(void) { return swap; }

    /*
     * Returns the entire contents of the stream buffer
     */
    const char* rdbuf(void) { return buff.data(); }

    /*
     * Returns the entire contents of the stream buffer
     */
    std::vector<char>& vbuf(void) { return buff; }

    /*
     * Resets the streams position
     */
    void reset(void) { pos = 0; }

    /*
     * Sets a streams position
     */
    void set_position(unsigned int pos) { this->pos = pos; }

    /*
     * Sets a streams swap status
     */
    void set_swap(unsigned int swap) { this->swap = swap; }

    /*
     * Returns the streams total size
     */
    size_t size(void) { return buff.size(); }

    /*
     * Returns a string representation of the stream
     */
    std::string to_string(void);
};

#endif // BYTE_STREAM_H_
