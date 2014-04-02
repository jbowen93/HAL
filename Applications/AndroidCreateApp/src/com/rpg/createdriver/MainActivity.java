package com.rpg.createdriver;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.ParcelUuid;
import android.os.Parcelable;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class MainActivity extends Activity {

	BluetoothAdapter btAdapter;
//	BluetoothSocket btSocket = null;
//	BluetoothDevice selectedDevice = null;
//	OutputStream oStream = null;
    Set<BluetoothDevice>pairedDevices;
    ArrayList<String> deviceNames;
    ListView listView;
    ArrayAdapter<String> adapter;
    Parcelable[] uuids;
    boolean foundUUID;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        deviceNames = new ArrayList<String>();
        foundUUID = false;
        listView = (ListView) findViewById(R.id.listView1);
        adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, deviceNames);
        listView.setAdapter(adapter);

        listView.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View v, int position,
					long id) {
				// TODO Auto-generated method stub
				String[] full_text = listView.getItemAtPosition(position).toString().split(" ");
				String address = full_text[full_text.length - 1];
				btAdapter.cancelDiscovery();
				
				BluetoothDevice selectedDevice = btAdapter.getRemoteDevice(address);
				
				if (!btAdapter.checkBluetoothAddress(address)) {
					Log.d( "Bluetooth Socket", "Bluetooth address is not valid for: " + address );
				}
				try{
					selectedDevice.fetchUuidsWithSdp();
					UUID uuid;
					uuid = UUID.fromString(uuids[0].toString());					
					foundUUID = true;
				}
				catch(Exception e) {
					Log.d("UUID", "UUID Problem");
				}
				if ( foundUUID ){
					Intent ControlIntent = new Intent(v.getContext(), ControlActivity.class);
					ControlIntent.putExtra("DeviceUUID", uuids[0].toString());
					ControlIntent.putExtra("DeviceAddress", address);
					startActivity(ControlIntent);
				}					        	
			}
        	
		});
        
    	IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND); 
    	registerReceiver(mReceiver, filter);

    	filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        this.registerReceiver(mReceiver, filter);
        
        filter = new IntentFilter(BluetoothDevice.ACTION_UUID);
        this.registerReceiver(mReceiver, filter);
        
    	btAdapter = BluetoothAdapter.getDefaultAdapter();
    	
        if (btAdapter == null) {
        	Toast toast = Toast.makeText(getApplicationContext(), "Your phone does not support bluetooth", 10);
        	toast.show();
        }
        if (!btAdapter.isEnabled()) {
        	Toast toast = Toast.makeText(getApplicationContext(), "This App Requires Bluetooth.\nPlease enable it.", 20);
        	toast.show();
        }        
    }

	BroadcastReceiver mReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();

			if (BluetoothDevice.ACTION_FOUND.equals(action)) 
			{
				BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);				

		        listView = (ListView) findViewById(R.id.listView1);
		        adapter = new ArrayAdapter<String>(context, android.R.layout.simple_list_item_1, deviceNames);
		        listView.setAdapter(adapter);
		        String info = device.getName() + " " + device.getAddress();
		        if (!deviceNames.contains(info)) {
		        	deviceNames.add(info);
		        }
			}
			else {
				if(BluetoothDevice.ACTION_UUID.equals(action)) {
					uuids = intent.getParcelableArrayExtra(BluetoothDevice.EXTRA_UUID);
				}
			}
		}
	};

	public ParcelUuid[] servicesFromDevice(BluetoothDevice device) {
	    try {
	        Class cl = Class.forName("android.bluetooth.BluetoothDevice");
	        Class[] par = {};
	        Method method = cl.getMethod("getUuids", par);
	        Object[] args = {};
	        ParcelUuid[] retval = (ParcelUuid[]) method.invoke(device, args);
	        return retval;
	    } catch (Exception e) {
	        e.printStackTrace();
	        return null;
	    }
	}	
	
	public void discover( View view )
	{
        deviceNames.clear();
		btAdapter.startDiscovery();
	}
	
    public void turnOn( View view )
    {
    	Intent enableBT = new Intent( BluetoothAdapter.ACTION_REQUEST_ENABLE);
    	startActivityForResult(enableBT, 0);
    }
            
}
