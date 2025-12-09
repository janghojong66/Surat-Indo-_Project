/** ****************************************************************************
  * \file    rs485com_thread.c
  * \author  Jang Ho Jong
  * \version V1.0.0
  * \date    2024.07.10
  * \brief   Rs485com Thread Body
  * ***************************************************************************/
/** \remark  Version History
  * V1.0.0   - 최초 버전 - 2024.07.10
*/
/* Includes ------------------------------------------------------------------*/
#include "rs485com_thread.h"
#include "lib_command.h"
#include "lib_buffer.h"
#include "lib_string.h"
 

/*----------------------------------------------------------------------------
 *      'Rs232com_Thread': Communication thread through Serial COM(rs232) to DEPC board
 *---------------------------------------------------------------------------*/

/** \addtogroup Applications
  * \{ */

/*----------------------------------------------------------------------------
 *      한국철도표준규격(CRC_16)
 *      KRS SG 0062 철도신호시스템(점대점 정보전송방식)
 *      MSB(최상의비트)에서 최하위 비트(LSB)로 전송
 *---------------------------------------------------------------------------*/
/* Polly : X16+X15+X2+1 : 0x8005  */
const uint16_t KRS_CRC16_TABLE[256] = {
		0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
		0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
		0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
		0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
		0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
		0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
		0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1, 
		0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082, 
		0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192, 
		0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1, 
		0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1, 
		0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2, 
		0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151, 
		0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162, 
		0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132, 
		0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101, 
		0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312, 
		0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321, 
		0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371, 
		0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342, 
		0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1, 
		0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2, 
		0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2, 
		0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381, 
		0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291, 
		0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2, 
		0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2, 
		0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1, 
		0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252, 
		0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261, 
		0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231, 
		0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};

/** \defgroup Rs232com_Thread RS232통신을 담당하는 쓰레드
  * \brief  객차탈선 장치와 통신 데이타를 처리하는 쓰레드
  * \{ */
 
/** \defgroup Rs232com_Thread_Private_Variables Rs232com_Thread 쓰레드 비공개 함수
  * \{ */


u32_t  g_Rs232com_request_timer = 0;          // 상태요구 패킷 송신 주기 타이머  
Bool_t g_monitor_232_tx;                      // console port를 통한 송신 송신 data monitoring 기능 on/off flag
Bool_t g_monitor_232_rx;                      // console port를 통한 송신 수신 data monitoring 기능 on/off flag

static void   Transmit_Data_To_Rs232(Rs232_t * rs);
static void   Rs232_Data_Fill_txBuf(void);
static void   Rs232_Data_Fill_rxBuf(Rs232_t * rs);
static void   Rs232_Monitor_Tx(void);
static void   Rs232_Monitor_Rx(void);

/** \defgroup Rs485com_Thread_Private_Variables Rs485com_Thread 쓰레드 비공개 변수
  * \{ */

static uint8_t        _rxBuf[2048U];                /**< 수신 버퍼 */
static uint32_t       _rxCnt;                       /**< 수신 버퍼의 입력 카운트 */
static uint8_t        _txBuf[2048U];                /**< 송신 버퍼 */
static uint32_t       _txCnt; 


/** \brief  Transmit_Data_To_Rs485 쓰레드 본체
  * \param  argument - 쓰레드로 전달된 인자
  * \note   RS485통신을 통하여 CU 보드와 PC 보드가 상호 통신하고 CU보드가 PC 보드의 탈선감지
  *         여부를 주기적으로 요청하여 응답을 받는다.
  * \retval 없음
  */
void Rs232com_Thread(void)
{
 
	/* 일정시간(500msec )마다 PC보드 상태 및 탈선 감지 상태에 대한 요구 패킷 송신 */
  if(g_Rs232com_request_timer >= RS232COM_REQUEST_TIME)
  {  
	   g_Rs232com_request_timer = 0U;
		 Transmit_Data_To_Rs232(serial1);
	}
  
  /* PC보드로 부터 데이타 입력이 발생하면 */
  if(serial1->rx_complete == True)
  {
  /* 명령어 버퍼를 수신된 데이터로 채운다. */
    Rs232_Data_Fill_rxBuf(serial1);  ///////
  /* RS-485 시리얼 포트의 수신을 복구시킨다. */
    Rs232_Restore_rx(serial1);
  /* 명령어 버퍼의 내용을 처리한다. */
//    Receive_Data_From_Rs485();
  }

}

static void Transmit_Data_To_Rs232(Rs232_t * rs)
{ 
  /* 송신 버퍼를 채운다. */
  Rs232_Data_Fill_txBuf();
  
  /* 송신 버퍼를 송신한다. */
  Rs232_Transmit_Data(serial1, _txBuf, _txCnt);
  
  /* 송신버퍼를 모니터링 한다  */	
  Rs232_Monitor_Tx();
}

