package mxxy.game.mod;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;

public class FloatServiceView extends Service {

    private Context mContext;

    private FloatControlView floatControlView;

    @Override
    public IBinder onBind(Intent Intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        this.mContext = this;
        floatControlView = new FloatControlView(mContext);
        if (floatControlView != null) {
            floatControlView.showView();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (floatControlView != null) {
            floatControlView.clearView();
        }
    }

}
