# enum class Bitmask

gcc already supports reflections:

``` shell
bazel test --config gcc //src/...
bazel test --config gcc //src:bitmask_operators_example
```

Compile with llvm once reflection are supported
``` shell
bazel test --config llvm //src/...
bazel test --config llvm //src:bitmask_operators_example
```

Compile with llvm from https://github.com/bloomberg/clang-p2996

``` shell
docker build -t clang-p2996-bazel -f docker/Dockerfile docker
docker run -ti --rm -v (pwd):/work -w /work clang-p2996-bazel
$ bazel test //src/...
$ bazel run //src:bitmask_operators_example
```
