package mxxy.game.mod;

import java.io.IOException;
import java.net.Proxy;
import java.util.concurrent.CountDownLatch;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class Http
{
    private static Response result = null;
    
    public static Response sendPost(String url, String header, String param, String cookie)
    {
        final CountDownLatch lt = new CountDownLatch(1);
        OkHttpClient okhttp = new OkHttpClient().newBuilder().proxy(Proxy.NO_PROXY).build();
        FormBody.Builder form = new FormBody.Builder();
        if(!param.equals(""))
        {
            String[] bodyArr = param.split("&");
            for(String item : bodyArr)
            {
                String[] itemArr = item.split("=");
                if(itemArr.length == 1)
                {
                    form.add(itemArr[0],"");
                }
                else
                {
                    form.add(itemArr[0],itemArr[1]);
                }
            }
        }
        Request request = new Request.Builder()
            .url(url)
            .addHeader("Cookie",cookie)
            .post(form.build())
            .build();

        okhttp.newCall(request).enqueue(new Callback(){

                @Override
                public void onFailure(Call p1, IOException p2)
                {
                    lt.countDown();
                }

                @Override
                public void onResponse(Call p1, Response p2) throws IOException
                {
                    result = p2;
                    lt.countDown();
                }
            });

        try
        {
            lt.await();
        }
        catch (InterruptedException e)
        {}

        return result;
    }
}
