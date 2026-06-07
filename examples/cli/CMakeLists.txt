set(TARGET sd-cli)

if(SD_EXAMPLES_GLOVE_GUI)
	add_executable(${TARGET}
		../common/common.cpp
		../common/log.cpp
		../common/media_io.cpp
		image_metadata.cpp
		main.cpp
		main_glove.h
		${CMAKE_SOURCE_DIR}/thirdparty/glove.h
	)
	target_compile_definitions(${TARGET} PUBLIC -D SD_EXAMPLES_GLOVE_GUI)
	if(SD_EXAMPLES_GLOVE_GUI_DESKTOP)
		target_compile_definitions(${TARGET} PUBLIC -D SD_EXAMPLES_GLOVE_GUI_DESKTOP)
	endif()
	if(SD_EXAMPLES_IMG2IMG_REPEAT)
		target_compile_definitions(${TARGET} PUBLIC -D SD_EXAMPLES_IMG2IMG_REPEAT)
	endif()
	
	if(APPLE)
		sd_set_macos_rpaths(${TARGET})
	endif()
	target_include_directories(${TARGET} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
		"${PROJECT_SOURCE_DIR}/src"
	)
	install(TARGETS ${TARGET} RUNTIME)
	target_link_libraries(${TARGET} PRIVATE stable-diffusion zip ${CMAKE_THREAD_LIBS_INIT} Qt${QT_VERSION}::Widgets)
	if(MSVC)
		target_compile_options(${TARGET} PUBLIC /bigobj)
		target_compile_options(stable-diffusion PUBLIC /bigobj)
	endif()
else()
	add_executable(${TARGET}
		../common/common.cpp
		../common/log.cpp
		../common/media_io.cpp
		image_metadata.cpp
		main.cpp
	)
	if(APPLE)
		sd_set_macos_rpaths(${TARGET})
	endif()
	target_include_directories(${TARGET} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
		"${PROJECT_SOURCE_DIR}/src"
	)
	install(TARGETS ${TARGET} RUNTIME)
	target_link_libraries(${TARGET} PRIVATE stable-diffusion zip ${CMAKE_THREAD_LIBS_INIT})
endif()

if(SD_WEBP)
    target_compile_definitions(${TARGET} PRIVATE SD_USE_WEBP)
    target_link_libraries(${TARGET} PRIVATE webp libwebpmux)
endif()
if(SD_WEBM)
    target_compile_definitions(${TARGET} PRIVATE SD_USE_WEBM)
    target_link_libraries(${TARGET} PRIVATE webm)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_11 cxx_std_17)

install(TARGETS ${TARGET} RUNTIME)

