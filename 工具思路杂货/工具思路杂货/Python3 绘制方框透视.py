from Memory64 import *
import numpy as np

pid = FindWindowPid("Valve001",None)
hwnd = FindWindow("valve001",None)
Addr = SetupProcess(pid)

Draws = FindWinDraw("valve001",None)
Drawss = Draws.SetupExGui()
Matarray = np.zeros([4,4])

Modlue_e = Addr.GetModlueAddr(pid,"engine_amxx.dll")
Modlue_m = Addr.GetModlueAddr(pid, "cstrike.exe")

Pom = Addr.ReadMemory64(Modlue_m + 0x1033240)
my_x = Addr.ReadMemory64_float(Pom + 0x88)
my_y = Addr.ReadMemory64_float(Pom + 0x8C)
my_z = Addr.ReadMemory64_float(Pom + 0x90)

pos = GetWindRect(hwnd)
Width = pos[0] / 2
Height = pos[1] / 2

while True:
    Draws.StartLoop()
    initNumber = -4
    for i in range(4):
        for j in range(4):
            initNumber += 4
            Matarray[i][j] = Addr.ReadMemory64_float(0x2C20100 + initNumber)

    Number = 133988
    for i in range(4):
        Number += 4
        Poe = Addr.ReadMemory64(Modlue_e + Number)
        enemy_x = Addr.ReadMemory64_float(Poe + 0x88)
        enemy_y = Addr.ReadMemory64_float(Poe + 0x8C)
        enemy_z = Addr.ReadMemory64_float(Poe + 0x90)
        Vorz = Matarray[0][2] * enemy_x + Matarray[1][2] * enemy_y + Matarray[2][2] * enemy_z + Matarray[3][2]
        row = 1 / Vorz

        VorX = Width + (Matarray[0][0] * enemy_x + Matarray[1][0] * enemy_y + Matarray[2][0] * enemy_z + Matarray[3][
            0]) * row * Width
        VorY = Height - (Matarray[0][1] * enemy_x + Matarray[1][1] * enemy_y + Matarray[2][1] * (enemy_z - 50) + Matarray[3][
            1]) * row * Height
        VorY2 = Height - (Matarray[0][1] * enemy_x + Matarray[1][1] * enemy_y + Matarray[2][1] * (enemy_z + 30)+ Matarray[3][
            1]) * row * Height
        if Vorz < 0:
            continue
        FanWidth = VorY2 - VorY
        FanHeight = FanWidth * 1
        Draws.DrawText("ACT忆梦", 20, 180, 100, (155, 254, 254))
        Draws.DrawRect(VorX-FanWidth / 2, VorY,FanWidth ,FanHeight,2,(155,254,254))
    Draws.EndLoop()