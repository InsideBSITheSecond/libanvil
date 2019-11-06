/*
 * int_array_tag.cpp
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
#include "../../include/tag/int_array_tag.h"

/*
 * Integer array tag assignment operator
 */
int_array_tag& int_array_tag::operator=(const int_array_tag& other) {

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
 * Integer array tag equals operator
 */
bool int_array_tag::operator==(const generic_tag& other) {

    // check for self
    if (this == &other)
        return true;

    // convert into same type
    const int_array_tag* other_tag = dynamic_cast<const int_array_tag*>(&other);
    if (!other_tag)
        return false;

    // check attributes
    if (name != other.name
        || type != other.type
        || value.size() != other_tag->value.size())
        return false;
    for (unsigned int i = 0; i < value.size(); ++i) {
        if (value.at(i) != other_tag->value.at(i))
            return false;
    }
    return true;
}

/*
 * Return a integer array tag's data
 */
std::vector<char> int_array_tag::get_data(bool list_ele) {
    byte_stream stream(byte_stream::SWAP_ENDIAN);

    get_data(list_ele, stream);

    return stream.vbuf();
}

/*
 * Save a int array tag's data to a stream
 */
void int_array_tag::get_data(bool list_ele, byte_stream& stream) {
    // form data representation
    if (!list_ele) {
        stream << (char) type;
        stream << (short) name.size();
        stream << name;
    }
    stream << (int) value.size();
    for (unsigned int i = 0; i < value.size(); ++i) {
        stream << value.at(i);
    }
}

/*
 * Return a int array tag's data size, equivalent to get_data().size(), but faster.
 */
unsigned int int_array_tag::get_data_size(bool list_ele) {
    unsigned int total = 0; //nothing yet

    if (!list_ele) {
        total += 1 + 2 + name.size(); //1 for type, 2 for short size, and every symbol in the name.
    }
    total += 4; //array size,  int = 4 bytes
    total += value.size() * 4; //4 bytes in an int, this many ints
    return total;
}


/*
 * Return a string representation of a integer array tag
 */
std::string int_array_tag::to_string(unsigned int tab) {
    std::stringstream ss;

    // form string representation
    ss << generic_tag::to_string(tab) << " (" << value.size() << ") { ";
    if (!value.empty())
        for (unsigned int i = 0; i < value.size(); ++i) {
            ss << value.at(i) << ", ";
        }
    ss << "}";
    return ss.str();
}
