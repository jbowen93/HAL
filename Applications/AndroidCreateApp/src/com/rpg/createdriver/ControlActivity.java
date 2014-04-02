package com.rpg.createdriver;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

public class ControlActivity extends Activity implements OnSeekBarChangeListener {

	BluetoothAdapter btAdapter;
	BluetoothDevice btDevice;
	BluetoothSocket btSocket;
	OutputStream oStream;
	InputStream iStream;
	int speed;
	
	private SeekBar bar;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_control);
		
		
//		bar = (SeekBar) findViewById(R.id.seekBar1);
//		bar.setOnSeekBarChangeListener(this);
		
		btAdapter = BluetoothAdapter.getDefaultAdapter();
		Intent intent = getIntent();
		String address = intent.getStringExtra("DeviceAddress");
		btDevice = btAdapter.getRemoteDevice(address);
		String s_uuid = intent.getStringExtra("DeviceUUID");
		  // Open a socket to connect to the device chosen.
		try {
			UUID uuid = UUID.fromString(s_uuid);
			btSocket = btDevice.createRfcommSocketToServiceRecord(uuid);
			btSocket.connect();
		} catch ( Exception e ) {
			Log.d( "Bluetooth Socket", "Bluetooth not available, or insufficient permissions" );
		}				
		try {
			oStream = btSocket.getOutputStream();
			iStream = btSocket.getInputStream();
			Log.d("OutputStream", "Opened output Stream");
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Log.d("OutputStream", "Unable to open output stream");
		}
		
		try {
			oStream.write((byte) 128);
			oStream.write((byte) 132);
			Log.d("Writing", "Sending start message");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing forward message");
		}
		
		SurfaceView RelativeView = (SurfaceView) findViewById(R.id.SurfaceView1);		
		RelativeView.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				byte[] byteSequence = new byte[5];
				float x = event.getX();
				float y = event.getY();

				float xm = v.getWidth() / 2;
				float ym = v.getHeight() / 2;

				x = xm - x;
				y = ym - y;
				
				int speed = Math.round(500 * (y / ym));
				int radius = -Math.round(2000* (x / xm));
				if (radius > 0) {
					radius = 2000 - radius;
				}
				else if (radius < 0) {
					radius = -2000 - radius;
				}
				boolean rotate = false;
				if ((speed == 0) && (radius != 0)) {
					speed = Math.round(500*(x / xm));
					rotate = true;
				}
				
				byte[] speedBytes = new byte[2];
				byte[] radiusBytes = new byte[2];
				if (speed < 0) {
					speed = 0 - speed;
					speedBytes[0] = (byte) (0xFF - (byte) (speed >>> 8));
					speedBytes[1] = (byte) (0xFF - (byte) speed);
				}
				else {
					speedBytes[0] = (byte) (speed >>> 8);
					speedBytes[1] = (byte) (speed);						
				}
				if (radius < 0) {
					radiusBytes[0] = (byte) (0xFF - radius >>> 8);
					radiusBytes[1] = (byte) (0xFF - radius);						
				}
				else {
					radiusBytes[0] = (byte) (0xFF - radius >>> 8);
					radiusBytes[1] = (byte) (0xFF - radius);												
				}
				if (rotate) {
					if (radius > 0) {
						radiusBytes[0] = (byte) ( 0xFF);
						radiusBytes[1] = (byte) ( 0xFF);
					}
					else {
						radiusBytes[0] = (byte) (  0 & 0xFF);
						radiusBytes[1] = (byte) (  1 & 0xFF);						
					}
				}
				if ((speed != 0) && (radius == 0)) {
					radiusBytes[0] = (byte) ( 80 & 0xFF);
					radiusBytes[1] = (byte) (  0 & 0xFF);
				}
				byteSequence[0] = (byte) (137 & 0xFF);
				byteSequence[1] = speedBytes[0];
				byteSequence[2] = speedBytes[1];
				byteSequence[3] = radiusBytes[0];
				byteSequence[4] = radiusBytes[1];
				
				if (event.getAction() != MotionEvent.ACTION_UP) {
					try {
						oStream.write(byteSequence);
						Log.d("Writing", "Go somewhere - Speed: " + speed + " Radius: "+ radius);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						Log.d("Writing", "Error writing stopping message");
					}
				}
				else {
					byteSequence[0] = (byte) (137 & 0xFF);
					byteSequence[1] = (byte) (  0 & 0xFF);
					byteSequence[2] = (byte) (  0 & 0xFF);
					byteSequence[3] = (byte) (  0 & 0xFF);
					byteSequence[4] = (byte) (  0 & 0xFF);
					try {
						oStream.write(byteSequence);
						Log.d("Writing", "Stopping");
					} catch (IOException e) {
						// TODO Auto-generated catch block
						Log.d("Writing", "Error writing stopping message");
					}
				}
				SystemClock.sleep(100);
				return true;
			}
		});


	}

	public void Forward( View v )
	{
		byte[] byteSequence = new byte[5];		
		
		byte[] speedBytes = new byte[2];
		speedBytes[0] = (byte) (speed >>> 8);
		speedBytes[1] = (byte) (speed);
		
		Log.d("Speed", "Speed = " + speed);
		Log.d("Speed", "Binary Speed = " + speedBytes[0] + " " + speedBytes[1]);
		
		byteSequence[0] = (byte) (137 & 0xFF);
		byteSequence[1] = speedBytes[0];
		byteSequence[2] = speedBytes[1];
		byteSequence[3] = (byte) ( 80 & 0xFF);
		byteSequence[4] = (byte) (  0 & 0xFF);

		try {
			oStream.write(byteSequence);
			Log.d("Writing", "And away we go!!");

		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing forward message");
		}

		SystemClock.sleep(1000);
		byteSequence[0] = (byte) (137 & 0xFF);
		byteSequence[1] = (byte) (  0 & 0xFF);
		byteSequence[2] = (byte) (  0 & 0xFF);
		byteSequence[3] = (byte) (  0 & 0xFF);
		byteSequence[4] = (byte) (  0 & 0xFF);
		try {
			oStream.write(byteSequence);
			Log.d("Writing", "Stopping");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing stopping message");
		}
	}

	public void onProgressChanged(SeekBar sb, int progress, boolean fromUser)
	{
		speed = progress;
	}
	
	public void Reverse( View v )
	{
		byte[] byteSequence = new byte[5];

		byte[] speedBytes = new byte[2];
		speedBytes[0] = (byte) (0xFF - (byte) (speed >> 8));
		speedBytes[1] = (byte) (0xFF - (byte) speed);

		Log.d("Speed", "Speed = " + speed);
		Log.d("Speed", "Binary Speed = " + speedBytes[0] + " " + speedBytes[1]);
		
		byteSequence[0] = (byte) (137 & 0xFF);
		byteSequence[1] = speedBytes[0];
		byteSequence[2] = speedBytes[1];
		byteSequence[3] = (byte) ( 80 & 0xFF);
		byteSequence[4] = (byte) (  0 & 0xFF);

		
		try {
			oStream.write(byteSequence);
			Log.d("Writing", "And away we go!!");

		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing forward message");
		}

		SystemClock.sleep(1000);
		byteSequence[0] = (byte) (137 & 0xFF);
		byteSequence[1] = (byte) (  0 & 0xFF);
		byteSequence[2] = (byte) (  0 & 0xFF);
		byteSequence[3] = (byte) (255 & 0xFF);
		byteSequence[4] = (byte) (255 & 0xFF);
		try {
			oStream.write(byteSequence);
			Log.d("Writing", "Stopping");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing stopping message");
		}

	}

	public void Left( View v )
	{
		byte[] byteSequence = new byte[5];

		byte[] speedBytes = new byte[2];
		speedBytes[0] = (byte) (speed >>> 8);
		speedBytes[1] = (byte) (speed);

		byteSequence[0] = (byte) (137 & 0xFF);
		byteSequence[1] = speedBytes[0];
		byteSequence[2] = speedBytes[1];
		byteSequence[3] = (byte) ( 000 & 0xFF);
		byteSequence[4] = (byte) ( 001 & 0xFF);

		try {
			oStream.write(byteSequence);
			Log.d("Writing", "And away we go!!");

		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing forward message");
		}

		SystemClock.sleep(500);
		byteSequence[0] = (byte) (137 & 0xFF);
		byteSequence[1] = (byte) (  0 & 0xFF);
		byteSequence[2] = (byte) (  0 & 0xFF);
		byteSequence[3] = (byte) (255 & 0xFF);
		byteSequence[4] = (byte) (255 & 0xFF);
		try {
			oStream.write(byteSequence);
			Log.d("Writing", "Stopping");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing stopping message");
		}		
	}

	public void Right( View v )
	{
		byte[] byteSequence = new byte[5];
		
		byte[] speedBytes = new byte[2];
		speedBytes[0] = (byte) (speed >>> 8);
		speedBytes[1] = (byte) (speed);

		byteSequence[0] = (byte) (137 & 0xFF);
		byteSequence[1] = speedBytes[0];
		byteSequence[2] = speedBytes[1];
		byteSequence[3] = (byte) (255 & 0xFF);
		byteSequence[4] = (byte) (255 & 0xFF);

		try {
			oStream.write(byteSequence);
			Log.d("Writing", "And away we go!!");

		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing forward message");
		}

		SystemClock.sleep(500);
		byteSequence[0] = (byte) (137 & 0xFF);
		byteSequence[1] = (byte) (  0 & 0xFF);
		byteSequence[2] = (byte) (  0 & 0xFF);
		byteSequence[3] = (byte) (  0 & 0xFF);
		byteSequence[4] = (byte) (  1 & 0xFF);
		try {
			oStream.write(byteSequence);
			Log.d("Writing", "Stopping");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d("Writing", "Error writing stopping message");
		}

	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar) {
		// TODO Auto-generated method stub
		
	}

}
