



from selenium import webdriver
from selenium.webdriver.common.by import By


import time

# 打开Chrome浏览器
browser = webdriver.Chrome()


import time

def stopWatch(m, s):

    browser.get("https://mall.mihoyo.com/goods/2025482260364419961856")
    #browser.get("https://mall.mihoyo.com/goods/202516389624193491")
    print("尽快登录限时60s")
    time.sleep(30)
    print("尽快登录还剩==30s")
    time.sleep(20)
    print("尽快登录还剩==10s")
    time.sleep(10)


    timeLeft = 60*m + s
    while timeLeft > 0:
        print("剩余秒:{0}\n".format(timeLeft))
        time.sleep(1)
        timeLeft -= 1

    print("倒计时")


    if browser.find_element(By.XPATH, '//*[@id="__layout"]/div/main/div[3]/main/section[1]/div[2]/section[3]/button[1]'):
        
        a=99999
        b=0
        while(a>b):
            #browser.find_element(By.XPATH, '//*[@id="__layout"]/div/main/div[3]/main/section[1]/div[2]/section[3]/button[1]').click()
            browser.find_element(By.XPATH, '//*[@id="__layout"]/div/main/div[3]/main/section[1]/div[2]/section[3]/button[1]').click()
            b=b+1
            print("抢数={0}".format(b))

        
        print(f"成功")




def login():
    browser.get("https://mall.mihoyo.com/goods/2025482260364419961856")
    #browser.get("https://mall.mihoyo.com/goods/202516389624193491")
    print("尽快登录限时60s")
    time.sleep(30)
    print("尽快登录还剩==30s")
    time.sleep(20)
    print("尽快登录还剩==10s")
    time.sleep(10)


    if browser.find_element(By.XPATH, '//*[@id="__layout"]/div/main/div[3]/main/section[1]/div[2]/section[3]/button[1]'):
        
        a=99999
        b=0
        while(a>b):
            #browser.find_element(By.XPATH, '//*[@id="__layout"]/div/main/div[3]/main/section[1]/div[2]/section[3]/button[1]').click()
            browser.find_element(By.XPATH, '//*[@id="__layout"]/div/main/div[3]/main/section[1]/div[2]/section[3]/button[1]').click()
            b=b+1
            print("抢数={0}".format(b))

        
        print(f"成功")


if __name__ == '__main__':

    stopWatch(60,52);
    #stopWatch(0,20);
    #login();
    time.sleep(90000)

    # 不自动关闭浏览器
    option = webdriver.ChromeOptions()
    option.add_experimental_option("detach", True)





