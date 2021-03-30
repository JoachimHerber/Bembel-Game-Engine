target_sources(Bembel
    PUBLIC
		logger.hpp
		log-sink.hpp
		log-message.hpp
        console-log-sink.hpp
	PRIVATE
		logger.cpp
		log-message.cpp
		console-log-sink.cpp
		CMakeLists.txt
)

