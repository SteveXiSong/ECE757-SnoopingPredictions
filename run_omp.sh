build/X86/gem5.opt --debug-flags=RubySlicc,RubyOwnerPred --debug-file=OwnerPred.out configs/example/se.py -c m5threads/tests/test_omp -o "8 16" --ruby --topology Crossbar -n 8 --l1i_size=32kB --l1d_size=32kB --l2_size=512MB --num-l2caches=1

