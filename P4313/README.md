# enum class Bitmask

gcc already supports reflections:

``` shell
bazel test --config gcc //src/...
bazel run --config gcc //src:example
```

Compile with llvm once reflection are supported
``` shell
bazel test --config llvm //src/...
bazel run --config llvm //src:example
```

# run the paper tests

2. compile barries fork of llvm with 
 ``` shell
git clone git@github.com:brevzin/llvm-project.git -b compiler-explorer/barry

cmake -G Ninja -S llvm -B build \
  -DLLVM_ENABLE_PROJECTS="clang;lld;clang-tools-extra;" \
  -DLLVM_ENABLE_RUNTIMES="compiler-rt;libcxx;libcxxabi;libunwind" \
  -C clang/cmake/caches/Release.cmake

ninja -v -C build package
```

