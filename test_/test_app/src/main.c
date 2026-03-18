#include <stdio.h>
#include <string.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xaxivdma.h"
#include "xv_tpg.h"
#include "xil_cache.h"

// 주소 정의 (xparameters.h와 대조 완료)
#define VDMA_BASEADDR    XPAR_AXI_VDMA_0_BASEADDR
#define TPG_BASEADDR     XPAR_V_TPG_0_BASEADDR
#define MEM_BASE_ADDR    0x01000000 
#define FRAME_WIDTH      1280U
#define FRAME_HEIGHT     720U
#define BYTES_PER_PIXEL  3U

XAxiVdma VdmaInst;
XV_tpg TpgInst;

int main() {
    int Status;
    Xil_DCacheDisable(); 
    xil_printf("\r\n--- Video Pipeline Start (Final Fix) ---\r\n");

    /* 1. TPG 초기화 및 설정 */
    XV_tpg_Config *TpgConfig = XV_tpg_LookupConfig(TPG_BASEADDR);
    if (TpgConfig == NULL) {
        xil_printf("TPG LookupConfig failed\r\n");
        while(1);
    }
    XV_tpg_CfgInitialize(&TpgInst, TpgConfig, TpgConfig->BaseAddress);

    XV_tpg_Set_height(&TpgInst, FRAME_HEIGHT);
    XV_tpg_Set_width(&TpgInst, FRAME_WIDTH);
    
    // [수정 완료] 헤더 154번 라인 이름: Set_bckgndId
    XV_tpg_Set_bckgndId(&TpgInst, XTPG_BKGND_COLOR_BARS); 
    
    XV_tpg_EnableAutoRestart(&TpgInst);
    XV_tpg_Start(&TpgInst);
    xil_printf("1. TPG Running (Color Bars)\r\n");

    /* 2. VDMA 초기화 및 설정 */
    XAxiVdma_Config *VdmaConfig = XAxiVdma_LookupConfig(VDMA_BASEADDR);
    if (VdmaConfig == NULL) {
        xil_printf("VDMA LookupConfig failed\r\n");
        while(1);
    }
    XAxiVdma_CfgInitialize(&VdmaInst, VdmaConfig, VdmaConfig->BaseAddress);
    XAxiVdma_Reset(&VdmaInst, XAXIVDMA_WRITE);
    XAxiVdma_Reset(&VdmaInst, XAXIVDMA_READ);
    while (XAxiVdma_ResetNotDone(&VdmaInst, XAXIVDMA_WRITE) ||
           XAxiVdma_ResetNotDone(&VdmaInst, XAXIVDMA_READ)) {
    }

    // [수정 완료] 헤더 468번 라인 구조체 이름: XAxiVdma_DmaSetup
    XAxiVdma_DmaSetup WriteCfg;
    XAxiVdma_DmaSetup ReadCfg;
    memset(&WriteCfg, 0, sizeof(XAxiVdma_DmaSetup)); 
    memset(&ReadCfg, 0, sizeof(XAxiVdma_DmaSetup));

    WriteCfg.VertSizeInput = FRAME_HEIGHT;
    WriteCfg.HoriSizeInput = FRAME_WIDTH * BYTES_PER_PIXEL;
    WriteCfg.Stride = FRAME_WIDTH * BYTES_PER_PIXEL;
    WriteCfg.EnableCircularBuf = 1;
    WriteCfg.EnableSync = 0; // Free Run

    ReadCfg.VertSizeInput = FRAME_HEIGHT;
    ReadCfg.HoriSizeInput = FRAME_WIDTH * BYTES_PER_PIXEL;
    ReadCfg.Stride = FRAME_WIDTH * BYTES_PER_PIXEL;
    ReadCfg.EnableCircularBuf = 1;
    ReadCfg.EnableSync = 0; // Free Run
    
    // 메모리 주소 설정
    UINTPTR FrameAddr[3] = {MEM_BASE_ADDR, MEM_BASE_ADDR + 0x1000000, MEM_BASE_ADDR + 0x2000000};

    Status = XAxiVdma_DmaConfig(&VdmaInst, XAXIVDMA_WRITE, &WriteCfg);
    if (Status != XST_SUCCESS) {
        xil_printf("S2MM DmaConfig failed: %d\r\n", Status);
        while(1);
    }
    Status = XAxiVdma_DmaSetBufferAddr(&VdmaInst, XAXIVDMA_WRITE, FrameAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("S2MM SetBufferAddr failed: %d\r\n", Status);
        while(1);
    }
    
    Status = XAxiVdma_DmaStart(&VdmaInst, XAXIVDMA_WRITE);
    if (Status != XST_SUCCESS) {
        xil_printf("S2MM DmaStart failed: %d\r\n", Status);
        while(1);
    }
    xil_printf("2. VDMA S2MM Running\r\n");

    Status = XAxiVdma_DmaConfig(&VdmaInst, XAXIVDMA_READ, &ReadCfg);
    if (Status != XST_SUCCESS) {
        xil_printf("MM2S DmaConfig failed: %d\r\n", Status);
        while(1);
    }
    Status = XAxiVdma_DmaSetBufferAddr(&VdmaInst, XAXIVDMA_READ, FrameAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("MM2S SetBufferAddr failed: %d\r\n", Status);
        while(1);
    }
    Status = XAxiVdma_DmaStart(&VdmaInst, XAXIVDMA_READ);
    if (Status != XST_SUCCESS) {
        xil_printf("MM2S DmaStart failed: %d\r\n", Status);
        while(1);
    }
    
    xil_printf("3. VDMA MM2S Running. Check HDMI/ILA!\r\n");

    // ... (기존 VDMA Start 코드 이후) ...
    xil_printf("Checking Memory Contents at 0x%08x...\r\n", MEM_BASE_ADDR);
    
    // 잠시 데이터가 써질 시간을 줍니다 (0.1초)
    for(int i=0; i<1000000; i++); 

    // 메모리 첫 20바이트(약 6~7픽셀) 출력
    unsigned int *mem_ptr = (unsigned int *)MEM_BASE_ADDR;
    for(int i=0; i<10; i++) {
        xil_printf("Addr[0x%08x] : Value[0x%08x]\r\n", &mem_ptr[i], mem_ptr[i]);
    }

    xil_printf("If you see non-zero values above, Memory Write is SUCCESS!\r\n");

    while(1);
    return 0;
}
