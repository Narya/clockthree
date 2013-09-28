EESchema Schematic File Version 2  date 28-Sep-13 5:37:05 PM
LIBS:Conn-raspberry
LIBS:power
LIBS:uControl
LIBS:uControl_v2-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title "uControl"
Date "28 sep 2013"
Rev "1"
Comp "WyoLum"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L BCM2835_GPIO P27
U 1 1 50E3D62C
P 4075 4700
F 0 "P27" H 4075 5400 60  0000 C CNN
F 1 "BCM2835_GPIO" V 4075 4700 50  0000 C CNN
F 2 "Header_GPIO" V 4175 4700 50  0001 C CNN
F 3 "" H 4075 4700 60  0001 C CNN
F 4 "CONN HEADER FMAL 26PS .1\" DL TIN" H 4075 5500 60  0001 L CNN "Field4"
F 5 "Header, Female Socket" H 4075 5600 60  0001 L CNN "Field5"
F 6 "0.1\", 2 row" H 4075 5700 60  0001 L CNN "Field6"
F 7 "Sullins Connector Solutions" H 4075 5800 60  0001 L CNN "Field7"
F 8 "PPTC132LFBN-RC" H 4075 5900 60  0001 L CNN "Field8"
F 9 "Digikey" H 4075 6000 60  0001 L CNN "Field9"
F 10 "S7081-ND" H 4075 6100 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/product-detail/en/PPTC132LFBN-RC/S7081-ND/810219" H 4075 6200 60  0001 L CNN "Field11"
	1    4075 4700
	1    0    0    -1  
$EndComp
$Comp
L TSX0108 U6
U 1 1 50E2BD02
P 1900 2575
F 0 "U6" H 1900 2975 40  0000 C CNN
F 1 "TSX0108" H 1900 2875 40  0000 C CNN
F 2 "TXS0108EPWR" H 1900 2975 40  0001 C CNN
F 3 "" H 1900 2575 60  0001 C CNN
F 4 "IC 8BIT NON-INV TRANSLTR 20TSSOP" H 1900 3075 60  0001 L CNN "Field4"
F 5 "Translator, Bidirectional, Open Drain" H 1900 3175 60  0001 L CNN "Field5"
F 6 "20-TSSOP (0.173\", 4.40mm Width)" H 1900 3275 60  0001 L CNN "Field6"
F 7 "Texas Instruments" H 1900 3375 60  0001 L CNN "Field7"
F 8 "TXS0108EPWR" H 1900 3475 60  0001 L CNN "Field8"
F 9 "Digikey" H 1900 3575 60  0001 L CNN "Field9"
F 10 "296-23011-1-ND" H 1900 3675 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/product-detail/en/TXS0108EPWR/296-23011-1-ND/1775304" H 1900 3775 60  0001 L CNN "Field11"
	1    1900 2575
	-1   0    0    -1  
$EndComp
$Comp
L R_10K R12
U 1 1 50E2B89D
P 3400 1625
F 0 "R12" H 3300 1775 50  0000 C CNN
F 1 "10k" V 3405 1625 50  0000 C CNN
F 2 "r_0603" V 3505 1625 50  0001 C CNN
F 3 "" H 3400 1625 60  0001 C CNN
F 4 "RES 10K OHM 1/8W 5% CF AXIAL" H 3400 1725 60  0001 L CNN "Field4"
F 5 "0.125W, 1/8W" H 3400 1825 60  0001 L CNN "Field5"
F 6 "Axial" H 3400 1925 60  0001 L CNN "Field6"
F 7 "Stackpole Electronics Inc" H 3400 2025 60  0001 L CNN "Field7"
F 8 "CF18JT10K0" H 3400 2125 60  0001 L CNN "Field8"
F 9 "Digikey" H 3400 2225 60  0001 L CNN "Field9"
F 10 "CF18JT10K0CT-ND" H 3400 2325 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/product-detail/en/CF18JT10K0/CF18JT10K0CT-ND/2022766?cur=USD" H 3400 2425 60  0001 L CNN "Field11"
	1    3400 1625
	1    0    0    -1  
