# Compatability

### Load/Store Operations:

| LDA          | Load Accumulator |     | Tested? |
| ------------ | ---------------- | --- | ------- |
| Immediate    | INS_LDA_IM       | $A9 |    T    |
| Zero Page    | INS_LDA_ZP       | $A5 |    T    |
| Zero Page X  | INS_LDA_ZP_X     | $B5 |    T    |
| Absolute     | INS_LDA_ABS      | $AD |    T    |
| Absolute X   | INS_LDA_ABS_X    | $BD |    T    |
| (Indirect) X | INS_LDA_IND_X    | $A1 |    T    |

| LDX         | Load X Register |     |
| ----------- | --------------- | --- |
| Immediate   | INS_LDX_IM      | $A2 |
| Zero Page   | INS_LDX_ZP      | $A6 |
| Zero Page Y | INS_LDX_ZP_Y    | $B6 |
| Absolute    | INS_LDX_ABS     | $AE |
| Absolute Y  | INS_LDX_ABS_Y   | $BE |

### Transfer Operations:

| TAX     | Transfer Acc to X |     |
| ------- | ----------------- | --- |
| Implied | INS_TAX           | $AA |

| TAY     | Transfer Acc to Y |     |
| ------- | ----------------- | --- |
| Implied | INS_TAY           | $A8 |

| TSX     | Transfer SP to X  |     |
| ------- | ----------------- | --- |
| Implied | INS_TSX           | $BA |

| TXA     | Transfer X to Acc |     |
| ------- | ----------------- | --- |
| Implied | INS_TXA           | $8A |

| TXS     | Transfer X to SP |     |
| ------- | ---------------- | --- |
| Implied | INS_TXS          | $9A |

| TYA     | Transfer Y to Acc |     |
| ------- | ----------------- | --- |
| Implied | INS_TYA           | $98 |



### Jumps & Calls

| JMP      | Jump T another Location |     |
| -------- | ----------------------- | --- |
| Absolute | INS_JMP_ABS             | $4C |
| Indirect | INS_JMP_IND             | $6C |

| JSR      | Jump to SubRoutine |     |
| -------- | ------------------ | --- |
| Absolute | INS_JSR            | $20 |


### Logical Operations:

| AND          | Bitwise AND      |     | Tested? |
| ------------ | ---------------- | --- | ------- |
| Immediate    | INS_AND_IM       | $29 |    T    |
| Zero Page    | INS_AND_ZP       | $25 |    T    |
| Zero Page X  | INS_AND_ZP_X     | $35 |    T    |
| Absolute     | INS_AND_ABS      | $2D |    T    |
| Absolute X   | INS_AND_ABS_X    | $3D |    T    |
| Absolute Y   | INS_AND_ABS_Y    | $39 |    T    |
| (Indirect) X | INS_AND_IND_X    | $21 |    T    |
| (Indirect) Y | INS_AND_IND_Y    | $31 |    T    |
