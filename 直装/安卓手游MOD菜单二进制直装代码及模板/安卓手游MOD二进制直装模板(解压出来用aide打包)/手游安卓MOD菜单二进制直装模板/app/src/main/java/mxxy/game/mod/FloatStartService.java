package mxxy.game.mod;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;

public class FloatStartService {

    private static Context mContext;

    public static void load(Context context) {
        mContext = context;
        //检测悬浮窗权限
        applyPermission();
        //启动服务视图
		mContext.startService(new Intent(mContext, FloatServiceView.class));
    }


    private static void applyPermission() {
        if (!Settings.canDrawOverlays(mContext)) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + mContext.getPackageName()));
                mContext.startActivity(intent);
            } else {
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION);
                mContext.startActivity(intent);
            }
		}
    }

}