$EndComp
$Comp
L C C25
U 1 1 50E29596
P 1400 1850
F 0 "C25" H 1400 1950 50  0000 L CNN
F 1 "100n" H 1400 1750 50  0000 L CNN
F 2 "c_0603" H 1400 1850 50  0001 C CNN
F 3 "" H 1400 1850 60  0001 C CNN
F 4 "CAP FILM 0.1UF 63VDC RADIAL" H 1400 2050 60  0001 L CNN "Field4"
F 5 "100nF, 63V" H 1400 2150 60  0001 L CNN "Field5"
F 6 "R82" H 1400 2250 60  0001 L CNN "Field6"
F 7 "Kemet" H 1400 2350 60  0001 L CNN "Field7"
F 8 "R82DC3100AA50J" H 1400 2450 60  0001 L CNN "Field8"
F 9 "Digikey" H 1400 2550 60  0001 L CNN "Field9"
F 10 "399-5863-ND" H 1400 2650 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/product-detail/en/R82DC3100AA50J/399-5863-ND/2571298?cur=USD" H 1400 2750 60  0001 L CNN "Field11"
	1    1400 1850
	-1   0    0    1   
$EndComp
$Comp
L C C24
U 1 1 50E29581
P 1125 1850
F 0 "C24" H 1125 1950 50  0000 L CNN
F 1 "100n" H 1125 1750 50  0000 L CNN
F 2 "c_0603" H 1125 1850 50  0001 C CNN
F 3 "" H 1125 1850 60  0001 C CNN
F 4 "CAP FILM 0.1UF 63VDC RADIAL" H 1125 2050 60  0001 L CNN "Field4"
F 5 "100nF, 63V" H 1125 2150 60  0001 L CNN "Field5"
F 6 "R82" H 1125 2250 60  0001 L CNN "Field6"
F 7 "Kemet" H 1125 2350 60  0001 L CNN "Field7"
F 8 "R82DC3100AA50J" H 1125 2450 60  0001 L CNN "Field8"
F 9 "Digikey" H 1125 2550 60  0001 L CNN "Field9"
F 10 "399-5863-ND" H 1125 2650 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/product-detail/en/R82DC3100AA50J/399-5863-ND/2571298?cur=USD" H 1125 2750 60  0001 L CNN "Field11"
	1    1125 1850
	-1   0    0    1   
$EndComp
Connection ~ 3350 4100
Wire Wire Line
	3350 4000 3350 4100
Wire Wire Line
	4550 3225 4875 3225
Wire Wire Line
	4875 3225 4875 3550
Wire Wire Line
	4875 3550 4175 3550
Connection ~ 3400 1325
Connection ~ 3625 1925
Wire Wire Line
	3625 1925 3400 1925
Wire Wire Line
	3400 1925 3400 1875
Connection ~ 1150 3500
Wire Wire Line
	1150 3600 1150 3500
Wire Wire Line
	1400 2125 1400 1975
Wire Wire Line
	1125 2125 1400 2125
Wire Wire Line
	1125 2125 1125 1975
Connection ~ 1125 1575
Wire Wire Line
	1125 1725 1125 1575
Wire Wire Line
	2050 1475 1400 1475
Wire Wire Line
	4725 4600 5300 4600
Wire Wire Line
	1550 2375 900  2375
Wire Wire Line
	1750 1575 1750 1875
Wire Wire Line
	900  3075 1550 3075
Wire Wire Line
	900  2875 1550 2875
Wire Wire Line
	900  2675 1550 2675
Wire Wire Line
	2250 3075 2575 3075
Wire Wire Line
	2575 3075 2575 5200
Wire Wire Line
	2575 5200 3425 5200
Wire Wire Line
	2250 2875 2775 2875
Wire Wire Line
	2775 2875 2775 5000
