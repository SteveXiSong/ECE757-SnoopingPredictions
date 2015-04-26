build/X86/gem5.opt --debug-flags=RubySnoopPred --debug-file=SnoopPred.out configs/example/se.py -c m5threads/tests/test_omp -o "4 16" --ruby --topology Crossbar -n 4 --l1i_size=32kB --l1d_size=32kB --l2_size=512MB --num-l2caches=1

