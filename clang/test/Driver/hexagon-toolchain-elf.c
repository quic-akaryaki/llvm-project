// -----------------------------------------------------------------------------
// Test standard include paths
// -----------------------------------------------------------------------------

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin %s 2>&1 | FileCheck -check-prefix=CHECK000 %s
// CHECK000: "-cc1" {{.*}} "-internal-externc-isystem" "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/include"

// RUN: %clangxx -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin %s 2>&1 | FileCheck -check-prefix=CHECK001 %s
// CHECK001: "-cc1" {{.*}} "-internal-isystem" "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/include/c++"
// CHECK001:   "-internal-externc-isystem" "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/include"

// -----------------------------------------------------------------------------
// Test -nostdinc, -nostdlibinc, -nostdinc++
// -----------------------------------------------------------------------------

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -nostdinc %s 2>&1 | FileCheck -check-prefix=CHECK110 %s
// CHECK110: "-cc1"
// CHECK110-NOT: "-internal-externc-isystem"

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -nostdlibinc %s 2>&1 | FileCheck -check-prefix=CHECK111 %s
// CHECK111: "-cc1"
// CHECK111-NOT: "-internal-externc-isystem"

// RUN: %clangxx -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -nostdinc++ %s 2>&1 | FileCheck -check-prefix=CHECK112 %s
// CHECK112: "-cc1"
// CHECK112-NOT: "-internal-isystem"
// CHECK112-DAG: "-internal-externc-isystem" "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/include"

// RUN: %clangxx -### --target=hexagon-unknown-elf -fno-integrated-as    \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/qc/bin \
// RUN:   -nostdlibinc %s 2>&1 | FileCheck -check-prefix=CHECK113 %s
// CHECK113: "-cc1"
// CHECK113-NOT: "-internal-isystem"
// CHECK113-NOT: "-internal-externc-isystem"

// -----------------------------------------------------------------------------
// Test -mcpu=<cpuname> -mv<number>
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv5 %s 2>&1 | FileCheck -check-prefix=CHECK221 %s
// CHECK221: "-cc1" {{.*}} "-target-cpu" "hexagonv5"
// CHECK221: {{hexagon-link|ld}}{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v5/crt0

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv55 %s 2>&1 | FileCheck -check-prefix=CHECK222 %s
// CHECK222: "-cc1" {{.*}} "-target-cpu" "hexagonv55"
// CHECK222: {{hexagon-link|ld}}{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v55/crt0

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 %s 2>&1 | FileCheck -check-prefix=CHECK223 %s
// CHECK223: "-cc1" {{.*}} "-target-cpu" "hexagonv60"
// CHECK223: {{hexagon-link|ld}}{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv62 %s 2>&1 | FileCheck -check-prefix=CHECK224 %s
// CHECK224: "-cc1" {{.*}} "-target-cpu" "hexagonv62"
// CHECK224: {{hexagon-link|ld}}{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v62/crt0

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv65 %s 2>&1 | FileCheck -check-prefix=CHECK225 %s
// CHECK225: "-cc1" {{.*}} "-target-cpu" "hexagonv65"
// CHECK225: {{hexagon-link|ld}}{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v65/crt0

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv66 %s 2>&1 | FileCheck -check-prefix=CHECK226 %s
// CHECK226: "-cc1" {{.*}} "-target-cpu" "hexagonv66"
// CHECK226: {{hexagon-link|ld}}{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v66/crt0

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -O3 %s 2>&1 | FileCheck -check-prefix=CHECK227 %s
// CHECK227-NOT: "-ffp-contract=fast"
// CHECK227: {{hexagon-link|ld}}

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -O3 -ffp-contract=off %s 2>&1 | FileCheck -check-prefix=CHECK228 %s
// CHECK228-NOT: "-ffp-contract=fast"
// CHECK228: {{hexagon-link|ld}}

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv65 -march=hexagon\
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK229 %s
// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mv65 -march=hexagon\
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK229 %s
// CHECK229: "-cc1" {{.*}} "-target-cpu" "hexagonv65"
// CHECK229: {{hexagon-link|ld}}{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v65/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv67 -fuse-ld=hexagon-link \
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK22A %s
// CHECK22A: "-cc1" {{.*}} "-target-cpu" "hexagonv67"
// CHECK22A: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v67/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv67t \
// RUN:   -fuse-ld=fake-value-to-ignore-CLANG_DEFAULT_LINKER %s 2>&1 | FileCheck -check-prefix=CHECK22B %s
// CHECK22B: "-cc1" {{.*}} "-target-cpu" "hexagonv67t"
// CHECK22B: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v67t/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv68 -fuse-ld=hexagon-link \
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK22C %s
// CHECK22C: "-cc1" {{.*}} "-target-cpu" "hexagonv68"
// CHECK22C: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v68/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv69 -fuse-ld=hexagon-link \
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK22D %s
// CHECK22D: "-cc1" {{.*}} "-target-cpu" "hexagonv69"
// CHECK22D: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v69/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv71 -fuse-ld=hexagon-link \
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK22E %s
// CHECK22E: "-cc1" {{.*}} "-target-cpu" "hexagonv71"
// CHECK22E: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v71/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv71t -fuse-ld=hexagon-link \
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK22F %s
// CHECK22F: "-cc1" {{.*}} "-target-cpu" "hexagonv71t"
// CHECK22F: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v71t/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv73 -fuse-ld=hexagon-link \
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK230 %s
// CHECK230: "-cc1" {{.*}} "-target-cpu" "hexagonv73"
// CHECK230: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v73/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv75 -fuse-ld=hexagon-link \
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK240 %s
// CHECK240: "-cc1" {{.*}} "-target-cpu" "hexagonv75"
// CHECK240: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v75/crt0

// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv79 -fuse-ld=hexagon-link \
// RUN:   %s 2>&1 | FileCheck -check-prefix=CHECK250 %s
// CHECK250: "-cc1" {{.*}} "-target-cpu" "hexagonv79"
// CHECK250: hexagon-link{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v79/crt0

// -----------------------------------------------------------------------------
// Test Linker related args
// -----------------------------------------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Defaults for C
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 %s 2>&1 | FileCheck -check-prefix=CHECK330 %s
// CHECK330: "-cc1"
// CHECK330: {{hexagon-link|ld}}
// CHECK330-NOT: "-static"
// CHECK330-NOT: "-shared"
// CHECK330: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0_standalone.o"
// CHECK330: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK330: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK330: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK330: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK330: "{{[^"]+}}.o"
// CHECK330: "--start-group" "-lstandalone" "-lc" "-lgcc" "--end-group"
// CHECK330: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Defaults for C++
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RUN: %clangxx -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 %s 2>&1 | FileCheck -check-prefix=CHECK331 %s
// CHECK331: "-cc1"
// CHECK331: {{hexagon-link|ld}}
// CHECK331-NOT: "-static"
// CHECK331-NOT: "-shared"
// CHECK331: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0_standalone.o"
// CHECK331-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK331-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK331-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK331-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK331-SAME: "{{[^"]+}}.o"
// CHECK331-SAME: "-lstdc++" "-lm" "--start-group" "-lstandalone" "-lc" "-lgcc" "--end-group"
// CHECK331-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Additional Libraries (-L)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -Lone -L two -L three %s 2>&1 | FileCheck -check-prefix=CHECK332 %s
// CHECK332: "-cc1"
// CHECK332: {{hexagon-link|ld}}
// CHECK332-NOT: "-static"
// CHECK332-NOT: "-shared"
// CHECK332: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0_standalone.o"
// CHECK332: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK332: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK332: "-Lone" "-Ltwo" "-Lthree"
// CHECK332: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK332: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK332: "{{[^"]+}}.o"
// CHECK332: "--start-group" "-lstandalone" "-lc" "-lgcc" "--end-group"
// CHECK332: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// -static, -shared
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -static %s 2>&1 | FileCheck -check-prefix=CHECK333 %s
// CHECK333: "-cc1"
// CHECK333: {{hexagon-link|ld}}
// CHECK333: "-static"
// CHECK333: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0_standalone.o"
// CHECK333: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK333: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK333: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK333: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK333: "{{[^"]+}}.o"
// CHECK333: "--start-group" "-lstandalone" "-lc" "-lgcc" "--end-group"
// CHECK333: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -shared %s 2>&1 | FileCheck -check-prefix=CHECK334 %s
// CHECK334: "-cc1"
// CHECK334: {{hexagon-link|ld}}
// CHECK334: "-shared" "-call_shared"
// CHECK334-NOT: crt0_standalone.o
// CHECK334-NOT: crt0.o
// CHECK334: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/G0/pic/initS.o"
// CHECK334: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/G0"
// CHECK334: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK334: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK334: "{{[^"]+}}.o"
// CHECK334: "--start-group"
// CHECK334-NOT: "-lstandalone"
// CHECK334-NOT: "-lc"
// CHECK334: "-lgcc"
// CHECK334: "--end-group"
// CHECK334: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/G0/pic/finiS.o"

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -shared \
// RUN:   -static %s 2>&1 | FileCheck -check-prefix=CHECK335 %s
// CHECK335: "-cc1"
// CHECK335: {{hexagon-link|ld}}
// CHECK335: "-shared" "-call_shared" "-static"
// CHECK335-NOT: crt0_standalone.o
// CHECK335-NOT: crt0.o
// CHECK335: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/G0/init.o"
// CHECK335-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/G0"
// CHECK335-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK335-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK335-SAME: "{{[^"]+}}.o"
// CHECK335-SAME: "--start-group"
// CHECK335-NOT: "-lstandalone"
// CHECK335-NOT: "-lc"
// CHECK335-SAME: "-lgcc" "--end-group"
// CHECK335-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/G0/fini.o"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// -nostdlib, -nostartfiles, -nodefaultlibs, -nolibc
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RUN: %clangxx -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -nostdlib %s 2>&1 | FileCheck -check-prefix=CHECK336 %s
// CHECK336: "-cc1"
// CHECK336: {{hexagon-link|ld}}
// CHECK336-NOT: crt0_standalone.o
// CHECK336-NOT: crt0.o
// CHECK336-NOT: init.o
// CHECK336: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK336: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK336: "{{[^"]+}}.o"
// CHECK336-NOT: "-lstdc++"
// CHECK336-NOT: "-lm"
// CHECK336-NOT: "--start-group"
// CHECK336-NOT: "-lstandalone"
// CHECK336-NOT: "-lc"
// CHECK336-NOT: "-lgcc"
// CHECK336-NOT: "--end-group"
// CHECK336-NOT: fini.o

