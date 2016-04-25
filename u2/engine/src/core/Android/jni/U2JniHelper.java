package org.u2.android;

import android.view.Surface;
import android.content.res.AssetManager;

public class U2JniHelper 
{	
	public native static void init(AssetManager mgr);	
	public native static void term();
}
