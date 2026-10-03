# UzuriumCircuit_2

Uzurium 制御基板（UzuriumCircuit_2）の設計データです。EasyEDA で設計しています（2026-10-04 出力）。

## ファイル

| ファイル | 内容 |
|---|---|
| [Gerber_PCB_UzuriumCircuit_2_2026-10-04.zip](Gerber_PCB_UzuriumCircuit_2_2026-10-04.zip) | 基板製造用 Gerber / ドリルデータ |
| [BOM_UzuriumCircuit_UzuriumCircuit_2026-10-04.csv](BOM_UzuriumCircuit_UzuriumCircuit_2026-10-04.csv) | 部品表（UTF-16・タブ区切り、EasyEDA 出力形式） |
| [PCB_PCB_UzuriumCircuit_2_2026-10-04.pdf](PCB_PCB_UzuriumCircuit_2_2026-10-04.pdf) | 基板レイアウト図 |
| [DXF_PCB_UzuriumCircuit_2_2026-10-04.dxf](DXF_PCB_UzuriumCircuit_2_2026-10-04.dxf) | 基板外形 DXF（ケース設計用） |

## 基板の発注

Gerber の zip ファイルをそのまま JLCPCB などの基板製造サービスにアップロードしてください。
発注手順は [EasyEDA のドキュメント](https://prodocs.easyeda.com/en/pcb/order-order-pcb/index.html) を参照してください。

## 部品表（BOM）

LCSC 列は LCSC / JLCPCB の部品番号です。

| No. | 数量 | 部品 | リファレンス | フットプリント | メーカー型番 | メーカー | LCSC |
|---|---|---|---|---|---|---|---|
| 1 | 9 | 100nF | C13, CA1, CM1, CM2, CM3, CM5, CM6, CPH1, CS1 | C0603 | CL10B104KC8NNNC | SAMSUNG(三星) | C15725 |
| 2 | 1 | 270uF | CM4 | CAP-SMD_BD10.0-L10.3-W10.3-FD | EEHZA1V271P | PANASONIC(松下) | C79124 |
| 3 | 1 | B3B-XH-AM-(LF)(SN) | CNA1 | CONN-TH_3P-P2.50_B3B-XH-AM-LF-SN | B3B-XH-AM(LF)(SN) | JST | C161870 |
| 4 | 2 | B2B-XH-A-M_C2858045 | CNB1, CNM1 | CONN-TH_2P-P2.50_JST_B2B-XH-A-M_C2858045 | B2B-XH-A-M | JST | C2858045 |
| 5 | 1 | M5 ATOM PCB Connector | CNM5 | M5ATOM |  |  | M5-ATOM-PCB-CONN |
| 6 | 2 | 22uF | CP1, CP2 | 0805 | GRM21BR60J226ME39L | MuRata | C77071 |
| 7 | 1 | B0520LW | DP1 | SOD-123 | B0520LW | STARSEA | C89640 |
| 8 | 1 | 1SMA4734A_C438355 | DP2 | SMA_L4.3-W2.5-LS5.0-RD | 1SMA4734A | 晶导微电子 | C438355 |
| 9 | 1 | DRV8839DSSR | ICM1 | WSON-12_L3.0-W2.0-P0.50-BL-EP | DRV8839DSSR | TI(德州仪器) | C128627 |
| 10 | 1 | SX1308 | ICP1 | SOT-23-6 | SX1308 | SX | C78162 |
| 11 | 1 | LBR-127HLD | ICPH1 | LBR-127HLD | RPR-220 | ROHM(罗姆) | C21062 |
| 12 | 1 | TC7S14FU,LF | ICPH2 | SOT-353-5_L2.0-W2.1-P0.65-BR | TC7S14FU,LF | TOSHIBA(东芝) | C7032 |
| 13 | 18 | WS2812B-XF02/W | LED1〜LED18 | LED-SMD_4P-L5.0-W5.0-TL | WS2812B-XF02/W | worldsemi | C4154882 |
| 14 | 1 | 4.7uH | LP1 | 5040 | SWPA5040S4R7MT | Sunlord | C48496 |
| 15 | 8 | 1kΩ | RM1, RM2, RM3, RPH2, RPH3, RPH4, RS1, RS2 | R0603 | WR06X102JTL | Walsin(华新科) | C384295 |
| 16 | 1 | 1.37kΩ | RP1 | R0805 | RTT051371FTP | RALEC(旺诠) | C103980 |
| 17 | 1 | 10kΩ | RP2 | R0805 | RTT051002FTP | RALEC(旺诠) | C103904 |
| 18 | 1 | 220 | RPH1 | R0603 | RC0603JR-07220RL | YAGEO(国巨) | C114683 |
| 19 | 1 | SK-22F03-G060 | SWB1 | SW-TH_G-SWITCH_SK-22F03-G060 | SK-22F03-G060 | G-Switch(品赞) | C2848866 |
| 20 | 1 | PS-5850DHA-3PNA | SWS1 | SW-TH_G-SWITCH_PS-585XX | PS-5850DHA-3PNA | G-Switch(品赞) | C5155592 |
| 21 | 3 | 5004 | TP33, TP50, TPBAT1 | TEST-TH_BD2.54-P1.39 | 5004 | Keystone | C2906762 |
| 22 | 1 | 5001 | TPGND1 | TEST-TH_BD2.54-P1.39 | 5001 | Keystone | C238122 |
