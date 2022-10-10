import "xfc"---导入悬浮窗布局
import "xfq"---导入悬浮球布局
import "android.provider.Settings"---导入调用系统设置函数

悬浮窗=activity.getSystemService(Context.WINDOW_SERVICE) --获取窗口管理器
HasFocus=false --是否有焦点
wmParams =WindowManager.LayoutParams() --对象

if Build.VERSION.SDK_INT >= 26 then---悬浮窗可显示在其他应用上层
  wmParams.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
 else
  wmParams.type = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT
end
if Build.VERSION.SDK_INT >= Build.VERSION_CODES.M&&!Settings.canDrawOverlays(this) then
 else
  if Build.VERSION.SDK_INT >= Build.VERSION_CODES.M&&!Settings.canDrawOverlays(this) then
  end
end

wmParams.format =PixelFormat.RGBA_8888 --设置背景
wmParams.flags=WindowManager.LayoutParams().FLAG_NOT_FOCUSABLE--焦点设置
wmParams.gravity = Gravity.LEFT| Gravity.TOP --重力设置
wmParams.x = activity.getWidth()/6
wmParams.y = activity.getHeight()/5
wmParams.width =WindowManager.LayoutParams.WRAP_CONTENT
wmParams.height =WindowManager.LayoutParams.WRAP_CONTENT
wmParams.flags=WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE| WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN| WindowManager.LayoutParams.FLAG_LAYOUT_INSET_DECOR| WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH;

窗口=loadlayout(xfc)---悬浮窗布局
收起悬浮窗=loadlayout(xfq)---悬浮球布局

function 悬浮球开关.onClick--打开悬浮球(弹出权限弹窗)
  if Build.VERSION.SDK_INT >= Build.VERSION_CODES.M&&!Settings.canDrawOverlays(this) then
    提示("未检测到悬浮窗权限，请打开权限")
    xxx=AlertDialog.Builder(this)
    xxx.setMessage("是否跳转悬浮窗设置❓")
    xxx.setPositiveButton("是",function()
      intent=Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION)
      activity.startActivityForResult(intent, 100)
    end)
    xxx.setNegativeButton("否",function()
      提示[=[你拒绝了悬浮窗权限❗
   悬浮窗无法开启❗]=]
    end)
    xxx.setCancelable(false)
    xxx=xxx.show()
   elseif 悬浮球开关.text=="未开启悬浮" then---如果按钮的文字是未开启悬浮，则执行下方的代码
    悬浮窗.addView(收起悬浮窗,wmParams)---显示悬浮球布局
    悬浮球开关.text="已开启悬浮"----按钮上的文改成已开启悬浮
   else--反之
    悬浮球开关.text="已开启悬浮"
    提示("你已经打开了悬浮窗❗")
  end
end


悬浮球.onClick=function()---点击悬浮球事件
  悬浮窗.addView(窗口,wmParams)---显示悬浮窗布局
  悬浮窗.removeView(收起悬浮窗)---销毁悬浮球布局
  local 当前时间=os.date("当前运行时间:%Y年-%m月-%d日 %H时:%M分:%S")--判断时间
  提示(当前时间)
end

收起.onClick=function()---收起悬浮窗
  悬浮窗.addView(收起悬浮窗,wmParams)---显示悬浮球布局
  悬浮窗.removeView(窗口)---销毁悬浮窗布局
end

退出.onClick=function()---关闭悬浮
  悬浮窗.removeView(窗口)---销毁悬浮窗布局
  悬浮球开关.text="未开启悬浮"---悬浮球开关按钮上的文字设置成未开启悬浮
  提示("悬浮窗已关闭")
end

function 悬浮球.OnTouchListener(v,event)---拖动悬浮球
  if event.getAction()==MotionEvent.ACTION_DOWN then
    firstX=event.getRawX()
    firstY=event.getRawY()
    wmX=wmParams.x
    wmY=wmParams.y
   elseif event.getAction()==MotionEvent.ACTION_MOVE then
    wmParams.x=wmX+(event.getRawX()-firstX)
    wmParams.y=wmY+(event.getRawY()-firstY)
    悬浮窗.updateViewLayout(收起悬浮窗,wmParams)
   elseif event.getAction()==MotionEvent.ACTION_UP then
  end
  return false
end

function 移动.OnTouchListener(v,event)---拖动悬浮窗
  if event.getAction()==MotionEvent.ACTION_DOWN then
    firstX=event.getRawX()
    firstY=event.getRawY()
    wmX=wmParams.x
    wmY=wmParams.y
   elseif event.getAction()==MotionEvent.ACTION_MOVE then
    wmParams.x=wmX+(event.getRawX()-firstX)
    wmParams.y=wmY+(event.getRawY()-firstY)
    悬浮窗.updateViewLayout(窗口,wmParams)
   elseif event.getAction()==MotionEvent.ACTION_UP then
  end
  return false
end





---悬浮窗闪动字体

闪动字体(移动,300,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a1,250,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a2,270,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a3,290,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a4,310,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a5,330,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a6,350,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a7,380,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a8,200,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a9,230,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(a10,180,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(by,170,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
闪动字体(时间,320,0xffFF8080,0xff8080FF,0xff80ffff,0xff80ff80)
------设置字体
import "android.graphics.Paint"
--设置加粗
悬浮球开关.getPaint().setFakeBoldText(true)
--设置下划线，前面是id
a1.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a2.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a3.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a4.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a5.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a6.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a7.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a8.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a9.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
a10.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
by.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
时间.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )
移动.getPaint().setFlags(Paint. UNDERLINE_TEXT_FLAG )