Wire Wire Line
	2775 5000 3425 5000
Wire Wire Line
	2250 2675 5200 2675
Wire Wire Line
	5200 2675 5200 4500
Wire Wire Line
	5200 4500 4725 4500
Wire Wire Line
	2250 2475 2875 2475
Wire Wire Line
	2875 2475 2875 4300
Wire Wire Line
	2875 4300 3425 4300
Wire Wire Line
	4875 4300 4725 4300
Wire Wire Line
	2250 2375 2975 2375
Wire Wire Line
	2975 2375 2975 4200
Wire Wire Line
	2975 4200 3425 4200
Wire Wire Line
	4725 4400 5100 4400
Wire Wire Line
	5100 4400 5100 2575
Wire Wire Line
	5100 2575 2250 2575
Wire Wire Line
	5300 4600 5300 2775
Wire Wire Line
	5300 2775 2250 2775
Wire Wire Line
	2250 2975 2675 2975
Wire Wire Line
	2675 2975 2675 5100
Wire Wire Line
	2675 5100 3425 5100
Wire Wire Line
	1550 2575 900  2575
Wire Wire Line
	1550 2775 900  2775
Wire Wire Line
	1550 2975 900  2975
Wire Wire Line
	900  3500 1900 3500
Wire Wire Line
	1900 3500 1900 3275
Wire Wire Line
	2050 1475 2050 1875
Wire Wire Line
	1550 2475 900  2475
Wire Wire Line
	900  1575 1750 1575
Wire Wire Line
	1400 1475 1400 1725
Wire Wire Line
	1275 2175 1275 2125
Connection ~ 1275 2125
Wire Wire Line
	3050 4100 3425 4100
Wire Wire Line
	3725 1425 3625 1425
Wire Wire Line
	3625 1425 3625 2050
Wire Wire Line
	3000 1325 3725 1325
Wire Wire Line
	3000 1325 3000 2175
Wire Wire Line
	3000 2175 2250 2175
Wire Wire Line
	4725 4100 4875 4100
Wire Wire Line
	4875 4100 4875 3650
Wire Wire Line
	4875 3650 4175 3650
Wire Wire Line
	3400 1375 3400 1325
Wire Wire Line
	3725 1225 3300 1225
$Comp
L PWR_FLAG #FLG022
U 1 1 50E00A24
P 3350 4000
F 0 "#FLG022" H 3350 4270 30  0001 C CNN
F 1 "PWR_FLAG" H 3350 4230 30  0000 C CNN
F 2 "" H 3350 4000 60  0001 C CNN
F 3 "" H 3350 4000 60  0001 C CNN
	1    3350 4000
	1    0    0    -1  
$EndComp
Text Label 3300 1225 0    40   ~ 0
Rpi_3V3
NoConn ~ 4175 3750
$Comp
L CONN_3 P26
U 1 1 500FBDD3
P 3825 3650
F 0 "P26" V 3775 3650 50  0000 C CNN
F 1 "5V_LINK" V 3875 3650 50  0000 C CNN
F 2 "ShortLink2" H 3675 3850 50  0001 C CNN
F 3 "" H 3825 3650 60  0001 C CNN
	1    3825 3650
	-1   0    0    -1  
$EndComp
Text Label 4875 3650 2    40   ~ 0
Rpi_5V0
Text Notes 3750 3400 0    40   Italic 8
allows hooking up Ras-Pi 5V \nto uControl
Text Label 4550 3225 0    40   ~ 0
5V0
Text Notes 3700 1600 0    60   Italic 12
Enable/Disable Voltage Translator U6,\nAllow/Dis-allow comm betn\nRasp-Pi and AlaMode
Text Label 2300 2175 0    40   ~ 0
Rpi_OE
$Comp
L GND #PWR023
U 1 1 4FD8C28A
P 3625 2050
F 0 "#PWR023" H 3625 2050 30  0001 C CNN
F 1 "GND" H 3625 1980 30  0001 C CNN
F 2 "" H 3625 2050 60  0001 C CNN
F 3 "" H 3625 2050 60  0001 C CNN
	1    3625 2050
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 P25
U 1 1 4FD8C245
P 4075 1325
F 0 "P25" V 4025 1325 50  0000 C CNN
F 1 "U6_OE" V 4125 1325 50  0000 C CNN
F 2 "ShortLink2" H 4075 1275 60  0001 C CNN
F 3 "" H 4075 1325 60  0001 C CNN
	1    4075 1325
	1    0    0    -1  