// RUN: %clangxx -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -nostartfiles %s 2>&1 | FileCheck -check-prefix=CHECK337 %s
// CHECK337: "-cc1"
// CHECK337: {{hexagon-link|ld}}
// CHECK337-NOT: crt0_standalone.o
// CHECK337-NOT: crt0.o
// CHECK337-NOT: init.o
// CHECK337: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK337: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK337: "{{[^"]+}}.o"
// CHECK337: "-lstdc++" "-lm" "--start-group" "-lstandalone" "-lc" "-lgcc" "--end-group"
// CHECK337-NOT: fini.o

// RUN: %clangxx -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -nodefaultlibs %s 2>&1 | FileCheck -check-prefix=CHECK338 %s
// CHECK338: "-cc1"
// CHECK338: {{hexagon-link|ld}}
// CHECK338: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0_standalone.o"
// CHECK338: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK338: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK338: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK338: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK338: "{{[^"]+}}.o"
// CHECK338-NOT: "-lstdc++"
// CHECK338-NOT: "-lm"
// CHECK338-NOT: "--start-group"
// CHECK338-NOT: "-lstandalone"
// CHECK338-NOT: "-lc"
// CHECK338-NOT: "-lgcc"
// CHECK338-NOT: "--end-group"
// CHECK338: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"