/** \brief  송신 버퍼를 채운다.
  * \note   송신할 내용으로 버퍼를 채우고 버퍼 카운트를 증가시킨다.
  * \retval 없음
*/
static void Rs232_Data_Fill_txBuf()
{
Rs232_Packet_t	*packet = (Rs232_Packet_t*)&_txBuf[0];
  
  packet->stx = 0x02;		      // 메세지 해더(STX)
  _txBuf[1] = 0x01;             // 명령어 길이
  _txBuf[2] = 'S';		          // 자기 진단 명령
  _txBuf[3] = 'A';	            // 메세지 END
  _txBuf[4] = 'M';	            // 
  _txBuf[5] = 'P';              // 
  	_txCnt    = sizeof(Rs232_Packet_t);  	
}

/** \brief  CU보드에서 PC보드로 RS485 Port로 송신하는 내용을 모니터링한다.
  * \note   g_monitor_485com_tx가 켜져 있으면 버퍼 카운트만큼 버퍼 내용을 출력한다.
  * \retval 없음
*/
static void Rs232_Monitor_Tx(void)
{
  uint32_t    i = 0U;

  /* 모니터링 플래그가 꺼져 있으면 종료   or  송신 버퍼 카운트가 0이면 종료 */
  if (g_monitor_232_tx == False || _txCnt == 0)
  {
    return;
  }

  /* 송신 버퍼 카운트만큼 버퍼 내용을 출력한다. */
  cprintf("TX to 232 Port [%3d] ", _txCnt);
  for (i = 0; i < _txCnt; i++)
  {
    cprintf("%02X", _txBuf[i]);
  }
  cprintf("\n");
}

/** \brief  전역변수 _rx_buf를 DEPC로 수신된 데이터로 채운다.
  * \param  rs - RS232 구조체 포인터
  * \note   _rx_buf의 여유공간과 수신된 길이가 _rx_buf 버퍼 크기를 초과한다면
            버퍼를 클리어하고 그렇지 않으면 수신된 길이만큼 버퍼에 복사한다.
  * \retval 없음
  */
static void Rs232_Data_Fill_rxBuf(Rs232_t * rs)
{

  /* 수신된 길이가 없다면 종료 */
  if (rs->rx_count !=0U)
  {
  /* 수신된 길이와 버퍼의 여유 길이의 합이 최대 크기를 초과했다면
     버퍼를 클리어한다. */
		if ((_rxCnt + rs->rx_count) > MAX_SIZE_CMD_BUF)
		{
			_rxCnt = 0U;
			Buffer08_Set(&_rxBuf[0], 0U, MAX_SIZE_CMD_BUF);
		}
  /* 수신된 길이가 적당하다면 버퍼에 복사하고 버퍼 카운트를 수신된 길이만큼 증가시킨다. */
		else
		{
			Buffer08_Copy(&_rxBuf[_rxCnt], rs->rx_buf, rs->rx_count);
			_rxCnt += rs->rx_count;
		}
	}
}

/** \brief  CU 보드에서 수신하는 내용을 모니터링한다.
  * \note   g_monitor_485_rx가 켜져 있으면 버퍼 카운트만큼 버퍼 내용을 출력한다.
  * \retval 없음
*/
static void Rs232_Monitor_Rx(void)
{
  uint32_t    i = 0U;

  /* 모니터링 플래그가 꺼져 있으면 종료 or 수신 버퍼 카운트가 0이면 종료*/
  if (g_monitor_232_rx == False || _rxCnt == 0)
  {
    return;
  }

  /* 수신 버퍼 카운트만큼 버퍼 내용을 출력한다. */
  tprintf("RCPU RX[%3d] ", _rxCnt);
  for (i = 0U; i < _rxCnt; i++)
  {
    cprintf("%02X", _rxBuf[i]);
  }
  cprintf("\n");
}

/*
		temp_crc = getCRC16_KRS(temp_txbuf,59);
		temp_txbuf[59] = (temp_crc & 0xFF);
		temp_txbuf[60] = ((temp_crc & 0xFF00) >> 8);
*/

/** \brief  CRC16 계산하여 결과를 반환한다.
  * \param  crc - 최초 계산에 사용할 값
  * \param  buf - 계산할 데이터가 들어있는 배열
  * \param  l - 계산할 길이
  * \note   l 길이만큼 순차적으로 buf의 데이터와 crc를 연산하여 인덱스를 찾아내고
            KRS_CRC16_TABLE[256]를 통해 crc를 계산한 다음 반환한다.
  * \retval crc - 최종 계산된 CRC값
  */

static uint16_t getCRC16_KRS(uint8_t* buffer, uint32_t l)
{
  uint32_t i;
  uint16_t val=0;

  for (i = 0; i < l; i++)
  {
    val = ((val & 0xFF) << 8) ^ KRS_CRC16_TABLE[((val >> 8) & 0xFF) ^ buffer[i]];
  }
  return val;

}
