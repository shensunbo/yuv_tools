cc_library(
    name = "watermark",
    srcs = ["Watermark.cpp"],
    hdrs = ["Watermark.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":log",
    ],
    copts = [
        "-I/usr/include/freetype2",
    ],
    linkopts = ["-lfreetype"],
)

cc_library(
    name = "log",
    hdrs = ["mylog.h"],
    visibility = ["//visibility:public"]
)   

cc_binary(
    name = "watermark_app",
    srcs = [
        "main.cpp",
        "stb_image_write.h"
    ],
    deps = [
        ":watermark",
    ],
    copts = [
        "-I/usr/include/freetype2",
        "-DNV12_PATH=\\\"./res/cam_1920x1536_nv12.raw\\\"",
        "-DNV12_WIDTH=1920",
        "-DNV12_HEIGHT=1536",
    ],
    linkopts = ["-lfreetype"],
)