// RUN: %clangxx -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin -mcpu=hexagonv60 \
// RUN:   -fuse-ld=lld -nolibc %s 2>&1 | FileCheck -check-prefix=CHECK-NOLIBC %s
// CHECK-NOLIBC: "-cc1"
// CHECK-NOLIBC: ld.lld
// CHECK-NOLIBC-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0_standalone.o"
// CHECK-NOLIBC-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK-NOLIBC-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK-NOLIBC-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK-NOLIBC-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK-NOLIBC-SAME: "{{[^"]+}}.o"
// CHECK-NOLIBC-SAME: "-lstdc++"
// CHECK-NOLIBC-SAME: "-lm"
// CHECK-NOLIBC-SAME: "--start-group"
// CHECK-NOLIBC-SAME: "-lstandalone"
// CHECK-NOLIBC-NOT: "-lc"
// CHECK-NOLIBC-SAME: "-lgcc"
// CHECK-NOLIBC-SAME: "--end-group"
// CHECK-NOLIBC-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// -moslib
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -moslib=first -moslib=second %s 2>&1 | FileCheck -check-prefix=CHECK339 %s
// CHECK339: "-cc1"
// CHECK339: {{hexagon-link|ld}}
// CHECK339-NOT: "-static"
// CHECK339-NOT: "-shared"
// CHECK339-NOT: crt0_standalone.o
// CHECK339: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK339-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK339-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK339-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK339-SAME: "{{[^"]+}}.o"
// CHECK339-SAME: "--start-group" "-lfirst" "-lsecond"
// CHECK339-NOT: "-lstandalone"
// CHECK339-SAME: "-lc" "-lgcc" "--end-group"
// CHECK339-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -moslib=first -moslib=second -moslib=standalone %s 2>&1 | FileCheck -check-prefix=CHECK33A %s
// CHECK33A: "-cc1"
// CHECK33A: {{hexagon-link|ld}}
// CHECK33A-NOT: "-static"
// CHECK33A-NOT: "-shared"
// CHECK33A:      "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0_standalone.o"
// CHECK33A-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK33A-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK33A-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK33A-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK33A-SAME: "{{[^"]+}}.o"
// CHECK33A-SAME: "--start-group"
// CHECK33A-SAME: "-lfirst" "-lsecond"
// CHECK33A-SAME: "-lstandalone"
// CHECK33A-SAME: "-lc" "-lgcc" "--end-group"
// CHECK33A-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Other args to pass to linker
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RUN: %clangxx -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 -fuse-ld=lld \
// RUN:   -s -t -e start_here -uFoo -undefined Bar %s 2>&1 | FileCheck -check-prefix=CHECK33B %s
// CHECK33B:      "-cc1"
// CHECK33B:      ld.lld
// CHECK33B-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0_standalone.o"
// CHECK33B-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/crt0.o"
// CHECK33B-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/init.o"
// CHECK33B-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60"
// CHECK33B-SAME: "-L{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib"
// CHECK33B-SAME: "-s" "-t" "-u" "Foo" "-undefined" "Bar"
// CHECK33B-SAME: "{{[^"]+}}.o"
// CHECK33B-SAME: "-lstdc++" "-lm" "--start-group" "-lstandalone" "-lc" "-lgcc" "--end-group"
// CHECK33B-SAME: "{{.*}}/Inputs/hexagon_tree/Tools/bin/../target/hexagon/lib/v60/fini.o"

// -----------------------------------------------------------------------------
// pic, small data threshold
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 %s 2>&1 | FileCheck -check-prefix=CHECK340 %s
// CHECK340:      "-cc1"
// CHECK340: {{hexagon-link|ld}}
// CHECK340-NOT:  "-G{{[0-9]+}}"

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -fpic %s 2>&1 | FileCheck -check-prefix=CHECK341 %s
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -fPIC %s 2>&1 | FileCheck -check-prefix=CHECK341 %s
// CHECK341:      "-cc1"
// CHECK341-NOT:  "-mrelocation-model" "static"
// CHECK341:      "-pic-level" "{{[12]}}"
// CHECK341:      "-mllvm" "-hexagon-small-data-threshold=0"
// CHECK341: {{hexagon-link|ld}}
// CHECK341:      "-G0"

