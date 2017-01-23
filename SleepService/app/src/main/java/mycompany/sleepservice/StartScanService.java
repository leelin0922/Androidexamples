package mycompany.sleepservice;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Binder;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.PowerManager;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import mycompany.sleepservice.Events.InputDevice;

public class StartScanService extends Service {

	private static Timer mTimer=null;
	private StartScanService ctx;
	boolean m_bMonitorOn=false;
	Events events = new Events();
	private Handler mThreadHandler;
	private HandlerThread mThread;
	private static int touchcount = 0;
	int m_type = 0;
	int m_code = 0;
	int m_value = 0;
	private int mthreadTask=2;
	@Override
	public void onCreate() {
		// INITIALIZE RECEIVER
		IntentFilter filter = new IntentFilter(Intent.ACTION_SCREEN_ON);
		filter.addAction(Intent.ACTION_SCREEN_OFF);
		BroadcastReceiver mReceiver = new ScreenOnOffReceiver();
		registerReceiver(mReceiver, filter);
		ComponentName componentToDisable = new ComponentName("mycompany.sleepservice", "mycompany.sleepservice.MainActivity");
		getPackageManager().setComponentEnabledSetting( componentToDisable, PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);
		// YOUR CODE
		if(mthreadTask==1) {
			mThread = new HandlerThread("name");
			mThread.start();
			mThreadHandler=new Handler(mThread.getLooper());
			mThreadHandler.post(RecheckScreen);
		}
	}
    public class LocalBinder extends Binder
    {
        StartScanService getService()
        {
            return StartScanService.this;
        }
    }
    // This is the object that receives interactions from clients.  See
    // RemoteService for a more complete example.
    private final IBinder mBinder = new LocalBinder();
    //
    @Override
    public IBinder onBind(Intent intent)
    {
        return mBinder;
    }

