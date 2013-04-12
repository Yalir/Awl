set(SAMPLE "short")

add_executable(
	${SAMPLE}
	main.cpp
)

target_link_libraries(
	${SAMPLE}
	${LIB_NAME}
	pthread
)
