package mycompany.sleepservice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class ScreenOnOffReceiver extends BroadcastReceiver {

    public static boolean mService=true;
    public static boolean screenOff=false;

    @Override
    public void onReceive(Context context, Intent intent) {

        //Toast.makeText(context, "BroadcastReceiver", Toast.LENGTH_SHORT).show();
        if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
            Log.i("onReceive","[BroadCastReciever] Screen went OFF");
            screenOff = true;
        } else if (intent.getAction().equals(Intent.ACTION_SCREEN_ON)) {
            Log.i("onReceive","[BroadCastReciever] Screen went ON");
            screenOff = false;
        }
        else
        {
            screenOff = false;
            Log.i("Check","unknown");
        }
    }
}