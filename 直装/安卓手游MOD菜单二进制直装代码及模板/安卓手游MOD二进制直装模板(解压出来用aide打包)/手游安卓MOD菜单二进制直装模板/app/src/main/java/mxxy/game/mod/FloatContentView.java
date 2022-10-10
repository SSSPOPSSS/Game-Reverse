package mxxy.game.mod;

import android.content.Context;
import android.content.Intent;
import android.graphics.Typeface;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import mxxy.game.mod.Miscellaneous;
import java.io.IOException;

   
   
   
   
   
   
public class FloatContentView extends PopupWindow {

    private Context mContext;
	
	private boolean view;

    public FloatContentView(Context context) {
        super(context);
        this.mContext = context;
        initView();
    }

    private void initView() {

        LinearLayout main = new LinearLayout(mContext);
		LinearLayout.LayoutParams mainParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
		main.setLayoutParams(mainParams);

		GradientDrawable mainBackground = new GradientDrawable();
        mainBackground.setColor(0xFFC0C0C0);
		mainBackground.setCornerRadius(30);
        mainBackground.setStroke(5, 0xFF007FFF);
		//设置背景
		main.setBackgroundDrawable(mainBackground);

		LinearLayout mainLayout = new LinearLayout(mContext);
		LinearLayout.LayoutParams mainLayoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
		mainLayout.setLayoutParams(mainLayoutParams);
		mainLayout.setOrientation(LinearLayout.VERTICAL);
		//设置图片背景
		//mainLayout.setBackgroundDrawable(drawable);
		main.addView(mainLayout);

		LinearLayout titleLayout = new LinearLayout(mContext);
		LinearLayout.LayoutParams titleLayoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
		titleLayout.setLayoutParams(titleLayoutParams);
		titleLayout.setGravity(Gravity.CENTER);
		titleLayout.setPadding(20, 20, 20, 20);
		mainLayout.addView(titleLayout);

		TextView title = new TextView(mContext);
		LinearLayout.LayoutParams titleParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
		title.setLayoutParams(titleParams);
		title.setText("༺小源荒野༻");
		title.setTextSize(15);
        title.setTextColor(0xFF4D4DFF);
		title.setTypeface(Typeface.defaultFromStyle(Typeface.BOLD));
		titleLayout.addView(title);

		ScrollView scroll = new ScrollView(mContext);
		ScrollView.LayoutParams scrollParams = new ScrollView.LayoutParams(ScrollView.LayoutParams.MATCH_PARENT, ScrollView.LayoutParams.WRAP_CONTENT);
        scrollParams.bottomMargin = 10;
		scroll.setLayoutParams(scrollParams);
		mainLayout.addView(scroll);

		LinearLayout layout = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
		layout.setLayoutParams(layoutParams);
		layout.setOrientation(LinearLayout.VERTICAL);
		layout.setPadding(5, 5, 5, 5);
		scroll.addView(layout);

		GradientDrawable ButtonBackground = new GradientDrawable();
        ButtonBackground.setColor(0xFF4D4DFF);
		ButtonBackground.setCornerRadius(10);
        ButtonBackground.setStroke(5, 0xFF007FFF);

		LinearLayout layout1 = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams1 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layout1.setLayoutParams(layoutParams1);
		layout1.setPadding(5, 5, 5, 5);
		layout.addView(layout1);

		Button button1 = new Button(mContext);
		LinearLayout.LayoutParams buttonParams1 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        button1.setLayoutParams(buttonParams1);
		button1.setText("9999⁺血量");
		button1.setTextSize(11);
        button1.setTextColor(0xFF4D4DFF);
		layout1.addView(button1);

		LinearLayout layout2 = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams2 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layout2.setLayoutParams(layoutParams2);
		layout2.setPadding(5, 5, 5, 5);
		layout.addView(layout2);
		
		Button button2 = new Button(mContext);
		LinearLayout.LayoutParams buttonParams2 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        button2.setLayoutParams(buttonParams2);
		button2.setText("待开发");
		button2.setTextSize(11);
        button2.setTextColor(0xFF4D4DFF);
		layout2.addView(button2);

		LinearLayout layout3 = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams3 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layout3.setLayoutParams(layoutParams3);
		layout3.setPadding(5, 5, 5, 5);
		layout.addView(layout3);
		
		Button button3 = new Button(mContext);
		LinearLayout.LayoutParams buttonParams3 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        button3.setLayoutParams(buttonParams3);
		button3.setText("待开发");
		button3.setTextSize(11);
        button3.setTextColor(0xFF4D4DFF);
		layout3.addView(button3);

		LinearLayout layout4 = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams4 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layout4.setLayoutParams(layoutParams4);
		layout4.setPadding(5, 5, 5, 5);
		layout.addView(layout4);
		
	    Button button4 = new Button(mContext);
		LinearLayout.LayoutParams buttonParams4 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        button4.setLayoutParams(buttonParams4);
		button4.setText("待开发");
		button4.setTextSize(11);
        button4.setTextColor(0xFF4D4DFF);
		layout4.addView(button4);

		LinearLayout layout5 = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams5 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layout5.setLayoutParams(layoutParams5);
		layout5.setPadding(5, 5, 5, 5);
		layout.addView(layout5);
		
        Button button5 = new Button(mContext);
		LinearLayout.LayoutParams buttonParams5 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        button5.setLayoutParams(buttonParams5);
		button5.setText("待开发");
		button5.setTextSize(11);
        button5.setTextColor(0xFF4D4DFF);
		layout5.addView(button5);

		LinearLayout layout6 = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams6 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layout6.setLayoutParams(layoutParams6);
		layout6.setPadding(5, 5, 5, 5);
		layout.addView(layout6);
		
        Button button6 = new Button(mContext);
		LinearLayout.LayoutParams buttonParams6 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        button6.setLayoutParams(buttonParams6);
		button6.setText("待开发");
		button6.setTextSize(11);
        button6.setTextColor(0xFF4D4DFF);
		layout6.addView(button6);

		LinearLayout layout7 = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams7 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layout7.setLayoutParams(layoutParams7);
		layout7.setPadding(5, 5, 5, 5);
		layout.addView(layout7);
		
        Button button7 = new Button(mContext);
		LinearLayout.LayoutParams buttonParams7 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        button7.setLayoutParams(buttonParams7);
		button7.setText("待开发");
		button7.setTextSize(11);
        button7.setTextColor(0xFF4D4DFF);
		layout7.addView(button7);

		LinearLayout layout8 = new LinearLayout(mContext);
		LinearLayout.LayoutParams layoutParams8 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layout8.setLayoutParams(layoutParams8);
		layout8.setPadding(5, 5, 5, 5);
		layout.addView(layout8);		
				
		WindowManager windowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
		DisplayMetrics metrics = new DisplayMetrics();
		windowManager.getDefaultDisplay().getRealMetrics(metrics);
		int metricsWidth = metrics.widthPixels;
		int metricsHeight = metrics.heightPixels;

		setWidth(metricsWidth > metricsHeight ? metricsHeight / 3: metricsWidth / 3);
		setHeight(metricsHeight > metricsWidth ? metricsWidth - 200 : metricsHeight - 200);
		setContentView(main);
		setBackgroundDrawable(new ColorDrawable(0));
		setOutsideTouchable(true);
        setFocusable(true);
        

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            setWindowLayoutType(WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY);
        } else {
            setWindowLayoutType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
        }
        //动态申请储存权限

		button1.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
		        	Miscellaneous.RunShell(("data/data/com.nidong.cmswat.nearme.gamecenter/lib/liba.so"));
					Miscellaneous.RunShell(("data/data/com.nidong.cmswat.nearme.gamecenter/lib/liba.so"));
					showToast("9999⁺血量\n 已注入");
			    }

				private void RunShell(String p0)
				{
					// TODO: Implement this method
				}

				private String getFilesDir() {
					return null;
				}
			});
			
		button2.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
				  showToast("待开发");
				}					
				private void RunShell(String p0)
				{
			     	// TODO: Implement this method
				}

				private String getFilesDir() {
					return null;
				}
			});
			
    	button3.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					showToast("待开发");
			    }

				private void RunShell(String p0)
				{
					// TODO: Implement this method
				}

				private String getFilesDir() {
					return null;
				}
			});
			
		
	}

    private void showToast(String str) {
        Toast.makeText(mContext, str, Toast.LENGTH_LONG).show();
	}


    public void showView() {
        this.showAtLocation(this.getContentView(), Gravity.LEFT, 20, 0);
	}

}

