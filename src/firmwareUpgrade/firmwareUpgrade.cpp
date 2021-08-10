#include "firmwareUpgrade.h"

OPENVIO *globalVio;
FirmwareUpgrade *firmwareUpgrade;
PARSE_STRUCT parse_usb;
uint8_t is_usb_recv = 0;

static void usb_printf_frame(FRAME_STRUCT *frame_s)
{
    printf("Version:%02X ", frame_s->Version);
    printf("FrameDataLen:%02X ", frame_s->FrameDataLen);
    printf("SourceID:%02X ", frame_s->SourceID);
    printf("TargetID:%02X ", frame_s->TargetID);
    printf("Cmd:%02X ", frame_s->Cmd);
    printf("DataIndex:%02X ", frame_s->DataIndex);
    printf("\r\n");
}

void recv_usb_farme(void *arg)
{
    //usb_printf_frame(&parse_usb.frame_s);
    if (parse_usb.frame_s.TargetID == LOCAL_ID)
    {
        if(parse_usb.frame_s.Cmd == CMD_IAP_ACK)
        {
            if(parse_usb.frame_s.FrameDataLen == 0)
            {
                if(parse_usb.frame_s.DataIndex == IAP_OK)
                {
                    firmwareUpgrade->reply_status = REPLY_OK;
                }else{
                    firmwareUpgrade->reply_status = REPLY_FAIL;
                    firmwareUpgrade->errorCode = parse_usb.frame_s.DataIndex;
                }
            }
        }
    }
    else
    {
    }
}

void send_usb_data(uint8_t *send_buffer, uint16_t send_len)
{
    globalVio->sendBulk(send_buffer, send_len);
}

FirmwareUpgrade::FirmwareUpgrade(OPENVIO *vio)
{
    parse_struct_init(&parse_usb);
    parse_set_rec_callback(&parse_usb, (rec_frame_callback)recv_usb_farme);
    parse_set_send_fun(&parse_usb, send_usb_data);

    firmwareUpgrade = this;
    globalVio = vio;
    this->vio = vio;

    upgradeThread = new UpgradeThread(this);
    connect(upgradeThread, SIGNAL(endSignals()), this, SLOT(endSlot()));


    upgradeRecvThread = new UpgradeRecvThread(vio);
    connect(upgradeRecvThread, SIGNAL(recvSignals(unsigned char *, int)), this, SLOT(recvSlot(unsigned char *, int)));
    
}

void FirmwareUpgrade::setBinPath(QString binPath)
{
    upgradeThread->setBinPath(binPath);
}

void FirmwareUpgrade::endSlot(void)
{
    upgradeRecvThread->is_loop = false;
}

int send_hello(void)
{
    unsigned char send_buf[6] = "hello";

    FRAME_STRUCT frame_s_tmp;
    frame_s_tmp.Version = PROTOCOL_VERSION;
    frame_s_tmp.FrameDataLen = 3;
    frame_s_tmp.SourceID = LOCAL_ID;
    frame_s_tmp.TargetID = TARGET_ID;
    frame_s_tmp.Cmd = 0;
    frame_s_tmp.DataIndex = 0x00;
    frame_s_tmp.frame_data = send_buf;
    frame_s_tmp.send_frame_fun = send_usb_data;

    return creat_send_cmd(&parse_usb, &frame_s_tmp);
}


void FirmwareUpgrade::upgradeStart()
{
    upgradeRecvThread->start();
    upgradeThread->start();
}

void FirmwareUpgrade::recvSlot(unsigned char *buffer, int len)
{
    parse_data(&parse_usb, buffer, len);
}

void FirmwareUpgrade::send_iap_begin(unsigned int size)
{
    unsigned char send_buf[4];

    send_buf[0] = (unsigned char)size;
    send_buf[1] = (unsigned char)(size>>8);
    send_buf[2] = (unsigned char)(size>>16);
    send_buf[3] = (unsigned char)(size>>24);

    frame_s_tmp.Version = PROTOCOL_VERSION;
    frame_s_tmp.FrameDataLen = 4;
    frame_s_tmp.SourceID = LOCAL_ID;
    frame_s_tmp.TargetID = TARGET_ID;
    frame_s_tmp.Cmd = CMD_IAP_BEGIN;
    frame_s_tmp.DataIndex = 0x00;
    frame_s_tmp.frame_data = send_buf;
    frame_s_tmp.send_frame_fun = send_usb_data;

    creat_send_cmd(&parse_usb, &frame_s_tmp);
}

void FirmwareUpgrade::send_iap_trans(unsigned short index,unsigned char *buffer,unsigned int len)
{

    frame_s_tmp.Version = PROTOCOL_VERSION;
    frame_s_tmp.FrameDataLen = len;
    frame_s_tmp.SourceID = LOCAL_ID;
    frame_s_tmp.TargetID = TARGET_ID;
    frame_s_tmp.Cmd = CMD_IAP_TRANS;
    frame_s_tmp.DataIndex = index;
    frame_s_tmp.frame_data = buffer;
    frame_s_tmp.send_frame_fun = send_usb_data;

    creat_send_cmd(&parse_usb, &frame_s_tmp);
}

void FirmwareUpgrade::send_iap_crc(unsigned int crc)
{
    unsigned char send_buf[4];

    send_buf[0] = (unsigned char)crc;
    send_buf[1] = (unsigned char)(crc>>8);
    send_buf[2] = (unsigned char)(crc>>16);
    send_buf[3] = (unsigned char)(crc>>24);


    frame_s_tmp.Version = PROTOCOL_VERSION;
    frame_s_tmp.FrameDataLen = 4;
    frame_s_tmp.SourceID = LOCAL_ID;
    frame_s_tmp.TargetID = TARGET_ID;
    frame_s_tmp.Cmd = CMD_IAP_VERIFY;
    frame_s_tmp.DataIndex = 0x00;
    frame_s_tmp.frame_data = send_buf;
    frame_s_tmp.send_frame_fun = send_usb_data;

    creat_send_cmd(&parse_usb, &frame_s_tmp);
}

void FirmwareUpgrade::send_iap_reboot()
{
    frame_s_tmp.Version = PROTOCOL_VERSION;
    frame_s_tmp.FrameDataLen = 0;
    frame_s_tmp.SourceID = LOCAL_ID;
    frame_s_tmp.TargetID = TARGET_ID;
    frame_s_tmp.Cmd = CMD_IAP_RESET;
    frame_s_tmp.DataIndex = 0x00;
    frame_s_tmp.frame_data = NULL;
    frame_s_tmp.send_frame_fun = send_usb_data;

    creat_send_cmd(&parse_usb, &frame_s_tmp);
}