$EndComp
Text Label 3050 4100 0    40   ~ 0
Rpi_3V3
$Comp
L GND #PWR024
U 1 1 4FD62C0F
P 1275 2175
F 0 "#PWR024" H 1275 2175 30  0001 C CNN
F 1 "GND" H 1275 2105 30  0001 C CNN
F 2 "" H 1275 2175 60  0001 C CNN
F 3 "" H 1275 2175 60  0001 C CNN
	1    1275 2175
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 4FD62BFD
P 1150 3600
F 0 "#PWR025" H 1150 3600 30  0001 C CNN
F 1 "GND" H 1150 3530 30  0001 C CNN
F 2 "" H 1150 3600 60  0001 C CNN
F 3 "" H 1150 3600 60  0001 C CNN
	1    1150 3600
	1    0    0    -1  
$EndComp
Text Label 2300 3075 0    40   ~ 0
Rpi_SCK
Text Label 2300 2975 0    40   ~ 0
Rpi_MISO
Text Label 2300 2875 0    40   ~ 0
Rpi_MOSI
Text Label 2300 2775 0    40   ~ 0
Rpi_RST
Text Label 2300 2675 0    40   ~ 0
Rpi_Rx
Text Label 2300 2575 0    40   ~ 0
Rpi_Tx
Text Label 2300 2475 0    40   ~ 0
Rpi_SCL
Text Label 2300 2375 0    40   ~ 0
Rpi_SDA
Text Label 1650 1475 0    40   ~ 0
Rpi_3V3
Text Label 1650 1575 0    40   ~ 0
5V0
Text Label 1150 3075 0    40   ~ 0
SCK
Text HLabel 900  3075 0    40   Input ~ 0
SCK
Text HLabel 900  2975 0    40   Input ~ 0
MISO
Text Label 1150 2975 0    40   ~ 0
MISO
Text Label 1150 2875 0    40   ~ 0
~MOSI
Text HLabel 900  2875 0    40   Input ~ 0
~MOSI
Text HLabel 900  2775 0    40   Output ~ 0
RESET
Text Label 1150 2775 0    40   ~ 0
RESET
Text HLabel 900  1575 0    40   BiDi ~ 0
5V0
Text Label 3050 5200 0    40   ~ 0
Rpi_SCK
Text Label 3050 5100 0    40   ~ 0
Rpi_MISO
Text Label 3050 5000 0    40   ~ 0
Rpi_MOSI
Text Label 4875 4600 0    40   ~ 0
Rpi_RST
Text Label 1150 2675 0    40   ~ 0
TXD
Text Label 1150 2575 0    40   ~ 0
RXD
Text Label 1150 3500 0    40   ~ 0
GND
Text HLabel 900  3500 0    40   BiDi ~ 0
GND
Text HLabel 900  2675 0    40   Input ~ 0
TXD
Text HLabel 900  2575 0    40   Output ~ 0
RXD
NoConn ~ 4725 4800
NoConn ~ 4725 4900
NoConn ~ 4725 5100
NoConn ~ 4725 5300
NoConn ~ 4725 5200
NoConn ~ 3425 4800
NoConn ~ 3425 4700
NoConn ~ 3425 4600
Text Label 4875 4300 0    40   ~ 0
GND
Text Label 4875 4500 0    40   ~ 0
Rpi_Rx
Text Label 4875 4400 0    40   ~ 0
Rpi_Tx
Text HLabel 900  2475 0    40   Input ~ 0
SCL
Text HLabel 900  2375 0    40   Input ~ 0
SDA
Text Label 1150 2475 0    40   ~ 0
SCL
Text Label 1150 2375 0    40   ~ 0
SDA
Text Label 3050 4300 0    40   ~ 0
Rpi_SCL
Text Label 3050 4200 0    40   ~ 0
Rpi_SDA
Text Notes 2600 5500 0    60   Italic 12
NOTE : GPIO voltage levels are 3v3 and are not 5v tolerant.
NoConn ~ 4725 4200
NoConn ~ 4725 4700
NoConn ~ 4725 5000
NoConn ~ 3425 5300
NoConn ~ 3425 4900
NoConn ~ 3425 4500
$Comp
L ADXL345 U2
U 1 1 520DE12A
P 9875 1975
F 0 "U2" H 9850 2525 60  0000 C CNN
F 1 "ADXL345" H 9875 1425 60  0000 C CNN
F 2 "ADXL345" H 9875 1625 60  0001 C CNN
F 3 "" H 9875 1625 60  0000 C CNN
	1    9875 1975
	-1   0    0    -1  
