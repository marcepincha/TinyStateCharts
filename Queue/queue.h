#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>

#define QUEUE_INDEX_TYPE uint16_t

#define MACRO_CREATE_QUEUE(__name,__type,__size)\
/*printf("%s %s %s",#__name,#__type,#__size);*/\
__type queue_##__name##_buffer[__size];\
    queue_t __name = {  .size=__size, .buffer=queue_##__name##_buffer, .sizeData=sizeof(__type),\
                        .count=0, .lastIn=0, .firstOut=0}

typedef struct {
    QUEUE_INDEX_TYPE count;
    QUEUE_INDEX_TYPE size;
	QUEUE_INDEX_TYPE lastIn;
	QUEUE_INDEX_TYPE firstOut;
	uint8_t sizeData;
	void* 	buffer;
} queue_t;



void queue_init(queue_t* this, QUEUE_INDEX_TYPE queue_size, uint8_t data_size, void* buffer);
void* queue_write_FIFO(queue_t* this, const void* in_data);
void* queue_write_LIFO(queue_t* this, const void* in_data);
void*  queue_read(queue_t* this, void* out_data);
void*  queue_peek(queue_t* this, void* out_data);


#endif // QUEUE_H_
