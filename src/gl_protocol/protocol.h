#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gl_protocol_config.h"

#define FRAME_OTHER_SIZE 11
#define FRAME_MAX (FRAME_DATA_MAX_SIZE + FRAME_OTHER_SIZE)

//(11 + 1024*4)
#define VERSION_HEAD 			0xAA

#define PROTOCOL_VERSION    	0xBB

typedef void (*rec_frame_callback)();
typedef void (*send_frame_fun)(uint8_t *arg1, uint16_t arg2);

typedef enum
{
	PARSE_OK,
	PARSE_FAIL
} PARSE_RETURN;

typedef enum
{
	USE_SERIAL,
	USE_TCP
} SEND_RECV_TYPE;

typedef enum
{
	NOHEAD,
	GETHEAD,
	GETVERSION,
	GETLENHALF,
	GETLEN
} PARSE_DATA_STA;

typedef struct frame_struct
{
	uint8_t Version;
	uint16_t FrameDataLen;
	uint8_t SourceID;
	uint8_t TargetID;
	uint8_t Cmd;
	uint16_t DataIndex;
	uint8_t *frame_data;
	send_frame_fun send_frame_fun;
} FRAME_STRUCT;

typedef struct parse_struct
{
	PARSE_DATA_STA sta;
	uint16_t frame_cnt;
	uint16_t frame_data_len;
	uint16_t frame_len;
	uint8_t version;
	uint8_t frame_buffer[FRAME_MAX];
	rec_frame_callback rec_frame_cb;
	FRAME_STRUCT frame_s;
} PARSE_STRUCT;

PARSE_RETURN creat_send_cmd(PARSE_STRUCT *parse_s,FRAME_STRUCT *frame_s);

PARSE_RETURN parse_struct_init(PARSE_STRUCT *parse_s);
void parse_data(PARSE_STRUCT *parse_s, uint8_t *receive_buffer, uint16_t receive_len);

void frame_set_send_fun(FRAME_STRUCT *frame_s, send_frame_fun send_frame_fun);
void parse_set_send_fun(PARSE_STRUCT *parse_s, send_frame_fun send_frame_fun);
void parse_set_rec_callback(PARSE_STRUCT *parse_s, rec_frame_callback rec_frame_cb);

#ifdef __cplusplus
}
#endif

#endif
