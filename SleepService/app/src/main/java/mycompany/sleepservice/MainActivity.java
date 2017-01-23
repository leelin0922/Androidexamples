package mycompany.sleepservice;

import android.content.Intent;
import android.os.Handler;
import android.os.ResultReceiver;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private Window window=null;
    MyServiceReceiver receiver=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        window = this.getWindow();
        window.addFlags( WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED | WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD | WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON );
        receiver = new MyServiceReceiver(null);
        Intent service = new Intent(this, StartScanService.class);
        startService(service);
        HideActivity();
   }

    public void buttonClicked(View v){
        if(v.getTag() == null){
            v.setTag("on");
            ((Button)v).setText("Stop Service");
            ScreenOnOffReceiver.mService=true;
            Toast.makeText(this, "Start Service", Toast.LENGTH_SHORT).show();
        }
        else{
            v.setTag(null);
            ((Button)v).setText("Start Service");
            ScreenOnOffReceiver.mService=false;
            Toast.makeText(this, "Stop Service", Toast.LENGTH_SHORT).show();
        }
    }
    protected void HideActivity()
    {
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.addCategory(Intent.CATEGORY_HOME);
        startActivity(intent);
    }
    class MyServiceReceiver extends ResultReceiver {
        private static final String TAG = "MyServiceReceiver";

        public MyServiceReceiver(Handler handler) {
            super(handler);
        }

        /*
         * MyService will pass results back to this method.
         */
        @Override
        protected void onReceiveResult(int resultCode, Bundle resultData) {
            switch (resultCode) {
                case 1:
                    Log.d(TAG,"yay running");
                    break;
                case 0:
                    Log.d(TAG,"yay finish");
                    break;
                case -1:
                    Log.d(TAG,"yay error");
                    break;
            }
        }
    }
}