$EndComp
NoConn ~ 10425 1825
NoConn ~ 10425 1925
Wire Wire Line
	10425 2125 10500 2125
Wire Wire Line
	10500 2125 10500 2775
Wire Wire Line
	10425 2425 10500 2425
Connection ~ 10500 2425
Wire Wire Line
	10825 2325 10425 2325
Connection ~ 10500 2325
Wire Wire Line
	10425 2225 10500 2225
Connection ~ 10500 2225
$Comp
L GND #PWR026
U 1 1 520DE13A
P 10500 2775
F 0 "#PWR026" H 10500 2775 30  0001 C CNN
F 1 "GND" H 10500 2705 30  0001 C CNN
F 2 "" H 10500 2775 60  0001 C CNN
F 3 "" H 10500 2775 60  0001 C CNN
	1    10500 2775
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 1625 10425 1625
Wire Wire Line
	10500 1000 10500 1625
Wire Wire Line
	10625 1625 10625 1725
Text Label 10550 1525 0    40   ~ 0
3V3
Wire Wire Line
	10425 1525 10825 1525
Connection ~ 10500 1525
$Comp
L C C15
U 1 1 520DE14E
P 10825 1850
F 0 "C15" V 10875 1900 50  0000 L CNN
F 1 "100n" V 10875 1600 50  0000 L CNN
F 2 "c_0603" H 10825 1850 50  0001 C CNN
F 3 "" H 10825 1850 60  0001 C CNN
F 4 "CAP FILM 0.1UF 63VDC RADIAL" H 10825 2050 60  0001 L CNN "Field4"
F 5 "100nF, 63V" H 10825 2150 60  0001 L CNN "Field5"
F 6 "R82" H 10825 2250 60  0001 L CNN "Field6"
F 7 "Kemet" H 10825 2350 60  0001 L CNN "Field7"
F 8 "R82DC3100AA50J" H 10825 2450 60  0001 L CNN "Field8"
F 9 "Digikey" H 10825 2550 60  0001 L CNN "Field9"
F 10 "399-5863-ND" H 10825 2650 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/product-detail/en/R82DC3100AA50J/399-5863-ND/2571298?cur=USD" H 10825 2750 60  0001 L CNN "Field11"
	1    10825 1850
	1    0    0    -1  
