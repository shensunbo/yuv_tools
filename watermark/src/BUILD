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
    srcs = ["main.cpp"],
    deps = [
        ":watermark",
    ],
    copts = [
        "-I/usr/include/freetype2",
        "-DNV12_PATH=\\\"./res/2560x1600_nv12.yuv\\\"",
        "-DNV12_WIDTH=2560",
        "-DNV12_HEIGHT=1600",
    ],
    linkopts = ["-lfreetype"],
)