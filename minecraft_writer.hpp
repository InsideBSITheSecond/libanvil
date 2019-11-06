/*
 * minecraft_writer.hpp
 * Copyright (C) 2015 Nikifar Lazouski
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

#ifndef MINECRAFT_WRITER_HPP_
#define MINECRAFT_WRITER_HPP_

#include <vector>
#include <map>
#include "region_file_writer.hpp"
#include "region_dim.hpp"


class minecraft_writer {
private:
    /*
     * Map for holding region writer pointers, (x,z) pair for a key
     */
    std::map<std::pair<int, int>, region_file_writer*> regions;

    void cleanup(void) {
        for (std::map<std::pair<int, int>, region_file_writer*>::iterator iter = regions.begin();
             iter != regions.end(); iter++) { //iterate over all regions

            delete iter->second; //delete reions
        }
    }

public:

    /*
     * No-arg minecraft_writer constructor
     */
    minecraft_writer(void);

    /*
     * Specifies the block at the given location in the world
     */
    void place_block_at(char block, int x, int y, int z);

    /*
     * Specifies the block data at the given location in the world
     */
    void place_data_at(char data, int x, int y, int z);

    /*
     * Returns a region file writer at the needed, creating it if needed
     */
    region_file_writer* get_region(int reg_x, int reg_z);

    /*
     * Writes all the regions to their respective files.
     */
    void write(void);

    /*
     * Returns the section tag for the current position, creating it as needed.
     */
    compound_tag* get_section_tag_at(int x, int y, int z);
};

#endif