$EndComp
$Comp
L C C16
U 1 1 520DE15C
P 11025 1850
F 0 "C16" V 11075 1900 50  0000 L CNN
F 1 "100n" V 11075 1600 50  0000 L CNN
F 2 "c_0603" H 11025 1850 50  0001 C CNN
F 3 "" H 11025 1850 60  0001 C CNN
F 4 "CAP FILM 0.1UF 63VDC RADIAL" H 11025 2050 60  0001 L CNN "Field4"
F 5 "100nF, 63V" H 11025 2150 60  0001 L CNN "Field5"
F 6 "R82" H 11025 2250 60  0001 L CNN "Field6"
F 7 "Kemet" H 11025 2350 60  0001 L CNN "Field7"
F 8 "R82DC3100AA50J" H 11025 2450 60  0001 L CNN "Field8"
F 9 "Digikey" H 11025 2550 60  0001 L CNN "Field9"
F 10 "399-5863-ND" H 11025 2650 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/product-detail/en/R82DC3100AA50J/399-5863-ND/2571298?cur=USD" H 11025 2750 60  0001 L CNN "Field11"
	1    11025 1850
	1    0    0    -1  
$EndComp
$Comp
L C_POL_1 C6
U 1 1 520DE16A
P 10625 1825
F 0 "C6" H 10525 1900 50  0000 L CNN
F 1 "100u,16V" V 10675 1450 40  0000 L CNN
F 2 "C_ELCO_SMD" H 10225 1800 50  0001 C CNN
F 3 "" H 10625 1825 60  0001 C CNN
F 4 "CAP ALUM 100UF 16V 20% RADIAL" H 10625 2025 60  0001 L CNN "Field4"
F 5 "100u,16V" H 10625 2125 60  0001 L CNN "Field5"
F 6 "Radial, Can, 6.3mm dia" H 10625 2225 60  0001 L CNN "Field6"
F 7 "Panasonic Electronic Components" H 10625 2325 60  0001 L CNN "Field7"
F 8 "ECE-A1CKA101" H 10625 2425 60  0001 L CNN "Field8"
F 9 "Digikey" H 10625 2525 60  0001 L CNN "Field9"
F 10 "P833-ND" H 10625 2625 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/product-detail/en/ECE-A1CKA101/P833-ND/44757?cur=USD" H 10625 2725 60  0001 L CNN "Field11"
	1    10625 1825
	1    0    0    -1  
$EndComp
Wire Wire Line
	10625 1975 10625 2225
Wire Wire Line
	10625 2225 11025 2225
Wire Wire Line
	11025 2225 11025 1975
Wire Wire Line
	10825 1975 10825 2325
Connection ~ 10825 2225
Wire Wire Line
	11025 1625 11025 1725
Wire Wire Line
	10625 1625 11025 1625
Wire Wire Line
	10825 1525 10825 1725
Connection ~ 10825 1625
Text Label 8950 1725 0    40   ~ 0
ACL_SDO
Text Label 8950 1625 0    40   ~ 0
ACL_CS
Wire Notes Line
	11125 575  11125 4525
Wire Notes Line
	7325 4525 7325 575 
Text Notes 7375 650  0    40   ~ 0
Accelerometer, 3V3 I2C
Wire Notes Line
	7325 575  11125 575 
Wire Notes Line
	11125 4525 7325 4525
Wire Wire Line
	8100 2125 9325 2125
Wire Wire Line
	8450 2225 9325 2225
Wire Wire Line
	9325 1925 8625 1925
Wire Wire Line
	8625 1825 9325 1825
Wire Wire Line
	7750 1725 9325 1725
Wire Wire Line
	8625 1625 9325 1625
Text Label 8625 1825 0    40   ~ 0
Rpi_SDA
Text Label 8625 1925 0    40   ~ 0
Rpi_SCL
Text Label 8950 2125 0    40   ~ 0
ACL_INT1
Text Label 8950 2225 0    40   ~ 0
ACL_INT2
Text HLabel 7675 1000 0    40   Input ~ 0
3V3
Wire Wire Line
	7675 1000 10500 1000
Text Label 3450 1925 0    40   ~ 0
GND
Wire Wire Line
	8625 1625 8625 1000
