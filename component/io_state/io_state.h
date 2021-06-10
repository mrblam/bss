/*
 * io_state.h
 *
 *  Created on: Apr 10, 2021
 *      Author: KhanhDinh
 */

#ifndef COMPONENT_IO_STATE_IO_STATE_H_
#define COMPONENT_IO_STATE_IO_STATE_H_

#include "stdlib.h"
#include "stdint.h"

typedef enum IO_STATE{
	IO_ST_OFF 	= 0,
	IO_ST_ON 	= 1,
	IO_ST_FAIL	= 2
} IO_STATE;

typedef struct IO_State_t IO_State;
typedef uint8_t (*ios_act)(IO_State* p_ios);

struct IO_State_t{
	IO_STATE	state;
	ios_act		get_io_state;
	void		(*ios_init)(IO_State* p_io);
};

IO_State* ios_construct(void);

static inline uint8_t io_get_state(IO_State* p_ios){
	 return p_ios->get_io_state(p_ios);
}

#endif /* COMPONENT_IO_STATE_IO_STATE_H_ */
