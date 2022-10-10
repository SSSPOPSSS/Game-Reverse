package mxxy.game.mod;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.Switch;
import android.widget.TextView;

import mxxy.game.mod.R;

import java.util.List;

public class MAdapter extends BaseAdapter {

    private Context context;
    private List<Data> list;
    private MAdapter.OnSwitchListener onSwitchListener;

    public MAdapter(Context context, List<Data> list) {
        this.context = context;
        this.list = list;
    }

    public void setOnSwitchListener(MAdapter.OnSwitchListener onSwitchListener) {
        this.onSwitchListener = onSwitchListener;
    }

    @Override
    public int getCount() {
        // TODO: Implement this method
        return list.size();
    }

    @Override
    public Object getItem(int p1) {
        // TODO: Implement this method
        return list.get(p1);
    }

    @Override
    public long getItemId(int p1) {
        // TODO: Implement this method
        return p1;
    }

    @Override
    public View getView(final int p1, View p2, ViewGroup p3) {
        ViewHolder holder;
        if (p2 == null) {
            holder = new ViewHolder(p2);
            p2.setTag(holder);
        }
        holder = (MAdapter.ViewHolder) p2.getTag();
        final Data d = (MAdapter.Data) getItem(p1);
        holder.tv.setText(d.getTitle());
        holder.swi.setEnabled(d.getEnable());
        //switch选择事件
        holder.swi.setOnCheckedChangeListener(new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton com, boolean p2) {
                d.setIsche(com.isChecked());
                onSwitchListener.listener(d, com.isChecked(), p1);
            }
        });
        return p2;
    }

    static class ViewHolder {
        TextView tv;
        Switch swi;

        public ViewHolder(View v) {
        }
    }

    public static class Data {
        private String title;
        private boolean ische;
        private boolean Enable = false;

        public Data(String title, boolean ische, boolean enable) {
            this.title = title;
            this.ische = ische;
            this.Enable = enable;
        }

        public boolean getEnable() {
            return this.Enable;
        }

        public void setTitle(String title) {
            this.title = title;
        }

        public String getTitle() {
            return title;
        }

        public void setIsche(boolean ische) {
            this.ische = ische;
        }

        public boolean ische() {
            return ische;
        }

        public void setEnable(boolean enable) {
            this.Enable = enable;
        }
    }

    public static interface OnSwitchListener {
        public void listener(MAdapter.Data d, boolean b, int pos);
    }
}
