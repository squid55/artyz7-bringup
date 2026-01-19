`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/01/18 21:57:51
// Design Name: 
// Module Name: artyz7_bringup_top
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


module artyz7_bringup_top (
    input  wire        clk,      // H16, 125MHz
    output reg  [0:0]  led       // led[0] -> R14 (LD0)
);

    // 125MHz 기준 약 1Hz 토글용 카운터
    // 125_000_000 / 2 ? 62_500_000
    reg [26:0] cnt;

    always @(posedge clk) begin
        cnt <= cnt + 1'b1;

        if (cnt == 27'd62_500_000) begin
            cnt     <= 27'd0;
            led[0]  <= ~led[0];
        end
    end

endmodule