Connection ~ 8625 1000
$Comp
L CONN_3 P2
U 1 1 52137E59
P 7750 3075
F 0 "P2" V 7700 3075 50  0000 C CNN
F 1 "Alt_Add" V 7800 3075 50  0000 C CNN
F 2 "ShortLink2" H 7750 3075 60  0001 C CNN
F 3 "" H 7750 3075 60  0000 C CNN
F 4 "CONN HEADER 50POS .100\" SGL GOLD" H 7750 3175 60  0001 L CNN "Field4"
F 5 "Header, Unshrouded, Male pin," H 7750 3275 60  0001 L CNN "Field5"
F 6 "0.1\" pitch x 50 nos" H 7750 3375 60  0001 L CNN "Field6"
F 7 "Samtec Inc" H 7750 3475 60  0001 L CNN "Field7"
F 8 "TSW-150-07-L-S" H 7750 3575 60  0001 L CNN "Field8"
F 9 "Digikey" H 7750 3675 60  0001 L CNN "Field9"
F 10 "SAM1031-50-ND" H 7750 3775 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/scripts/DkSearch/dksus.dll?WT.z_header=search_go&lang=en&keywords=SAM1031-50-ND&x=15&y=16&cur=USD" H 7750 3875 60  0001 L CNN "Field11"
	1    7750 3075
	0    1    1    0   
$EndComp
$Comp
L CONN_3 P3
U 1 1 52137E96
P 8100 3075
F 0 "P3" V 8050 3075 50  0000 C CNN
F 1 "Int1" V 8150 3075 50  0000 C CNN
F 2 "ShortLink2" H 8100 3075 60  0001 C CNN
F 3 "" H 8100 3075 60  0000 C CNN
F 4 "CONN HEADER 50POS .100\" SGL GOLD" H 8100 3175 60  0001 L CNN "Field4"
F 5 "Header, Unshrouded, Male pin," H 8100 3275 60  0001 L CNN "Field5"
F 6 "0.1\" pitch x 50 nos" H 8100 3375 60  0001 L CNN "Field6"
F 7 "Samtec Inc" H 8100 3475 60  0001 L CNN "Field7"
F 8 "TSW-150-07-L-S" H 8100 3575 60  0001 L CNN "Field8"
F 9 "Digikey" H 8100 3675 60  0001 L CNN "Field9"
F 10 "SAM1031-50-ND" H 8100 3775 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/scripts/DkSearch/dksus.dll?WT.z_header=search_go&lang=en&keywords=SAM1031-50-ND&x=15&y=16&cur=USD" H 8100 3875 60  0001 L CNN "Field11"
	1    8100 3075
	0    1    1    0   
$EndComp
$Comp
L CONN_3 P6
U 1 1 52137EA4
P 8450 3075
F 0 "P6" V 8400 3075 50  0000 C CNN
F 1 "Int2" V 8500 3075 50  0000 C CNN
F 2 "ShortLink2" H 8450 3075 60  0001 C CNN
F 3 "" H 8450 3075 60  0000 C CNN
F 4 "CONN HEADER 50POS .100\" SGL GOLD" H 8450 3175 60  0001 L CNN "Field4"
F 5 "Header, Unshrouded, Male pin," H 8450 3275 60  0001 L CNN "Field5"
F 6 "0.1\" pitch x 50 nos" H 8450 3375 60  0001 L CNN "Field6"
F 7 "Samtec Inc" H 8450 3475 60  0001 L CNN "Field7"
F 8 "TSW-150-07-L-S" H 8450 3575 60  0001 L CNN "Field8"
F 9 "Digikey" H 8450 3675 60  0001 L CNN "Field9"
F 10 "SAM1031-50-ND" H 8450 3775 60  0001 L CNN "Field10"
F 11 "http://www.digikey.com/scripts/DkSearch/dksus.dll?WT.z_header=search_go&lang=en&keywords=SAM1031-50-ND&x=15&y=16&cur=USD" H 8450 3875 60  0001 L CNN "Field11"
	1    8450 3075
	0    1    1    0   
