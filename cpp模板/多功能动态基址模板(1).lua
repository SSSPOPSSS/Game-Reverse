



bqt={"๐คก","๐ค ","๐","๐ด","๐ฎ","๐จ","๐ค","โก","๐","๐",}--่กจๆ

local sjbq=bqt[math.random(1,#bqt)]
print(sjbq)





function Main()
  SN =gg.multiChoice({
  "๐ถ๐ฟ๐พๅผๅฏ้ฃๅคฉ๐ถ๐ฟ๐พ",
  "๐ถ๐ฟ๐พๅณ้ญ้ฃๅคฉ๐ถ๐ฟ๐พ",
  "๐ถ๐ฟ๐พๅ่ฝ3๐ถ๐ฟ๐พ",
  "๐ถ๐ฟ๐พๅ่ฝ4๐ถ๐ฟ๐พ",
  "๐ถ๐ฟ๐พๅ่ฝ5๐ถ๐ฟ๐พ",
  "๐ถ๐ฟ๐พๅ่ฝ6๐ถ๐ฟ๐พ",
  "้ๅบ่ๆฌ๐ค ๐จ๐ค"
 }, nil,os.date"่ๆฌๅผๅฏๆถ้ดไธบ:%Yๅนด%mๆ%dๆฅ%Hๆถ%Mๅ%S็ง")
  if SN == nil then
  else
  if SN[1] == true then
    a1()
  end
  if SN[2] == true then
   a2()
  end
  if SN[3] == true then
   a3()
  end
  if SN[4] == true then
   a4()
  end
  if SN[5] == true then
   a5()
  end
  if SN[6] == true then
   a6()
  end
  if SN[7] == true then
  sj= os.date("%Y%m%d%H%M%S")-bx    gg.alert("ๆฌๆฌก่ๆฌไฝฟ็จไบ"..sj.."็ง") os.exit()
   Exit()
   
  end
end
  XGCK = -1
end

function a1()
function PS() end
function S_Pointer(t_So, t_Offset, _bit)
	local function getRanges()
		local ranges = {}
		local t = gg.getRangesList('^/data/*.so*$')
		for i, v in pairs(t) do
			if v.type:sub(2, 2) == 'w' then
				table.insert(ranges, v)
			end
		end
		return ranges
	end
	local function Get_Address(N_So, Offset, ti_bit)
		local ti = gg.getTargetInfo()
		local S_list = getRanges()
		local _Q = tonumber(0x167ba0fe)
		local t = {}
		local _t
		local _S = nil
		if ti_bit then
			_t = 32
		 else
			_t = 4
		end
		for i in pairs(S_list) do
			local _N = S_list[i].internalName:gsub('^.*/', '')
			if N_So[1] == _N and N_So[2] == S_list[i].state then
				_S = S_list[i]
				break
			end
		end
		if _S then
			t[#t + 1] = {}
			t[#t].address = _S.start + Offset[1]
			t[#t].flags = _t
			if #Offset ~= 1 then
				for i = 2, #Offset do
					local S = gg.getValues(t)
					t = {}
					for _ in pairs(S) do
						if not ti.x64 then
							S[_].value = S[_].value & 0xFFFFFFFF
						end
						t[#t + 1] = {}
						t[#t].address = S[_].value + Offset[i]
						t[#t].flags = _t
					end
				end
			end
			_S = t[#t].address
			
		end
		return _S
	end
	local _A = string.format('0x%X', Get_Address(t_So, t_Offset, _bit))
	return _A
end


local t = {"libUE4.so:bss", "Cb"}
local tt = {0x1B7B8, 0x5E8, 0x640, 0x53C}
local ttt = S_Pointer(t, tt, true)
gg.setValues({{address = ttt, flags = 4, value = 41736}})


local t = {"libUE4.so:bss", "Cb"}
local tt = {0x1B7B8, 0x5E8, 0x640, 0x2B4}
local ttt = S_Pointer(t, tt, true)
gg.setValues({{address = ttt, flags = 4, value = 5}})
gg.toast("้ฃๅคฉๅผๅฏๆๅ")               
end



function a2()
function PS() end
function S_Pointer(t_So, t_Offset, _bit)
	local function getRanges()
		local ranges = {}
		local t = gg.getRangesList('^/data/*.so*$')
		for i, v in pairs(t) do
			if v.type:sub(2, 2) == 'w' then
				table.insert(ranges, v)
			end
		end
		return ranges
	end
	local function Get_Address(N_So, Offset, ti_bit)
		local ti = gg.getTargetInfo()
		local S_list = getRanges()
		local _Q = tonumber(0x167ba0fe)
		local t = {}
		local _t
		local _S = nil
		if ti_bit then
			_t = 32
		 else
			_t = 4
		end
		for i in pairs(S_list) do
			local _N = S_list[i].internalName:gsub('^.*/', '')
			if N_So[1] == _N and N_So[2] == S_list[i].state then
				_S = S_list[i]
				break
			end
		end
		if _S then
			t[#t + 1] = {}
			t[#t].address = _S.start + Offset[1]
			t[#t].flags = _t
			if #Offset ~= 1 then
				for i = 2, #Offset do
					local S = gg.getValues(t)
					t = {}
					for _ in pairs(S) do
						if not ti.x64 then
							S[_].value = S[_].value & 0xFFFFFFFF
						end
						t[#t + 1] = {}
						t[#t].address = S[_].value + Offset[i]
						t[#t].flags = _t
					end
				end
			end
			_S = t[#t].address
			
		end
		return _S
	end
	local _A = string.format('0x%X', Get_Address(t_So, t_Offset, _bit))
	return _A
end


local t = {"libUE4.so:bss", "Cb"}
local tt = {0x1B7B8, 0x5E8, 0x640, 0x53C}
local ttt = S_Pointer(t, tt, true)
gg.setValues({{address = ttt, flags = 4, value = 41792}})


local t = {"libUE4.so:bss", "Cb"}
local tt = {0x1B7B8, 0x5E8, 0x640, 0x2B4}
local ttt = S_Pointer(t, tt, true)
gg.setValues({{address = ttt, flags = 4, value = 1}})
gg.toast("้ฃๅคฉๅณ้ญๆๅ")
end

function a3()
function PS() end
function S_Pointer(t_So, t_Offset, _bit)
	local function getRanges()
		local ranges = {}
		local t = gg.getRangesList('^/data/*.so*$')
		for i, v in pairs(t) do
			if v.type:sub(2, 2) == 'w' then
				table.insert(ranges, v)
			end
		end
		return ranges
	end
	local function Get_Address(N_So, Offset, ti_bit)
		local ti = gg.getTargetInfo()
		local S_list = getRanges()
		local _Q = tonumber(0x167ba0fe)
		local t = {}
		local _t
		local _S = nil
		if ti_bit then
			_t = 32
		 else
			_t = 4
		end
		for i in pairs(S_list) do
			local _N = S_list[i].internalName:gsub('^.*/', '')
			if N_So[1] == _N and N_So[2] == S_list[i].state then
				_S = S_list[i]
				break
			end
		end
		if _S then
			t[#t + 1] = {}
			t[#t].address = _S.start + Offset[1]
			t[#t].flags = _t
			if #Offset ~= 1 then
				for i = 2, #Offset do
					local S = gg.getValues(t)
					t = {}
					for _ in pairs(S) do
						if not ti.x64 then
							S[_].value = S[_].value & 0xFFFFFFFF
						end
						t[#t + 1] = {}
						t[#t].address = S[_].value + Offset[i]
						t[#t].flags = _t
					end
				end
			end
			_S = t[#t].address
			
		end
		return _S
	end
	local _A = string.format('0x%X', Get_Address(t_So, t_Offset, _bit))
	return _A
end


local t = {"libil2cpp.so:bss", "Cb"}
local tt = {0x7A84, 0x5C, 0x80, 0x10}--่ทณ่ฝฌๆ้ๅ็งป ็ฌฌไธไธชๆฏไปSo:bssๅผๅงๅ็งป ็ฌฌไบไธชๆฏไป็ฌฌไธไธช่ทณ่ฝฌ็ๆ้ๅผๅงๅ็งป
local ttt = S_Pointer(t, tt, true)--ๅฝๆฐๅ ๅฑ้จๅ้t, tt ่ทๅ็ไธบๅฐๅ trueไธบ64ไฝ,32ไฝๆธธๆ้ๅปๆtrue
gg.setValues({{address = ttt, flags = 4, value = 999,freeze = true}})--ๅฐๅ,็ฑปๅ,ไฟฎๆนๅผ freeze=true่กจ็คบๅป็ป,ไธๅป็ปๅฏๅปๆ
gg.toast("ๅ่ฝ3ๅผๅฏๆๅ")
end


function a4()
function PS() end
function S_Pointer(t_So, t_Offset, _bit)
	local function getRanges()
		local ranges = {}
		local t = gg.getRangesList('^/data/*.so*$')
		for i, v in pairs(t) do
			if v.type:sub(2, 2) == 'w' then
				table.insert(ranges, v)
			end
		end
		return ranges
	end
	local function Get_Address(N_So, Offset, ti_bit)
		local ti = gg.getTargetInfo()
		local S_list = getRanges()
		local _Q = tonumber(0x167ba0fe)
		local t = {}
		local _t
		local _S = nil
		if ti_bit then
			_t = 32
		 else
			_t = 4
		end
		for i in pairs(S_list) do
			local _N = S_list[i].internalName:gsub('^.*/', '')
			if N_So[1] == _N and N_So[2] == S_list[i].state then
				_S = S_list[i]
				break
			end
		end
		if _S then
			t[#t + 1] = {}
			t[#t].address = _S.start + Offset[1]
			t[#t].flags = _t
			if #Offset ~= 1 then
				for i = 2, #Offset do
					local S = gg.getValues(t)
					t = {}
					for _ in pairs(S) do
						if not ti.x64 then
							S[_].value = S[_].value & 0xFFFFFFFF
						end
						t[#t + 1] = {}
						t[#t].address = S[_].value + Offset[i]
						t[#t].flags = _t
					end
				end
			end
			_S = t[#t].address
			
		end
		return _S
	end
	local _A = string.format('0x%X', Get_Address(t_So, t_Offset, _bit))
	return _A
end


local t = {"libUE4.so:bss", "Cb"}
local tt = {0x1B6048, 0x0, 0xD08, 0x628}
local ttt = S_Pointer(t, tt, true)
gg.setValues({{address = ttt, flags = 4, value = 999}})
gg.toast("ๅ่ฝ4ๅผๅฏๆๅ")
end



function a5()
function PS() end
function S_Pointer(t_So, t_Offset, _bit)
	local function getRanges()
		local ranges = {}
		local t = gg.getRangesList('^/data/*.so*$')
		for i, v in pairs(t) do
			if v.type:sub(2, 2) == 'w' then
				table.insert(ranges, v)
			end
		end
		return ranges
	end
	local function Get_Address(N_So, Offset, ti_bit)
		local ti = gg.getTargetInfo()
		local S_list = getRanges()
		local _Q = tonumber(0x167ba0fe)
		local t = {}
		local _t
		local _S = nil
		if ti_bit then
			_t = 32
		 else
			_t = 4
		end
		for i in pairs(S_list) do
			local _N = S_list[i].internalName:gsub('^.*/', '')
			if N_So[1] == _N and N_So[2] == S_list[i].state then
				_S = S_list[i]
				break
			end
		end
		if _S then
			t[#t + 1] = {}
			t[#t].address = _S.start + Offset[1]
			t[#t].flags = _t
			if #Offset ~= 1 then
				for i = 2, #Offset do
					local S = gg.getValues(t)
					t = {}
					for _ in pairs(S) do
						if not ti.x64 then
							S[_].value = S[_].value & 0xFFFFFFFF
						end
						t[#t + 1] = {}
						t[#t].address = S[_].value + Offset[i]
						t[#t].flags = _t
					end
				end
			end
			_S = t[#t].address
			
		end
		return _S
	end
	local _A = string.format('0x%X', Get_Address(t_So, t_Offset, _bit))
	return _A
end



local t = {"libUE4.so:bss", "Cb"}
local tt = {0x1B6048, 0x0, 0xD08, 0x628}
local ttt = S_Pointer(t, tt, true)
gg.setValues({{address = ttt, flags = 4, value = 999}})
gg.toast("ๅ่ฝ5ๅผๅฏๆๅ")
end

function a6()
function PS() end
function S_Pointer(t_So, t_Offset, _bit)
	local function getRanges()
		local ranges = {}
		local t = gg.getRangesList('^/data/*.so*$')
		for i, v in pairs(t) do
			if v.type:sub(2, 2) == 'w' then
				table.insert(ranges, v)
			end
		end
		return ranges
	end
	local function Get_Address(N_So, Offset, ti_bit)
		local ti = gg.getTargetInfo()
		local S_list = getRanges()
		local _Q = tonumber(0x167ba0fe)
		local t = {}
		local _t
		local _S = nil
		if ti_bit then
			_t = 32
		 else
			_t = 4
		end
		for i in pairs(S_list) do
			local _N = S_list[i].internalName:gsub('^.*/', '')
			if N_So[1] == _N and N_So[2] == S_list[i].state then
				_S = S_list[i]
				break
			end
		end
		if _S then
			t[#t + 1] = {}
			t[#t].address = _S.start + Offset[1]
			t[#t].flags = _t
			if #Offset ~= 1 then
				for i = 2, #Offset do
					local S = gg.getValues(t)
					t = {}
					for _ in pairs(S) do
						if not ti.x64 then
							S[_].value = S[_].value & 0xFFFFFFFF
						end
						t[#t + 1] = {}
						t[#t].address = S[_].value + Offset[i]
						t[#t].flags = _t
					end
				end
			end
			_S = t[#t].address
			
		end
		return _S
	end
	local _A = string.format('0x%X', Get_Address(t_So, t_Offset, _bit))
	return _A
end



local t = {"libUE4.so:bss", "Cb"}
local tt = {0x1B6048, 0x0, 0xD08, 0x628}
local ttt = S_Pointer(t, tt, true)
gg.setValues({{address = ttt, flags = 4, value = 999}})
gg.toast("ๅ่ฝ6ๅผๅฏๆๅ")
end



function Exit()
os.exit()
end
cs = ""


while true do
  if gg.isVisible(true) then
    XGCK = 1
    gg.setVisible(false)
  end
  gg.clearResults()
  if XGCK == 1 then
    Main()
  end
end

