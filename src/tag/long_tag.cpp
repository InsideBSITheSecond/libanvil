/*
 * long_tag.cpp
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

#include <sstream>
#include "../../include/byte_stream.h"
#include "../../include/tag/long_tag.h"

/*
 * Long tag assignment operator
 */
long_tag& long_tag::operator=(const long_tag& other) {

    // check for self
    if (this == &other)
        return *this;

    // assign attributes
    name = other.name;
    type = other.type;
    value = other.value;
    return *this;
}

/*
 * Long tag equals operator
 */
bool long_tag::operator==(const generic_tag& other) {

    // check for self
    if (this == &other)
        return true;

    // convert into same type
    const long_tag* other_tag = dynamic_cast<const long_tag*>(&other);
    if (!other_tag)
        return false;

    // check attributes
    return name == other.name
           && type == other.type
           && value == other_tag->value;
}

/*
 * Return a long tag's data
 */
std::vector<char> long_tag::get_data(bool list_ele) {
    byte_stream stream(byte_stream::SWAP_ENDIAN);

    get_data(list_ele, stream);

    return stream.vbuf();
}

/*
 * Save a long tag's data to a stream
 */
void long_tag::get_data(bool list_ele, byte_stream& stream) {
    // form data representation
    if (!list_ele) {
        stream << (char) type;
        stream << (short) name.size();
        stream << name;
    }
    int32_t temp; //get 4 bit pieces

    temp = value >> 32; //top 32 bits
    stream << temp; //top 32 bits in

    temp = value; //should truncate, lower 32 bits
    stream << temp; //lower 32 bits in
}

/*
 * Return a long tag's data size, equivalent to get_data().size(), but faster.
 */
unsigned int long_tag::get_data_size(bool list_ele) {
    unsigned int total = 0; //nothing yet

    if (!list_ele) {
        total += 1 + 2 + name.size(); //1 for type, 2 for short size, and every symbol in the name.
    }

    total += 8; //8 bytes in a long
    return total;
}

/*
 * Return a string representation of a long tag
 */
std::string long_tag::to_string(unsigned int tab) {
    std::stringstream ss;

    // form string representation
    ss << generic_tag::to_string(tab) << ": " << value;
    return ss.str();
}
