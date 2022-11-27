target_sources(Bembel
	PRIVATE
		gui-core.ixx

		interaction-handle.ixx
		input-handler.ixx
		input-handler.cpp

		style.ixx
		style.cpp
		gui-renderer.ixx
		gui-renderer.cpp
		render-batch.ixx
		render-batch.cpp

		widget-layout.ixx
		widget-layout.cpp

		widget.ixx
		widget.cpp
		group-widget.ixx
		group-widget.cpp
		
		graphical-user-interface.ixx
		graphical-user-interface.cpp

		CMakeLists.txt
)