/*
 * list_tag.cpp
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
#include "../../include/tag/list_tag.h"

/*
 * List tag assignment operator
 */
list_tag& list_tag::operator=(const list_tag& other) {

    // check for self
    if (this == &other)
        return *this;

    // assign attributes
    name = other.name;
    type = other.type;
    value = other.value;
    ele_type = other.ele_type;
    return *this;
}

/*
 * List tag equals operator
 */
bool list_tag::operator==(const generic_tag& other) {

    // check for self
    if (this == &other)
        return true;

    // convert into same type
    const list_tag* other_tag = dynamic_cast<const list_tag*>(&other);
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
 * Return a list tag's data
 */
std::vector<char> list_tag::get_data(bool list_ele) {
    byte_stream stream(byte_stream::SWAP_ENDIAN);

    get_data(list_ele, stream);

    return stream.vbuf();
}

/*
 * Save a list tag's data to a stream
 */
void list_tag::get_data(bool list_ele, byte_stream& stream) {
    // form data representation
    if (!list_ele) {
        stream << (char) type;
        stream << (short) name.size();
        stream << name;
    }
    stream << (char) ele_type;
    stream << (int) value.size();
    for (unsigned int i = 0; i < value.size(); ++i) {
        value.at(i)->get_data(true, stream);
    }
}

/*
 * Return a list tag's data size, equivalent to get_data().size(), but faster.
 */
unsigned int list_tag::get_data_size(bool list_ele) {
    unsigned int total = 0; //nothing yet

    if (!list_ele) {
        total += 1 + 2 + name.size(); //1 for type, 2 for short size, and every symbol in the name.
    }

    total += 1 + 4; //1 for ele type, 4 for size
    for (unsigned int i = 0; i < value.size(); ++i) {
        total += value.at(i)->get_data_size(true);
    }  //get individual parts

    return total;
}

/*
 * Insert a tag into a list tag at a given index
 */
bool list_tag::insert(generic_tag* value, unsigned int index) {

    // check for valid type
    if (value->get_type() != ele_type)
        return false;
    this->value.insert(this->value.begin() + index, value);
    return true;
}

/*
 * Insert a tag onto the tail of a list tag
 */
bool list_tag::push_back(generic_tag* value) {

    // check for valid type
    if (value->get_type() != ele_type)
        return false;
    this->value.push_back(value);
    return true;
}

/*
 * Return a string representation of a list tag
 */
std::string list_tag::to_string(unsigned int tab) {
    std::stringstream ss;

    // form string representation
    ss << generic_tag::to_string(tab) << " (" << value.size() << ") {";
    if (!value.empty()) {
        ss << std::endl;
        for (unsigned int i = 0; i < value.size(); ++i) {
            ss << value.at(i)->to_string(tab + 1) << std::endl;
        }
        generic_tag::append_tabs(tab, ss);
    }
    ss << "}";
    return ss.str();
}
