# REQUIRES: hexagon
# RUN: llvm-mc -filetype=obj -triple=hexagon-unknown-linux-musl %s -o %t.o
# RUN: ld.lld %t.o -o %t
# RUN: llvm-objdump -d %t 2>&1 | FileCheck --check-prefix=CHECK-NONPIC %s
# RUN: llvm-mc -filetype=obj --position-independent \
# RUN:         -triple=hexagon-unknown-linux-musl %s -o %t.o
# RUN: ld.lld --pie %t.o -o %t
# RUN: llvm-objdump -d %t 2>&1 | FileCheck --check-prefix=CHECK-PIC %s

# Packets with pc-relative relocations are more interesting because
# the offset must be relative to the start of the source, destination
# packets and not necessarily the instruction word containing the jump/call.

# CHECK:  Disassembly of section .text:

# CHECK-NONPIC: 000200b4 <__trampoline_for_myfn_a_from_.text.thunk>:
# CHECK-NONPIC: { immext(#0x1000040)
# CHECK-NONPIC:   jump 0x1020110 }

# CHECK-PIC:    00010150 <__trampoline_for_myfn_a_from_.text.thunk>:
# CHECK-PIC:    { immext(#0x1000040)
# CHECK-PIC:      r14 = add(pc,##0x1000060) }
# CHECK-PIC:    { jumpr r14 }

# CHECK-NONPIC: 000200bc <myfn_b>:
# CHECK-NONPIC: { jumpr r31 }
# CHECK-PIC:    0001015c <myfn_b>:
# CHECK-PIC:    { jumpr r31 }
    .globl myfn_b
    .type  myfn_b, @function
myfn_b:
    jumpr r31
    .size  myfn_b, .-myfn_b

# CHECK-PIC:    00010160 <main>:
    .globl main
    .type  main, @function
main:
    { r0 = #0
      call myfn_a }
# CHECK-PIC:      { call 0x10150
# CHECK-NONPIC:   { call 0x200b4
# CHECK:            r0 = #0x0 }
    call myfn_a
# CHECK-PIC:    call 0x10150
# CHECK-NONPIC: call 0x200b4
    call myfn_b
# CHECK-PIC:    call 0x1015c
# CHECK-NONPIC: call 0x200bc

    { r2 = add(r0, r1)
      if (p0) call #myfn_b
      if (!p0) call #myfn_a }
# CHECK-PIC:     { if (p0) call 0x1015c
# CHECK-PIC:       if (!p0) call 0x10150
# CHECK-NONPIC:  { if (p0) call 0x200bc
# CHECK-NONPIC:    if (!p0) call 0x200b4
# CHECK:           r2 = add(r0,r1) }

    { r2 = add(r0, r1)
      if (p0) call #myfn_a
      if (!p0) call #myfn_a }
# CHECK-PIC:  { if (p0) call 0x10150
# CHECK-PIC:    if (!p0) call 0x10150
# CHECK-NONPIC:  { if (p0) call 0x200b4
# CHECK-NONPIC:    if (!p0) call 0x200b4
# CHECK:           r2 = add(r0,r1) }

    { r2 = add(r0, r1)
      r1 = r4
      r4 = r5
      if (r0 == #0) jump:t #myfn_a }
# CHECK-PIC:     { if (r0==#0) jump:t 0x10150
# CHECK-NONPIC:  { if (r0==#0) jump:t 0x200b4
# CHECK:           r2 = add(r0,r1)
# CHECK:           r1 = r4; r4 = r5 }

    { r2 = add(r0, r1)
      r4 = r5
      if (r0 <= #0) jump:t #myfn_a
      p1 = cmp.eq(r0, #0); if (p1.new) jump:nt #myfn_a }
# CHECK-NONPIC:  { if (r0<=#0) jump:t 0x200b4
# CHECK-NONPIC:    p1 = cmp.eq(r0,#0x0); if (p1.new) jump:nt 0x200b4
# CHECK-PIC:     { if (r0<=#0) jump:t 0x10150
# CHECK-PIC:       p1 = cmp.eq(r0,#0x0); if (p1.new) jump:nt 0x10150
# CHECK:           r2 = add(r0,r1)
# CHECK:           r4 = r5 }

    {r0 = #0; jump #myfn_a}
# CHECK-PIC:    { r0 = #0x0 ; jump 0x10150 }
# CHECK-NONPIC: { r0 = #0x0 ; jump 0x200b4 }
    {r0 = #0; jump #myfn_b}
# CHECK-PIC:    { r0 = #0x0 ; jump 0x1015c }
# CHECK-NONPIC: { r0 = #0x0 ; jump 0x200bc }
    jumpr r31
    .size   main, .-main

    .section .text.foo
    .skip 0x1000000

    .globl myfn_a
    .type  myfn_a, @function
myfn_a:
    {r0 = #0; jump #myfn_b}
    jumpr r31
    .size  myfn_a, .-myfn_a

# CHECK-NONPIC: 01020110 <myfn_a>:
# CHECK-NONPIC: { r0 = #0x0 ; jump 0x1020118 }
# CHECK-NONPIC: { jumpr r31 }

# CHECK-NONPIC: 01020118 <__trampoline_for_myfn_b_from_.text.thunk>:
# CHECK-NONPIC: { immext(#0xfeffff80)
# CHECK-NONPIC:   jump 0x200bc }

# CHECK-PIC:    010101b8 <__trampoline_for_myfn_b_from_.text.thunk>:
# CHECK-PIC:    { immext(#0xfeffff80)
# CHECK-PIC:      r14 = add(pc,##0xfeffffa4) }
# CHECK-PIC:    { jumpr r14 }
