# RISC-V Vectorized Bencmark Suite

## Overview

The RISC-V Vectorized Benchmark Suite is a collection composed of seven data-parallel applications from different domains. The suite focuses on benchmarking vector microarchitectures; nevertheless, it can be used as well for Multimedia SIMD microarchitectures. Current implementation is targeting RISC-V Architectures; however, it can be easily ported to any Vector/SIMD ISA thanks to a wrapper library which we developed to map vector intrinsics and math functions to the target architecture.

The benchmark suite with all its applications and input sets is available as open source free of charge. Some of the benchmark programs have their own licensing terms which might limit their use in some cases.

The implementation is based on the working draft of the proposed [RISC-V V vector extension v1.0 Spec](https://github.com/riscv/riscv-v-spec) and [intrinsic API](https://github.com/riscv/rvv-intrinsic-doc).

If you use this software or a modified version of it for your research, please cite the paper:
Cristóbal Ramirez, César Hernandez, Oscar Palomar, Osman Unsal, Marco Ramírez, and Adrián Cristal. 2020. A RISC-V Simulator and Benchmark Suite for Designing and Evaluating Vector Architectures. ACM Trans. Archit. Code Optim. 17, 4, Article 38 (October 2020), 29 pages. https://doi.org/10.1145/3422667

## Vectorized apps

| Application Name  | Application Domain            | Algorithmic Model     | Taken From  |
| ----------------- |------------------------------ | --------------------- | ----------- |
| Axpy              | High Performance Computing    | BLAS                  | -           |
| Blackscholes      | Financial Analysis            | Dense Linear Algebra  | PARSEC      |
| Canneal           | Engineering                   | Unstructured Grids    | PARSEC      |
| LavaMD2           | Molecular Dynamics            | N-Body                | Rodinia     |
| Particle Filter   | Medical Imaging               | Structured Grids      | Rodinia     |
| Somier            | Physics Simulation            | Dense Linear Algebra  | -           |
| Jacobi-2D         | Engineering                   | Dense Linear Algebra  | PolyBench   |
| Pathfinder        | Grid Traversal                | Dynamic Programming   | Rodinia     |
| Streamcluster     | Data Mining                   | Dense Linear Algebra  | PARSEC      |
| Swaptions         | Financial Analysis            | MapReduce Regular     | PARSEC      |


## Building Vectorized Applications 

The RISC-V Vectorized Bencmark Suite has been successfully tested on [Spike RISC-V ISA Simulator](https://github.com/riscv/riscv-isa-sim)

### Setting up the environment

The Suite includes a makefile to compile every application, in order to use it, you must define the path to the RISC-V vector compiler.

Setting the Vector Compiler path
```
export LLVM=$TOP/riscv-toolchain
```

Currently, the Suite can only be compiled by the [LLVM from PLCT](https://github.com/isrc-cas/rvv-llvm). We will update the Suite to be compiled by the upstream LLVM when it is available.

### Compile using  clang for RISCV Vector Version

We provide precompiled binaries found in the folder bin.

To compile any application you first enter in the subfolder and run the command make followed by the application name
```
cd _application
make application 
```
For example to compile blackscholes:
```
cd _blackscholes
make blackscholes 
```
The same for the other applications ...


### Running applications

There are provided 5 different simulation sizes (arguments to run the application). But I only use it for functional testing, without calculating the time.
```
simtiny 
simsmall
simmedium
simlarge 
```

Whe you are executing an application, you must write the following arguments to run a predefined simsize.
#### simtiny 
```
blackscholes_args   = "1 input/in_512.input output_prices.txt"
canneal_args        = "1 100 300 input/100.nets 8"
streamcluster_args  = "3 10 128 128 128 10 none output.txt 1"
swaptions_args      = "-ns 8 -sm 512 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 2 -np 256"
pathfinder_args     = "32 32 output.txt"
heatequation_args   = " input/test_small.input output.ppm"
matmul_args         = "8 8 8"
axpy_args           = "256"
```

#### simsmall 
```
blackscholes_args   = "1 input/in_4K.input output_prices.txt"
canneal_args        = "1 10000 2000 input/100000.nets 32"
streamcluster_args  = "10 20 128 4096 4096 1000 none output.txt 1"
swaptions_args      = "-ns 8 -sm 4096 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 8 -np 1024"
pathfinder_args     = "1024 128 output.txt"
heatequation_args   = " input/test_small.input output.ppm"
matmul_args         = "128 128 128"
axpy_args           = "512"
```  

#### simmedium
```
blackscholes_args   = "1 input/in_16K.input prices.txt"
canneal_args        = "1 15000 2000 input/200000.nets 64"
streamcluster_args  = "10 20 128 8192 8192 1000 none output.txt 1"
swaptions_args      = "-ns 32 -sm 8192 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 16 -np 4096"
pathfinder_args     = "2048 256 output.txt"
heatequation_args   = " input/test_medium.input output.ppm"
matmul_args         = "256 256 256"
axpy_args           = "1024"
```  

#### simlarge
```
blackscholes_args   = "1 input/in_64K.input prices.txt"
canneal_args        = "1 15000 2000 input/400000.nets 128"
streamcluster_args  = "10 20 128 8192 8192 1000 none output.txt 1"
swaptions_args      = "-ns 64 -sm 16384 -nt 1"
particlefilter_args = "-x 128 -y 128 -z 24 -np 8192"
pathfinder_args     = "2048 1024 output.txt"
heatequation_args   = " input/test_large.input output.ppm"
matmul_args         = "1024 1024 1024"
axpy_args           = "2048"
```  

#### Example of execution blackscholes serial version.
```
./blackscholes_serial 1 input/in_64K.input prices.txt

```
#### Example of execution blackscholes vector version.
```
./blackscholes_vector 1 input/in_64K.input prices.txt

```

You will also need to install the [riscv-pk](https://github.com/riscv/riscv-pk) in order for Spike to run

## Contact
Cristóbal Ramírez Lazo: cristobal.ramirez@bsc.es
PhD. Student at UPC Barcelona   
BSC - Barcelona Supercomputing Center

Chunyu Liao chunyu@iscas.ac.cn
PLCT Lab

## Instructions Used

```bash
git grep -Po '\b_MM_[a-zA-Z0-9_]+\b' -- _* | sort | uniq | sed 's#^_\(.*\)/src.*:_MM_\(.*\)$#| \1 | `\L\2` |#' >> README.md
```

| Application | Intrinsic |
| --- | --- |
| axpy | `load_f64` |
| axpy | `macc_f64` |
| axpy | `set_f64` |
| axpy | `store_f64` |
| blackscholes | `add_f64` |
| blackscholes | `align64` |
| blackscholes | `div_f64` |
| blackscholes | `exp_f64` |
| blackscholes | `load_f64` |
| blackscholes | `load_i64` |
| blackscholes | `log_f64` |
| blackscholes | `macc_f64` |
| blackscholes | `madd_f64` |
| blackscholes | `merge_f64` |
| blackscholes | `mul_f64` |
| blackscholes | `nmsub_f64` |
| blackscholes | `set_f64` |
| blackscholes | `set_i64` |
| blackscholes | `sqrt_f64` |
| blackscholes | `store_f64` |
| blackscholes | `sub_f64` |
| blackscholes | `sub_f64_mask` |
| blackscholes | `vflt_f64` |
| blackscholes | `vfsgnjn_f64` |
| blackscholes | `vfsgnjx_f64` |
| blackscholes | `vmseq_i64` |
| blackscholes | `add_f32` |
| blackscholes | `div_f32` |
| blackscholes | `exp_f32` |
| blackscholes | `load_f32` |
| blackscholes | `load_i32` |
| blackscholes | `log_f32` |
| blackscholes | `macc_f32` |
| blackscholes | `madd_f32` |
| blackscholes | `merge_f32` |
| blackscholes | `mul_f32` |
| blackscholes | `nmsub_f32` |
| blackscholes | `set_f32` |
| blackscholes | `set_i32` |
| blackscholes | `sqrt_f32` |
| blackscholes | `store_f32` |
| blackscholes | `sub_f32` |
| blackscholes | `sub_f32_mask` |
| blackscholes | `vflt_f32` |
| blackscholes | `vfsgnjn_f32` |
| blackscholes | `vfsgnjx_f32` |
| blackscholes | `vmseq_i32` |
| canneal | `cast_i1_i32` |
| canneal | `load_i32` |
| canneal | `merge_i32` |
| canneal | `set_i32` |
| canneal | `add_f32` |
| canneal | `load_i64` |
| canneal | `load_index_i64` |
| canneal | `redsum_f32` |
| canneal | `set_f32` |
| canneal | `sub_i32` |
| canneal | `vfcvt_f_x_f32` |
| canneal | `vfsgnjx_f32` |
| canneal | `vgetfirst_f32` |
| jacobi-2d | `add_f64` |
| jacobi-2d | `load_f64` |
| jacobi-2d | `mul_f64` |
| jacobi-2d | `set_f64` |
| jacobi-2d | `store_f64` |
| jacobi-2d | `vslide1down_f64` |
| jacobi-2d | `vslide1up_f64` |
| jacobi-2d | `add_f64` |
| jacobi-2d | `load_f64` |
| jacobi-2d | `macc_f64` |
| jacobi-2d | `mul_f64` |
| jacobi-2d | `redsum_f64` |
| jacobi-2d | `set_f64` |
| jacobi-2d | `store_f64` |
| jacobi-2d | `sub_f64` |
| jacobi-2d | `vgetfirst_f64` |
| jacobi-2d | `vslide1down_f64` |
| jacobi-2d | `vslide1up_f64` |
| particlefilter | `add_f64` |
| particlefilter | `add_i64` |
| particlefilter | `cast_i1_i64` |
| particlefilter | `cast_i64_i1` |
| particlefilter | `cos_f64` |
| particlefilter | `div_f64` |
| particlefilter | `load_f64` |
| particlefilter | `load_i64` |
| particlefilter | `log_f64` |
| particlefilter | `merge_i64` |
| particlefilter | `mul_f64` |
| particlefilter | `mul_i64` |
| particlefilter | `or_i64` |
| particlefilter | `rem_i64` |
| particlefilter | `set_f64` |
| particlefilter | `set_i64` |
| particlefilter | `sqrt_f64` |
| particlefilter | `store_f64` |
| particlefilter | `store_i64` |
| particlefilter | `vfcvt_f_x_f64` |
| particlefilter | `vfge_f64` |
| particlefilter | `vfsgnjx_f64` |
| particlefilter | `vmfirst_i64` |
| particlefilter | `vmpopc_i64` |
| particlefilter | `xor_i64` |
| pathfinder | `add_i32` |
| pathfinder | `load_i32` |
| pathfinder | `min_i32` |
| pathfinder | `store_i32` |
| pathfinder | `vslide1down_i32` |
| pathfinder | `vslide1up_i32` |
| streamcluster | `load_f32` |
| streamcluster | `macc_f32` |
| streamcluster | `redsum_f32` |
| streamcluster | `set_f32` |
| streamcluster | `sub_f32` |
| streamcluster | `vgetfirst_f32` |
| swaptions | `add_f64` |
| swaptions | `div_f64` |
| swaptions | `load_f64` |
| swaptions | `log_f64` |
| swaptions | `merge_f64` |
| swaptions | `mul_f64` |
| swaptions | `set_f64` |
| swaptions | `store_f64` |
| swaptions | `sub_f64` |
| swaptions | `sub_f64_mask` |
| swaptions | `vfgt_f64` |
| swaptions | `vflt_f64` |
| swaptions | `vfsgnjn_f64` |
| swaptions | `vfsgnjx_f64` |
| swaptions | `exp_f64` |
| swaptions | `load_f64` |
| swaptions | `mul_f64` |
| swaptions | `set_f64` |
| swaptions | `store_f64` |
| swaptions | `vfsgnjn_f64` |
| swaptions | `add_f64` |
| swaptions | `load_f64` |
| swaptions | `mul_f64` |
| swaptions | `set_f64` |
| swaptions | `store_f64` |
| swaptions | `load_f64` |
| swaptions | `macc_f64` |
| swaptions | `max_f64` |
| swaptions | `mul_f64` |
| swaptions | `redsum_f64` |
| swaptions | `set_f64` |
| swaptions | `store_f64` |
| swaptions | `sub_f64` |
| swaptions | `add_i64_mask` |
| swaptions | `div_i64` |
| swaptions | `load_i64` |
| swaptions | `mul_f64` |
| swaptions | `mul_i64` |
| swaptions | `set_f64` |
| swaptions | `set_i64` |
| swaptions | `store_f64` |
| swaptions | `sub_i64` |
| swaptions | `vfcvt_f_x_f64` |
| swaptions | `vmslt_i64` |
| tests | `load_f32` |
| tests | `load_i32` |
| tests | `mul_f32` |
| tests | `store_f64` |

| Application | Instruction |
| --- | --- |
| axpy | `vfmacc.vv` |
| axpy | `vfmv.v.f` |
| axpy | `vle.v` |
| axpy | `vsetvli` |
| axpy | `vse.v` |
| blackscholes | `vadd.vv` |
| blackscholes | `vand.vv` |
| blackscholes | `vfadd.vv` |
| blackscholes | `vfcvt.f.x.v` |
| blackscholes | `vfcvt.x.f.v` |
| blackscholes | `vfdiv.vv` |
| blackscholes | `vfmacc.vv` |
| blackscholes | `vfmadd.vv` |
| blackscholes | `vfmax.vv` |
| blackscholes | `vfmin.vv` |
| blackscholes | `vfmul.vv` |
| blackscholes | `vfmv.v.f` |
| blackscholes | `vfsgnjn.vv` |
| blackscholes | `vfsgnjx.vv` |
| blackscholes | `vfsqrt.v` |
| blackscholes | `vfsub.vv` |
| blackscholes | `vle.v` |
| blackscholes | `vmerge.vvm` |
| blackscholes | `vmfle.vv` |
| blackscholes | `vmflt.vv` |
| blackscholes | `vmseq.vv` |
| blackscholes | `vmv.v.i` |
| blackscholes | `vmv.v.v` |
| blackscholes | `vmv.v.x` |
| blackscholes | `vor.vv` |
| blackscholes | `vsetvl` |
| blackscholes | `vsetvli` |
| blackscholes | `vse.v` |
| blackscholes | `vsll.vv` |
| blackscholes | `vsrl.vv` |
| blackscholes | `vsub.vv` |
| canneal | `vfadd.vv` |
| canneal | `vfcvt.f.x.v` |
| canneal | `vfmv.f.s` |
| canneal | `vfmv.v.f` |
| canneal | `vfredsum.vs` |
| canneal | `vfsgnjx.vv` |
| canneal | `vle.v` |
| canneal | `vlxe.v` |
| canneal | `vmerge.vvm` |
| canneal | `vmv.v.v` |
| canneal | `vmv.v.x` |
| canneal | `vsetvl` |
| canneal | `vsetvli` |
| canneal | `vse.v` |
| canneal | `vsub.vv` |
| jacobi-2d | `vfadd.vv` |
| jacobi-2d | `vfmul.vv` |
| jacobi-2d | `vfmv.v.f` |
| jacobi-2d | `vle.v` |
| jacobi-2d | `vmv.v.v` |
| jacobi-2d | `vsetvl` |
| jacobi-2d | `vsetvli` |
| jacobi-2d | `vse.v` |
| jacobi-2d | `vslide1down.vx` |
| jacobi-2d | `vslide1up.vx` |
| particlefilter | `vadd.vv` |
| particlefilter | `vand.vv` |
| particlefilter | `vfadd.vv` |
| particlefilter | `vfcvt.f.x.v` |
| particlefilter | `vfcvt.x.f.v` |
| particlefilter | `vfmacc.vv` |
| particlefilter | `vfmadd.vv` |
| particlefilter | `vfmax.vv` |
| particlefilter | `vfmul.vv` |
| particlefilter | `vfmv.v.f` |
| particlefilter | `vfsqrt.v` |
| particlefilter | `vfsub.vv` |
| particlefilter | `vle.v` |
| particlefilter | `vmerge.vvm` |
| particlefilter | `vmfirst.m` |
| particlefilter | `vmfle.vv` |
| particlefilter | `vmflt.vv` |
| particlefilter | `vmpopc.m` |
| particlefilter | `vmseq.vv` |
| particlefilter | `vmv.v.i` |
| particlefilter | `vmv.v.v` |
| particlefilter | `vmv.v.x` |
| particlefilter | `vor.vv` |
| particlefilter | `vsetvl` |
| particlefilter | `vsetvli` |
| particlefilter | `vse.v` |
| particlefilter | `vsll.vv` |
| particlefilter | `vsrl.vv` |
| particlefilter | `vsub.vv` |
| particlefilter | `vxor.vv` |
| pathfinder | `vadd.vv` |
| pathfinder | `vle.v` |
| pathfinder | `vmin.vv` |
| pathfinder | `vsetvli` |
| pathfinder | `vse.v` |
| pathfinder | `vslide1down.vx` |
| pathfinder | `vslide1up.vx` |
| streamcluster | `vfmacc.vv` |
| streamcluster | `vfmv.f.s` |
| streamcluster | `vfmv.v.f` |
| streamcluster | `vfredsum.vs` |
| streamcluster | `vfsub.vv` |
| streamcluster | `vle.v` |
| streamcluster | `vmv.v.v` |
| streamcluster | `vsetvl` |
| streamcluster | `vsetvli` |
| swaptions | `vadd.vv` |
| swaptions | `vand.vv` |
| swaptions | `vdiv.vv` |
| swaptions | `vfadd.vv` |
| swaptions | `vfcvt.f.x.v` |
| swaptions | `vfcvt.x.f.v` |
| swaptions | `vfdiv.vv` |
| swaptions | `vfmacc.vv` |
| swaptions | `vfmadd.vv` |
| swaptions | `vfmax.vv` |
| swaptions | `vfmin.vv` |
| swaptions | `vfmul.vv` |
| swaptions | `vfmv.v.f` |
| swaptions | `vfredsum.vs` |
| swaptions | `vfsgnjn.vv` |
| swaptions | `vfsgnjx.vv` |
| swaptions | `vfsub.vv` |
| swaptions | `vle.v` |
| swaptions | `vmerge.vvm` |
| swaptions | `vmfle.vv` |
| swaptions | `vmflt.vv` |
| swaptions | `vmslt.vv` |
| swaptions | `vmul.vv` |
| swaptions | `vmv.v.i` |
| swaptions | `vmv.v.v` |
| swaptions | `vmv.v.x` |
| swaptions | `vor.vv` |
| swaptions | `vsetvl` |
| swaptions | `vsetvli` |
| swaptions | `vse.v` |
| swaptions | `vsll.vv` |
| swaptions | `vsrl.vv` |
| swaptions | `vsub.vv` |
| tests | `vadd.vv` |
| tests | `vand.vv` |
| tests | `vfadd.vv` |
| tests | `vfcvt.f.x.v` |
| tests | `vfcvt.x.f.v` |
| tests | `vfdiv.vv` |
| tests | `vfmacc.vv` |
| tests | `vfmadd.vv` |
| tests | `vfmax.vv` |
| tests | `vfmin.vv` |
| tests | `vfmul.vv` |
| tests | `vfmv.v.f` |
| tests | `vfsgnjn.vv` |
| tests | `vfsgnjx.vv` |
| tests | `vfsqrt.v` |
| tests | `vfsub.vv` |
| tests | `vle.v` |
| tests | `vmerge.vvm` |
| tests | `vmfle.vv` |
| tests | `vmflt.vv` |
| tests | `vmseq.vv` |
| tests | `vmv.v.i` |
| tests | `vmv.v.v` |
| tests | `vmv.v.x` |
| tests | `vor.vv` |
| tests | `vsetvl` |
| tests | `vsetvli` |
| tests | `vse.v` |
| tests | `vsll.vv` |
| tests | `vsrl.vv` |
| tests | `vsub.vv` |
