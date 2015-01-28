/*
 * flash.h
 *
 *  Created on: Feb 26, 2012
 *      Author: Mike
 */

#ifndef FLASH_H_
#define FLASH_H_


void init_Flash_funcs(void) ;
Uint16 erase_FlashH(void) ;
void write_data_to_flash(Uint16 *DataPrt,Uint16 Length,Uint16 Add) ;
void read_data_from_flash(Uint16 *DataPrt,Uint16 Length,Uint16 offset) ;
#endif /* FLASH_H_ */
