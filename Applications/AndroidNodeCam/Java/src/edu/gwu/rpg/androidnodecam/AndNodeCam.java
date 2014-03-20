package edu.gwu.rpg.androidnodecam;

public class AndNodeCam {

	static {
		System.loadLibrary("opencv_java");
		System.loadLibrary("pbmsgs");		
		System.loadLibrary("miniglog");
		System.loadLibrary("node");
		System.loadLibrary("hal");
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("AndNodeCam");
	}
	
	private native void sendData(byte[] bytes);
	
	private native void Initialize();
	
	public void InitANC( ) 
	{
		Initialize();
	}
	
	public void sendCamData( byte[] bytes )
	{ // 0x5305e2b4
		sendData(bytes);
	}
	
}