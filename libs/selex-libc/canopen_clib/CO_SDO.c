#include "../../../libs/selex-libc/canopen_clib/CO_SDO.h"

#include "stdlib.h"
#include "stdio.h"

#include "../../../libs/selex-libc/canopen_clib/CO_OD.h"
#include "../../../libs/selex-libc/canopen_clib/CO_utils.h"

static inline void sdo_process_init_transfer_msg(CO_SDO* p_sdo,const CO_OD* p_od);
static inline int32_t sdo_start_write_to_object(CO_SDO* p_sdo,const CO_OD* p_od);
static inline int32_t sdo_start_read_from_object(CO_SDO* p_sdo,const CO_OD* p_od);
static inline int32_t sdo_parse_object_mux(CO_SDO* p_sdo,const CO_OD* p_od);
static inline int32_t sdo_read_segment_from_object(CO_SDO* p_sdo);
static inline int32_t sdo_write_segment_to_object(CO_SDO* p_sdo);

void sdo_init(CO_SDO* p_sdo,const uint16_t client_to_server_id,
		const uint16_t server_to_client_id){

	if(p_sdo==NULL) return;
	p_sdo->client_server_id=client_to_server_id;
	p_sdo->server_client_id=server_to_client_id;
}

static inline int32_t sdo_check_data_exist(CO_SDO* p_sdo,CO_OD* p_od){

	CO_Object* p_obj =NULL;
	sdo_parse_object_mux(p_sdo,p_od);

        if(p_sdo->entryNo ==OD_UNSUPPORT_ENTRY){
        	sdo_abort(p_sdo,CO_SDO_AB_NOT_EXIST);
        	return -1;
        }
        p_obj=&p_od->entries[p_sdo->entryNo];

        if(p_obj->sub_no < p_sdo->sub){

        	sdo_abort(p_sdo,CO_SDO_AB_SUB_UNKNOWN);
        	return -1;
        }
        return 0;
}

int32_t sdo_process(CO_SDO* p_sdo,CO_OD* p_od,const uint32_t time_diff){

	if((p_sdo->state==CO_SDO_ST_IDLE) && (p_sdo->rx_msg->is_new==false))
		return 0;

	uint8_t ccs= p_sdo->rx_msg->data[0];
        uint8_t cs= (ccs >>5); /* get 3 bits of CCS */
       /* unsupport message command type */
        if((cs != CCS_DOWNLOAD_INITIATE) && (cs!= CCS_UPLOAD_INITIATE) &&
        		(cs != CCS_DOWNLOAD_SEGMENT) && ( cs!= CCS_UPLOAD_SEGMENT)){
        	sdo_abort(p_sdo,CO_SDO_AB_CMD);
        	return -1;
        }

        if(cs==CCS_ABORT){
                p_sdo->state=CO_SDO_ST_IDLE;
                return -1;
        }

        p_sdo->timeout_tmr+= time_diff;
        if(sdo_is_timeout(p_sdo)==true){
                sdo_abort(p_sdo,CO_SDO_AB_TIMEOUT);
                return -1;
        }

        /* write data to an object on server */
        if(cs==CCS_DOWNLOAD_INITIATE){
        	if(p_sdo->state != CO_SDO_ST_IDLE){
        		sdo_abort(p_sdo,CO_SDO_AB_NO_RESOURCE);
        		return -1;
        	}

        	if(sdo_check_data_exist(p_sdo,p_od) != 0) return -1;

        	return sdo_start_write_to_object(p_sdo,p_od);
        }
        /* read data from an object on server */

        if(cs==CCS_UPLOAD_INITIATE){

        	if(p_sdo->state != CO_SDO_ST_IDLE){
        		sdo_abort(p_sdo,CO_SDO_AB_NO_RESOURCE);
        		return -1;
        	}
        	if(sdo_check_data_exist(p_sdo,p_od) != 0) return -1;

                return sdo_start_read_from_object(p_sdo,p_od);
        }

        if(cs==CCS_DOWNLOAD_SEGMENT){

        	if((p_sdo->state != CO_SDO_ST_DOWNLOAD_INITIATE) &&
        			(p_sdo->state != CO_SDO_ST_DOWNLOAD_SEGMENTED)){
        		sdo_abort(p_sdo,CO_SDO_AB_NO_RESOURCE);
        		return -1;
        	}
                sdo_write_segment_to_object(p_sdo);
                return 1;
        }

        if(cs==CCS_UPLOAD_SEGMENT){

        	if((p_sdo->state != CO_SDO_ST_UPLOAD_INITIATE) &&
        			(p_sdo->state != CO_SDO_ST_UPLOAD_SEGMENTED)){

        		sdo_abort(p_sdo,CO_SDO_AB_NO_RESOURCE);
        		return -1;
        	}
                 /* toggle segments bit */ 
                p_sdo->tx_msg.data[0] ^= (1UL<<4);
                sdo_read_segment_from_object(p_sdo);
                return 1;
        }

        return 0;
}

