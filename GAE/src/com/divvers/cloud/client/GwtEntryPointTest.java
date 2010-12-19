package com.divvers.cloud.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Button;

public class GwtEntryPointTest implements EntryPoint {

	public void onModuleLoad() {
		RootPanel rootPanel = RootPanel.get();
		
		Button button = new Button("New button");
		rootPanel.add(button, 70, 121);
	}
}
