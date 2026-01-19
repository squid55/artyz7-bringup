`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/01/18 22:01:36
// Design Name: 
// Module Name: tb_artyz7_bringup_top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module tb_artyz7_bringup_top;

    // DUT 포트에 대응되는 신호
    reg         clk;
    wire [0:0]  led;

    // DUT 인스턴스
    artyz7_bringup_top dut (
        .clk (clk),
        .led (led)
    );

    // ----------------------------------------------------------------
    // Clock generation
    // Arty Z7-10 system clock: 125 MHz → period = 8 ns
    // ----------------------------------------------------------------
    initial clk = 1'b0;
    always #4 clk = ~clk;   // 8ns period

    // ----------------------------------------------------------------
    // Simulation control
    // ----------------------------------------------------------------
    initial begin
        // 시뮬레이션 시간 충분히 확보
        // 실제 1초 토글은 너무 길기 때문에
        // 파형 확인만 하고 종료
        #50_000_000;  // 50 ms 시뮬레이션
        $finish;
    end

endmodule