static inline int32_t sdo_write_segment_to_object(CO_SDO* p_sdo){

	uint8_t* dst=p_sdo->object_data_buffer+p_sdo->buffer_offset;
	if(p_sdo->data_len < p_sdo->buffer_offset+7){
		/* if remain byte < message length - 7bytes */
		CO_memcpy(dst,&p_sdo->rx_msg->data[1],p_sdo->data_len-p_sdo->buffer_offset);
		p_sdo->state=CO_SDO_ST_IDLE;
		return 0;
	}

	CO_memcpy(dst,&p_sdo->rx_msg->data[1],7);
        p_sdo->buffer_offset+=7;
        p_sdo->state=CO_SDO_ST_DOWNLOAD_SEGMENTED;
        return 1;
}

static inline int32_t sdo_read_segment_from_object(CO_SDO* p_sdo){

	uint8_t* src=p_sdo->object_data_buffer+ p_sdo->buffer_offset;

	/* if remain bytes is < message length - 7 bytes */
        if(p_sdo->data_len  < p_sdo->buffer_offset+7){

        	CO_memcpy(&p_sdo->tx_msg.data[1],src,
        			p_sdo->data_len-p_sdo->buffer_offset);
        	/* set last segment indicator bit */
        	p_sdo->tx_msg.data[0] |= (1<<0);
                p_sdo->state=CO_SDO_ST_IDLE;
                return 0;
        }

        CO_memcpy(&p_sdo->tx_msg.data[1],src,7);

        p_sdo->state=CO_SDO_ST_UPLOAD_SEGMENTED;
        p_sdo->buffer_offset+=8;
        return 1;
}

static inline int32_t sdo_start_write_to_object(CO_SDO* p_sdo,const CO_OD* p_od){

        sdo_process_init_transfer_msg(p_sdo,p_od);

        uint8_t ccs=p_sdo->rx_msg->data[0];
        //p_sdo->state = CO_SDO_ST_DOWNLOAD_INITIATE;
        p_sdo->tx_msg.data[0]= SCS_DOWNLOAD_INITIATED<<5;
        /* if expitide transfer */
        if(ccs & (1UL<<1)){
        	p_sdo->data_len= (ccs & 0x0C) >>2;
                CO_memcpy(p_sdo->object_data_buffer,
                                &p_sdo->rx_msg->data[4],p_sdo->data_len);        
                p_sdo->state= CO_SDO_ST_IDLE;
                return 0;
        }

        p_sdo->data_len= CO_getUint32(&(p_sdo->rx_msg->data[4]));
        p_sdo->state = CO_SDO_ST_DOWNLOAD_INITIATE;
        return 1;
}

static inline int32_t sdo_start_read_from_object(CO_SDO* p_sdo,const CO_OD* p_od){

        sdo_process_init_transfer_msg(p_sdo,p_od);

        p_sdo->state = CO_SDO_ST_UPLOAD_INITIATE;
        p_sdo->tx_msg.data[0]= SCS_UPLOAD_INITIATE<<5;
        p_sdo->data_len= p_sdo->object_data_len;
        /* if expedited transfer */
        if(p_sdo->data_len <= 4) {
               p_sdo->tx_msg.data[0] += (p_sdo->data_len <<2);
               p_sdo->tx_msg.data[0] |= (1UL<<1);
               CO_memcpy(&p_sdo->tx_msg.data[4],p_sdo->object_data_buffer,p_sdo->data_len);
                p_sdo->state= CO_SDO_ST_IDLE;
                return 0;
        }

        p_sdo->tx_msg.data[0] &= ~(1UL<<1);
        p_sdo->state= CO_SDO_ST_UPLOAD_INITIATE;
        return 1;
}

static inline void sdo_process_init_transfer_msg(CO_SDO* p_sdo,const CO_OD* p_od){

        /* reset buffer */
        p_sdo->sequence=0;
        p_sdo->buffer_offset=0;
        p_sdo->timeout_tmr=0;
        /* copy mux - index and sub index data bytes */
        p_sdo->tx_msg.data[1] = p_sdo->rx_msg->data[1];
        p_sdo->tx_msg.data[2] = p_sdo->rx_msg->data[2];
        p_sdo->tx_msg.data[3] = p_sdo->rx_msg->data[3];
}

/* parse index, sub index mux from message data and retrieve object info in od */
static inline int32_t sdo_parse_object_mux(CO_SDO* p_sdo,const CO_OD* p_od){
       
       p_sdo->index = CO_getUint16(&p_sdo->rx_msg->data[1]); 
       p_sdo->sub= p_sdo->rx_msg->data[3];
       p_sdo->entryNo= od_get_entry(p_od,p_sdo->index);
       p_sdo->object_data_buffer= (uint8_t*)od_get_data_pointer(p_od,p_sdo->entryNo,p_sdo->sub);
       p_sdo->object_data_len  = od_get_data_len(p_od,p_sdo->entryNo,p_sdo->sub);
       return 0;
}

void sdo_abort(CO_SDO* p_sdo,const uint32_t code){
        
        CO_memcpy(&(p_sdo->tx_msg.data[4]),(uint8_t*) &code,4);
        p_sdo->state=CO_SDO_ST_IDLE;
}
