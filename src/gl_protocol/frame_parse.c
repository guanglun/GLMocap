#include "protocol.h"

const unsigned char AUTH_PARAM[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

PARSE_RETURN parse_to_frame_struct(PARSE_STRUCT *parse_s);
int check_frame(uint8_t *frame_buffer, uint16_t frame_len);

PARSE_RETURN parse_struct_init(PARSE_STRUCT *parse_s)
{
    parse_s->sta = NOHEAD;
    parse_s->frame_cnt = 0;
    parse_s->frame_data_len = 0;
    parse_s->frame_len = 0;
    parse_s->version = 0;
    parse_s->rec_frame_cb = NULL;

    return PARSE_OK;
}

void parse_set_send_fun(PARSE_STRUCT *parse_s, send_frame_fun send_frame_fun)
{
    parse_s->frame_s.send_frame_fun = send_frame_fun;
}

void frame_set_send_fun(FRAME_STRUCT *frame_s, send_frame_fun send_frame_fun)
{
    frame_s->send_frame_fun = send_frame_fun;
}

void parse_set_rec_callback(PARSE_STRUCT *parse_s, rec_frame_callback rec_frame_cb)
{
    parse_s->rec_frame_cb = rec_frame_cb;
}

void reset_frame(PARSE_STRUCT *parse_s, uint8_t recv)
{
    if (recv == VERSION_HEAD)
    {
        parse_s->sta = GETHEAD;
        parse_s->frame_cnt = 0;
        parse_s->frame_data_len = 0;
        parse_s->version = 0;
    }
    else
    {
        parse_s->sta = NOHEAD;
    }
}

void parse_data(PARSE_STRUCT *parse_s, uint8_t *receive_buffer, uint16_t receive_len)
{
    uint16_t data_cnt = 0;
    for (; data_cnt < receive_len; data_cnt++)
    {
        switch (parse_s->sta)
        {
        //NOHEAD
        case NOHEAD:
            if (receive_buffer[data_cnt] == VERSION_HEAD)
            {
                parse_s->sta = GETHEAD;
                parse_s->frame_cnt = 0;
                parse_s->frame_data_len = 0;
                parse_s->version = 0;
            }
            break;
            //GETHEAD
        case GETHEAD:
            if (receive_buffer[data_cnt] == PROTOCOL_VERSION)
            {
                parse_s->sta = GETVERSION;
                parse_s->version = PROTOCOL_VERSION;
            }
            else
            {
                reset_frame(parse_s, receive_buffer[data_cnt]);
            }
            break;
            //GETVERSION
        case GETVERSION:
            if (parse_s->version == PROTOCOL_VERSION)
            {
                parse_s->frame_data_len = receive_buffer[data_cnt];
                parse_s->sta = GETLENHALF;
            }
            else
            {
                reset_frame(parse_s, receive_buffer[data_cnt]);
            }
            break;
            //GETLENHALF
        case GETLENHALF:
            if (parse_s->version == PROTOCOL_VERSION)
            {
                parse_s->frame_data_len |= (uint16_t)(receive_buffer[data_cnt] << 8);

                if (parse_s->frame_data_len > FRAME_MAX)
                {
                    parse_s->sta = NOHEAD;
                }
                else
                {
                    if ((parse_s->frame_data_len + 11) <= sizeof(parse_s->frame_buffer))
                    {
                        *(parse_s->frame_buffer + 0) = VERSION_HEAD;
                        *(parse_s->frame_buffer + 1) = PROTOCOL_VERSION;
                        *(parse_s->frame_buffer + 2) = (uint8_t)(parse_s->frame_data_len);
                        *(parse_s->frame_buffer + 3) = receive_buffer[data_cnt];

                        parse_s->sta = GETLEN;
                    }
                    else
                    {
                        parse_s->sta = NOHEAD;
                    }
                }
            }
            else
            {
                reset_frame(parse_s, receive_buffer[data_cnt]);
            }
            break;
            //GETLEN
        case GETLEN:
            if (parse_s->version == PROTOCOL_VERSION)
            {
                *(parse_s->frame_buffer + 4 + parse_s->frame_cnt) = receive_buffer[data_cnt];
                parse_s->frame_cnt++;

                if (parse_s->frame_cnt >= parse_s->frame_data_len + 7)
                {

                    if (check_frame(parse_s->frame_buffer + 2, parse_s->frame_data_len + 9) == 0)
                    {

                        parse_s->frame_len = parse_s->frame_data_len + 11;
                        parse_to_frame_struct(parse_s);

                        if (parse_s->rec_frame_cb != NULL)
                        {
                            parse_s->rec_frame_cb();
                        }

                        parse_s->sta = NOHEAD;
                    }
                    else
                    {
                        reset_frame(parse_s, receive_buffer[data_cnt]);
                    }
                }
            }
            else
            {
                reset_frame(parse_s, receive_buffer[data_cnt]);
            }
            break;

        default:
            reset_frame(parse_s, receive_buffer[data_cnt]);
            break;
        }
    }
}

int check_frame(uint8_t *frame_buffer, uint16_t frame_len)
{
    uint16_t frame_cnt = 0;
    uint16_t check_sum = 0;
    uint8_t check_sum_low = 0, check_sum_high = 0;

    for (frame_cnt = 0; frame_cnt < frame_len - 2; frame_cnt++)
    {
        check_sum += frame_buffer[frame_cnt];
    }

    check_sum = ~check_sum;

    check_sum_low = (uint8_t)check_sum;
    check_sum_high = (uint8_t)(check_sum >> 8);

    if (check_sum_low == frame_buffer[frame_len - 2] && check_sum_high == frame_buffer[frame_len - 1])
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

PARSE_RETURN parse_to_frame_struct(PARSE_STRUCT *parse_s)
{
    uint16_t count = 3;

    parse_s->frame_s.Version = parse_s->version;
    parse_s->frame_s.FrameDataLen = parse_s->frame_data_len;

    if (parse_s->version == PROTOCOL_VERSION)
    {
        count++;
    }

    parse_s->frame_s.SourceID = parse_s->frame_buffer[count++];
    parse_s->frame_s.TargetID = parse_s->frame_buffer[count++];
    parse_s->frame_s.Cmd = parse_s->frame_buffer[count++];
    parse_s->frame_s.DataIndex = parse_s->frame_buffer[count++];

    if (parse_s->version == PROTOCOL_VERSION)
    {
        parse_s->frame_s.DataIndex |= (parse_s->frame_buffer[count++] << 8);
    }

    parse_s->frame_s.frame_data = parse_s->frame_buffer + count;

    return PARSE_OK;
}
