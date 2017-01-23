package mycompany.sleepservice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class BootReceiver extends BroadcastReceiver {
    public void onReceive(Context context, Intent intent) {
        if (Intent.ACTION_BOOT_COMPLETED.equals(intent.getAction())) {
            Intent service = new Intent(context, StartScanService.class);
            context.startService(service);
        //    Intent pintent = new Intent(context, MainActivity.class);
        //    pintent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        //    context.startActivity(pintent);
        }
    }
}
