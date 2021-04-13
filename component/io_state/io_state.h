/*
 * io_state.h
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_IO_STATE_IO_STATE_H_
#define COMPONENT_IO_STATE_IO_STATE_H_

#include "stdlib.h"

typedef enum IO_STATE{
	IO_ST_ON,
	IO_ST_OFF
} IO_STATE;

typedef struct IO_State_t IO_State;
struct IO_State_t{
	IO_STATE	state;
	IO_STATE	(*io_get_state)(IO_State* p_ios);
	void		(*io_init)(IO_State* p_io);
};

IO_State* ios_construct(void);

static inline IO_STATE io_get_state(IO_State* p_ios){
	 return p_ios->io_get_state(p_ios);
}

#endif /* COMPONENT_IO_STATE_IO_STATE_H_ */
