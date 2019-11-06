/*
 * byte_tag.cpp
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
#include "../../include/tag/byte_tag.h"

/*
 * Byte tag assignment operator
 */
byte_tag& byte_tag::operator=(const byte_tag& other) {

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
 * Byte tag equals operator
 */
bool byte_tag::operator==(const generic_tag& other) {

    // check for self
    if (this == &other)
        return true;

    // convert into same type
    const byte_tag* other_tag = dynamic_cast<const byte_tag*>(&other);
    if (!other_tag)
        return false;

    // check attributes
    return name == other.name
           && type == other.type
           && value == other_tag->value;
}

/*
 * Return a byte tag's data
 */
std::vector<char> byte_tag::get_data(bool list_ele) {
    byte_stream stream(byte_stream::SWAP_ENDIAN);

    get_data(list_ele, stream);

    return stream.vbuf();
}

/*
 * Save a byte tag's data to a stream
 */
void byte_tag::get_data(bool list_ele, byte_stream& stream) {
    // form data representation
    if (!list_ele) {
        stream << (char) type;
        stream << (short) name.size();
        stream << name;
    }
    stream << value;
}

/*
 * Return a byte tag's data size, equivalent to get_data().size(), but faster.
 */
unsigned int byte_tag::get_data_size(bool list_ele) {
    unsigned int total = 0; //nothing yet

    if (!list_ele) {
        total += 1 + 2 + name.size(); //1 for type, 2 for short size, and every symbol in the name.
    }

    total += 1; //1 byte in the value
    return total;
}

/*
 * Return a string representation of a byte tag
 */
std::string byte_tag::to_string(unsigned int tab) {
    std::stringstream ss;

    // form string representation
    ss << generic_tag::to_string(tab) << ": " << (int) value;
    return ss.str();
}
