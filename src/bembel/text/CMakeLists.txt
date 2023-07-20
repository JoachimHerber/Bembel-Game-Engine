target_sources(Bembel
	PUBLIC
		text.natvis
		
	PRIVATE
        text.ixx
				
        font.ixx
        font.cpp
        formated-text.ixx
        formated-text.cpp
        text-layout.ixx
        text-layout.cpp
		
        CMakeLists.txt
)

add_subdirectory(i18n)