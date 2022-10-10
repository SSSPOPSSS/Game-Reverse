--[[
    -- 原生醉月底包函数
    DrawTool:[ --绘制工具。
        drawLine() --绘制线段。 用法：drawLine(线段开头的x轴, 线段开头的y轴, 线段结尾的x轴, 线段结尾的y轴) 示例：drawLine(100, 100, 100, 200)
        drawCircle() --绘制圆形。 用法：drawCircle(圆心的x轴, 圆心的y轴, 圆的半径) 示例：drawCircle(200, 200, 100)
        drawRect() --绘制方形。 用法：drawRect(方形的左上角x坐标, 方形的左上角y坐标, 方形的右下角x坐标, 方形的右下角y坐标) 示例：drawRect(200, 200, 400, 400)
        drawProgress() --绘制进度条。
        drawPicture() --绘制原比例大小的图片。 用法：drawPicture(图片文件路径, x轴, y轴) 示例：drawPicture("/sdcard/图片文件.png", 300, 300)
        drawScaled() --绘制自定义比例的图片。 用法：drawScaled(图片文件路径, x轴, y轴, 设置图片宽度, 设置图片长度) 示例：drawScaled("/sdcard/图片文件.png", 500, 500, 200, 200)
        drawText() --绘制文本。 用法：drawText(文本内容, x坐标, y坐标) 示例：drawText("Test", 200,200)
        removeAll() --删除全部绘制内容。 用法：removeAll()
        removeDraw() --删除指定绘制内容。 用法：removeDraw(绘制内容) 示例：removeDraw(drawText("Test", 200,200))
        setColor() --设置颜色。
        setOrigin() --设置坐标轴的原点。 用法：setOrigin(x坐标, y坐标) 示例：setOrigin(200,300)
        setRange() --设置全部绘制内容的最大显示范围(注意：设置显示范围的形状为方形)。 示例：setRange(方形的左上角x坐标, 方形的左上角y坐标, 方形的右下角x坐标, 方形的右下角y坐标) 示例：setRange(100,100,300,300)
        setSize() --设置绘制文本的大小(注意：只对文本绘制有效)。 用法：setSize(数值) 示例：setSize(20)
        setStyle() --设置绘制的样式为描边还是填充(注意：只对绘制圆、方形有效)。 用法：setStyle("样式") 示例：一.setStyle("描边") 二.setStyle("填充") 三.setStyle("描边并填充")
        setWidth() --设置画笔的粗细(注意：对绘制文本无效)。 用法：setWidth(数值) 示例：setWidth(5)
        updateColor() --更新已绘制颜色。 
        updateDraw() --更新已绘制内容(注意：一.绘制图片的图片文件不支持更新 二.第二个参数是多参数，函数多少个参数就填多少个在后面)。 用法：updatDraw(已绘制内容, 新绘制内容) 示例：一.updatDraw(drawPicture("/sdcard/图片文件.png", 300, 300), "/sdcard/图片文件.png", 200, 200) 二.updatDraw(drawLine(100, 100, 100, 200), 150, 150, 150, 250)
        updateText() --更新已绘制文本。 
    ]
]]

print(drawText) -- 此时 drawText 为 nil

-- 根据module说明,使用 draw.new() 将会得到醉月底包函数
draw.new()

print(drawText) -- 此时 drawText 为 draw.text

draw.text('RLGG',200,200)

drawText('RLGG',300,300)
-- drawText 和 draw.text 是一样的效果