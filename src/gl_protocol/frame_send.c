#include "protocol.h"

PARSE_RETURN send_frame_creat(PARSE_STRUCT *parse_s,FRAME_STRUCT *frame_s,uint8_t *frame_buffer, uint16_t *length)
{
	uint16_t check_sum = 0;
	uint16_t temp_cnt = 2;
	uint16_t count = 0;
	uint16_t frame_flag;

	if(frame_s->FrameDataLen > FRAME_DATA_MAX_SIZE)
	{
		return PARSE_FAIL;
	}
	

    frame_buffer[count++] = VERSION_HEAD;

	frame_buffer[count++] = frame_s->Version;

	frame_buffer[count++] = (uint8_t)frame_s->FrameDataLen;

	if(frame_s->Version == PROTOCOL_VERSION)
	{
		frame_buffer[count++] = (uint8_t)(frame_s->FrameDataLen>>8);
	}

	frame_buffer[count++] = frame_s->SourceID;
	frame_buffer[count++] = frame_s->TargetID;
	frame_buffer[count++] = frame_s->Cmd;

	frame_buffer[count++] = (uint8_t)frame_s->DataIndex;

	if (frame_s->Version == PROTOCOL_VERSION)
	{
		frame_buffer[count++] = (uint8_t)(frame_s->DataIndex >> 8);
	}

	memcpy(frame_buffer + count, frame_s->frame_data, frame_s->FrameDataLen);

	if (frame_s->Version == PROTOCOL_VERSION)
	{
		frame_flag = frame_s->FrameDataLen + 9;
	}
	
	for (temp_cnt = 2; temp_cnt < frame_flag; temp_cnt++)
	{
		check_sum += frame_buffer[temp_cnt];
	}

	check_sum = ~check_sum;

	frame_buffer[frame_flag++] = (uint8_t)check_sum;
	frame_buffer[frame_flag++] = (uint8_t)(check_sum >> 8);

	*length = frame_flag;
	return PARSE_OK;
}

PARSE_RETURN creat_send_cmd(PARSE_STRUCT *parse_s,FRAME_STRUCT *frame_s)
{
	uint16_t frame_len = 0;
	uint8_t frame_buffer[FRAME_MAX];
    
    if(send_frame_creat(parse_s,frame_s, frame_buffer,&frame_len) != PARSE_OK)
	{
		return PARSE_FAIL;
	}
    
    if (frame_s->send_frame_fun != NULL && frame_len > 0)
    {
        frame_s->send_frame_fun(frame_buffer, frame_len);
    }

    return PARSE_OK;
}