$EndComp
Wire Wire Line
	8450 2725 8450 2225
Wire Wire Line
	8100 2125 8100 2725
Wire Wire Line
	7750 2725 7750 1725
Wire Wire Line
	8350 2500 8350 2725
Wire Wire Line
	7650 2500 8350 2500
Wire Wire Line
	7650 2500 7650 2725
Wire Wire Line
	8000 1000 8000 2725
Connection ~ 8000 2500
Connection ~ 8000 1000
Wire Wire Line
	7850 2725 7850 2600
Wire Wire Line
	7850 2600 8950 2600
Wire Wire Line
	8550 2600 8550 2725
Wire Wire Line
	8200 2725 8200 2600
Connection ~ 8200 2600
Wire Wire Line
	8950 2600 8950 3200
Connection ~ 8550 2600
$Comp
L GND #PWR027
U 1 1 5213835E
P 8950 3200
F 0 "#PWR027" H 8950 3200 30  0001 C CNN
F 1 "GND" H 8950 3130 30  0001 C CNN
F 2 "" H 8950 3200 60  0001 C CNN
F 3 "" H 8950 3200 60  0001 C CNN
	1    8950 3200
	1    0    0    -1  
$EndComp
Text Label 8950 2600 2    40   ~ 0
GND
Text Label 10825 2325 2    40   ~ 0
GND
Wire Wire Line
	3050 6300 3425 6300
Wire Wire Line
	4875 6300 4725 6300
Wire Wire Line
	3050 6200 3425 6200
Wire Wire Line
	3050 6100 3425 6100
Wire Wire Line
	4725 6100 4875 6100
Text Label 3050 6100 0    40   ~ 0
Rpi_3V3
NoConn ~ 4725 6800
NoConn ~ 4725 6900
NoConn ~ 4725 7100
NoConn ~ 4725 7300
NoConn ~ 4725 7200
NoConn ~ 3425 6800
NoConn ~ 3425 6700
NoConn ~ 3425 6600
Text Label 4875 6300 0    40   ~ 0
GND
Text Label 3050 6300 0    40   ~ 0
Rpi_SCL
Text Label 3050 6200 0    40   ~ 0
Rpi_SDA
NoConn ~ 4725 6200
NoConn ~ 4725 6700
NoConn ~ 4725 7000
NoConn ~ 3425 7300
NoConn ~ 3425 6900
NoConn ~ 3425 6500
NoConn ~ 4725 6400
NoConn ~ 4725 6500
NoConn ~ 4725 6600
NoConn ~ 3425 7000
NoConn ~ 3425 7100
NoConn ~ 3425 7200
Text Label 4875 6100 0    40   ~ 0
Rpi_5V0
Wire Wire Line
	3425 4400 3050 4400
Text Label 3050 4400 0    40   ~ 0
GPIO4
Text Label 3050 6400 0    40   ~ 0
GPIO4
Wire Wire Line
	3050 6400 3425 6400
$Comp
L PIPOWER P12
U 1 1 5218C5A8
P 4075 6700
F 0 "P12" H 4075 7400 60  0000 C CNN
F 1 "PIPOWER" V 4075 6700 50  0000 C CNN
F 2 "PiPower" H 4075 6700 60  0001 C CNN
F 3 "" H 4075 6700 60  0000 C CNN
	1    4075 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4875 7500 4725 7500
Text Label 4875 7500 0    40   ~ 0
GND
Wire Wire Line
	3425 7400 3300 7400
Wire Wire Line
	3300 7400 3300 7500
Wire Wire Line
	900  7500 3425 7500
Connection ~ 3300 7500
Text Label 3050 7500 0    40   ~ 0
PWR_IN
Wire Wire Line
	4725 7400 4800 7400
Wire Wire Line
	4800 7400 4800 7500
Connection ~ 4800 7500
Text HLabel 900  7500 0    40   BiDi ~ 0
PWR_IN
$EndSCHEMATC
