# enum class Bitmask

## build the paper
``` shell
make P4313R2.html
```

## run the tests

1. Compile barries fork of llvm with 
 ``` shell
git clone git@github.com:brevzin/llvm-project.git -b compiler-explorer/barry

cmake -G Ninja -S llvm -B build \
  -DLLVM_ENABLE_PROJECTS="clang;lld;clang-tools-extra;" \
  -DLLVM_ENABLE_RUNTIMES="compiler-rt;libcxx;libcxxabi;libunwind" \
  -C clang/cmake/caches/Release.cmake

ninja -v -C build package
```

2. Run the tests using Barry's llvm fork
``` shell
bazel test //src/...
bazel run //src:example
```
