#!/bin/bash
#build/X86/gem5.opt --debug-flags=RubySlicc --debug-file=RubySlicc.out configs/example/se.py -c tests/test-progs/hello/bin/x86/linux/hello --ruby --topology Crossbar -n 4 --l1i_size=32kB --l1d_size=32kB --l2_size=512MB --num-l2caches=1
build/X86/gem5.opt --debug-flags=RubySlicc,RubySnoopPred --debug-file=Ruby.out configs/example/se.py -c tests/test-progs/hello/bin/x86/linux/hello --ruby --topology Crossbar -n 4 --l1i_size=32kB --l1d_size=32kB --l2_size=512MB --num-l2caches=1
