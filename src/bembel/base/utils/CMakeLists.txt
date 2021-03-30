target_sources(Bembel
    PUBLIC
        concurrent-queue.hpp
		concurrent-queue.inl
		conversion.hpp
		factory.hpp
		factory.inl
		utils.hpp
		signal.hpp
		signal.inl
		slot.hpp
		slot.inl
	PRIVATE
		conversion.cpp
		CMakeLists.txt
)