	@Override
	public void onDestroy() {
		Log.d("thread", "onDestroy");
		if(mTimer!=null)
		{
			mTimer.cancel();
			mTimer = null;
		}
		if(events!=null) {
			events.Release();
			events = null;
		}
		stopForeground(true);
        //Intent localIntent = new Intent();
        //localIntent.setClass(this, StartScanService.class); //銷毀時重新啟動Service
        //this.startService(localIntent);
		try {
			PowerManager pm = (PowerManager) getApplicationContext().getSystemService(Context.POWER_SERVICE);
			PowerManager.WakeLock wakeLock = pm.newWakeLock((PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP), "TAG");
			wakeLock.acquire();
			Thread.sleep(3000);
			wakeLock.release();
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		super.onDestroy();
	}

	private Runnable checkScreen=new Runnable () {
	    public void run() {
    		int recounter=0;

    		mTimer=null;
	    	events.intEnableDebug(0);
		    events.Init();
    		for (InputDevice idev:events.m_Devs) {
    			if (idev.Open(false)){
	    			if(idev.getName().contains("ouch")==true)
		    		{
			    	}
				    else
					    idev.Close();
    			}
	    	}
            while (m_bMonitorOn) {
    			for (InputDevice idev : events.m_Devs) {
	    			if (idev.getOpen() && (0 == idev.getPollingEvent())) {
		    			m_type = idev.getSuccessfulPollingType();
			    		m_code = idev.getSuccessfulPollingCode();
				    	m_value = idev.getSuccessfulPollingValue();
    					if (m_type == 1 && m_code == 330 && m_value == 0) {
	    					touchcount++;
    					} else {
	    					if (m_type == 3 && m_code == 57 && m_value == -1) {
		    					touchcount++;
    						}
	    				}
		    		}
    			}
	    		recounter++;
		    	if(recounter%100==0)
			    	Log.d("thread", "recounter "+recounter);
    			if (touchcount > 0 && ScreenOnOffReceiver.screenOff == true) {
	    			try {
		    			PowerManager pm = (PowerManager) getApplicationContext().getSystemService(Context.POWER_SERVICE);
			    		PowerManager.WakeLock wakeLock = pm.newWakeLock((PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP), "TAG");
				    	wakeLock.acquire();
					    Thread.sleep(3000);
    					wakeLock.release();
	    				Thread.sleep(1000);
		    		} catch (InterruptedException e) {
			    		e.printStackTrace();
				    }
    			}
	    		touchcount = 0;
		    	try {
			    	Thread.sleep(10);
    			} catch (InterruptedException e) {
	    			e.printStackTrace();
		    	}
            }
	    }
	};

	private Runnable RecheckScreen=new Runnable () {
		public void run() {
		int touch_detect=0;
		int recounter=0;
		int time_counter=0;
        int minutes_counter=0;

		m_bMonitorOn=true;
		while(m_bMonitorOn==true) {
			//Log.d("thread", "start ");
			events.intEnableDebug(0);
			events.Init();
			touch_detect=0;
			time_counter=0;
			if (ScreenOnOffReceiver.screenOff == true) {
				for (InputDevice idev : events.m_Devs) {
					//* Finds an open device that has a name containing keypad. This probably is the keypad associated event node
					if (idev.Open(false)) {
						if (idev.getName().contains("ouch") == true) {
							//touch_id = idev.getId();
							touch_detect++;
						} else {
							idev.Close();
						}
					}
				}
				//Log.d("thread", "touch_detect  " + touch_detect);
				while (ScreenOnOffReceiver.screenOff == true) {
					for (InputDevice idev : events.m_Devs) {
						if(idev.getOpen() ==true){
						 	if(idev.getPollingEvent()==0) {
								m_type = idev.getSuccessfulPollingType();
								m_code = idev.getSuccessfulPollingCode();
								m_value = idev.getSuccessfulPollingValue();
								if (m_type == 1 && m_code == 330 && m_value == 0) {
									touchcount++;
									Log.d("threadR", "wake up");
								} else {
									if (m_type == 3 && m_code == 57 && m_value == -1) {
										touchcount++;
										Log.d("threadP", "wake up");
									}
								}
							}
						}
					}
					recounter++;
					if(recounter%100==0) {
						recounter=0;
						time_counter++;
						Log.d("thread", "time counter  " + time_counter + " secs.");
						if(time_counter>59) {
                            minutes_counter++;
							//Log.d("thread", "sleep over "+ minutes_counter + " mins.");
							break;
						}
					}
					if (touchcount > 0 && ScreenOnOffReceiver.screenOff == true) {
						try {
							PowerManager pm = (PowerManager) getApplicationContext().getSystemService(Context.POWER_SERVICE);
							PowerManager.WakeLock wakeLock = pm.newWakeLock((PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP), "TAG");
							wakeLock.acquire();
							//Log.d("Tuoch", "wake up");
							Thread.sleep(1000);
							wakeLock.release();
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
					touchcount = 0;
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
            else {
                minutes_counter = 0;
            }
			for (InputDevice idev : events.m_Devs) {
				if (idev.getOpen()) {
					idev.Close();
				}
			}
			try {
				//Log.d("thread", "Sleep 1000");
				Thread.sleep(1000);
                post();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		}
	};

	private void timerTask(){
		//int touch_id=-1;

		mTimer=null;
		events.intEnableDebug(0);
		events.Init();
		for (InputDevice idev:events.m_Devs) {
			//* Finds an open device that has a name containing keypad. This probably is the keypad associated event node
			if (idev.Open(false)){
				if(idev.getName().contains("ouch")==true)
				{
					//touch_id=idev.getId();
				}
				else
					idev.Close();
				//if(touch_id>-1)
				//{
				//	m_bMonitorOn=true;
				//	break;
				//}
			}
		}
		if(mTimer != null) {
			mTimer.cancel();
		}
		mTimer = new Timer();
		mTimer.schedule(new TimerTask() {
			@Override
			public void run() {
				for (InputDevice idev : events.m_Devs) {
					if (idev.getOpen() && (0 == idev.getPollingEvent())) {
						m_type = idev.getSuccessfulPollingType();
						m_code = idev.getSuccessfulPollingCode();
						m_value = idev.getSuccessfulPollingValue();
						//String line = idev.getName() + ":" + m_type + " " + m_code + " " + m_value;
						//Log.d("thread", "Event:" + line);
						if (m_type == 1 && m_code == 330 && m_value == 0) {
							touchcount++;
							Log.d("TimerTask", "Event:"+touchcount);
						} else {
							if (m_type == 3 && m_code == 57 && m_value == -1) {
								touchcount++;
								Log.d("TimerTask", "Event:"+touchcount);
							}
						}
					}
				}
				if (touchcount > 0 && ScreenOnOffReceiver.screenOff == true) {
					touchcount = 0;
					try {
						//PowerManager.WakeLock wakeLock = pm.newWakeLock((PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.FULL_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP), "TAG");
						//PowerManager.WakeLock wakeLock = pm.newWakeLock((PowerManager.ACQUIRE_CAUSES_WAKEUP), "TAG");
						PowerManager pm = (PowerManager) getApplicationContext().getSystemService(Context.POWER_SERVICE);
						PowerManager.WakeLock wakeLock = pm.newWakeLock((PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP), "TAG");
						wakeLock.acquire();
						Thread.sleep(3000);
						wakeLock.release();
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				} else {
					touchcount = 0;
				}
			}
		}, 10, 20);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {

		ctx = this;
		try {
			PowerManager pm = (PowerManager) getApplicationContext().getSystemService(Context.POWER_SERVICE);
			PowerManager.WakeLock wakeLock = pm.newWakeLock((PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP), "TAG");
			wakeLock.acquire();
			//Log.d("Tuoch", "wake up");
			Thread.sleep(1000);
			wakeLock.release();
		//	Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		Log.d("thread", "onStartCommand");
		if(mthreadTask==0){
			timerTask();
		}
		if(mthreadTask==2){
			Thread threadTask = new Thread(RecheckScreen);
			threadTask.start();
		}
		return Service.START_STICKY;
	}
    private void post()
    {
        Intent intent2 = new Intent(ctx, MainActivity.class);
        PendingIntent pi = PendingIntent.getActivity(ctx, 0, intent2,   PendingIntent.FLAG_UPDATE_CURRENT);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(ctx);

        //builder.setSmallIcon(R.drawable.ic_launcher);
        builder.setTicker("App info string");
        builder.setContentIntent(pi);
        builder.setOngoing(true);
        long[] vibraPattern = {0, 500, 250, 500 };
        builder.setVibrate(vibraPattern);
        builder.setOnlyAlertOnce(true);
        builder.setAutoCancel(true);
        Notification notification = builder.build();
        notification.flags = Notification.FLAG_AUTO_CANCEL;
        // optionally set a custom view
        startForeground(1234, notification);
    }
}
