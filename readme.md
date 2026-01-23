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
```

# TODO list
- [ ] bazel使用系统库的优雅解决方案
- [ ] bazel 定义宏的方案
- [ ] 水印使用字符预渲染+查表的方法，now total 298 us, 218 us is FT_Load_Char