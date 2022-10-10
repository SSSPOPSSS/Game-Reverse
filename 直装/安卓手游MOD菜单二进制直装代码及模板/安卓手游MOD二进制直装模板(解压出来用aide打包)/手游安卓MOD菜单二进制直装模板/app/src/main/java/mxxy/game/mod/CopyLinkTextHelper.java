package mxxy.game.mod;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;

public class CopyLinkTextHelper {

    private static CopyLinkTextHelper instance = null;
    private static ClipboardManager manager;

    private CopyLinkTextHelper(Context context) {
        //获取剪贴板管理器：
        manager = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
    }

    public synchronized static CopyLinkTextHelper getInstance(Context context) {
        if (instance == null) {
            instance = new CopyLinkTextHelper(context);
        }
        return instance;
    }

    /**
     * 复制文字到剪切板
     *
     * @param text
     */
    public  void CopyClipping(String text) {
        // 创建能够存入剪贴板的ClipData对象
        //‘Label’这是任意文字标签
        ClipData mClipData = ClipData.newPlainText("Label", text);
        //将ClipData数据复制到剪贴板：
        manager.setPrimaryClip(mClipData);
    }

    /**
     * 复制链接url到剪切板
     *
     * @param url
     */
    public void CopyUrl(String url) {
        //‘Label’这是任意文字标签
        ClipData mClipData = ClipData.newRawUri("Label", Uri.parse(url));
        //将ClipData数据复制到剪贴板：
        manager.setPrimaryClip(mClipData);
    }

    /**
     * 复制Intent到剪切板
     *
     * @param intent
     */
    public void CopyIntent(Intent intent) {
        //‘Label’这是任意文字标签
        ClipData mClipData = ClipData.newIntent("Label", intent);
        //将ClipData数据复制到剪贴板：
        manager.setPrimaryClip(mClipData);
    }

    /**
     * 从剪贴板中获取数据,如text文字，链接等，
     *
     * @return
     */
    public String getCopyString() {
        ClipData clipData = manager.getPrimaryClip();
        if (clipData != null && clipData.getItemCount() > 0) {
            return clipData.getItemAt(0).getText().toString();
        }
        return null;
    }

}
