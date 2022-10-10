package mxxy.game.mod;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.util.Log;
import android.widget.Toast;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import android.app.Activity;
import android.os.Build;
import android.view.Window;
import android.view.WindowManager;


public class Miscellaneous {

    /****************
     *
     * 发起添加群流程。群号：android学习交流(1126670095) 的 key 为： lkhCw_ShTnxcfy9XRcdZG6kAgNW0B5Fr
     * 调用 加入群聊(lkhCw_ShTnxcfy9XRcdZG6kAgNW0B5Fr) 即可发起手Q客户端申请加群 流星国际体验服(1126670095)
     *
     * @param key 由官网生成的key
     * @return 返回true表示呼起手Q成功，返回fals表示呼起失败
     ******************/
    public static boolean 加入群聊(Context context, String key) {
        Intent jrql = new Intent();
        jrql.setData(Uri.parse("https://jq.qq.com/?_wv=1027&k=3qGghiU2" + key));
        // 此Flag可根据具体产品需要自定义，如设置，则在加群界面按返回，返回手Q主界面，不设置，按返回会返回到呼起产品界面    //intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
        try {
            context.startActivity(jrql);

            return true;
        } catch (Exception e) {
            // 未安装手Q或安装的版本不支持
            
            return false;
        }
    }


    public static boolean 联系QQ(Context context, String qq) {
        Intent lxqq = new Intent();
        String url = "http://wpa.qq.com/msgrd?v=3&uin=2938990845&site=qq&menu=yes" + qq;
        try {
            context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));

            return true;
        } catch (Exception e) {
            // 未安装手Q或安装的版本不支持
            e.printStackTrace();

            return false;
        }
    }

    public static boolean 邮箱反馈(Context context, String 收件人, String 抄送人, String 内容标题, String 内容) {
        Uri uri = Uri.parse("mailto:" + 收件人);//收件人
        String[] email = {抄送人};//抄送人
        Intent intent = new Intent(Intent.ACTION_SENDTO, uri);
        intent.putExtra(Intent.EXTRA_CC, email); // 抄送人
        intent.putExtra(Intent.EXTRA_SUBJECT, 内容标题);
        intent.putExtra(Intent.EXTRA_TEXT, 内容);
        try {
            context.startActivity(Intent.createChooser(intent, "邮件类应用"));

            return true;
        } catch (Exception e) {
            Toast.makeText(context, "未知错误", Toast.LENGTH_LONG).show();

            return false;
        }

    }
    // 可逆的加密算法
    public static String 可逆加密(String inStr) {   
        // String s = new String(inStr);   
        char[] a = inStr.toCharArray();   
        for (int i = 0; i < a.length; i++) {   
            a[i] = (char) (a[i] ^ 't');   
        }   
        String s = new String(a);   
        return s;   
    }   

    // 加密后解密   
    public static String 可逆解密(String inStr) {   
        char[] a = inStr.toCharArray();   
        for (int i = 0; i < a.length; i++) {   
            a[i] = (char) (a[i] ^ 't');   
        }   
        String k = new String(a);   
        return k;   
	}   

    /*写出assets资源文件
     *例子:
     *		写出assets资源文件(this,getFilesDir() + "/assets", "文件名");//这里写要写出的二进制文件
     */
    public static boolean 写出assets资源文件(Context context, String outPath, String fileName) {
        File file = new File(outPath);
        if (!file.exists()) {
            if (!file.mkdirs()) {
                Log.e("--Method--", "copyAssetsSingleFile: cannot create directory.");
                return false;
            }
        }
        try {
            InputStream inputStream = context.getAssets().open(fileName);
            File outFile = new File(file, fileName);
            FileOutputStream fileOutputStream = new FileOutputStream(outFile);
            // Transfer bytes from inputStream to fileOutputStream
            byte[] buffer = new byte[1024];
            int byteRead;
            while (-1 != (byteRead = inputStream.read(buffer))) {
                fileOutputStream.write(buffer, 0, byteRead);
            }
            inputStream.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    //调用事件S,运行二进制，等shell

    /**
     * 第一种:不能执行内存的二进制调用
     * ExecuteElf("su");//申请root
     * ExecuteElf("chmod 777 "+getFilesDir()+"/assets/二进制文件名字");//,写二进制777权限
     * ExecuteElf(getFilesDir()+"/assets/二进制文件名字");//执行二进制
     * -------------------
     * 第二种:什么都可以执行
     * ExecuteElf("su -c");
     * ExecuteElf("chmod 777 " + getFilesDir() + "/assets/二进制文件名字");//,写二进制777权限
     * ExecuteElf("su -c " + getFilesDir() + "/assets/二进制文件名字");//执行二进制
     *
     * @param shell
     */
    public static void RunShell(String shell) {
        String s = shell;

        try {
            Runtime.getRuntime().exec(s, null, null);//执行
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    //点击返回桌面事件
    public static void 返回桌面(Context context) {
        Intent mHomeIntent = new Intent(Intent.ACTION_MAIN);
        mHomeIntent.addCategory(Intent.CATEGORY_HOME);
        mHomeIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
                             | Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
        context.startActivity(mHomeIntent);
	};

    //打开隐藏MIUI性能模式
    public static void 打开MIUI性能模式(Context context) {
        Intent intent = new Intent();
        intent.setComponent(new ComponentName("com.android.settings",
                                              "com.android.settings.fuelgauge.PowerModeSettings"));

        context.startActivity(intent);
    }

    // 网络连接判断
    public static boolean 网络检测(Context context) {
        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo info = cm.getActiveNetworkInfo();
        if (info != null) {
            return info.isConnected();
        } else {
            return false;
        }
    }

    public static void StatusNavigationColor(Activity activity, int colorResId) {
        try {

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                //顶部导航栏
                Window window = activity.getWindow();
                window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                window.setStatusBarColor(activity.getResources().getColor(colorResId));
                //底部导航栏
                window.setNavigationBarColor(activity.getResources().getColor(colorResId));

            }

        } catch (Exception e) {

            e.printStackTrace();

        }
    }



}
