/*
 * minecraft_writer.cpp
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


#include "minecraft_writer.hpp"
#include "tag/list_tag.hpp"
#include "tag/byte_tag.hpp"
#include "tag/byte_array_tag.hpp"


minecraft_writer::minecraft_writer(void) {}

/*
 * Specifies the block at the given location in the world
 */
void minecraft_writer::place_block_at(char block, int x, int y, int z) {

    int block_x = x % int(region_dim::BLOCK_WIDTH);
    int block_y = y % region_dim::BLOCK_WIDTH;
    int block_z = z % int(region_dim::BLOCK_WIDTH);

    block_x += ((x < 0 && block_x != 0) ? region_dim::BLOCK_WIDTH : 0);
    block_z += ((z < 0 && block_z != 0) ? region_dim::BLOCK_WIDTH : 0);

    compound_tag* sec = get_section_tag_at(x, y, z); //section to be used

    byte_array_tag* blocks = static_cast<byte_array_tag*>(sec->get_subtag("Blocks")); //get the blocks

    blocks->get_value()[block_y * region_dim::BLOCK_WIDTH * region_dim::BLOCK_WIDTH +
                        block_z * region_dim::BLOCK_WIDTH + block_x] = block; //assign the block value at the location

}

/*
 * Specifies the block data at the given location in the world
 */
void minecraft_writer::place_data_at(char block_data, int x, int y, int z) {

    int block_x = x % int(region_dim::BLOCK_WIDTH);
    int block_y = y % region_dim::BLOCK_WIDTH;
    int block_z = z % int(region_dim::BLOCK_WIDTH);

    block_x += ((x < 0 && block_x != 0) ? region_dim::BLOCK_WIDTH : 0);
    block_z += ((z < 0 && block_z != 0) ? region_dim::BLOCK_WIDTH : 0);

    compound_tag* sec = get_section_tag_at(x, y, z); //section to be used

    byte_array_tag* data = static_cast<byte_array_tag*>(sec->get_subtag("Data")); //get the additional data

    if (!data) { //if the data is not there
        data = new byte_array_tag("Data"); //make it
        sec->push_back(data); //add the data to the section
    }
    char mask;
    if (block_x % 2) { //if the location of the block is odd valued, we will need to shift the bits
        block_data = (block_data << 4);
        mask = 15; //00001111
    } else {
        mask = 240; //11110000
    }
    (data->get_value()[(block_y * region_dim::BLOCK_WIDTH * region_dim::BLOCK_WIDTH +
                        block_z * region_dim::BLOCK_WIDTH + block_x) / 2] &= mask) |= block_data; //assign the data value at the location
}

/*
 * Returns a region file writer at the needed, creating it if needed
 */
region_file_writer* minecraft_writer::get_region(int reg_x, int reg_z) {

    std::pair<int, int> target(reg_x, reg_z);

    if (!regions[target]) { //if the region writer has not been made, make one
        std::stringstream name;
        name << "r." << reg_x << "." << reg_z << ".mca"; //format the region name
        region_file_writer* region = new region_file_writer(name.str()); //make it
        regions[target] = region; //save it
    }

    return regions[target]; //return the region
}

/*
 * Returns the chunk tag for the current position, creating it as needed.
 */
compound_tag* minecraft_writer::get_section_tag_at(int x, int y, int z) {

    static int CHUNK_WIDTH = int(region_dim::CHUNK_WIDTH);
    static int BLOCK_WIDTH = int(region_dim::BLOCK_WIDTH);

    //region location
    int reg_x = std::floor(1. * x / int(CHUNK_WIDTH * BLOCK_WIDTH));
    int reg_z = std::floor(1. * z / int(CHUNK_WIDTH * BLOCK_WIDTH));

    //chunk locations within regions
    int chunk_x = (x - reg_x * CHUNK_WIDTH * BLOCK_WIDTH) / BLOCK_WIDTH % CHUNK_WIDTH;
    int chunk_z = (z - reg_z * CHUNK_WIDTH * BLOCK_WIDTH) / BLOCK_WIDTH % CHUNK_WIDTH;
    int chunk_y = y / BLOCK_WIDTH;
    int chunk_ind = chunk_z * CHUNK_WIDTH + chunk_x;

    //block locations within chunk
    int block_x = (x - reg_x * CHUNK_WIDTH * BLOCK_WIDTH) % BLOCK_WIDTH;
    int block_z = (z - reg_z * CHUNK_WIDTH * BLOCK_WIDTH) % BLOCK_WIDTH;
    int block_y = y % BLOCK_WIDTH;

    region_file_writer* reg = get_region(reg_x, reg_z); //get the region of interest

    if (!reg->get_region().is_filled(chunk_ind)) { //if the chunk hasn't been generated yet,
        reg->generate_chunk(chunk_x, chunk_z); //generate it
    }

    compound_tag* level = static_cast<compound_tag*>(reg->get_region().get_tag_at(chunk_ind).
        get_root_tag().get_subtag("Level")); //get the level tag

    list_tag* sections = static_cast<list_tag*>(level->get_subtag("Sections")); //get all sections
    compound_tag* sec = NULL; //section to be used

    for (int i = 0; i < sections->size(); i++) { //iterate over all sections
        sec = static_cast<compound_tag*>(sections->at(i));
        byte_tag* sec_y = static_cast<byte_tag*>(sec->get_subtag("Y"));
        if (sec_y->get_value() == char(chunk_y)) //if the Y is the one we needed, break
            break;
        sec = NULL;
    }

    if (!sec) { //section wasn't found, we'll make our own
        sec = new compound_tag();

        byte_tag* Y = new byte_tag("Y", chunk_y);

        byte_array_tag* blocks = new byte_array_tag("Blocks");
        //byte_array_tag* add = new byte_array_tag("Add");
        byte_array_tag* data = new byte_array_tag("Data");
        byte_array_tag* blocklight = new byte_array_tag("BlockLight");
        byte_array_tag* skylight = new byte_array_tag("SkyLight");

        //for (int i=0;i<4096;i++) //make 4096 spaces
        //	blocks->push_back(0);

        //for (int i=0;i<2048;i++){ //make 2048 spaces
        //	//add->push_back(0);
        //	data->push_back(0);
        //	blocklight->push_back(0);
        //	skylight->push_back(0);
        //}

        std::vector<char> blocks_vec(4096, 0);
        std::vector<char> data_vec(2048, 0);
        std::vector<char> blocklight_vec(2048, 0);
        std::vector<char> skylight_vec(2048, 0);

        blocks->set_value(blocks_vec);
        data->set_value(data_vec);
        blocklight->set_value(blocklight_vec);
        skylight->set_value(skylight_vec);

        sec->push_back(Y);
        sec->push_back(blocks);
        sec->push_back(data);
        sec->push_back(blocklight);
        sec->push_back(skylight);
        //sec->push_back(add);

        sections->push_back(sec); //stick the section into the chunk
    }

    return sec; //return the section, however we found it
}

/*
 * Writes all the regions to their respective files.
 */
void minecraft_writer::write(void) {

    for (std::map<std::pair<int, int>, region_file_writer*>::iterator iter = regions.begin();
         iter != regions.end(); iter++) { //iterate over all regions

        iter->second->write(); //write themselves
    }
}
