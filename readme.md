# usage
```
mkdir build
cd build
cmake ..
make
```

## bazel
```
bazel build watermark/src:watermark

bazel build watermark/src:watermark_app

bazel build watermark/src:fast_watermark_app
```

# TODO list
- [ ] bazel使用系统库的优雅解决方案
- [ ] bazel 定义宏的方案
- [X] 水印使用字符预渲染+查表的方法，now total 298 us, 218 us is FT_Load_Char. Now using preload, time is 70 us