// RUN: %clang -### --target=hexagon-unknown-elf -fno-integrated-as -fno-pie -no-pie \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -G=8 %s 2>&1 | FileCheck -check-prefix=CHECK342 %s
// RUN: %clang -### --target=hexagon-unknown-elf -fno-integrated-as -fno-pie -no-pie \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -G 8 %s 2>&1 | FileCheck -check-prefix=CHECK342 %s
// RUN: %clang -### --target=hexagon-unknown-elf -fno-integrated-as -fno-pie -no-pie \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -msmall-data-threshold=8 %s 2>&1 | FileCheck -check-prefix=CHECK342 %s
// CHECK342:      "-cc1"
// CHECK342:      "-mrelocation-model" "static"
// CHECK342:      "-mllvm" "-hexagon-small-data-threshold=8"
// CHECK342-NEXT: llvm-mc
// CHECK342:      "-gpsize=8"
// CHECK342: {{hexagon-link|ld}}
// CHECK342:      "-G8"

// -----------------------------------------------------------------------------
// pie
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -pie %s 2>&1 | FileCheck -check-prefix=CHECK350 %s
// CHECK350:      "-cc1"
// CHECK350:      {{hexagon-link|ld}}
// CHECK350:      "-pie"

// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -pie -shared %s 2>&1 | FileCheck -check-prefix=CHECK351 %s
// CHECK351:      "-cc1"
// CHECK351:      {{hexagon-link|ld}}
// CHECK351-NOT:  "-pie"

// -----------------------------------------------------------------------------
// Test Assembler related args
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf -fno-integrated-as    \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -gdwarf-2 \
// RUN:   -Wa,--noexecstack,--trap \
// RUN:   -Xassembler --keep-locals %s 2>&1 | FileCheck -check-prefix=CHECK360 %s
// CHECK360:      "-cc1"
// CHECK360-NEXT: llvm-mc
// CHECK360:      "--noexecstack" "--trap" "--keep-locals"
// CHECK360:      {{hexagon-link|ld}}

// -----------------------------------------------------------------------------
// ffixed-r19
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf -ffixed-r19 %s 2>&1 \
// RUN:        | FileCheck --check-prefix=CHECK370 %s
// CHECK370: "-target-feature" "+reserved-r19"
// RUN: %clang -### --target=hexagon-unknown-elf %s 2>&1 \
// RUN:        | FileCheck --check-prefix=CHECK371 %s
// CHECK371-NOT: "+reserved-r19"

// -----------------------------------------------------------------------------
// mcabac
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf -mcabac %s 2>&1 \
// RUN:        | FileCheck --check-prefix=CHECK372 %s
// CHECK372: "-target-feature" "+cabac"
// RUN: %clang -### --target=hexagon-unknown-elf %s 2>&1 \
// RUN:        | FileCheck --check-prefix=CHECK373 %s
// CHECK373-NOT: "+cabac"

// -----------------------------------------------------------------------------
// Misc Defaults
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 %s 2>&1 | FileCheck -check-prefix=CHECK380 %s
// CHECK380:      "-cc1"
// CHECK380-SAME: "-Wreturn-type"

// -----------------------------------------------------------------------------
// Default, hexagon-link is used
// -----------------------------------------------------------------------------
// RUN: not %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -fuse-ld=fake-value-to-ignore-CLANG_DEFAULT_LINKER %s 2>&1 | FileCheck -check-prefix=CHECK381 %s
// CHECK381:      "-march=hexagon"
// CHECK381:      "-mcpu=hexagonv60"
// -----------------------------------------------------------------------------
// Passing -fuse-ld=lld
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   -fuse-ld=lld %s 2>&1 | FileCheck -check-prefix=CHECK382 %s
// CHECK382:          "--eh-frame-hdr
// CHECK382-NOT:      "-march=
// CHECK382-NOT:      "-mcpu=
// -----------------------------------------------------------------------------
// Passing --sysroot
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 \
// RUN:   --sysroot=/hexagon %s 2>&1 | FileCheck -check-prefix=CHECK383 %s
// CHECK383:          "-isysroot" "/hexagon"
// CHECK383:          "-internal-externc-isystem" "/hexagon{{/|\\\\}}include"
// -----------------------------------------------------------------------------
// Passing -fno-use-init-array
// -----------------------------------------------------------------------------
// RUN: %clang -### --target=hexagon-unknown-elf \
// RUN:   -ccc-install-dir %S/Inputs/hexagon_tree/Tools/bin \
// RUN:   -mcpu=hexagonv60 %s 2>&1 | FileCheck -check-prefix=CHECK384 %s
// CHECK384:          "-fno-use-init-array"
