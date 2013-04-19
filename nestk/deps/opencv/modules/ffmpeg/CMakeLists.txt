project(opencv_ffmpeg)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(
    "${OpenCV_SOURCE_DIR}/3rdparty/include"
    "${OpenCV_SOURCE_DIR}/3rdparty/include/ffmpeg_"
    )
include_directories(
    "${OpenCV_SOURCE_DIR}/modules/highgui/src"
    "${OpenCV_SOURCE_DIR}/modules/core/include"
    "${OpenCV_SOURCE_DIR}/modules/imgproc/include"
    "${OpenCV_SOURCE_DIR}/modules/highgui/include"
    )
link_directories("${OpenCV_SOURCE_DIR}/3rdparty/lib")

set(the_target opencv_ffmpeg)
add_library(${the_target} ffopencv.cpp ffopencv.h)

add_dependencies(${the_target} opencv_core)
target_link_libraries(${the_target} opencv_core)

if (NOT MSVC)
    #target_link_libraries(${the_target}
    #                  ${OpenCV_SOURCE_DIR}/3rdparty/lib/libgcc_.a
    #                  ${OpenCV_SOURCE_DIR}/3rdparty/lib/libmingwex_.a)
    if(WIN32 AND MINGW64)
        target_link_libraries(${the_target} ${OpenCV_SOURCE_DIR}/3rdparty/lib/libavformat64.a
                              ${OpenCV_SOURCE_DIR}/3rdparty/lib/libavcodec64.a
                              ${OpenCV_SOURCE_DIR}/3rdparty/lib/libavutil64.a
                              ${OpenCV_SOURCE_DIR}/3rdparty/lib/libswscale64.a
                              libws2_32.a)
    else()
        target_link_libraries(${the_target} ${OpenCV_SOURCE_DIR}/3rdparty/lib/libavformat.a
                              ${OpenCV_SOURCE_DIR}/3rdparty/lib/libavcodec.a
                              ${OpenCV_SOURCE_DIR}/3rdparty/lib/libavutil.a
                              ${OpenCV_SOURCE_DIR}/3rdparty/lib/libswscale.a
                              ${OpenCV_SOURCE_DIR}/3rdparty/lib/libwsock32_.a)
    endif()
endif ()

set_target_properties(${the_target} PROPERTIES
                      DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
                      VERSION ${OPENCV_VERSION}
                      OUTPUT_NAME "${the_target}${OPENCV_DLLVERSION}"
                      DEFINE_SYMBOL "CVAPI_EXPORTS"
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
                      )

install(TARGETS ${the_target}
    RUNTIME DESTINATION bin COMPONENT main
    ARCHIVE DESTINATION lib COMPONENT main
    LIBRARY DESTINATION lib COMPONENT main
    )
